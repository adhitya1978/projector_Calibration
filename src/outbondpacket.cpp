#include <mutex>
#include <condition_variable>
#include <QHash>

#include "outbondpacket.h"
#include "log_widget.h"
#include "packetreferenceobject.h"

//!
//! \brief scan refletor
//! \param coordinate system
//! \param projector
//! \param object
//!
OutbondPacket::OutbondPacket(ZlpThriftClient *client, QObject *parent) :
    m_clientinterface(client),
    m_isactiv(false),
    checkTracePoint(false),
    checkPoint(false),
    QObject(parent)
{
    //! pointer state string value
    DeviceState = new QHash<QString, FunctionModuleStates::type>();
    DeviceState->insert("0", FunctionModuleStates::type::UNITIALIZED);
    DeviceState->insert("1", FunctionModuleStates::type::IDLE);
    DeviceState->insert("2", FunctionModuleStates::type::RUNNING);
    DeviceState->insert("3", FunctionModuleStates::type::STOP_REQUESTED);
    DeviceState->insert("4", FunctionModuleStates::type::RECOVERABLE_ERROR);
    DeviceState->insert("5", FunctionModuleStates::type::CRITICAL_ERROR);

    CMDTOBOOL = new QHash<QString, bool>();
    CMDTOBOOL->insert("0", false);
    CMDTOBOOL->insert("1", true);
}

OutbondPacket::~OutbondPacket()
{
    //delete m_referenceObject;
}


void OutbondPacket::setParameterTracePoint(const QList<SCANNING_POINT *> positions)
{
    qDebug() << "OutbondPacket::setParameterTracePoint()";
    for(auto i =0; i < positions.count(); i++){
        m_referenceObject.refPointList[i].__set_tracePoint(positions[i]->tracepoint);
        m_referenceObject.refPointList[i].__set_distance(positions[i]->distance);
        m_referenceObject.refPointList[i].__set_activated(positions[i]->activated);
        m_referenceObject.refPointList[i].__set_crossSize(positions[i]->crosssize);
        m_referenceObject.refPointList[i].__set_name(positions[i]->name.toStdString());
    }

}

Referenceobject OutbondPacket::getCoordSys(QString coordname, QString refobjname)
{
    qDebug() << "OutbondPacket::getCoordinate()";
    try{
        std::list<zlaser::thrift::Referenceobject> Refobjects;

        m_clientinterface->getAllReferenceObjectatService(Refobjects);

        for(Referenceobject Refobject : Refobjects){
            if((QString::compare(QString::fromStdString(Refobject.name).toUpper(), refobjname.toUpper()) != 1)
                    && (QString::compare(QString::fromStdString(Refobject.coordinateSystem).toUpper(), coordname.toUpper())!= 1))
                return Refobject;
        }
    }
    catch(zlaser::thrift::ProcessInCriticalErrorState& e ){
        qDebug() << tr(e.what());
    }
    catch(zlaser::thrift::ServiceLockTimeout& e){
        qDebug() << tr(e.what());
    }
    catch(zlaser::thrift::InvalidRelativePath& e){
        qDebug() << tr(e.what());
    }
}

FunctionModuleStates::type OutbondPacket::getProjectorState()
{
    try{
        string state = m_clientinterface->getFunctionModuleProperty(m_module_id, "state");
        FunctionModuleStates::type currentstate = (FunctionModuleStates::type)DeviceState->value(state.c_str());
        m_oldstate = currentstate;
    }
    catch (ZlpThriftClient::Exception& e)
    {
        qDebug() << QString("Interface exception: %1").arg(e.what());
    }
    return m_oldstate;
}

void OutbondPacket::setReferenceObject(Referenceobject referenceObject)
{
    m_referenceObject = referenceObject;
}

void OutbondPacket::setAddress(const QString IP, int port)
{
    m_IP = IP;
    m_port = port;
}

QHash<QString, QString> OutbondPacket::getAllNameReferenceObjectAndCoordSystem()
{
    qDebug() << "OutbondPacket::getAllNameReferenceObjectAndCoordSystem()";
    QHash<QString, QString> result;
    try{
        std::list<zlaser::thrift::Referenceobject> Refobjects;

        m_clientinterface->getAllReferenceObjectatService(Refobjects);


        for(Referenceobject _Refobjects : Refobjects){
            result.insert(QString::fromStdString(_Refobjects.coordinateSystem), QString::fromStdString(_Refobjects.name));
        }

    }
    catch(zlaser::thrift::ProcessInCriticalErrorState& e ){
        qDebug() << tr(e.what());
    }
    catch(zlaser::thrift::ServiceLockTimeout& e){
        qDebug() << tr(e.what());
    }
    catch(zlaser::thrift::InvalidRelativePath& e){
        qDebug() << tr(e.what());
    }

    return result;
}

