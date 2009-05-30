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

#include <sstream>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <QMutexLocker>
#include <QSettings>
#include <QtDebug>
#include <QtGui>

extern "C" {
#include <libcwiimote/wiimote_api.h>
}

#include <bluetooth.h>
#include <brick.h>
#include <error.h>

#include "connectdialog.h"
#include "portsetup.h"
#include "setupdialog.h"

#include "mainwindow.h"


using namespace std;


MainWindow::MainWindow(int argc, char *argv[])
    : m_wiiThread(this),
      m_connectedToWii(false),
      m_topWidget(0),
      m_actionSlider(0),
      m_actionValueLabel(0),
      m_portSetup(new PortSetup)
{
    QSettings settings;
    m_portSetup->Load(settings);

    CreateActions();
    CreateMenus();

    setWindowTitle(tr("NXT Remote"));
    setMinimumSize(160, 160);
    resize(480, 320);

    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
        ("help", "Show this help message")
        ("connect-nxt,n", boost::program_options::value<string>(), "Connect to specified NXT device")
        ("connect-wiimote,w", boost::program_options::value<string>(), "Connect to specified Wiimote device");
    
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);    

    if (vm.count("help"))
    {
        ostringstream s;
        desc.print(s);
        QMessageBox::information(this, tr("Options"), s.str().c_str());
    }
    if (vm.count("connect-wiimote"))
    {
        m_wiimoteAddress = vm["connect-wiimote"].as<string>().c_str();
    }
    if (vm.count("connect-nxt"))
    {
        m_nxtAddress = vm["connect-nxt"].as<string>().c_str();
    }

    statusBar()->showMessage("Ready");
}


void MainWindow::Show()
{
    show();
    if (!m_wiimoteAddress.isEmpty())
        QTimer::singleShot(200, this, SLOT(ConnectToSelectedWiimote()));

    if (!m_nxtAddress.isEmpty())
        QTimer::singleShot(400, this, SLOT(ConnectToSelectedNxt()));
}


PortSetup MainWindow::GetSetup() const
{
    QMutexLocker lock(&m_portSetupMutex);
    return *m_portSetup;
}


boost::shared_ptr<nxt::Bluetooth> MainWindow::GetNxtConnection()
{
    QMutexLocker lock(&m_nxtConnectionMutex);
    return m_nxtConnection;
}


void MainWindow::Quit()
{
    m_wiiThread.terminate();
    m_wiiThread.wait();
}


void MainWindow::ConnectToNxt()
{
    ConnectDialog dlg(this, true);
    if (dlg.exec() != QDialog::Accepted)
        return;
    QString device = dlg.GetSelection();
    qDebug() << "Selected device:" << device;
    m_nxtAddress = device.section('(', 1).left(17);
}


void MainWindow::ConnectToSelectedNxt()
{
    statusBar()->showMessage("Connecting to NXT");
    qApp->processEvents();

    m_nxtConnection.reset(new nxt::Bluetooth);
    try
    {
        m_nxtConnection->connect(m_nxtAddress.toAscii());

        nxt::Brick brick(m_nxtConnection.get());
        nxt::Device_info info;
        brick.get_device_info(info);
        qDebug() << "Connected to NXT named " << info.name.c_str();

        // Play a sound to indicate successful connection
        brick.play_tone(440, 200, true);
    }
    catch (const nxt::Nxt_exception& e)
    {
        QMessageBox::critical(this, tr("NXT"),
                              tr("Unable to connect to NXT: %1").arg(e.what()));
        m_nxtConnection.reset();
        statusBar()->clearMessage();
        return;
    }

    m_connectNxtAct->setEnabled(false);
    m_disconnectNxtAct->setEnabled(true);
    statusBar()->showMessage("Connected to NXT");
}


void MainWindow::ConnectToWiimote()
{
    ConnectDialog dlg(this, false);
    if (dlg.exec() != QDialog::Accepted)
        return;
    QString device = dlg.GetSelection();
    qDebug() << "Selected device:" << device;
    m_wiimoteAddress = device.section('(', 1).left(17);
    qDebug() << "Address:" << m_wiimoteAddress;

    ConnectToSelectedWiimote();
}


