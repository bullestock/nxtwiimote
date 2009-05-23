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

#include <QMainWindow>
#include <QMutex>
#include <QThread>

// libwiimote
extern "C"
{
#include <libcwiimote/wiimote.h>
}

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

class PortSetup;


// Main window
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    PortSetup GetSetup() const;

    boost::shared_ptr<nxt::Bluetooth> GetNxtConnection();

public slots:
    void Quit();

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

    void CreateActions();
    void CreateMenus();
    void HideControls();
    void ShowWiimoteControls();
    void ShowMouseControls();
    
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
    QMutex m_nxtConnectionMutex;
    QWidget* m_topWidget;
    PortSetup* m_portSetup;
    mutable QMutex m_portSetupMutex;
};

#endif
