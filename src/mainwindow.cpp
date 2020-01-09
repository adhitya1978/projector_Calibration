#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "packetreferencepoint.h"
#include "toolbars.h"
#include "labelcorner.h"

static QPushButton *portrait_button  = nullptr;
static QPushButton *landscape_button = nullptr;

//! construct point Portrait
PointIndentifier MainWindow::Portrait[MainWindow::PORTRAIT] = {
    PointIndentifier("T1", ElementUtils::CreatePointXYZ(0, 0, 0)),
    PointIndentifier("T2", ElementUtils::CreatePointXYZ(0,  297, 0)),
    PointIndentifier("T3", ElementUtils::CreatePointXYZ( 210,  297, 0)),
    PointIndentifier("T4", ElementUtils::CreatePointXYZ( 210, 0, 0))
};
//! construct point Landscape
PointIndentifier MainWindow::Landscape[MainWindow::LANDSCAPE] = {
    PointIndentifier("T1", ElementUtils::CreatePointXYZ(0, 0, 0)),
    PointIndentifier("T2", ElementUtils::CreatePointXYZ(0,  210, 0)),
    PointIndentifier("T3", ElementUtils::CreatePointXYZ( 297,  210, 0)),
    PointIndentifier("T4", ElementUtils::CreatePointXYZ( 297, 0, 0))
};

PointIndentifier MainWindow::TERESHOLD[MainWindow::LANDSCAPE] = {
    PointIndentifier("T1", ElementUtils::CreatePointXYZ(0, 0, 0)),
    PointIndentifier("T2", ElementUtils::CreatePointXYZ(0,  3000, 0)),
    PointIndentifier("T3", ElementUtils::CreatePointXYZ( 3000,  3000, 0)),
    PointIndentifier("T4", ElementUtils::CreatePointXYZ( 3000, 0, 0))
};



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_client(ZlpThriftClient::create()),
    is_new_coordinate(false),
    is_on(false),
    m_Projections(new Projections),
    m_Parameters(new Parameters),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    main_initialize();
    init_event();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::RegisterNewCoordinateSystem(PointIndentifier *pointindentifier, VESelectionRect::TYPE_POLYGON poly_type, POINTS_OF_POLYGON pointsize)
{
    this->editMode = true;

    mainControlButton();

    qDebug() << "RegisterNewCoordinateSystem()";

    if(poly_type != VESelectionRect::TYPE_POLYGON::UNKNOWN){
        workPlaceScene->removeItem(m_VESelectionRect);
        foreach(QGraphicsItem * item, workPlaceScene->items()){
            if(item->type() == QGraphicsTextItem().type())
                workPlaceScene->removeItem(item);
        }
        m_VESelectionRect->setType(poly_type);
        workPlaceScene->addItem(m_VESelectionRect);
        m_VESelectionRect->setPos(0,0);
    }

    workPlaceScene->setPreviousPointIndentifier(pointindentifier);

    //! create gourp tree at service
    m_GroupTree = outboundPacket->createGroupTreePolyline(m_refObjectname);

    //! [0]
    //! update point
    m_Projections->clear();
    m_Projections->add(new PacketPolyline(pointindentifier, m_GroupTree, "Polyline" , this));
    m_Projections->add(new PacketText(pointindentifier[0].m_name, ElementUtils::CreatePointXYZ(pointindentifier[0].m_point.x, pointindentifier[0].m_point.y, 0.0), m_GroupTree, "Text1", this));
    m_Projections->add(new PacketText(pointindentifier[1].m_name, ElementUtils::CreatePointXYZ(pointindentifier[1].m_point.x, pointindentifier[1].m_point.y, 0.0), m_GroupTree, "Text2", this));
    m_Projections->add(new PacketText(pointindentifier[2].m_name, ElementUtils::CreatePointXYZ(pointindentifier[2].m_point.x, pointindentifier[2].m_point.y, 0.0), m_GroupTree, "Text3", this));
    m_Projections->add(new PacketText(pointindentifier[3].m_name, ElementUtils::CreatePointXYZ(pointindentifier[3].m_point.x, pointindentifier[3].m_point.y, 0.0), m_GroupTree, "Text4", this));

    SetupParameter(pointindentifier, pointsize);

    //! for new scene we need to initialize editing mode
    m_PacketReferencePoint = new PacketReferencePoint(m_Parameters, this);

    //! need it to construct PacketReferenceobject with default object
    vector<Referencepoint> refPointList = m_PacketReferencePoint->get();


    //! [3] set false if not commited
    m_PacketReferenceObject = new PacketReferenceObject(refPointList,
                                                        ui->comboBox->itemText(selected_index),
                                                        m_coordname, false);
    //! [4]
    //! set new name for Referenceobject
    m_PacketReferenceObject->setname(m_refObjectname);


    //! we have done create it one with default FCW
    Referenceobject refObject = m_PacketReferenceObject->getPacketReferenceobject();

    //! [5] upload to projector
    outboundPacket->setReferenceObject(refObject);

    //! [6] set it to object mode
    OutbondPacket::PROJECTION_MODE mode = OutbondPacket::PROJECTION_MODE::OBJECT;
    //! [9]
    //! set IP address & Service Port
    outboundPacket->setAddress(ui->textEdit_ip->toPlainText(), ui->textEdit_port->toPlainText().toInt());

    if(mode == OutbondPacket::PROJECTION_MODE::OBJECT)
    {
        //! [7]
        zlaser::thrift::Polyline polyline = m_PacketReferenceObject->ConstructPolyLinesFromObject();
        //! [8]
        outboundPacket->setElementPolyline(polyline);
    }
    //emit readyforupload(mode);
    emit SignalUploading(m_Projections, mode);

}