void MainWindow::ConnectToSelectedWiimote()
{
    statusBar()->showMessage("Connecting to Wiimote");
    qApp->processEvents();

    memset(&(m_wiiThread.m_wiimoteInfo), 0, sizeof(wiimote_t));
    m_wiiThread.m_wiimoteInfo.link.device = 1;

    if (wiimote_connect(&m_wiiThread.m_wiimoteInfo, m_wiimoteAddress.toAscii()) < 0)
    {
        QMessageBox::critical(this, tr("Wiimote"),
                              tr("Unable to connect to Wiimote: %1").arg(wiimote_get_error()));
        statusBar()->clearMessage();
        return;
    }
    m_connectedToWii = true;
    m_connectWiimoteAct->setEnabled(false);
    m_disconnectWiimoteAct->setEnabled(true);

    m_wiiThread.start();

    ShowWiimoteControls();
    statusBar()->showMessage("Connected to Wiimote");
}


void MainWindow::DisconnectNxt()
{
    if (m_nxtConnection)
        m_nxtConnection->disconnect();
    m_nxtConnection.reset();
    m_connectNxtAct->setEnabled(true);
    m_disconnectNxtAct->setEnabled(false);
    HideControls();
}


void MainWindow::DisconnectWiimote()
{
    if (m_connectedToWii)
        wiimote_disconnect(&m_wiiThread.m_wiimoteInfo);
    m_connectedToWii = false;
    m_disconnectWiimoteAct->setEnabled(false);
    m_connectWiimoteAct->setEnabled(true);
    if (m_nxtConnection)
        ShowMouseControls();
    else
        HideControls();
}


void MainWindow::ShowSetupDialog()
{
    QMutexLocker lock(&m_portSetupMutex);
    PortSetup ps(*m_portSetup);
    lock.unlock();
    
    SetupDialog dlg(this, &ps);

    if (dlg.exec() != QDialog::Accepted)
        return;

    QSettings settings;
    ps.Save(settings);
    ostringstream s;
    s << ps;
    qDebug() << "New setup:" << s.str().c_str();
    lock.relock();
    *m_portSetup = ps;
}


void MainWindow::NunchukClicked()
{
    m_wiiThread.SetMode(WiiThread::Nunchuk);
}


void MainWindow::TiltClicked()
{
    m_wiiThread.SetMode(WiiThread::Tilt);
}


void MainWindow::ActionSliderMoved(int val)
{
    m_actionValueLabel->setText(QString("%1").arg(val));
}


void MainWindow::About()
{
    QMessageBox::about(this, tr("About nxtwiimote"),
                       tr("Nxtwiimote is free software: you can redistribute it and/or modify\n"
                          "it under the terms of the GNU General Public License as published by\n"
                          "the Free Software Foundation, either version 3 of the License, or\n"
                          "(at your option) any later version."));
}

void MainWindow::CreateActions()
{
    m_connectNxtAct = new QAction(tr("Connect to &NXT..."), this);
    m_connectNxtAct->setStatusTip(tr("Connect to a NXT device"));
    connect(m_connectNxtAct, SIGNAL(triggered()), this, SLOT(ConnectToNxt()));

    m_connectWiimoteAct = new QAction(tr("Connect to &Wiimote..."), this);
    m_connectWiimoteAct->setStatusTip(tr("Connect to a Wiimote"));
    connect(m_connectWiimoteAct, SIGNAL(triggered()), this, SLOT(ConnectToWiimote()));

    m_disconnectNxtAct = new QAction(tr("&Disconnect from NXT"), this);
    m_disconnectNxtAct->setEnabled(false);
    connect(m_disconnectNxtAct, SIGNAL(triggered()), this, SLOT(DisconnectNxt()));

    m_disconnectWiimoteAct = new QAction(tr("D&isconnect from Wii"), this);
    m_disconnectWiimoteAct->setEnabled(false);
    connect(m_disconnectWiimoteAct, SIGNAL(triggered()), this, SLOT(DisconnectWiimote()));

    m_setupAct = new QAction(tr("&Ports"), this);
    connect(m_setupAct, SIGNAL(triggered()), this, SLOT(ShowSetupDialog()));
    
    m_aboutAct = new QAction(tr("&About"), this);
    m_aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(m_aboutAct, SIGNAL(triggered()), this, SLOT(About()));
}

void MainWindow::CreateMenus()
{
    m_deviceMenu = menuBar()->addMenu(tr("&Device"));
    m_deviceMenu->addAction(m_connectNxtAct);
    m_deviceMenu->addAction(m_connectWiimoteAct);
    m_deviceMenu->addAction(m_disconnectNxtAct);
    m_deviceMenu->addAction(m_disconnectWiimoteAct);

    m_setupMenu = menuBar()->addMenu(tr("&Setup"));
    m_setupMenu->addAction(m_setupAct);
    
    m_helpMenu = menuBar()->addMenu(tr("&Help"));
    m_helpMenu->addAction(m_aboutAct);
}


