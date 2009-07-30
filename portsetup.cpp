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

#include <QSettings>

#include "portsetup.h"


using namespace std;


PortSetup::PortSetup()
{
    sensors[0] = nxt::TOUCH;
    sensors[1] = nxt::SOUND_DB;
    sensors[2] = nxt::LIGHT_ACTIVE;
    sensors[3] = nxt::SONAR_METRIC;
    single = false;
    motor1 = nxt::OUT_B;
    motor2 = nxt::OUT_C;
    driveReverse = false;
    driveCoast = false;
    actionMotor = nxt::OUT_A;
    actionReverse = false;
    actionCoast = false;
}


void PortSetup::Load(QSettings& s)
{
    if (!s.contains("sensor0"))
        return; // No settings
    for (int i = 0; i < PortSetup::NOF_SENSOR_PORTS; ++i)
        sensors[i] = static_cast<nxt::Sensor_type>(s.value(QString("sensor%1").arg(i)).toInt());
    single = s.value("driveSingle").toBool();
    motor1 = static_cast<nxt::Motor_port>(s.value("motor1").toInt());
    motor2 = static_cast<nxt::Motor_port>(s.value("motor2").toInt());
    driveReverse = s.value("driveReverse").toBool();
    driveCoast = s.value("driveCoast").toBool();
    actionMotor = static_cast<nxt::Motor_port>(s.value("actionMotor").toInt());
    actionReverse = s.value("actionReverse").toBool();
    actionCoast = s.value("actionCoast").toBool();
}


void PortSetup::Save(QSettings& s)
{
    for (int i = 0; i < PortSetup::NOF_SENSOR_PORTS; ++i)
        s.setValue(QString("sensor%1").arg(i), sensors[i]);
    s.setValue("driveSingle", single);
    s.setValue("motor1", motor1);
    s.setValue("motor2", motor2);
    s.setValue("driveReverse", driveReverse);
    s.setValue("driveCoast", driveCoast);
    s.setValue("actionMotor", actionMotor);
    s.setValue("actionReverse", actionReverse);
    s.setValue("actionCoast", actionCoast);
}


ostream& operator<<(ostream& s, const PortSetup& p)
{
    for (int i = 0; i < PortSetup::NOF_SENSOR_PORTS; ++i)
        s << "Sensor " << i << ": " << p.sensors[i] << endl;
    s << "Drive: " << (p.single ? "Single" : "Differential") << endl
      << "Motors: " << p.motor1 << " " << p.motor2 << " Reverse: "
      << boolalpha << p.driveReverse << " Coast: " << p.driveCoast << endl
      << "Action: " << p.actionMotor << " Reverse: " << p.actionReverse
      << " Coast: " << p.actionCoast << endl;
    return s;
}