void MainWindow::Load(QString coordname, QString refobjname)
{
    Referenceobject refobject = outboundPacket->getCoordSys(coordname, refobjname);
    outboundPacket->setReferenceObject(refobject);
    OutbondPacket::PROJECTION_MODE mode = OutbondPacket::PROJECTION_MODE::OBJECT;
    if(mode == OutbondPacket::PROJECTION_MODE::TRACEPOINT){
        //outboundPacket->setParameterTracePoint();
    }
    outboundPacket->setAddress(ui->textEdit_ip->toPlainText(), ui->textEdit_port->toPlainText().toInt());

    emit readyforupload(mode);

}

//! default or template
void MainWindow::SetupParameter(PointIndentifier *pointindentifier, POINTS_OF_POLYGON pointsize)
{
    qDebug() << "SetupParameter()";
    // setup parameter for 4 point
    QList<SCANNING_POINT*> setupParameters;
    for(int i = 0; i < pointsize; i++){
        struct SCANNING_POINT *parameter = new SCANNING_POINT;
        parameter->name         = pointindentifier[i].m_name;
        parameter->refpoint     = ElementUtils::CreatePointXYZ(TERESHOLD[i].m_point.x, TERESHOLD[i].m_point.y, 0.0);
        parameter->distance     = 3533.4;
        parameter->activated    = true;
        parameter->tracepoint   = ElementUtils::CreatePointXY(pointindentifier[i].m_point.x, pointindentifier[i].m_point.y);
        setupParameters.append(parameter);

    }
    //! init previous value
    previousDistance    = setupParameters.at(0)->distance;
    previousCross       = setupParameters.at(0)->crosssize;
    previousTracepoint  = setupParameters.at(0)->tracepoint;

    if(is_new_coordinate){
        m_Parameters->add(new ParameterDistance(setupParameters, this));
        m_Parameters->add(new ParameterCross(setupParameters, this));
        m_Parameters->add(new ParameterTracePoint(setupParameters, this));
    }
}

//! update while parameter change
void MainWindow::UpdateParameter()
{
    m_PacketReferencePoint = new PacketReferencePoint(m_Parameters, this);
    //! needit to construct PacketReferenceobject with default object
    vector<Referencepoint> refPointList = m_PacketReferencePoint->get();
    //! [3] set false if not commited
    m_PacketReferenceObject = new PacketReferenceObject(refPointList,
                                                        ui->comboBox->itemText(selected_index),
                                                        m_coordname, false);
    //! [4]
    //! set new name for Referenceobject
    m_PacketReferenceObject->setname(m_refObjectname);
    //! we have done create it one with default FCW
    Referenceobject refObject = m_PacketReferenceObject->getPacketReferenceobject();
    //! [5] upload to projector
    outboundPacket->setReferenceObject(refObject);
    //! [6] set it to object mode
    OutbondPacket::PROJECTION_MODE mode = OutbondPacket::PROJECTION_MODE::OBJECT;
    if(mode == OutbondPacket::PROJECTION_MODE::OBJECT)
    {
        //! [7]
        zlaser::thrift::Polyline polyline = m_PacketReferenceObject->ConstructPolyLinesFromObject();
        //! [8]
        outboundPacket->setElementPolyline(polyline);

    }
    if(mode == OutbondPacket::PROJECTION_MODE::TRACEPOINT){
        // outboundPacket->setParameterTracePoint();
    }
    //! [9]
    //! set IP address & Service Port
    outboundPacket->setAddress(ui->textEdit_ip->toPlainText(), ui->textEdit_port->toPlainText().toInt());
    //! [10] emited
    emit SignalUploading(m_Projections, mode);
}