void MainWindow::HideControls()
{
    delete m_topWidget;
    m_topWidget = 0;
}


void MainWindow::ShowWiimoteControls()
{
    HideControls();

    QGroupBox* bGroup = new QGroupBox(tr("Control"), this);
    QVBoxLayout* controlVBox = new QVBoxLayout;
    QRadioButton* nunchukButton = new QRadioButton(tr("&Nunchuk"));
    QRadioButton* tiltButton = new QRadioButton(tr("&Tilt (not implemented yet)"));
    nunchukButton->setChecked(true);
    controlVBox->addWidget(nunchukButton);
    controlVBox->addWidget(tiltButton);
    controlVBox->addStretch(1);
    bGroup->setLayout(controlVBox);

    QVBoxLayout* vbox = new QVBoxLayout;
    vbox->addWidget(bGroup);
    vbox->addWidget(CreateStandardControls(true));

    m_topWidget = new QWidget;
    m_topWidget->setLayout(vbox);
    setCentralWidget(m_topWidget);

    connect(nunchukButton, SIGNAL(clicked()), this, SLOT(NunchukClicked()));
    connect(tiltButton, SIGNAL(clicked()), this, SLOT(TiltClicked()));
    // Set default
    NunchukClicked();
}


void MainWindow::ShowMouseControls()
{
}


QWidget* MainWindow::CreateStandardControls(bool wiimote)
{
    QGroupBox* aGroup = new QGroupBox(tr("Action"), this);
    QVBoxLayout* aVBox = new QVBoxLayout;
    m_actionValueLabel = new QLabel(this);
    m_actionValueLabel->setAlignment(Qt::AlignHCenter);
    aVBox->addWidget(m_actionValueLabel);
    QHBoxLayout* aHBox = new QHBoxLayout;
    QLabel* leftLabel = new QLabel(this);
    leftLabel->setText("1");
    aHBox->addWidget(leftLabel);
    m_actionSlider = new QSlider(Qt::Horizontal, this);
    m_actionSlider->setMinimum(0);
    m_actionSlider->setMaximum(100);
    connect(m_actionSlider, SIGNAL(valueChanged(int)), this, SLOT(ActionSliderMoved(int)));
    aHBox->addWidget(m_actionSlider);
    QLabel* rightLabel = new QLabel(this);
    rightLabel->setText("2");
    aHBox->addWidget(rightLabel);
    aVBox->addLayout(aHBox);
    QLabel* helpLabel = new QLabel(this);
    helpLabel->setText(wiimote ? tr("Use C and Z to control action") :
                       tr("Use - and + to control action"));
    aVBox->addWidget(helpLabel);
    aVBox->addStretch(1);
    aGroup->setLayout(aVBox);
    return aGroup;
}

// ----------------------------------------------------------------


MainWindow::WiiThread::WiiThread(MainWindow* parent)
    : m_parent(parent),
      m_mode(Nunchuk)
{
}


void MainWindow::WiiThread::SetMode(Mode mode)
{
    m_mode = mode;
}