QString OutbondPacket::createGroupTreePolyline(QString groupname)
{
    qDebug() << QString("outbondPacket::CreateGroupTree(%1)").arg(groupname);
    std::string GroupElement;
    try{
        m_clientinterface->CreatePolylinesFromRefObject(GroupElement, groupname.toStdString(), false);
        qDebug() << QString("Result:%1").arg(QString::fromStdString(GroupElement));
    }
    catch(ProcessInCriticalErrorState &e){
        qDebug() << QString::fromStdString(e.what());
    }
    catch(ServiceLockTimeout& e){
        qDebug() << QString::fromStdString(e.what());
    }
    catch(InvalidRelativePath &e){
        qDebug() << QString::fromStdString(e.what());
    }
    catch(ElementHasWrongType &e){
        qDebug() << QString::fromStdString(e.what());
    }
    catch(ElementDoesNotExist &e){
        qDebug() << QString::fromStdString(e.what());
    }
    //! if not thrown
    return QString::fromStdString(GroupElement);
}

QList<string> OutbondPacket::printAvailableCoordinateSystems()
{
    qDebug() << "OutbondPacket::printAvailableCoordinateSystems()";
    QList<string> list_cordinatesystems;
    try{
        vector<string> coordinateSystems = m_clientinterface->getCoordinateSystems();
        for (const string& cs : coordinateSystems)
        {
            qDebug() << QString::fromStdString(cs);
            list_cordinatesystems.append(cs);
        }
    }catch(zlaser::thrift::ProcessInCriticalErrorState& e){
        DPRINT(0, QString("Interface exception: %1").arg(e.what()));
    }
    return list_cordinatesystems;
}

QList<string> *OutbondPacket::printAvailableProjector()
{
    QList<std::string> *projector_list = new QList<std::string>();
    try
    {
        //cout << "Example: Search and Activate Projector." << endl;
        \
        m_clientinterface->connect(m_port, m_IP.toStdString());

        vector<string> projectors = m_clientinterface->scanProjectors();

        if (!projectors.empty())
        {
            for (const string& serialNumber : projectors){
                projector_list->append(serialNumber);
            }

        }
        else{
            DPRINT(0, QString("no projector at IP %1 port %2").arg(this->m_IP).arg(this->m_port));
        }


        m_clientinterface->disconnect(); // not neccessary because client is also disconnected on destruction
    }
    catch (ZlpThriftClient::Exception& e)
    {
        DPRINT(0, QString("Client exception:%0").arg(e.what()));
        m_clientinterface->disconnect();
    }
    return  projector_list;
}

bool OutbondPacket::runProjector()
{
    qDebug() << "OutbondPacket::runProjector()";

    std::string status;
    m_clientinterface->FunctionModuleGetProperty(status, m_module_id, "runMode");

    if(m_isactiv && m_oldstate != PROJECTOR_RUNNING && CMDTOBOOL->value(QString::fromStdString(status)) != true)
        //! check projector already active, state not running and projector status not running
    {
        m_clientinterface->FunctionModuleSetProperty(m_module_id, "runMode", "1");
        m_clientinterface->FunctionModuleGetProperty(status, m_module_id, "runMode");
        if(CMDTOBOOL->value(QString::fromStdString(status)) != true){
            qDebug() << "Function Set property failed";
            return false; // error set property
        }
        // Projector run
        m_clientinterface->FunctionModuleRun(m_module_id);
        // get state
        status = m_clientinterface->getFunctionModuleProperty(m_module_id, "state");
        m_oldstate = DeviceState->value(QString::fromStdString(status));

    }
}

void OutbondPacket::setElementPolyline(Polyline polyline)
{
    m_polyline_element = polyline;
}

bool OutbondPacket::checkGeoTreeElement(string elementName)
{
    try
    {
        return m_clientinterface->GeoTreeElemExist(elementName);
    }
    catch(ProcessInCriticalErrorState& ex)
    {
        qDebug() << (ex.what());
    }
    catch(ServiceLockTimeout& ex)
    {
        qDebug() << (ex.what());
    }
    catch(InvalidRelativePath& ex)
    {
        qDebug() << (ex.what());
    }
    return false;
}

