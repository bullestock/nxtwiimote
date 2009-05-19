#include <QProcess>

#include "ui_connectdialog.h"


class QProgressDialog;


class ConnectDialog : public QDialog,
                      private Ui::ConnectDialog
{
    Q_OBJECT

public:
    ConnectDialog(QWidget* parent,
                  bool isNxt);

    QString GetSelection() const;

private slots:
    void ScanClicked(QAbstractButton*);
    void CancelScan();
    void ScanDone(int exitCode, QProcess::ExitStatus exitStatus);
    
private:
    bool m_isNxt;
    QPushButton* m_scanButton;
    QProcess* m_process;
    QProgressDialog* m_progressDialog;
};
