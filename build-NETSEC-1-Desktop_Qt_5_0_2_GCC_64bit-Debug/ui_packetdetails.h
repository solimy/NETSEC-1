/********************************************************************************
** Form generated from reading UI file 'packetdetails.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
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
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_PacketDetails
{
public:
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *PacketDetails)
    {
        if (PacketDetails->objectName().isEmpty())
            PacketDetails->setObjectName(QStringLiteral("PacketDetails"));
        PacketDetails->resize(400, 300);
        buttonBox = new QDialogButtonBox(PacketDetails);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        retranslateUi(PacketDetails);
        QObject::connect(buttonBox, SIGNAL(accepted()), PacketDetails, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), PacketDetails, SLOT(reject()));

        QMetaObject::connectSlotsByName(PacketDetails);
    } // setupUi

    void retranslateUi(QDialog *PacketDetails)
    {
        PacketDetails->setWindowTitle(QApplication::translate("PacketDetails", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class PacketDetails: public Ui_PacketDetails {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PACKETDETAILS_H