bool OutbondPacket::removeGeoTreeElement(string elementName, GeoTreeElemTypes::type typeOfElement)
{
    try
    {
        if(!elementName.empty() && checkGeoTreeElement(elementName) == true)
        {
            m_clientinterface->RemoveGeoTreeElem(elementName);
            return true;
        }
    }
    catch(zlaser::thrift::ElementDoesNotExist & ex)
    {
        qDebug() << ex.what();
    }
    catch( apache::thrift::TException& ex)
    {
        qDebug() << (QObject::tr("Could not remove geoTreeElement %1 ").arg(elementName.c_str()),ex.what());
        qDebug() << ex.what();
    }
}

bool OutbondPacket::getIsactiv() const
{
    return m_isactiv;
}

void OutbondPacket::RegisterCoordinateSystem(QString refobjname)
{
    qDebug() << "OutbondPacket::RegisterCoordinateSystem()";
    try{
        if(m_isactiv){
            // get result
            qDebug() << "FunctionModuleSetProperty";
            //! clearing cache names refobject
            QList<string> cordSysList = this->printAvailableCoordinateSystems();
            for(string str: cordSysList){
                QString cur = QString::fromStdString(str);
                if(cur.contains(refobjname))
                    m_clientinterface->RemoveGeoTreeElem(str);
            }
            m_clientinterface->SetReferenceobject(m_referenceObject);
            m_clientinterface->FunctionModuleSetProperty(m_module_id, "referenceData", m_referenceObject.name);
            m_clientinterface->FunctionModuleSetProperty(m_module_id, "runMode", "1");
            m_clientinterface->FunctionModuleRun(m_module_id);

            string res;
            m_clientinterface->FunctionModuleGetProperty(res, m_module_id, "result.averageDistance");

            //Activate reference object only if the calculated transformation was succesfully
            m_clientinterface->GetReferenceobject(m_referenceObject, m_referenceObject.name);
            m_referenceObject.activated = true;
            m_clientinterface->SetReferenceobject(m_referenceObject);

            DPRINT(2, QString("Finished to register projector (aveDist: %0)").arg(QString::fromStdString(res)));

            m_clientinterface->FunctionModuleSetProperty(m_module_id,"showAllRefPts","1");

        }
    }
    catch(ServiceLockTimeout &e){
        qDebug() << QString::fromStdString(e.what());
    }
    catch (PropertyNotFound& e)
    {
        qDebug() << QString::fromStdString(e.what());
    }
    catch(FunctionModuleNotExistent& e){
        qDebug() << QString::fromStdString(e.what());
    }
    catch(ClientNotRegisteredForFunctionModule& e){
        qDebug() << QString::fromStdString(e.what());
    }
    catch(ProcessInCriticalErrorState& e){
        qDebug() << QString::fromStdString(e.what());
    }
}

void OutbondPacket::setElementText(const zlaser::thrift::TextElement &text)
{
    m_text_element = text;
}

void OutbondPacket::LoadReferenceObject(const QString coordname, const QString referenceObjectName)
{
    //! will used later
    Q_UNUSED(coordname)
    qDebug() << "LoadReferenceObject()";
    try{
        m_referenceObject = m_clientinterface->getRefenceObjectFromService(referenceObjectName.toStdString());

        PacketReferenceObject *refObject = new PacketReferenceObject(m_referenceObject.refPointList, NULL, NULL);
        //! trying to get polyline
        zlaser::thrift::Polyline polyline = refObject->ConstructPolyLinesFromObject();
        this->setElementPolyline(polyline);
        //!
        m_clientinterface->SetReferenceobject(m_referenceObject);
        emit readyforupload(m_projection_mode);
    }
    catch(zlaser::thrift::ElementDoesNotExist & e)
    {
        DPRINT(0, QString("Interface exception: %0").arg(e.what()));
        qDebug() << QString::fromStdString(e.what());
    }
}

void OutbondPacket::removeReferenceObject(const QString coordname, const QString referenceObjectName)
{
    //! will used later
    Q_UNUSED(coordname)
    qDebug() << "removeReferenceObject()";
    try{
        Referenceobject removing_referenceObject = m_clientinterface->getRefenceObjectFromService(referenceObjectName.toStdString());
        qDebug() << QString::fromStdString(removing_referenceObject.name);
        removing_referenceObject.coordinateSystem.clear();
        m_clientinterface->RemoveGeoTreeElem(removing_referenceObject.name);
        //m_clientinterface->SetReferenceobject(removing_referenceObject);
    }
    catch(zlaser::thrift::ElementDoesNotExist & e)
    {
        qDebug() << QString::fromStdString(e.what());
    }
}