void MainWindow::flushobject()
{
    adjustdistance = false;
}

void MainWindow::main_initialize()
{
    ui->textEdit_ip->setText("localhost");
    ui->textEdit_port->setText("9090");

    //! button rectangle
    portrait_button = new swith_button("Portrait", ui->centralwidget);
    landscape_button = new swith_button("Landscape", ui->centralwidget);

    //! scene
    //! ================================================

    workPlaceScene = new RenderArea(this);
    workPlaceScene->setSceneRect(-1500, -1500, 3000, 3000);
    ui->workspaceView->setScene(workPlaceScene);
    ui->workspaceView->setRenderHint(QPainter::Antialiasing);
    ui->workspaceView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->workspaceView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->workspaceView->setCursor(QCursor());
    workPlaceScene->addLine(-175,0,175,0,QPen(Qt::darkBlue));   // Add horizontal line via center
    workPlaceScene->addLine(0,-175,0,175,QPen(Qt::darkBlue));   // Add vertical line via center
    //!===========================================
    //! scene end
    m_VESelectionRect = new VESelectionRect();

    //! layout log
    QHBoxLayout *layoutLog = new QHBoxLayout();
    //! layout tools
    QVBoxLayout *layoutTools = new QVBoxLayout();

    //! Logging widget
    this->Widget_Log = new log_widget(ui->centralwidget);
    this->Widget_Log->create_log_widget(ui->groupBox_LOG, layoutLog);
    //! Logging

    //! tools button
    layoutTools->addWidget(portrait_button);
    layoutTools->addWidget(landscape_button);

    ui->groupBox_LOG->setLayout(layoutLog);
    ui->groupBox_Tools->setLayout(layoutTools);

    //! button style for switch projector
    this->sw_button_stylw(false);
    // bridge projector
    outboundPacket = new OutbondPacket(m_client);
    // dialog Filename
    dialogFileName = new DialogFileName(this);

    //toolbar
    m_toolbars = new ToolBars(this);
    m_toolbars->setObjectName(QStringLiteral("toolBars"));
    this->addToolBar(Qt::TopToolBarArea, m_toolbars);
    ui->groupBox_View->setTitle("no active coordinate system!");
    mainControlButton();
    dialogAbout = new Dialogabout(this);

}

void MainWindow::init_event()
{
    connect(portrait_button, SIGNAL(clicked()), this, SLOT(portrait_button_clicked()));
    connect(landscape_button, SIGNAL(clicked()), this, SLOT(landscape_button_clicked()));
    //! set projector state
    //! projector start calibration
    //! update referenceobject on projector
    //! switch on projector
    //! switch off projector
    connect(this, SIGNAL(changeprojectorstate(int)),    outboundPacket, SLOT(changeprojectorstate(int)));
    connect(this, SIGNAL(startcalibration(QString)),    outboundPacket, SLOT(startcalibration(QString)));
    connect(this, SIGNAL(readyforupload(int)),          outboundPacket, SLOT(readyforupload(int)));
    connect(this, SIGNAL(switchoffprojector(QString)),  outboundPacket, SLOT(switchoffprojector(QString)));
    //connect(this, SIGNAL(signaltransformPolylinesfromRefObject()), outboundPacket, SLOT(transformPolylinesfromRefObject()));
    connect(this, SIGNAL(switchonprojector(QString, QString, int)),  outboundPacket, SLOT(switchonprojector(QString, QString, int)));
    // toolbars connection
    connect(m_toolbars, SIGNAL(action(int)), this, SLOT(actionOnToolbarSlot(int)));
    connect(m_VESelectionRect, SIGNAL(SignalLabelBottomLeft(QString, QPoint)), this, SLOT(SlotLabelBottomLeft(QString, QPoint)));
    connect(m_VESelectionRect, SIGNAL(SignalLabelTopLeft(QString, QPoint)), this, SLOT(SlotLabelTopLeft(QString, QPoint)));
    connect(m_VESelectionRect, SIGNAL(SignalLabelTopRight(QString, QPoint)), this, SLOT(SlotLabelTopRight(QString, QPoint)));
    connect(m_VESelectionRect, SIGNAL(SignalLabelBottomRight(QString, QPoint)), this, SLOT(SlotLabelBottomRight(QString, QPoint)));
    connect(this, SIGNAL(SignalUploading(Projections*,int)), outboundPacket, SLOT(Uploading(Projections*,int)));
    //! update polyline
    connect(workPlaceScene, SIGNAL(signalUpdatePointIndentifier(PointIndentifier*)), this, SLOT(SlotUpdatePoint(PointIndentifier*)));

}

