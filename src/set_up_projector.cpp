#include <iostream>
#include <vector>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <memory>
#include <thread>

#include <boost/algorithm/string.hpp>

#include "common/client.h"

using namespace std;
using namespace zlaser::thrift;

void printAvailableCoordinateSystems(shared_ptr<ZlpThriftClient> client)
{
    printf("Available coordinate systems:\n");

    vector<string> coordinateSystems = client->getCoordinateSystems();

    for (const string& cs : coordinateSystems)
    {
        cout << cs << endl;
    }
}

void registerProjector(const string ip, const int port, const bool doPointSearch)
{
    try
    {
        if (doPointSearch)
            cout << "Example: Create custom coordinate system with reference point search" << endl;
        else
            cout << "Example: Create custom coordinate system without reference point search" << endl;

        // initialize system

        shared_ptr<ZlpThriftClient> client(ZlpThriftClient::create());
        client->connect(port, ip);
// cari projector
        vector<string> projectors = client->scanProjectors();

        if (projectors.size() == 0)
            throw runtime_error("No projector found.");

        const string projector = projectors.front();
       // activate
        client->activateProjector(projector);
        printAvailableCoordinateSystems(client);
// persiapan data
        // data reference object
        Referenceobject refObject;
        // nama
        const string refObjectName = "RefObject";

        // set nama
        refObject.__set_name(refObjectName);
// list data reference point
        vector<Referencepoint> refPointList;
        Referencepoint refPoint;
        // input data reference point adalah point
        Vector3D point;
// mulai dari sini persiapan data... data yang akan jadi coordinate system
        refPoint.__set_name("T1");
        point.__set_x(0);
        point.__set_y(0);
        point.__set_z(0);
        refPoint.__set_refPoint(point);
        refPointList.push_back(refPoint);

        refPoint.__set_name("T2");
        point.__set_x(1000);
        point.__set_y(0);
        point.__set_z(0);
        refPoint.__set_refPoint(point);
        refPointList.push_back(refPoint);

        refPoint.__set_name("T3");
        point.__set_x(0);
        point.__set_y(1000);
        point.__set_z(0);
        refPoint.__set_refPoint(point);
        refPointList.push_back(refPoint);

        refPoint.__set_name("T4");
        point.__set_x(1000);
        point.__set_y(1000);
        point.__set_z(0);
        refPoint.__set_refPoint(point);
        refPointList.push_back(refPoint);
// di input ke data reference object
        refObject.__set_refPointList(refPointList);

        // set global cross size for all reference points
 // data cross sisze di projector berupa + untuk besar kecilnya
        Vector2D crossSize;
        crossSize.x = 50;
        crossSize.y = 50;

        // - define coordinates in system of factory calibration wall [mm]
        // - activate reference point to use for transformation
        // - set cross size to set search area

        // data ref point list adalah data point yang akan jadi coordinate system
        // di simulasikan 4 point
        // adapun parameternye adalah
        // jarak atau distance di set 3533.4 sekitar 3.5 meter
        // set parameter cross size
        // set activate
        //data untuk projecsi berupa trace dan bisa jadi posisinya tidak sama dengan data yang akan jadi coordinate
        refObject.refPointList[0].tracePoint.__set_x(143.2);
        refObject.refPointList[0].tracePoint.__set_y(-283.9);
        refObject.refPointList[0].__set_distance(3533.4);
        refObject.refPointList[0].__set_activated(true);
        refObject.refPointList[0].__set_crossSize(crossSize);

        refObject.refPointList[1].tracePoint.__set_x(151.1);
        refObject.refPointList[1].tracePoint.__set_y(264.8);
        refObject.refPointList[1].__set_distance(3533.4);
        refObject.refPointList[1].__set_activated(true);
        refObject.refPointList[1].__set_crossSize(crossSize);

        refObject.refPointList[2].tracePoint.__set_x(-351 );
        refObject.refPointList[2].tracePoint.__set_y(-283.9);
        refObject.refPointList[2].__set_distance(3533.4);
        refObject.refPointList[2].__set_activated(true);
        refObject.refPointList[2].__set_crossSize(crossSize);

        refObject.refPointList[3].tracePoint.__set_x(-339.7);
        refObject.refPointList[3].tracePoint.__set_y(278.3);
        refObject.refPointList[3].__set_distance(3533.4);
        refObject.refPointList[3].__set_activated(true);
        refObject.refPointList[3].__set_crossSize(crossSize);
// set coordinate system ke  data referenceobject
        refObject.__set_coordinateSystem("MyCoordinateSystem"); // Name of the coordinate system that will be created
// set projector id
        refObject.__set_projectorID(projector);
// upload ke projector
        client->SetReferenceobject(refObject);

        // create function module to register the projector to coordinate system
// module id adalah parameter registrasi service
        string moduleId = client->createFunctionModule("zFunctModRegister3d", "3DReg");
// set service dengan property referenceData.
        // referenceData ???????

        client->FunctionModuleSetProperty(moduleId, "referenceData", refObjectName);
// membaca dan merubah state dari projector
        mutex functionModuleCallbackMutex;
        condition_variable stateChangedCondition;
        client->setFunctionModuleStateChangedCallback(
            [&stateChangedCondition, &functionModuleCallbackMutex]
            (const std::string& modID, FunctionModuleStates::type oldState, FunctionModuleStates::type newState)
            {
                (void)modID;
                (void)oldState;
                if (newState != FunctionModuleStates::type::RUNNING)
                {
                    unique_lock<mutex> lock(functionModuleCallbackMutex);
                    cout << "Function module stopped running" << endl;
                    stateChangedCondition.notify_one();
                }
            }
        );

        // set up reflector point search

        if (doPointSearch)
        {
            client->FunctionModuleSetProperty(moduleId, "runMode", "0");

            string pointSearchPropPath = "config.projectorManager.projectors." + projector + ".search.";
            client->SetProperty(pointSearchPropPath + "threshold", "5");

            // run point search

            unique_lock<std::mutex> callbackLock(functionModuleCallbackMutex);

            cout << "Reference point search is running...\n";
            client->FunctionModuleRun(moduleId);

            auto status = stateChangedCondition.wait_for(callbackLock, chrono::minutes(5));
            callbackLock.unlock();

            if (status == cv_status::timeout)
                throw runtime_error("Point search did not finish in time.\n");

            string state = client->getFunctionModuleProperty(moduleId, "state");

            if (state != "1")  // "idle"
                throw runtime_error("An error occured. Function module is not in idle state.");

            cout << "Finished point search\n";

            // Check if all points were found

            client->GetReferenceobject(refObject, refObjectName);

            bool foundAll = true;
            for (size_t i = 0; i < refObject.refPointList.size(); i++)
            {
                string resultPath = "result.tracePoints." + to_string(i);

                if (client->getFunctionModuleProperty(moduleId, resultPath + ".found") != "true")
                {
                    foundAll = false;
                    break;
                }
            }

            if (!foundAll)
                cout << "Not all points have been found!\n";
            else
                cout << "All points have been found.\n";
        }

        // register projector to coordinate system
// set service running mode value 1
        client->FunctionModuleSetProperty(moduleId, "runMode", "1");

        cout << "Register projector to coordinate system..." << endl;

        unique_lock<std::mutex> callbackLock(functionModuleCallbackMutex);
// kick run projector
        client->FunctionModuleRun(moduleId);

        auto status = stateChangedCondition.wait_for(callbackLock, chrono::minutes(5));
        callbackLock.unlock();

        if (status == cv_status::timeout)
            throw runtime_error("Could not calculate transformation in time.\n");
// state dirubah ke idle
        string state = client->getFunctionModuleProperty(moduleId, "state");
// kalau gagal projector what error
        if (state != "1")  // "idle"
            throw runtime_error("An error occured. Function module is not in idle state.");
        else
        {
            // get result dari projector calibration
            string res;
            client->FunctionModuleGetProperty(res, moduleId, "result.averageDistance");

            //Activate reference object only if the calculated transformation was succesfully
            client->GetReferenceobject(refObject, refObjectName);
            refObject.activated = true;
            client->SetReferenceobject(refObject);

            printf("Finished to register projector (aveDist: %s)\n", res.c_str());
            printAvailableCoordinateSystems(client);

            // show reference point
            cout << "Show result of point search for 5 seconds\n";
            client->FunctionModuleSetProperty(moduleId,"showAllRefPts","1");
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }

        // clean-up

        client->RemoveGeoTreeElem(refObjectName);
        client->FunctionModuleRelease(moduleId);
        client->deactivateProjector(projector);
        client->disconnect();
    }
    catch (ZlpThriftClient::Exception& e)
    {
        printf("Interface exception: %s\n", e.what());
    }
    catch (zlaser::thrift::CantReadFile& e)
    {
        printf("Cannot read file: %s\n", e.what());
    }
    catch (zlaser::thrift::ElementDoesNotExist& e)
    {
        printf("Element does not exist: %s\n", e.what());
    }
    catch (zlaser::thrift::PropertyNotFound& e)
    {
        printf("Property not found: %s\n", e.what());
    }
    catch (zlaser::thrift::PropertyChangeFailed& e)
    {
        printf("Property change failed: %s\n", e.what());
    }
    catch (zlaser::thrift::ClientNotRegisteredForFunctionModule& e)
    {
        printf("Client is not registered for function module: %s\n", e.clientId.c_str());
    }
    catch (zlaser::thrift::FunctionModuleIsAlreadyRunning& e)
    {
        printf("Function module is already running: %s\n", e.fModUID.c_str());
    }
    catch (zlaser::thrift::FunctionModuleNotExistent& e)
    {
        printf("Function module does not existent: %s\n", e.fModUID.c_str());
    }
    catch (zlaser::thrift::FunctionModuleCantRunFromThisState& e)
    {
        printf("Function module cannot run from this state: %s\n", e.fModUID.c_str());
    }
    catch (zlaser::thrift::FunctionModuleRunFailedCritical& e)
    {
        printf("Function module run failed critical: %s\n", e.fModUID.c_str());
    }
    catch (zlaser::thrift::FunctionModuleRunFailedRecoverable& e)
    {
        printf("Function module run failed recoverable: %s\n", e.fModUID.c_str());
    }
    catch (zlaser::thrift::FunctionModuleClassNotRegistered& e)
    {
        printf("Function module class not registered: %s\n", e.which.c_str());
    }
    catch (zlaser::thrift::FunctionModulePropertyBranchAlreadyInUse& e)
    {
        printf("Function module property branch already in use: %s\n", e.branchName.c_str());
    }
    catch (zlaser::thrift::FunctionModuleClassNotLicensed& e)
    {
        printf("Function module class not licensed: %s\n", e.which.c_str());
    }
}