void OutbondPacket::slotSetMode(int mode)
{
    OutbondPacket::PROJECTION_MODE _mode = (OutbondPacket::PROJECTION_MODE)mode;
    try{
        if(m_isactiv
                && _mode == PROJECTION_MODE::OBJECT
                && DeviceState->value(QString::fromStdString(m_clientinterface->getFunctionModuleProperty(m_module_id, "state")))
                == FunctionModuleStates::type::RUNNING){
            m_clientinterface->FunctionModuleSetProperty(m_module_id,"showAllRefPts","0");
            m_clientinterface->TriggerProjection();

        }
        if(m_isactiv
                && _mode == PROJECTION_MODE::OBJECT
                && DeviceState->value(QString::fromStdString(m_clientinterface->getFunctionModuleProperty(m_module_id, "state")))
                == FunctionModuleStates::type::RUNNING){
            m_clientinterface->FunctionModuleSetProperty(m_module_id,"showAllRefPts","1");
        }
    }
    catch (ZlpThriftClient::Exception& e)
    {
        qDebug() << QString("Interface exception: %1").arg(e.what());
    }
    catch (CantReadFile& e)
    {
        qDebug() << QString("Interface exception: %1").arg(e.what());
    }
    catch(ProcessInCriticalErrorState &e){
        qDebug() << QString::fromStdString(e.what());
    }
    catch(ServiceLockTimeout& e){
        qDebug() << QString::fromStdString(e.what());
    }
    catch(InvalidRelativePath &e){
        qDebug() << QString::fromStdString(e.what());
    }
    catch(ElementHasWrongType &e){
        qDebug() << QString::fromStdString(e.what());
    }
    catch(ElementDoesNotExist &e){
        qDebug() << QString::fromStdString(e.what());
    }
    catch (PropertyNotFound& e)
    {
        qDebug() << QString::fromStdString(e.what());
    }
    catch (PropertyChangeFailed& e)
    {
        qDebug() << QString::fromStdString(e.what());
    }
    catch(zlaser::thrift::InvalidProjectionCoordinate& e){
        qDebug() << QString::fromStdString(e.what());
    }
    catch(ErrorInProjectorCommunication& e){
        qDebug() << QString::fromStdString(e.what());
    }
    catch(LimitedProjection& e){
        qDebug() << QString::fromStdString(e.what());
    }
}

void OutbondPacket::transformPolylinesfromRefObject()
{
    qDebug() << "OutbondPacket::transformPolylinesfromRefObject()";
    std::string GroupElement;
    try{
        qDebug() << "ZlpThriftClient::SetReferenceobject";
        m_clientinterface->SetReferenceobject(m_referenceObject);
        qDebug() << "ZlpThriftClient::CreatePolylinesFromRefObject";
        //! only got group name of object
        m_clientinterface->CreatePolylinesFromRefObject(GroupElement, m_referenceObject.name, false);
        qDebug() << QString("Polyline name:%1").arg(QString::fromStdString(GroupElement));
        m_polyline_element.__set_activated(true);
        m_polyline_element.__set_name(GroupElement + ELEMENT_POLYLINE_NAME);
        //m_polyline.__set_name(polylinegroup);
        vector<string> selectedCoordinateSystems;
        selectedCoordinateSystems.push_back(m_projectorsn.toStdString() + "_FCW");
        qDebug() << "Zlaser::thrift::Polyline::Set_coordinateSystemList";
        m_polyline_element.__set_coordinateSystemList(selectedCoordinateSystems);
        qDebug() << "ZlpThriftClient::SetPolyLine";
        m_clientinterface->SetPolyLine(m_polyline_element);
        qDebug() << "ZlpThriftClient::TriggerProjection";
        m_clientinterface->TriggerProjection();
    }
    catch(ProcessInCriticalErrorState &e){
        qDebug() << QString::fromStdString(e.what());
    }
    catch(ServiceLockTimeout& e){
        qDebug() << QString::fromStdString(e.what());
    }
    catch(InvalidRelativePath &e){
        qDebug() << QString::fromStdString(e.what());
    }
    catch(ElementHasWrongType &e){
        qDebug() << QString::fromStdString(e.what());
    }
    catch(ElementDoesNotExist &e){
        qDebug() << QString::fromStdString(e.what());
    }
    catch (zlaser::thrift::PropertyNotFound& e)
    {
        qDebug() << QString::fromStdString(e.what());
    }
    catch (zlaser::thrift::PropertyChangeFailed& e)
    {
        qDebug() << QString::fromStdString(e.what());
    }
}