void MainWindow::sw_button_stylw(bool sw)
{
    if(!sw){
        ui->pushButton_connect->setText("OFF");
        ui->pushButton_connect->setStyleSheet("QPushButton { background-color: red }");
    }
    else{
        ui->pushButton_connect->setText("ON");
        ui->pushButton_connect->setStyleSheet("QPushButton { background-color: green }");
    }
}

void MainWindow::mainControlButton()
{
    if(this->is_on){
        m_toolbars->enable(ToolBars::NEW, true);
        m_toolbars->enable(ToolBars::DEVICEMANAGER, true);
        m_toolbars->disable(ToolBars::SAVE, true);
        m_toolbars->disable(ToolBars::TRACE_OBJECT, true);
        m_toolbars->disable(ToolBars::ZOOMIN, true);
        m_toolbars->disable(ToolBars::ZOOMOUT, true);
    }
    else{
        m_toolbars->disable(ToolBars::NEW, true);
        m_toolbars->disable(ToolBars::DEVICEMANAGER, true);
        m_toolbars->disable(ToolBars::SAVE, true);

    }
    if(this->is_new_coordinate && is_on){
        portrait_button->setEnabled(true);
        landscape_button->setEnabled(true);
        m_toolbars->enable(ToolBars::SAVE, true);
        m_toolbars->enable(ToolBars::TRACE_OBJECT, true);
        m_toolbars->enable(ToolBars::ZOOMIN, true);
        m_toolbars->enable(ToolBars::ZOOMOUT, true);
    }else{
        portrait_button->setEnabled(false);
        landscape_button->setEnabled(false);
        m_toolbars->disable(ToolBars::TRACE_OBJECT, true);
        m_toolbars->disable(ToolBars::ZOOMIN, true);
        m_toolbars->disable(ToolBars::ZOOMOUT, true);
    }
    if(this->editMode && this->is_new_coordinate && this->is_on){
        m_toolbars->enable(ToolBars::SAVE, true);
    }
}

ParameterCross * MainWindow::UpdateCrossModel(double dx, double dy)
{
    Q_FOREACH(Parameter *parameter, m_Parameters->items()){
        if(parameter->type() == CROSS_MODEL){
            ParameterCross *_curr = (ParameterCross *)parameter;
            _curr->setCross(dx, dy);
            previousCross = _curr->get()[0]->crosssize;
            return _curr;
        }
    }
}

ParameterDistance *MainWindow::UpdateDistance(double curdist)
{
    qDebug() << "MainWindow::UpdateDistance()";
    Q_FOREACH(Parameter *parameter, m_Parameters->items()){
        qDebug() << QString("Parameters count: %1").arg(m_Parameters->items().count());
        if(parameter->type() == DISTANCE){
            ParameterDistance *current = (ParameterDistance*)parameter;
            current->set(curdist);
            qDebug() << QString("Distance count: %1").arg(current->get().count());
            qDebug() << QString("Distance name: %1").arg(current->get().at(0)->name);
            previousDistance = current->get().at(0)->distance;
            return current;
        }
    }
}

ParameterTracePoint * MainWindow::UpdateTracePointModel(double dx, double dy)
{
    Q_FOREACH(Parameter *parameter, m_Parameters->items()){
        if(parameter->type() == POINT_MODEL){
            ParameterTracePoint *current = (ParameterTracePoint *)parameter;
            current->setTracePoint(dx, dy);
            previousTracepoint = current->get()[0]->tracepoint;
            return current;
        }
    }
}

