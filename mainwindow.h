
#ifndef mainwindow_h
#define mainwindow_h

#include <QMainWindow>
#include <QMutex>
#include <QThread>

// libwiimote
extern "C"
{
#include <libcwiimote/wiimote.h>
}

// libnxtbt
#include <motor.h>
#include <sensor.h>

// Forward declarations
namespace nxt
{
    class Bluetooth;
}

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QComboBox;
class QLabel;
class QMenu;
QT_END_NAMESPACE


// Main window
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void ConnectToNxt();
    void ConnectToWiimote();
    void DisconnectNxt();
    void DisconnectWiimote();
    void ShowSetupDialog();
    void NunchukClicked();
    void TiltClicked();
    void About();

private:
    class WiiThread : public QThread
    {
    public:
        enum Mode
        {
            Nunchuk,
            Tilt
        };
        
        WiiThread(MainWindow* parent);

        void SetMode(Mode mode);

        void SetInfoLabel(QLabel* label);
        
        wiimote_t m_wiimoteInfo;
        
    private:
        virtual void run();

        MainWindow* m_parent;
        Mode m_mode;
        QLabel* m_infoLabel;
        QMutex m_mutex;
    };

    static const int NOF_SENSOR_PORTS = 4;
    static const int NOF_MOTOR_PORTS = 3;
    
    struct PortSetup
    {
        PortSetup();
        
        nxt::Sensor_type sensors[NOF_SENSOR_PORTS];
        nxt::Motor_port  motors[NOF_MOTOR_PORTS];
        bool             reverse[NOF_MOTOR_PORTS];
        bool             coast[NOF_MOTOR_PORTS];
    };
        
    void CreateActions();
    void CreateMenus();
    void HideControls();
    void ShowWiimoteControls();
    void ShowMouseControls();
    void SetSensorCombo(nxt::Sensor_type type, QComboBox* cb);
    void SetMotorCombo(nxt::Motor_port port, QComboBox* cb);
    static QString GetAsString(nxt::Sensor_type type);
    
    QMenu* m_deviceMenu;
    QMenu* m_setupMenu;
    QMenu* m_helpMenu;
    QAction* m_connectNxtAct;
    QAction* m_connectWiimoteAct;
    QAction* m_disconnectNxtAct;
    QAction* m_disconnectWiimoteAct;
    QAction* m_setupAct;
    QAction* m_aboutAct;
    WiiThread m_wiiThread;
    bool m_connectedToWii;
    nxt::Bluetooth* m_nxtConnection;
    QWidget* m_topWidget;
    PortSetup m_portSetup;
};

#endif