void OutbondPacket::changeprojectorstate(int state)
{
    qDebug() << "changeprojectorstate()";
    if(m_isactiv){
        qDebug() << m_module_id.c_str();
        string state_string = m_clientinterface->getFunctionModuleProperty(m_module_id, "state");
        qDebug() <<"state_string";
        FunctionModuleStates::type currentstate = (FunctionModuleStates::type)DeviceState->value(state_string.c_str());
        if(m_oldstate != currentstate)
        {
            m_clientinterface->FunctionModuleSetProperty(m_module_id, "state", QString("%0").arg(state, 0, 10).toStdString());
            //! need 5 second
            QTest::qSleep(5);
            state_string = m_clientinterface->getFunctionModuleProperty(m_module_id, "state");
            currentstate = (FunctionModuleStates::type)DeviceState->value(state_string.c_str());
            if(currentstate == state){
                DPRINT(0, QString("change projector state error."));
                qDebug() << QString("change projector state error. projector state:%1").arg(currentstate);
                return;
            }
            m_oldstate = currentstate;

        }
    }
}

void OutbondPacket::startcalibration(const QString &projectorsn)
{
    try{
        m_clientinterface->activateProjector(projectorsn.toStdString());
        //! register projector
        string moduleuid = m_clientinterface->createFunctionModule(MODULE_TYPE, MODULE_BRANCH);
        //! set property
        m_clientinterface->FunctionModuleSetProperty(moduleuid, PROPERTY_NAME, PROPERTY_VALUE);
        //! callback
        mutex functionModuleCallbackMutex;
        condition_variable stateChangedCondition;
        m_clientinterface->setFunctionModuleStateChangedCallback(
                    [&stateChangedCondition, &functionModuleCallbackMutex]
                    (const std::string& modID, FunctionModuleStates::type oldState, FunctionModuleStates::type newState)
        {
            (void)modID;
            (void)oldState;
            if (newState != FunctionModuleStates::type::RUNNING)
            {
                unique_lock<mutex> lock(functionModuleCallbackMutex);
                DPRINT(0,QString("Function module stopped running"));
                stateChangedCondition.notify_one();
            }
        }
        );

        m_clientinterface->FunctionModuleSetProperty(moduleuid, "runMode", "0");
        QString pointSearchPropPath = QString("config.projectorManager.projectors.%0.search.threshold").arg(projectorsn);
        //string pointSearchPropPath = CONFIG_PROJECTOR + projector + ".search.";
        m_clientinterface->SetProperty(pointSearchPropPath.toStdString(), "5");

        // run point search

        unique_lock<std::mutex> callbackLock(functionModuleCallbackMutex);
        DPRINT(1, QString("Start Calibration..."));

        m_clientinterface->FunctionModuleRun(moduleuid);

        auto status = stateChangedCondition.wait_for(callbackLock, chrono::minutes(5));
        callbackLock.unlock();

        if (status == cv_status::timeout){
            DPRINT(0, QString("Calibration timeout.."));
            return;
        } //throw runtime_error("Point search did not finish in time.\n");

        string state = m_clientinterface->getFunctionModuleProperty(moduleuid, "state");

        if (state != "1")  // "idle"
        {
            DPRINT(0, QString("An error occured. Function module is not in idle state."));
        } // throw runtime_error("An error occured. Function module is not in idle state.");

        DPRINT(1, QString("Calibration done."));
        //cout << "Finished point search\n";

        // Check if all points were found

        m_clientinterface->GetReferenceobject(m_referenceObject, "refObjectName");

        bool foundAll = true;
        for (size_t i = 0; i < m_referenceObject.refPointList.size(); i++)
        {
            string resultPath = "result.tracePoints." + to_string(i);

            if (m_clientinterface->getFunctionModuleProperty(moduleuid, resultPath + ".found") != "true")
            {
                foundAll = false;
                break;
            }
        }

        if (!foundAll)
            DPRINT(0, QString("Not all points have been found!\n"));
        else
            DPRINT(1, QString("All points have been found.\n"));

        // register projector to coordinate system

        m_clientinterface->FunctionModuleSetProperty(moduleuid, "runMode", "1");

        DPRINT(1, QString("Register projector to coordinate system..."));


        m_clientinterface->FunctionModuleRun(moduleuid);

        status = stateChangedCondition.wait_for(callbackLock, chrono::minutes(5));
        callbackLock.unlock();

        if (status == cv_status::timeout){
            DPRINT(0, QString("Could not calculate transformation in time."));
            return;
        }// throw runtime_error("Could not calculate transformation in time.\n");

        state = m_clientinterface->getFunctionModuleProperty(moduleuid, "state");

        if (state != "1")  // "idle"
        {
            DPRINT(0, QString("An error occured. Function module is not in idle state."));
            return;
        }//throw runtime_error("An error occured. Function module is not in idle state.");
        else
        {
            // get result
            string res;
            m_clientinterface->FunctionModuleGetProperty(res, moduleuid, "result.averageDistance");

            //Activate reference object only if the calculated transformation was succesfully
            m_clientinterface->GetReferenceobject(m_referenceObject, "refObjectName");
            m_referenceObject.activated = true;
            m_clientinterface->SetReferenceobject(m_referenceObject);

            //printf("Finished to register projector (aveDist: %s)\n", res.c_str());
            //printAvailableCoordinateSystems(m_clientinterface);

            DPRINT(1, QString("Show result of point search for 5 seconds\n"));;
            m_clientinterface->FunctionModuleSetProperty(moduleuid,"showAllRefPts","1");
            QTest::qSleep(5);
            //std::this_thread::sleep_for(std::chrono::seconds(5));
        }

        // clean-up

        m_clientinterface->RemoveGeoTreeElem("refObjectName");
        m_clientinterface->FunctionModuleRelease(moduleuid);
        m_clientinterface->deactivateProjector(projectorsn.toStdString());
        m_clientinterface->disconnect();
    }
    catch (ZlpThriftClient::Exception& e)
    {
        DPRINT(0, QString("Interface exception: %0").arg(e.what()));
    }
    catch (zlaser::thrift::CantReadFile& e)
    {
        DPRINT(0, QString("Cannot read file: %0").arg(e.what()));
    }
    catch (zlaser::thrift::ElementDoesNotExist& e)
    {
        DPRINT(0, QString("Element does not exist: %0").arg(e.what()));
    }
    catch (zlaser::thrift::PropertyNotFound& e)
    {
        DPRINT(0, QString("Property not found: %0").arg(e.what()));
    }
    catch (zlaser::thrift::PropertyChangeFailed& e)
    {
        DPRINT(0, QString("Property change failed: %0").arg(e.what()));
    }
    catch (zlaser::thrift::ClientNotRegisteredForFunctionModule& e)
    {
        DPRINT(0, QString("Client is not registered for function module: %0").arg (e.clientId.c_str()));
    }
    catch (zlaser::thrift::FunctionModuleIsAlreadyRunning& e)
    {
        DPRINT(0, QString("Function module is already running: %0").arg(e.fModUID.c_str()));
    }
    catch (zlaser::thrift::FunctionModuleNotExistent& e)
    {
        DPRINT(0, QString("Function module does not existent: %0").arg(e.fModUID.c_str()));
    }
    catch (zlaser::thrift::FunctionModuleCantRunFromThisState& e)
    {
        DPRINT(0, QString("Function module cannot run from this state: %0").arg (e.fModUID.c_str()));
    }
    catch (zlaser::thrift::FunctionModuleRunFailedCritical& e)
    {
        DPRINT(0, QString("Function module run failed critical: %0").arg (e.fModUID.c_str()));
    }
    catch (zlaser::thrift::FunctionModuleRunFailedRecoverable& e)
    {
        DPRINT(0, QString("Function module run failed recoverable: %0").arg (e.fModUID.c_str()));
    }
    catch (zlaser::thrift::FunctionModuleClassNotRegistered& e)
    {
        DPRINT(0, QString("Function module class not registered: %0").arg (e.which.c_str()));
    }
    catch (zlaser::thrift::FunctionModulePropertyBranchAlreadyInUse& e)
    {
        DPRINT(0, QString("Function module property branch already in use: %0").arg (e.branchName.c_str()));
    }
    catch (zlaser::thrift::FunctionModuleClassNotLicensed& e)
    {
        DPRINT(0, QString("Function module class not licensed: %0").arg (e.which.c_str()));
    }


}
//! old
void OutbondPacket::readyforupload(int mode)
{
    qDebug()<< "OutbondPacket::readyforupload()";

    m_projection_mode = (PROJECTION_MODE)mode;
    try{
        if(m_isactiv && mode == PROJECTION_MODE::TRACEPOINT){
            //! state tracepoint mode
            //! make sure the Referenceobject not null
            qDebug() << QString::fromStdString(m_referenceObject.name);
            qDebug() << "SetReferenceobject";
            m_clientinterface->SetReferenceobject(m_referenceObject);
            qDebug() << "FunctionModuleSetProperty";
            m_clientinterface->FunctionModuleSetProperty(m_module_id, "referenceData", m_referenceObject.name);
            qDebug() << "functionModuleCallbackMutex";
            mutex functionModuleCallbackMutex;
            condition_variable stateChangedCondition;
            m_clientinterface->setFunctionModuleStateChangedCallback(
                        [&stateChangedCondition, &functionModuleCallbackMutex]
                        (const std::string& modID, FunctionModuleStates::type oldState, FunctionModuleStates::type newState)
            {
                (void)modID;
                (void)oldState;
                if (newState != FunctionModuleStates::type::RUNNING)
                {
                    unique_lock<mutex> lock(functionModuleCallbackMutex);
                    qDebug() << "Function module stopped running" ;
                    stateChangedCondition.notify_one();
                }
            }
            );
            qDebug() << "FunctionModuleSetProperty";
            m_clientinterface->FunctionModuleSetProperty(m_module_id, "runMode", "1");
            qDebug() << "Register projector to coordinate system...";

            unique_lock<std::mutex> callbackLock(functionModuleCallbackMutex);

            qDebug() << "FunctionModuleRun";
            m_clientinterface->FunctionModuleRun(m_module_id);
            auto status = stateChangedCondition.wait_for(callbackLock, chrono::minutes(5));
            callbackLock.unlock();

            if (status == cv_status::timeout){
                DPRINT(0, QString("Could not calculate transformation in time.\n"));
                return;
                //throw runtime_error("Could not calculate transformation in time.\n");
            }

            qDebug() << "GetFunctionModuleProperty";
            string strstate = m_clientinterface->getFunctionModuleProperty(m_module_id, "state");

            qDebug() << QString::fromStdString(strstate);

            FunctionModuleStates::type fromdevice = DeviceState->value(QString::fromStdString(strstate));

            qDebug() << QString("state:%1").arg(fromdevice);

            if(fromdevice != FunctionModuleStates::type::IDLE){
                DPRINT(0, QString("An error occured. Function module is not in idle state."));
                return;
            }
            else
            {
                // get result
                qDebug() << "FunctionModuleGetProperty";
                string res;
                m_clientinterface->FunctionModuleGetProperty(res, m_module_id, "result.averageDistance");

                //Activate reference object only if the calculated transformation was succesfully
                m_clientinterface->GetReferenceobject(m_referenceObject, m_referenceObject.name);
                m_referenceObject.activated = true;
                m_clientinterface->SetReferenceobject(m_referenceObject);

                DPRINT(0, QString("Finished to register projector (aveDist: %0)").arg(QString::fromStdString(res)));

                //cout << "Show result of point search for 5 seconds\n";
                m_clientinterface->FunctionModuleSetProperty(m_module_id,"showAllRefPts","1");
                //std::this_thread::sleep_for(std::chrono::seconds(5));
            }

        }
        if(m_isactiv && mode == PROJECTION_MODE::OBJECT){
            transformPolylinesfromRefObject();
        }
    }
    catch (ZlpThriftClient::Exception& e)
    {
        DPRINT(0, QString("Interface exception: %1").arg(e.what()));
        qDebug() << QString("Interface exception: %1").arg(e.what());
    }
    catch (zlaser::thrift::CantReadFile& e)
    {
        DPRINT(0, QString("Interface exception: %1").arg(e.what()));
        qDebug() << QString("Interface exception: %1").arg(e.what());
    }
}

