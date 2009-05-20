
#ifndef mainwindow_h
#define mainwindow_h

#include <QMainWindow>
#include <QMutex>
#include <QThread>

extern "C"
{
#include <libcwiimote/wiimote.h>
}

namespace nxt
{
    class Bluetooth;
}

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
QT_END_NAMESPACE


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
    
    void CreateActions();
    void CreateMenus();
    void HideControls();
    void ShowWiimoteControls();
    void ShowMouseControls();

    QMenu* m_deviceMenu;
    QMenu* m_helpMenu;
    QAction* m_connectNxtAct;
    QAction* m_connectWiimoteAct;
    QAction* m_disconnectNxtAct;
    QAction* m_disconnectWiimoteAct;
    QAction* m_aboutAct;
    WiiThread m_wiiThread;
    bool m_connectedToWii;
    nxt::Bluetooth* m_nxtConnection;
    QWidget* m_topWidget;
};

#endif
