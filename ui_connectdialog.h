/********************************************************************************
** Form generated from reading ui file 'connectdialog.ui'
**
** Created: Thu May 14 22:33:14 2009
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CONNECTDIALOG_H
#define UI_CONNECTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>

QT_BEGIN_NAMESPACE

class Ui_ConnectDialog
{
public:
    QDialogButtonBox *m_buttonBox;
    QComboBox *m_deviceCombo;

    void setupUi(QDialog *ConnectDialog)
    {
    if (ConnectDialog->objectName().isEmpty())
        ConnectDialog->setObjectName(QString::fromUtf8("ConnectDialog"));
    ConnectDialog->resize(420, 108);
    m_buttonBox = new QDialogButtonBox(ConnectDialog);
    m_buttonBox->setObjectName(QString::fromUtf8("m_buttonBox"));
    m_buttonBox->setGeometry(QRect(10, 60, 401, 32));
    m_buttonBox->setOrientation(Qt::Horizontal);
    m_buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    m_deviceCombo = new QComboBox(ConnectDialog);
    m_deviceCombo->setObjectName(QString::fromUtf8("m_deviceCombo"));
    m_deviceCombo->setGeometry(QRect(10, 20, 401, 27));

    retranslateUi(ConnectDialog);
    QObject::connect(m_buttonBox, SIGNAL(accepted()), ConnectDialog, SLOT(accept()));
    QObject::connect(m_buttonBox, SIGNAL(rejected()), ConnectDialog, SLOT(reject()));

    QMetaObject::connectSlotsByName(ConnectDialog);
    } // setupUi

    void retranslateUi(QDialog *ConnectDialog)
    {
    ConnectDialog->setWindowTitle(QApplication::translate("ConnectDialog", "Dialog", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ConnectDialog);
    } // retranslateUi

};

namespace Ui {
    class ConnectDialog: public Ui_ConnectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECTDIALOG_H
