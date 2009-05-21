
#include <QMutexLocker>
#include <QtDebug>
#include <QtGui>

extern "C" {
#include <libcwiimote/wiimote_api.h>
}

#include <bluetooth.h>
#include <brick.h>
#include <error.h>

#include "connectdialog.h"
#include "ui_setupdialog.h"

#include "mainwindow.h"


MainWindow::MainWindow()
    : m_wiiThread(this),
      m_connectedToWii(false),
      m_nxtConnection(0),
      m_topWidget(0)
{
    CreateActions();
    CreateMenus();

    // TODO: Create common controls for motor mode etc.
    
    setWindowTitle(tr("NXT Remote"));
    setMinimumSize(160, 160);
    resize(480, 320);
}


void MainWindow::ConnectToNxt()
{
    ConnectDialog dlg(this, true);
    if (dlg.exec() != QDialog::Accepted)
        return;
    QString device = dlg.GetSelection();
    qDebug() << "Selected device:" << device;
    QString address = device.section('(', 1).left(17);

    m_nxtConnection = new nxt::Bluetooth;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    try
    {
        m_nxtConnection->connect(address.toAscii());

        nxt::Brick brick(m_nxtConnection);
        nxt::Device_info info;
        brick.get_device_info(info);
        qDebug() << "Connected to NXT named " << info.name.c_str();

        // Play a sound to indicate successful connection
        brick.play_tone(440, 200, true);
    }
    catch (const nxt::Nxt_exception& e)
    {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, tr("NXT"),
                              tr("Unable to connect to NXT: %1").arg(e.what()));
        delete m_nxtConnection;
        m_nxtConnection = 0;
        return;
    }
    QApplication::restoreOverrideCursor();

    m_connectNxtAct->setEnabled(false);
    m_disconnectNxtAct->setEnabled(true);
/*
    nxt::Motor m(nxt::OUT_A, m_nxtConnection);
    m.on(50, 90, true);
*/
}


void MainWindow::ConnectToWiimote()
{
    ConnectDialog dlg(this, false);
    if (dlg.exec() != QDialog::Accepted)
        return;
    QString device = dlg.GetSelection();
    qDebug() << "Selected device:" << device;
    QString address = device.section('(', 1).left(17);
    qDebug() << "Address:" << address;

    memset(&(m_wiiThread.m_wiimoteInfo), sizeof(wiimote_t), 0);
    m_wiiThread.m_wiimoteInfo.link.device = 1;

    if (wiimote_connect(&m_wiiThread.m_wiimoteInfo, address.toAscii()) < 0)
    {
        QMessageBox::critical(this, tr("Wiimote"),
                              tr("Unable to connect to Wiimote: %1").arg(wiimote_get_error()));
        return;
    }
    m_connectedToWii = true;
    m_connectWiimoteAct->setEnabled(false);
    m_disconnectWiimoteAct->setEnabled(true);

    m_wiiThread.start();

    ShowWiimoteControls();
}


void MainWindow::DisconnectNxt()
{
    if (m_nxtConnection)
        m_nxtConnection->disconnect();
    delete m_nxtConnection;
    m_nxtConnection = 0;
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
    QDialog dlg(this);
    Ui::SetupDialog setupDlg;
    setupDlg.setupUi(&dlg);
    // Center checkboxes
    for (int i = 0; i < 4; ++i)
    {
        setupDlg.gridLayout->itemAtPosition(i, 1)->setAlignment(Qt::AlignHCenter);
        setupDlg.gridLayout->itemAtPosition(i, 2)->setAlignment(Qt::AlignHCenter);
    }
    SetSensorCombo(m_portSetup.sensors[0], setupDlg.m_sensorCombo1);
    SetSensorCombo(m_portSetup.sensors[1], setupDlg.m_sensorCombo2);
    SetSensorCombo(m_portSetup.sensors[2], setupDlg.m_sensorCombo3);
    SetSensorCombo(m_portSetup.sensors[3], setupDlg.m_sensorCombo4);
    setupDlg.m_motor1RevCheck->setChecked(m_portSetup.reverse[0]);
    setupDlg.m_motor1CoastCheck->setChecked(m_portSetup.coast[0]);
    setupDlg.m_motor2RevCheck->setChecked(m_portSetup.reverse[1]);
    setupDlg.m_motor2CoastCheck->setChecked(m_portSetup.coast[1]);
    setupDlg.m_motor3RevCheck->setChecked(m_portSetup.reverse[2]);
    setupDlg.m_motor3CoastCheck->setChecked(m_portSetup.coast[2]);
    
/*
  SetMotorCombo(m_portSetup.motors[0], setupDlg.m_motorCombo1);
    SetMotorCombo(m_portSetup.motors[1], setupDlg.m_motorCombo2);
    SetMotorCombo(m_portSetup.motors[2], setupDlg.m_motorCombo3);
*/
    if (dlg.exec() != QDialog::Accepted)
        return;
    int motor1 = setupDlg.m_motorCombo1->currentIndex();
}


