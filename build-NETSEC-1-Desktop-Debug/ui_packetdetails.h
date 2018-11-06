/********************************************************************************
** Form generated from reading UI file 'packetdetails.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PACKETDETAILS_H
#define UI_PACKETDETAILS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPlainTextEdit>

QT_BEGIN_NAMESPACE

class Ui_PacketDetails
{
public:
    QPlainTextEdit *plainTextEdit;

    void setupUi(QDialog *PacketDetails)
    {
        if (PacketDetails->objectName().isEmpty())
            PacketDetails->setObjectName(QStringLiteral("PacketDetails"));
        PacketDetails->resize(558, 648);
        plainTextEdit = new QPlainTextEdit(PacketDetails);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(10, 10, 541, 631));

        retranslateUi(PacketDetails);

        QMetaObject::connectSlotsByName(PacketDetails);
    } // setupUi

    void retranslateUi(QDialog *PacketDetails)
    {
        PacketDetails->setWindowTitle(QApplication::translate("PacketDetails", "Packet details", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class PacketDetails: public Ui_PacketDetails {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PACKETDETAILS_H
