
#ifndef mainwindow_h
#define mainwindow_h

#include <QMainWindow>
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
    void About();

private:
    class WiiThread : public QThread
    {
    public:
        WiiThread(MainWindow* parent);
        
        wiimote_t m_wiimoteInfo;
    private:
        MainWindow* m_parent;
        
        virtual void run();
    };
    
    void CreateActions();
    void CreateMenus();

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
};

#endif
