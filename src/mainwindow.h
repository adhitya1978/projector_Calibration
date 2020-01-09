#ifndef MAINWINDOW_H
#define MAINWINDOW_H
// ui
#include <QMainWindow>
#include <QStyleFactory>
#include <QPoint>
#include "log_widget.h"
#include "workspaceview.h"
#include "workspace_view.h"
#include "swith_button.h"
#include "renderarea.h"
#include "veselectionrect.h"
#include "toolbars.h"
#include "dialogcoordinatesystems.h"
#include "dialogfilename.h"
#include "dialogdevicemanager.h"
#include "dialogabout.h"

//bridge client
#include "outbondpacket.h"

// Coordinate systems
#include "pointindentifier.h"
#include "packetreferenceobject.h"
#include "packetreferencepoint.h"

// projection
#include "projections.h"
#include "packettext.h"
#include "packetpolyline.h"

// parameters
#include "parameters.h"
#include "parametercross.h"
#include "parameterdistance.h"
#include "parametertracepoint.h"

QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE


using namespace std;
using namespace zlaser::thrift;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    //! support point
    enum POINTS_OF_POLYGON{
        LANDSCAPE = 4,
        PORTRAIT = 4
    };

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow          *ui;
    //! widget logger
    log_widget              *Widget_Log;
    //! WorkPlace Scene
    RenderArea              *workPlaceScene;
    //! toolbar
    ToolBars                *m_toolbars;
    //! dialog coordinate
    DialogCoordinateSystems *dialogCordinateSystems;
    //! dialog file name
    DialogFileName          *dialogFileName;
    //! dialog device manager
    DialogDeviceManager     *dialogDeviceManager;
    //! dialog about
    Dialogabout             *dialogAbout;
    //! rectangle object
    VESelectionRect         *m_VESelectionRect;
    //! interface client projector
    ZlpThriftClient         *m_client;
    //! main packet reference poin which mean current active
    PacketReferencePoint    *m_PacketReferencePoint;
    //! main list point
    vector<Referencepoint>  currentpoints;
    //! packet reference object
    PacketReferenceObject   *m_PacketReferenceObject;
    //! Bridge projector
    OutbondPacket           *outboundPacket;

    //! process parameter change
    //! need indicator is distance adjustment or other
    bool                    adjustdistance = false;

    //! previous value after adjust
    double                  previousDistance;
    Point2D                 previousCross;
    Point2D                 previousTracepoint;

    VESelectionRect::TYPE_POLYGON m_Polygon;

    //! projector have been select
    int selected_index = -1;
    //! projector have been connected
    bool is_on;
    //! coordinate system name / reference object has been loaded
    bool is_new_coordinate;
    //! indicator for save coordinate system
    bool editMode;
    //! current Group active
    QString m_GroupTree;

    //! current active coordinate name
    QString m_coordname;
    //! current active Referensi object
    QString m_refObjectname;

    //! projections
    Projections *m_Projections;

    //! Parameters
    Parameters *m_Parameters;

    //! construct point Rectangle type landscape
    static PointIndentifier Portrait[LANDSCAPE];
    //! construct point Rectangle type portrait
    static PointIndentifier Landscape[PORTRAIT];

    //! terehold area
    static PointIndentifier TERESHOLD[PORTRAIT];

    //! construct default parameter
    void SetupParameter(PointIndentifier *pointindentifier, POINTS_OF_POLYGON pointsize);
    void UpdateParameter();

    //! main constructor
    void main_initialize();
    //! main initialize an events
    void init_event();
    //! animated switch on off projetor led ren / led green
    void sw_button_stylw(bool);
    //! control toolbars
    void mainControlButton();

    //! update cross size
    ParameterCross *UpdateCrossModel(double dx, double dy);
    //! update distance
    ParameterDistance *UpdateDistance(double curdist);
    //! update tracepoint model
    ParameterTracePoint *UpdateTracePointModel(double dx, double dy);

    //! process register new coordinate
    void RegisterNewCoordinateSystem(PointIndentifier *pointindentifier,
                                     VESelectionRect::TYPE_POLYGON _type,
                                     POINTS_OF_POLYGON pointsize);
    void Load(QString coordname, QString refobjname);

    //! reset to default
    void flushobject();

    //! save as coordinate
    void save();

signals:
    //! set projector state
    void changeprojectorstate(int state);
    //! projector start calibration
    void startcalibration(const QString &projectorsn);
    //! update referenceobject on projector
    void readyforupload(int mode);
    //! Transmit into client
    void SignalUploading(Projections *projection, int mode);
    //! switch on projector
    void switchonprojector(const QString projectorsn, const QString IP, int port);
    //! switch off projector
    void switchoffprojector(const QString projectorsn);
    //! transform to polyline (obselete)
    void signaltransformPolylinesfromRefObject();

private slots:
    void on_pushButton_connect_clicked();
    void on_pushButton_scan_clicked();
    void on_comboBox_currentIndexChanged(int index);
    void portrait_button_clicked();
    void landscape_button_clicked();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void on_actionExit_triggered();
    void on_actionHelp_triggered();
    void on_actionAbout_triggered();

public slots:
    /// function triger for tool bar button
    void actionOnToolbarSlot(int);
    //! set new name coordinate system
    void slotCreateCoordinate(QString, QString);
    void SlotLabelBottomLeft(QString, QPoint);
    void SlotLabelTopLeft(QString name, QPoint position);
    void SlotLabelBottomRight(QString name, QPoint position);
    void SlotLabelTopRight(QString name, QPoint position);
    void SlotUpdatePoint(PointIndentifier *PreviousPointIndentifier);

};

#endif // MAINWINDOW_H
