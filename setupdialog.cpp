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

#include <iostream>
#include <iomanip>

#include <QDebug>
#include <QMessageBox>

#include "setupdialog.h"


using namespace std;


static void EnsureItemExists(QString item, const QVariant& data, QComboBox* cb);
static void SetSensorCombo(nxt::Sensor_type type, QComboBox* cb);
//static void SetMotorCombo(nxt::Motor_port port, QComboBox* cb);


SetupDialog::SetupDialog(QWidget* parent,
                         PortSetup* portSetup)
    : QDialog(parent),
      m_portSetup(*portSetup),
      m_oldPortSetup(portSetup)
{
    setupUi(this);

    SetSensorCombo(m_portSetup.sensors[0], m_sensorCombo1);
    SetSensorCombo(m_portSetup.sensors[1], m_sensorCombo2);
    SetSensorCombo(m_portSetup.sensors[2], m_sensorCombo3);
    SetSensorCombo(m_portSetup.sensors[3], m_sensorCombo4);

    m_steerLabel->setVisible(m_portSetup.single);
    m_steerCombo->setVisible(m_portSetup.single);
    m_singleButton->setChecked(m_portSetup.single);
    m_diffButton->setChecked(!m_portSetup.single);

    RefreshControls(false);

    if (m_portSetup.single)
    {
        m_steerCombo->setCurrentIndex(m_steerCombo->findData(m_portSetup.motor1));
        m_driveCombo->setCurrentIndex(m_driveCombo->findData(m_portSetup.motor2));
    }
    else
    {
        QString m1(QString("%1").arg(static_cast<char>('A'+m_portSetup.motor1-nxt::OUT_A)));
        QString m2(QString("%1").arg(static_cast<char>('A'+m_portSetup.motor2-nxt::OUT_A)));
        QString motors(m1 + m2);
        m_driveCombo->setCurrentIndex(m_driveCombo->findText(motors));
    }
    m_driveRevCheck->setChecked(m_portSetup.driveReverse);
    m_driveCoastCheck->setChecked(m_portSetup.driveCoast);
    m_actionRevCheck->setChecked(m_portSetup.actionReverse);
    m_actionCoastCheck->setChecked(m_portSetup.actionCoast);
    
    connect(m_driveCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(DriveMotorChanged(int)));
    connect(m_steerCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(SteeringMotorChanged(int)));
    connect(m_singleButton, SIGNAL(clicked(bool)), this, SLOT(DriveMethodChanged(bool)));
    connect(m_diffButton, SIGNAL(clicked(bool)), this, SLOT(DriveMethodChanged(bool)));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(OkClicked()));
}


void SetupDialog::OkClicked()
{
    m_portSetup.driveReverse = m_driveRevCheck->isChecked();
    m_portSetup.driveCoast = m_driveCoastCheck->isChecked();
    m_portSetup.actionReverse = m_actionRevCheck->isChecked();
    m_portSetup.actionCoast = m_actionCoastCheck->isChecked();
    *m_oldPortSetup = m_portSetup;
}

void SetupDialog::DriveMotorChanged(int)
{
    RefreshControls();
}


void SetupDialog::SteeringMotorChanged(int)
{
    RefreshControls();
}


void SetupDialog::DriveMethodChanged(bool)
{
    m_portSetup.single = m_singleButton->isChecked();
    m_steerLabel->setVisible(m_portSetup.single);
    m_steerCombo->setVisible(m_portSetup.single);
    m_driveCombo->clear();
    m_steerCombo->clear();
    RefreshControls();
}


void SetupDialog::RefreshControls(bool store)
{
    m_steerCombo->blockSignals(true);
    m_driveCombo->blockSignals(true);

    nxt::Motor_port driveMotor =
        static_cast<nxt::Motor_port>(m_driveCombo->itemData(m_driveCombo->currentIndex()).toInt());
    nxt::Motor_port steerMotor =
        static_cast<nxt::Motor_port>(m_steerCombo->itemData(m_steerCombo->currentIndex()).toInt());

    if (m_portSetup.single)
    {
        if (driveMotor == steerMotor)
        {
            steerMotor = static_cast<nxt::Motor_port>(steerMotor+1);
            if (steerMotor > nxt::OUT_C)
                steerMotor = nxt::OUT_A;
        }
        EnsureItemExists("A", nxt::OUT_A, m_driveCombo);
        EnsureItemExists("B", nxt::OUT_B, m_driveCombo);
        EnsureItemExists("C", nxt::OUT_C, m_driveCombo);
        EnsureItemExists("A", nxt::OUT_A, m_steerCombo);
        EnsureItemExists("B", nxt::OUT_B, m_steerCombo);
        EnsureItemExists("C", nxt::OUT_C, m_steerCombo);

        m_steerCombo->setCurrentIndex(m_steerCombo->findData(steerMotor));
        m_driveCombo->setCurrentIndex(m_driveCombo->findData(driveMotor));
    }
    else
    {
        EnsureItemExists("AB", 0, m_driveCombo);
        EnsureItemExists("AC", 1, m_driveCombo);
        EnsureItemExists("BC", 2, m_driveCombo);

        // In differential mode, index is the same as the value
        m_driveCombo->setCurrentIndex(driveMotor);
        string motors(m_driveCombo->currentText().toAscii());
        driveMotor = static_cast<nxt::Motor_port>(motors[0]-'A'+nxt::OUT_A);
        steerMotor = static_cast<nxt::Motor_port>(motors[1]-'A'+nxt::OUT_A);
    }

    if (store)
    {
        m_portSetup.motor1 = driveMotor;
        m_portSetup.motor2 = steerMotor;
    
        for (int i = 0; i < 3; ++i)
        {
            nxt::Motor_port motor = static_cast<nxt::Motor_port>(nxt::OUT_A+i);
            if ((motor != driveMotor) && (motor != steerMotor))
            {
                m_actionEdit->setText(QString("%1").arg(static_cast<char>('A'+i)));
                m_portSetup.actionMotor = motor;
                break;
            }
        }
    }
    
    m_steerCombo->blockSignals(false);
    m_driveCombo->blockSignals(false);
}


static void EnsureItemExists(QString item, const QVariant& data, QComboBox* cb)
{
    if (cb->findText(item) >= 0)
        return;
    cb->addItem(item, data);
}


static void SetSensorCombo(nxt::Sensor_type type, QComboBox* cb)
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
