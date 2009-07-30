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

#include "sensorutil.h"


using namespace std;


QString GetAsString(nxt::Sensor_type type,
                    bool brief)
{
    switch (type)
    {
    case nxt::NO_SENSOR:
        return "No sensor";

    case nxt::TOUCH:
        return brief ? "Touch" : "Touch sensor (NXT or RCX)";

    case nxt::TEMPERATURE:
        return brief ? "Temperature" : "RCX temperature sensor";

    case nxt::REFLECTION:
        return brief ? "Light" : "RCX light sensor";

    case nxt::ANGLE:
        return brief ? "Rotation" : "RCX rotation sensor";

    case nxt::LIGHT_ACTIVE:
        return brief ? "Light" : "NXT light sensor, LED on";

    case nxt::LIGHT_INACTIVE:
        return brief ? "Light" : "NXT light sensor, LED off";

    case nxt::SOUND_DB:
        return brief ? "Sound" : "NXT sound sensor, dB";

    case nxt::SOUND_DBA:
        return brief ? "Sound" : "NXT sound sensor, dBA";

    case nxt::SONAR_METRIC:
        return brief ? "Sonar" : "Sonar sensor, metric";

    case nxt::SONAR_INCH:
        return brief ? "Sonar" : "Sonar sensor, inches";

    default:
        break;
    }
    return "?";
}


QString GetUnit(nxt::Sensor_type type)
{
    switch (type)
    {
    case nxt::TEMPERATURE:
        return QString(0x00B0)+QString("C");
        
    case nxt::ANGLE:
        return QString(0x00B0);

    case nxt::SOUND_DB:
        return "dB";

    case nxt::SOUND_DBA:
        return "dBA";

    case nxt::SONAR_METRIC:
        return "cm";

    case nxt::SONAR_INCH:
        return "\"";

    default:
        break;
    }
    return "";
}


nxt::Sensor_mode GetMode(nxt::Sensor_type type)
{
    switch (type)
    {
    case nxt::NO_SENSOR:
    case nxt::SOUND_DB:
    case nxt::SOUND_DBA:
    case nxt::SONAR_METRIC:
    case nxt::SONAR_INCH:
    default:

    case nxt::REFLECTION:
    case nxt::LIGHT_ACTIVE:
    case nxt::LIGHT_INACTIVE:
        return nxt::PERCENT_MODE;

    case nxt::TOUCH:
        return nxt::BOOL_MODE;

    case nxt::TEMPERATURE:
        return nxt::CELSIUS_MODE;

    case nxt::ANGLE:
        return nxt::ANGLE_MODE;
    }
    return nxt::RAW_MODE;
}