void OutbondPacket::Uploading(Projections *projections, int mode)
{
    qDebug()<< "OutbondPacket::Uploading()";
    string isShow = m_clientinterface->getFunctionModuleProperty(m_module_id, "showAllRefPts");

    if(CMDTOBOOL->value(QString::fromStdString(isShow))){
        //! stop referencepoints
        m_clientinterface->FunctionModuleSetProperty(m_module_id,"showAllRefPts","0");
        m_clientinterface->FunctionModuleStop(m_module_id);
    }

    string strstate = m_clientinterface->getFunctionModuleProperty(m_module_id, "state");

    m_projection_mode = (PROJECTION_MODE)mode;
    try{
        if(m_isactiv && mode == PROJECTION_MODE::OBJECT && DeviceState->value(QString::fromStdString(strstate)) != FunctionModuleStates::type::RUNNING){

            //this->removeReferenceObject(QString::fromStdString(m_referenceObject.coordinateSystem), QString::fromStdString(m_referenceObject.name));
            m_clientinterface->SetReferenceobject(m_referenceObject);
            for(int i =0; i < projections->count(); i++){
                if(projections->items().at(i)->type() == POLYLINE){
                    PacketPolyline *packet = (PacketPolyline*)projections->items().at(i);
                    //! set packet coordinate system
                    packet->setCoordinateSystem(m_projectorsn + "_FCW");
                    //! upload into client
                    m_clientinterface->SetPolyLine(packet->get());
                    m_referenceObject.__set_coordinateSystem(packet->get_group_name().replace("/","").toStdString());
                }
                if(projections->items().at(i)->type() == TEXT){
                    PacketText *packet = (PacketText*)projections->items().at(i);
                    packet->setCoordinateSystem(m_projectorsn + "_FCW");
                    m_clientinterface->SetTextElement(packet->get());
                }
            }
            qDebug() << "ZlpThriftClient::TriggerProjection";
            //! triger projector
            m_clientinterface->TriggerProjection();
        }

    }
    catch (ZlpThriftClient::Exception& e)
    {
        qDebug() << QString("Interface exception: %1").arg(e.what());
    }
    catch (CantReadFile& e)
    {
        qDebug() << QString("Interface exception: %1").arg(e.what());
    }
    catch(ProcessInCriticalErrorState &e){
        qDebug() << QString::fromStdString(e.what());
    }
    catch(ServiceLockTimeout& e){
        qDebug() << QString::fromStdString(e.what());
    }
    catch(InvalidRelativePath &e){
        qDebug() << QString::fromStdString(e.what());
    }
    catch(ElementHasWrongType &e){
        qDebug() << QString::fromStdString(e.what());
    }
    catch(ElementDoesNotExist &e){
        qDebug() << QString::fromStdString(e.what());
    }
    catch (PropertyNotFound& e)
    {
        qDebug() << QString::fromStdString(e.what());
    }
    catch (PropertyChangeFailed& e)
    {
        qDebug() << QString::fromStdString(e.what());
    }
    catch(zlaser::thrift::InvalidProjectionCoordinate& e){
        qDebug() << QString::fromStdString(e.what());
    }
    catch(ErrorInProjectorCommunication& e){
        qDebug() << QString::fromStdString(e.what());
    }
    catch(LimitedProjection& e){
        qDebug() << QString::fromStdString(e.what());
    }
}