void MainWindow::WiiThread::run()
{
    m_wiimoteInfo.led.bits = 1;
    m_wiimoteInfo.rumble = 0;

    QTime lastNxtUpdate;
    lastNxtUpdate.start();

    int motorsOld[3] = { 0, 0, 0 };
    int motorAction = 0;
    
    while (wiimote_is_open(&m_wiimoteInfo))
    {
        if (wiimote_update(&m_wiimoteInfo) < 0)
        {
            QMessageBox::critical(m_parent, tr("Wiimote"),
                                  tr("Lost connection to Wiimote: %1").arg(wiimote_get_error()));
            m_parent->DisconnectWiimote();
            break;
        }
        
        // Enable IR-sensor and accelerometer
        m_wiimoteInfo.mode.ir = 1;
        m_wiimoteInfo.mode.acc = 1;

        int x = 0;
        int y = 0;
        switch (m_mode)
        {
        case Nunchuk:
        {
            // Value in neutral is around 128, extremes are approx. 32 and 224
            // Subtracting 128 scales these values nicely to around +- 100.
            // We add a dead zone of +- 8.
            x = m_wiimoteInfo.ext.nunchuk.joyx - 128;
            if (abs(x) < 8)
                x = 0;
            else if (abs(x) < 20)
                x *= 2; // Increase power at low levels
              y = m_wiimoteInfo.ext.nunchuk.joyy - 128;
            if (abs(y) < 8)
                y = 0;
            else if (abs(y) < 20)
                y *= 2; // Increase power at low levels

            QSlider* slider = m_parent->GetActionSlider();
            int sliderValue = slider->value();
            if (m_wiimoteInfo.keys.one)
            {
                if (sliderValue)
                    --sliderValue;
            }
            else if (m_wiimoteInfo.keys.two)
            {
                if (sliderValue < 100)
                    ++sliderValue;
            }
            slider->setValue(sliderValue);
            qDebug() << "NK" << m_wiimoteInfo.ext.nunchuk.keys.z << m_wiimoteInfo.ext.nunchuk.keys.c;
            if (m_wiimoteInfo.ext.nunchuk.keys.z)
            {
                motorAction = -sliderValue;
            }
            else if (m_wiimoteInfo.ext.nunchuk.keys.c)
            {
                motorAction = sliderValue;
            }
        }
        break;

        case Tilt:
            // TODO
            break;
        }

        int motor1 = 0;
        int motor2 = 0;
        
        PortSetup ps = m_parent->GetSetup();
        if (ps.single)
        {
            motor1 = y;
            motor2 = x;
        }
        else
        {
            motor1 = max(-100, min(100, y + x));
            motor2 = max(-100, min(100, y - x));
        }

        if (ps.driveReverse)
            motor2 *= -1;
        
        int motors[3] = { 0, 0, 0 };
        motors[ps.motor1 - nxt::OUT_A] = motor1;
        motors[ps.motor2 - nxt::OUT_A] = motor2;
        motors[ps.actionMotor - nxt::OUT_A] = motorAction;

        //qDebug() << "M:" << motors[0] << motors[1] << motors[2];

        boost::shared_ptr<nxt::Bluetooth> nxtConnection = m_parent->GetNxtConnection();
        if (nxtConnection && (lastNxtUpdate.elapsed() >= 100))
        {
            for (int i = 0; i < 3; ++i)
            {
                if (motors[i] == motorsOld[i])
                    continue;
                qDebug() << "Update motor" << ('A'+i);
                motorsOld[i] = motors[i];
                nxt::Motor m(static_cast<nxt::Motor_port>(nxt::OUT_A+i),
                             nxtConnection.get());
                try
                {
                    m.on(motors[i], 0, false);
                    motorsOld[i] = motors[i];
                }
                catch (const nxt::Nxt_exception& e)
                {
                    qDebug() << "Motor" << i << "exception: P" << motors[i] << e.what() << e.who().c_str();
                }
            }
        }

        /*
        qDebug() << "MODE" << m_wiimoteInfo.mode.bits;

        qDebug() << "KEYS"
                 << m_wiimoteInfo.keys.one
                 << m_wiimoteInfo.keys.two
                 << m_wiimoteInfo.keys.a
                 << m_wiimoteInfo.keys.b
                 << m_wiimoteInfo.keys.left
                 << m_wiimoteInfo.keys.right
                 << m_wiimoteInfo.keys.up
                 << m_wiimoteInfo.keys.down
                 << m_wiimoteInfo.keys.home
                 << m_wiimoteInfo.keys.plus
                 << m_wiimoteInfo.keys.minus;
            

        qDebug() << "AXIS"
                 << m_wiimoteInfo.axis.x
                 << m_wiimoteInfo.axis.y
                 << m_wiimoteInfo.axis.z;

        qDebug() << "TILT"
                 << m_wiimoteInfo.tilt.x
                 << m_wiimoteInfo.tilt.y
                 << m_wiimoteInfo.tilt.z;

        qDebug() << "FORCE"
                 << m_wiimoteInfo.force.x
                 << m_wiimoteInfo.force.y
                 << m_wiimoteInfo.force.z
                 << sqrt(m_wiimoteInfo.force.x*m_wiimoteInfo.force.x+
                         m_wiimoteInfo.force.y*m_wiimoteInfo.force.y+
                         m_wiimoteInfo.force.z*m_wiimoteInfo.force.z);
        */
//      qDebug() << "IR1"
//                  << m_wiimoteInfo.ir1.x
//                  << m_wiimoteInfo.ir1.y
//                  << m_wiimoteInfo.ir1.size;

//      qDebug() << "IR2"
//                  << m_wiimoteInfo.ir2.x
//                  << m_wiimoteInfo.ir2.y
//                  << m_wiimoteInfo.ir2.size;

//      qDebug() << "IR3"
//                  << m_wiimoteInfo.ir3.x
//                  << m_wiimoteInfo.ir3.y
//                  << m_wiimoteInfo.ir3.size;
            
//      qDebug() << "IR4"
//                  << m_wiimoteInfo.ir4.x
//                  << m_wiimoteInfo.ir4.y
//                  << m_wiimoteInfo.ir4.size;

    }
}