void MainWindow::save()
{
    //! save & register coodinate system
    outboundPacket->RegisterCoordinateSystem(this->m_refObjectname);
    this->editMode = false;
    this->mainControlButton();
}

void MainWindow::on_pushButton_connect_clicked()
{
    if(QString(ui->comboBox->itemText(selected_index)).isEmpty() || QString(ui->comboBox->itemText(selected_index)).isNull()){
        return;
    }

    if(!outboundPacket->getIsactiv()){
        emit switchonprojector(ui->comboBox->itemText(selected_index), ui->textEdit_ip->toPlainText(), ui->textEdit_port->toPlainText().toInt());
        sw_button_stylw(true);
        this->is_on =true;
    }
    else{
        emit switchoffprojector(ui->comboBox->itemText(selected_index));
        sw_button_stylw(false);
        this->is_on = false;
    }
    mainControlButton();
}

void MainWindow::on_pushButton_scan_clicked()
{
    if(QString(ui->textEdit_ip->toPlainText()).isEmpty() || QString(ui->textEdit_ip->toPlainText()).isNull())
    {
        DPRINT(0, QString("fill up the IP"));
        return;
    }
    if(QString(ui->textEdit_port->toPlainText()).isEmpty() || QString(ui->textEdit_port->toPlainText()).isNull())
    {
        DPRINT(0, QString("fill up port number"));
        return;
    }

    QString _localhost = "";
    if(ui->textEdit_ip->toPlainText().toUpper() == "LOCALHOST")
        _localhost = "127.0.0.1";
    else
        _localhost = ui->textEdit_ip->toPlainText().trimmed();

    qDebug() << ui->textEdit_ip->toPlainText();

    outboundPacket->setAddress(_localhost, ui->textEdit_port->toPlainText().toInt() );

    QList<std::string> *list = outboundPacket->printAvailableProjector();

    QStringList lizt;
    ui->comboBox->clear();

    DPRINT(2, QString("availble projector"));
    DPRINT(2, QString("==================="));

    QList<std::string>::const_iterator i;
    for(i = list->begin(); i != list->end(); i++){
        DPRINT(2, QString("Id:%0").arg(QString::fromStdString(*i)));
        lizt << QString::fromStdString(*i);

    }
    ui->comboBox->insertItems(list->count(), lizt);
    ui->comboBox->itemText(0);

}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    this->selected_index = index;
}

void MainWindow::portrait_button_clicked()
{
    RegisterNewCoordinateSystem(Portrait, VESelectionRect::TYPE_POLYGON::PORTRAIT, POINTS_OF_POLYGON::PORTRAIT);
}

void MainWindow::landscape_button_clicked()
{
    RegisterNewCoordinateSystem(Landscape, VESelectionRect::TYPE_POLYGON::LANDSCAPE, POINTS_OF_POLYGON::LANDSCAPE);
}

void MainWindow::on_actionSave_triggered()
{
    // todo auto save coordinate
}

void MainWindow::on_actionSaveAs_triggered()
{
    // todo save as name coordinate
}

void MainWindow::on_actionExit_triggered()
{
    // todo exit
    this->close();
}

void MainWindow::on_actionHelp_triggered()
{
    // todo help menu action
}

void MainWindow::on_actionAbout_triggered()
{
    // todo action about
}