void MainWindow::NunchukClicked()
{
    m_wiiThread.SetMode(WiiThread::Nunchuk);
}


void MainWindow::TiltClicked()
{
    m_wiiThread.SetMode(WiiThread::Tilt);
}


void MainWindow::About()
{
    QMessageBox::about(this, tr("About nxtremote"),
                       tr("TODO."));
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
    m_wiiThread.SetInfoLabel(0);
    delete m_topWidget;
    m_topWidget = 0;
}


void MainWindow::ShowWiimoteControls()
{
    HideControls();

    QLabel* label = new QLabel(this);
    m_wiiThread.SetInfoLabel(label);
    
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
    vbox->addWidget(label);

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


void MainWindow::SetSensorCombo(nxt::Sensor_type type, QComboBox* cb)
{
    cb->clear();
    for (int i = nxt::NO_SENSOR; i <= nxt::SONAR_INCH; ++i)
    {
        if ((i == nxt::CUSTOM) ||
            (i == nxt::LOWSPEED) ||
            (i == nxt::LOWSPEED_9V))
            continue;
        cb->addItem(GetAsString(static_cast<nxt::Sensor_type>(i)), i);
    }
    cb->setCurrentIndex(cb->findData(type));
}


// static
QString MainWindow::GetAsString(nxt::Sensor_type type)
{
    switch (type)
    {
    case nxt::NO_SENSOR:
        return "No sensor";

    case nxt::TOUCH:
        return "Touch sensor (NXT or RCX)";

    case nxt::TEMPERATURE:
        return "RCX temperature sensor";

    case nxt::REFLECTION:
        return "RCX light sensor";

    case nxt::ANGLE:
        return "RCX rotation sensor";

    case nxt::LIGHT_ACTIVE:
        return "NXT light sensor, LED on";

    case nxt::LIGHT_INACTIVE:
        return "NXT light sensor, LED off";

    case nxt::SOUND_DB:
        return "NXT sound sensor, dB";

    case nxt::SOUND_DBA:
        return "NXT sound sensor, dBA";

    case nxt::SONAR_METRIC:
        return "Sonar sensor, metric";

    case nxt::SONAR_INCH:
        return "Sonar sensor, inches";

    default:
        break;
    }
    return "?";
}


// ----------------------------------------------------------------


MainWindow::WiiThread::WiiThread(MainWindow* parent)
    : m_parent(parent),
      m_mode(Nunchuk),
      m_infoLabel(0)
{
}


void MainWindow::WiiThread::SetMode(Mode mode)
{
    m_mode = mode;
}


void MainWindow::WiiThread::SetInfoLabel(QLabel* label)
{
    QMutexLocker lock(&m_mutex);
    m_infoLabel = label;
}


void MainWindow::WiiThread::run()
{
    m_wiimoteInfo.led.bits = 1;
    m_wiimoteInfo.rumble = 0;

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
            // Value in neutral is around 128, extremes are approx. 32 and 224
            // Subtracting 128 scales these values nicely to around +- 100.
            // We add a dead zone of +- 8.
            x = m_wiimoteInfo.ext.nunchuk.joyx - 128;
            if (abs(x) < 8)
                x = 0;
              y = m_wiimoteInfo.ext.nunchuk.joyy - 128;
            if (abs(y) < 8)
                y = 0;
            {
                QMutexLocker lock(&m_mutex);
                if (m_infoLabel)
                {
                    m_infoLabel->setText(QString("X: %1\nY: %2").arg(x).arg(y));
                }
            }
            break;

        case Tilt:
            // TODO
            break;
        }
        
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


// ----------------------------------------------------------------


MainWindow::PortSetup::PortSetup()
{
    sensors[0] = nxt::TOUCH;
    sensors[1] = nxt::SOUND_DB;
    sensors[2] = nxt::LIGHT_ACTIVE;
    sensors[3] = nxt::SONAR_METRIC;
    motors[0] = nxt::OUT_B;
    motors[1] = nxt::OUT_C;
    motors[2] = nxt::OUT_A;
    for (int i = 0; i < NOF_MOTOR_PORTS; ++i)
    {
        reverse[i] = false;
        coast[i] = false;
    }
}
