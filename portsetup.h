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

#ifndef portsetup_h
#define portsetup_h

#include <QString>

#include <motor.h>
#include <sensor.h>

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE


struct PortSetup
{
    static const int NOF_SENSOR_PORTS = 4;

    PortSetup();

    void Load(QSettings& s);
    void Save(QSettings& s);
        
    nxt::Sensor_type sensors[NOF_SENSOR_PORTS];
    // True: Single
    // False: Differential
    bool             single;
    // Single:       Drive motor
    // Differential: Left motor
    nxt::Motor_port  motor1;
    // Single:       Steering motor
    // Differential: Right motor
    nxt::Motor_port  motor2;
    bool             driveReverse;
    bool             driveCoast;
    // Action motor
    nxt::Motor_port  actionMotor;
    bool             actionReverse;
    bool             actionCoast;
};

std::ostream& operator<<(std::ostream&, const PortSetup&);

#endif