void OutbondPacket::switchonprojector(const QString projectorsn, const QString IP, int port)
{
    qDebug() << "switchonprojector()";
    qDebug() << QString("switch on: %0").arg(projectorsn);
    try{
        m_clientinterface->connect(port, IP.toStdString());
        m_clientinterface->activateProjector(projectorsn.toStdString());
        m_module_id = m_clientinterface->createFunctionModule(MODULE_TYPE, MODULE_BRANCH);
        //!string state_string = m_clientinterface->getFunctionModuleProperty(m_module_id, "state");
        //!qDebug() << QString().fromStdString(state_string);
        //! projector state initialize
        this->getProjectorState();
        this->m_IP = IP;
        this->m_port = port;
        this->m_projectorsn = projectorsn;
        m_isactiv = true;
    }
    catch (ZlpThriftClient::Exception& e)
    {
        qDebug() << QString("Interface exception: %1").arg(e.what());
    }
    // need exception license not found

}

void OutbondPacket::switchoffprojector(const QString projectorsn)
{
    qDebug() << QString("switch off: %0").arg(projectorsn);
    try{
        // todo check element then should be flush
        //if(!m_referenceObject){
        //   m_clientinterface->RemoveGeoTreeElem(m_referenceObject.name);
        //   m_clientinterface->FunctionModuleRelease(m_module_id);
        //}
        m_clientinterface->FunctionModuleRelease(m_module_id);
        m_clientinterface->deactivateProjector(projectorsn.toStdString());
        m_clientinterface->disconnect();
        m_oldstate = FunctionModuleStates::type::IDLE;
        m_isactiv = false;
    }
    catch (ZlpThriftClient::Exception& e)
    {
        qDebug() << QString("Interface exception: %1").arg(e.what());

    }
}



