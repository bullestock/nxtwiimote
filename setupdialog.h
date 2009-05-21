#include "ui_setupdialog.h"


class QProgressDialog;


class SetupDialog : public QDialog,
                      private Ui::SetupDialog
{
    Q_OBJECT

public:
    SetupDialog(QWidget* parent);
};

