#include <thread>
#include <condition_variable>
#include <iostream>
#include <memory>

#include <boost/filesystem.hpp>

#include "common/client.h"

using namespace std;

void transferLicense(const string ip, const int port, const string license)
{
    try
    {
        cout << "Example: Transfer Licence and Request Licence Information." << endl;

        boost::filesystem::path licensePath(license);
        if (!boost::filesystem::exists(licensePath))
        {
            cout << "License file does not exist: " << licensePath << endl;
            return;
        }

        shared_ptr<ZlpThriftClient> client(ZlpThriftClient::create());
        client->connect(port, ip);

        cout << "Load licence: " << licensePath << endl;
        client->transferFile(licensePath.string(), licensePath.filename().string(), true);

        vector<string> projectors = client->scanProjectors();
        const string projector = projectors.front();
        client->activateProjector(projector);

        // set callback for event "PropertyChanged"
        mutex mx;
        condition_variable propertyChangedCondition;
        client->setPropertyChangedCallback(
            [&propertyChangedCondition, &mx]
            (const string& property, const string& value)
            {
                unique_lock<mutex> lock(mx);
                cout << "Property changed callback: " << property << " = " << value << endl;
                propertyChangedCondition.notify_one();
            }
        );

        // Activate notification for Property
        client->RegisterForChangedProperty("config.licenseState.IsValid");

        // Lock the mutex for the callback function
        unique_lock<std::mutex> lock(mx);

        // load previously transfered license; only the basename will be used, the path
        // to the license file will be stripped by `loadLicense()` in zService
        client->LoadLicense(license);

        // Wait for property change
        auto status = propertyChangedCondition.wait_for(lock, chrono::seconds(10));
        if (status == cv_status::no_timeout)
        {
            string valid = client->getProperty("config.licenseState.IsValid");
            if (valid == "true")
            {
                cout << "The license is valid." << endl;

                string customerName = client->getProperty("license.Customer.Name");
                cout << "Customer: " << customerName << endl;

                string grantorName = client->getProperty("license.Grantor.Name");
                cout << "Grantor: " << grantorName << endl;

                // Validity for function modules

                vector<string> modules = client->getPropertyChildren("config.licenseState.Modules");
                for (const string& module : modules)
                {
                    string status = client->getProperty("config.licenseState.Modules." + module);
                    cout << "Function Module: " << module << ", Validity: " << status << endl;
                }
            }
            else
            {
                cout << "The license is invalid." << endl;
            }
        }
        else
        {
            cout << "Could not obtain license information." << endl;
        }

        client->deactivateProjector(projector);
        client->disconnect();
    }
    catch (ZlpThriftClient::Exception& e)
    {
        printf("Interface exception: %s\n", e.what());
    }
    catch (zlaser::thrift::CantWriteFile& e)
    {
        cout << "Cannot write file: " << e.what() << endl;
    }
    catch (zlaser::thrift::PropertyNotFound& e)
    {
        printf("Property not found: %s\n", e.what());
    }
    catch (zlaser::thrift::PropertyChangeFailed& e)
    {
        printf("Property change failed: %s\n", e.what());
    }
}