void MainWindow::actionOnToolbarSlot(int action)
{
    switch(action){
    case ToolBars::NEW:
    {
        dialogCordinateSystems = new DialogCoordinateSystems(outboundPacket->getAllNameReferenceObjectAndCoordSystem(), this);
        //! coonection between dialogfilename, dialogcoordinatesystem and outbond class
        //! control dialog
        connect(dialogFileName, SIGNAL(set_new_name_cordinatesystem(QString, QString)), dialogCordinateSystems, SLOT(set_new_name_cordinatesystem(QString, QString)) );
        //! load or edit coordinate system
        //connect(dialogCordinateSystems, SIGNAL(signalLoadReferenceObject(QString, QString)), outboundPacket, SLOT(LoadReferenceObject(QString, QString)));
        //! remove coordinate system
        connect(dialogCordinateSystems, SIGNAL(signalRemoveReferenceObject(QString, QString)), outboundPacket, SLOT(removeReferenceObject(QString,QString)));
        //! set current coordinate system new Scene
        QObject::connect(dialogCordinateSystems, SIGNAL(signalLoadReferenceObject(QString,QString)), this, SLOT(slotCreateCoordinate(QString, QString)) );
        dialogCordinateSystems->exec();
        break;
    }
        //! register new coordinate systems
    case ToolBars::SAVE:
        this->save();
        break;
        //! manage projector list
    case ToolBars::DEVICEMANAGER:
        dialogDeviceManager = new DialogDeviceManager(this->m_client, ui->comboBox->itemText(selected_index), this);
        dialogDeviceManager->exec();
        break;
        //! setup distance in
    case ToolBars::ZOOMIN:{
        double d = previousDistance + 500;
        adjustdistance = true;
        ParameterDistance *p = UpdateDistance(d);
        m_Parameters->remove(m_Parameters->get(p));
        m_Parameters->add(p);
        UpdateParameter();}
        break;
        //! setup distance out
    case ToolBars::ZOOMOUT:{
        double d = previousDistance - 500;
        adjustdistance = true;
        ParameterDistance *p = UpdateDistance(d);
        m_Parameters->remove(m_Parameters->get(p));
        m_Parameters->add(p);
        UpdateParameter();}
        break;
        //! change object
    case ToolBars::TRACE_OBJECT: break;
    case ToolBars::HELP: break;
    case ToolBars::ABOUT:
        dialogAbout->exec();
        break;
    default: break;
    }
}
//! creating,select and load coordinate system
//! emit from dialog coordinate system
void MainWindow::slotCreateCoordinate(QString coordname, QString refobjname)
{
    this->m_coordname = coordname;
    this->m_refObjectname = refobjname;
    this->is_new_coordinate = true; //outboundPacket->getCoordSys(coordname, refobjname).name.length() > 0;

    // check coordinatesystems in service
    //! we need to check coordinate system creating new or load from existing
    //! if create new one we should construct new object e.g Landscape
    if(is_new_coordinate)
        RegisterNewCoordinateSystem(Landscape,
                                    VESelectionRect::TYPE_POLYGON::LANDSCAPE,
                                    POINTS_OF_POLYGON::LANDSCAPE);
    else
        Load(coordname, refobjname);
    mainControlButton();
    ui->groupBox_View->setTitle(coordname);
}

//! region label point view
void MainWindow::SlotLabelBottomLeft(QString name, QPoint position)
{
    LabelCorner *point = new LabelCorner(name);
    workPlaceScene->addItem(point);
    point->setPos(position.rx(), position.ry());
    connect(point, SIGNAL(SignalSetLabelType(int,bool)), workPlaceScene, SLOT(SlotSetLabelType(int,bool)));
}

void MainWindow::SlotLabelTopLeft(QString name,QPoint position)
{
    LabelCorner *point = new LabelCorner(name);
    workPlaceScene->addItem(point);
    point->setPos(position.rx(), position.ry());
    connect(point, SIGNAL(SignalSetLabelType(int,bool)), workPlaceScene, SLOT(SlotSetLabelType(int,bool)));
}

void MainWindow::SlotLabelBottomRight(QString name,QPoint position)
{
    LabelCorner *point = new LabelCorner(name);
    workPlaceScene->addItem(point);
    point->setPos(position.rx(), position.ry());
    connect(point, SIGNAL(SignalSetLabelType(int,bool)), workPlaceScene, SLOT(SlotSetLabelType(int,bool)));
}

void MainWindow::SlotLabelTopRight(QString name,QPoint position)
{
    LabelCorner *point = new LabelCorner(name);
    workPlaceScene->addItem(point);
    point->setPos(position.rx(), position.ry());
    connect(point, SIGNAL(SignalSetLabelType(int,bool)), workPlaceScene, SLOT(SlotSetLabelType(int,bool)));
}

void MainWindow::SlotUpdatePoint(PointIndentifier *PreviousPointIndentifier)
{
    QTest::qSleep(250);
    RegisterNewCoordinateSystem(PreviousPointIndentifier, VESelectionRect::TYPE_POLYGON::UNKNOWN, POINTS_OF_POLYGON::PORTRAIT);
}


