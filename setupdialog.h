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

#include "ui_setupdialog.h"

#include "portsetup.h"


class SetupDialog : public QDialog,
                    private Ui::SetupDialog
{
    Q_OBJECT

public:
    SetupDialog(QWidget* parent,
                PortSetup* portSetup);

private slots:
    void DriveMotorChanged(int);
    void SteeringMotorChanged(int);
    void DriveMethodChanged(bool);
    void OkClicked();

private:
    void RefreshControls(bool store = true);

    PortSetup m_portSetup;
    PortSetup* m_oldPortSetup;
};
