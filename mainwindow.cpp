
#include <QtDebug>
#include <QtGui>

extern "C" {
#include <libcwiimote/wiimote_api.h>
}

#include <bluetooth.h>
#include <brick.h>
#include <error.h>

#include "connectdialog.h"

#include "mainwindow.h"



#define DUR1   400
#define DUR2   500
#define PAUSE  150

#define TONE_C 264
#define TONE_D 297
#define TONE_E 330
#define TONE_F 352
#define TONE_G 396
#define TONE_A 440

MainWindow::MainWindow()
    : m_wiiThread(this),
      m_connectedToWii(false),
      m_nxtConnection(0)
{
    QWidget *widget = new QWidget;
    setCentralWidget(widget);
    /*
    QWidget *topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget *bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(5);
    layout->addWidget(topFiller);
    layout->addWidget(infoLabel);
    layout->addWidget(bottomFiller);
    widget->setLayout(layout);
    */
    CreateActions();
    CreateMenus();

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

    nxt::Brick brick(m_nxtConnection);

    nxt::Device_info info;
    brick.get_device_info(info);
    
    qDebug() << "BDA:" << info.bt_address.c_str();
    qDebug() << "Name:" << info.name.c_str();
    
    brick.play_tone(TONE_C, DUR1, true); // Al -
    usleep((DUR1+PAUSE)*1000);
    /*
    playtone(m_nxt, TONE_D,DUR1); // le
    playtone(m_nxt, TONE_E,DUR1); // mei -
    playtone(m_nxt, TONE_F,DUR1); // ne
    playtone(m_nxt, TONE_G,DUR2); // Ent -
    playtone(m_nxt, TONE_G,DUR2); // chen
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
}


void MainWindow::DisconnectNxt()
{
    if (m_nxtConnection)
    {
        m_nxtConnection->disconnect();
        delete m_nxtConnection;
        m_nxtConnection = 0;
    }
    m_connectNxtAct->setEnabled(true);
    m_disconnectNxtAct->setEnabled(false);
}


void MainWindow::DisconnectWiimote()
{
    if (m_connectedToWii)
    {
        wiimote_disconnect(&m_wiiThread.m_wiimoteInfo);
        m_connectedToWii = false;
    }
    m_disconnectWiimoteAct->setEnabled(false);
    m_connectWiimoteAct->setEnabled(true);
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

    m_helpMenu = menuBar()->addMenu(tr("&Help"));
    m_helpMenu->addAction(m_aboutAct);
}



MainWindow::WiiThread::WiiThread(MainWindow* parent)
    : m_parent(parent)
{
}


void MainWindow::WiiThread::run()
{
    m_wiimoteInfo.led.bits = 1;
    // Enable IR-sensor and accelerometer
    m_wiimoteInfo.mode.bits = WIIMOTE_MODE_ACC_IR_EXT;
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
			
// 		qDebug() << "JOY1"
//                  << m_wiimoteInfo.ext.nunchuk.joyx
//                  << m_wiimoteInfo.ext.nunchuk.joyy
//                  << m_wiimoteInfo.ext.nunchuk.axis.x
//                  << m_wiimoteInfo.ext.nunchuk.axis.y
//                  << m_wiimoteInfo.ext.nunchuk.axis.z
//                  << m_wiimoteInfo.ext.nunchuk.keys.z
//                  << m_wiimoteInfo.ext.nunchuk.keys.c;

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

// 		qDebug() << "IR1"
//                  << m_wiimoteInfo.ir1.x
//                  << m_wiimoteInfo.ir1.y
//                  << m_wiimoteInfo.ir1.size;

// 		qDebug() << "IR2"
//                  << m_wiimoteInfo.ir2.x
//                  << m_wiimoteInfo.ir2.y
//                  << m_wiimoteInfo.ir2.size;

// 		qDebug() << "IR3"
//                  << m_wiimoteInfo.ir3.x
//                  << m_wiimoteInfo.ir3.y
//                  << m_wiimoteInfo.ir3.size;
			
// 		qDebug() << "IR4"
//                  << m_wiimoteInfo.ir4.x
//                  << m_wiimoteInfo.ir4.y
//                  << m_wiimoteInfo.ir4.size;

	}
}
