// Copyright 2009 Torsten Martinsen <torsten@bullestock.net>

// This file is part of nxtwiimote.

// Nxtwiimote is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// Nxtwiimote is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with nxtwiimote.  If not, see <http://www.gnu.org/licenses/>.


#ifndef mainwindow_h
#define mainwindow_h

#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <QMainWindow>
#include <QMutex>
#include <QThread>
#include <QTime>

// libwiimote
extern "C"
{
#include <libcwiimote/wiimote.h>
}

#include <sensor.h>

// Forward declarations
namespace nxt
{
    class Bluetooth;
}

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QBoxLayout;
class QComboBox;
class QGridLayout;
class QProgressBar;
class QLabel;
class QMenu;
class QSlider;
QT_END_NAMESPACE

class PortSetup;


// Main window
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int argc, char *argv[]);

    void Show();
    
    PortSetup GetSetup() const;

    QSlider* GetActionSlider() { return m_actionSlider; }

    void SetMotors(int motors[3], QString info);
                                 
public slots:
    void Quit();

private slots:
    void ConnectToNxt();
    void ConnectToWiimote();
    void ConnectToSelectedNxt();
    void ConnectToSelectedWiimote();
    void DisconnectNxt();
    void DisconnectWiimote();
    void ShowSetupDialog();
    void NunchukClicked();
    void TiltClicked();
    void ActionSliderMoved(int);
    void About();
    void UpdateNxt();
    void ButtonNWClicked();    
    void ButtonNClicked();    
    void ButtonNEClicked();    
    void ButtonWClicked();    
    void ButtonEClicked();    
    void ButtonSWClicked();    
    void ButtonSClicked();    
    void ButtonSEClicked();    

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

        wiimote_t m_wiimoteInfo;
        
    private:
        virtual void run();

        MainWindow* m_parent;
        Mode m_mode;
        QMutex m_mutex;
    };

    void CreateActions();
    void CreateMenus();
    void HideControls();
    void ShowWiimoteControls();
    void ShowMouseControls();
    QWidget* CreateStandardControls(bool wiimote);
    QBoxLayout* CreateSensorControls();
    
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
    boost::shared_ptr<nxt::Bluetooth> m_nxtConnection;

    int m_motors[3];
    int m_motorsOld[3];
    QString m_motorInfo;
    QMutex m_motorsMutex;
    QWidget* m_topWidget;
    QSlider* m_actionSlider;
    QLabel* m_actionValueLabel;
    QProgressBar* m_batteryLevel;
    QList<QLabel*> m_sensorLabels;
    QGridLayout* m_sensorsLayout;
    PortSetup* m_portSetup;
    boost::ptr_vector<nxt::Sensor> m_sensors;
    mutable QMutex m_portSetupMutex;
    QString m_wiimoteAddress;
    QString m_nxtAddress;
    QTime m_lastBatteryUpdate;
};

#endif
