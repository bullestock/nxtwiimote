#include <iostream>
#include <iomanip>

#include <QDebug>
#include <QMessageBox>
#include <QProgressDialog>
#include <QPushButton>
#include <QSettings>

#include "connectdialog.h"


using namespace std;


ConnectDialog::ConnectDialog(QWidget* parent,
                             bool isNxt)
    : QDialog(parent),
      m_isNxt(isNxt),
      m_process(0),
      m_progressDialog(0)
{
    setupUi(this);

    setWindowTitle(isNxt ? tr("Connect to NXT") : tr("Connect to Wiimote"));

    QSettings settings;
    QVariant devices(settings.value("btdevices"));
    if (!devices.isNull())
    {
        QStringListIterator it(devices.toStringList());
        while (it.hasNext())
        {
            QStringList parts = it.next().split("\t", QString::SkipEmptyParts);
            if (parts.size() != 2)
                continue;
            QString address(parts[0]);
            QString name(parts[1]);
            const bool isWiimote = (name == "Nintendo RVL-CNT-01");
            if (isNxt != isWiimote)
                m_deviceCombo->addItem(name + QString(" (") + address + QString(")"));
        }
    }

    m_scanButton = m_buttonBox->addButton("&Scan", QDialogButtonBox::ActionRole);
    QObject::connect(m_buttonBox, SIGNAL(clicked(QAbstractButton*)),
                     this, SLOT(ScanClicked(QAbstractButton*)));
}


QString ConnectDialog::GetSelection() const
{
    return m_deviceCombo->currentText();
}


void ConnectDialog::ScanClicked(QAbstractButton* b)
{
    if (b != m_scanButton)
        return;

    m_process = new QProcess;
    QStringList args;
    args << "scan";
    m_process->start("hcitool", args);
    if (!m_process->waitForStarted())
    {
        QMessageBox::critical(this, tr("Scan"),
                              tr("Could not start hcitool."));
        delete m_process;
        m_process = 0;
        return;
    }

    connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(ScanDone(int, QProcess::ExitStatus)));

    m_progressDialog = new QProgressDialog(tr("Scanning..."), tr("Cancel"),
                                           0, 0, this);
    connect(m_progressDialog, SIGNAL(canceled()), this, SLOT(CancelScan()));
    m_progressDialog->show();
}


void ConnectDialog::CancelScan()
{
    m_process->close();
    delete m_process;
    m_process = 0;
    delete m_progressDialog;
    m_progressDialog = 0;
}


void ConnectDialog::ScanDone(int exitCode, QProcess::ExitStatus exitStatus)
{
    delete m_progressDialog;
    m_progressDialog = 0;
    QString output = m_process->readAllStandardOutput();
    delete m_process;
    m_process = 0;
    if ((exitStatus != QProcess::NormalExit) || exitCode)
    {
        QMessageBox::critical(this, tr("Scan"),
                              tr("hcitool failed with exit code %1:\n%2").arg(exitCode).arg(output));
        return;
    }
    QStringList lines = output.split("\n", QString::SkipEmptyParts);
    if (lines.isEmpty())
        return;
    m_deviceCombo->clear();
    QStringListIterator it(lines);
    QStringList devices;
    while (it.hasNext())
    {
        QString line(it.next());
        QStringList parts = line.split("\t", QString::SkipEmptyParts);
        if (parts.size() != 2)
            continue;
        QString address(parts[0]);
        QString name(parts[1]);
        // Skip bogus lines
        if ((address.size() != 17) || (address[2] != ':'))
            continue;
        const bool isWiimote = (name == "Nintendo RVL-CNT-01");
        if (m_isNxt != isWiimote)
            m_deviceCombo->addItem(name + QString(" (") + address + QString(")"));
        devices << (address + QString("\t") + name);
    }
    QSettings settings;
    settings.setValue("btdevices", devices);
}
