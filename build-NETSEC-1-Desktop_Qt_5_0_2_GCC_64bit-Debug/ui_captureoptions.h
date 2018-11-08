/********************************************************************************
** Form generated from reading UI file 'captureoptions.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAPTUREOPTIONS_H
#define UI_CAPTUREOPTIONS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_CaptureOptions
{
public:
    QFrame *frame;
    QPushButton *pushButton;
    QFrame *frame_2;
    QPushButton *pushButton_2;
    QLineEdit *lineEdit;
    QFrame *frame_3;
    QPushButton *pushButton_3;

    void setupUi(QDialog *CaptureOptions)
    {
        if (CaptureOptions->objectName().isEmpty())
            CaptureOptions->setObjectName(QStringLiteral("CaptureOptions"));
        CaptureOptions->resize(841, 277);
        frame = new QFrame(CaptureOptions);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(350, 10, 161, 80));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        pushButton = new QPushButton(frame);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 10, 141, 61));
        frame_2 = new QFrame(CaptureOptions);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setGeometry(QRect(10, 100, 821, 80));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        pushButton_2 = new QPushButton(frame_2);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(10, 10, 141, 61));
        lineEdit = new QLineEdit(frame_2);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(160, 10, 651, 61));
        frame_3 = new QFrame(CaptureOptions);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setGeometry(QRect(350, 190, 161, 80));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        pushButton_3 = new QPushButton(frame_3);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(10, 10, 141, 61));

        retranslateUi(CaptureOptions);

        QMetaObject::connectSlotsByName(CaptureOptions);
    } // setupUi

    void retranslateUi(QDialog *CaptureOptions)
    {
        CaptureOptions->setWindowTitle(QApplication::translate("CaptureOptions", "Dialog", 0));
        pushButton->setText(QApplication::translate("CaptureOptions", "Capture from Network", 0));
        pushButton_2->setText(QApplication::translate("CaptureOptions", "Load from File", 0));
        pushButton_3->setText(QApplication::translate("CaptureOptions", "Stop Capture", 0));
    } // retranslateUi

};

namespace Ui {
    class CaptureOptions: public Ui_CaptureOptions {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAPTUREOPTIONS_H
