/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QTableView *tableView;
    QLabel *label;
    QComboBox *comboBox;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QLabel *label_2;
    QComboBox *comboBox_2;
    QComboBox *comboBox_3;
    QPushButton *pushButton_2;
    QTableWidget *tableWidget;
    QScrollBar *verticalScrollBar;
    QPushButton *pushButton_3;
    QLineEdit *lineEdit_2;
    QLabel *label_3;
    QWidget *tab_2;
    QTableView *tableView_2;
    QLabel *label_4;
    QLineEdit *lineEdit_3;
    QLabel *label_5;
    QLineEdit *lineEdit_4;
    QLabel *label_6;
    QLabel *label_7;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_6;
    QLabel *label_8;
    QComboBox *comboBox_4;
    QPushButton *pushButton_4;
    QLabel *label_14;
    QPushButton *pushButton_7;
    QWidget *tab_3;
    QTableView *tableView_4;
    QLabel *label_10;
    QLabel *label_11;
    QLineEdit *lineEdit_7;
    QLabel *label_12;
    QComboBox *comboBox_5;
    QPushButton *pushButton_5;
    QWidget *tab_4;
    QTableView *tableView_3;
    QLabel *label_9;
    QPlainTextEdit *plainTextEdit;
    QLabel *label_13;
    QLineEdit *lineEdit_8;
    QRadioButton *star1;
    QRadioButton *star2;
    QRadioButton *star3;
    QRadioButton *star4;
    QRadioButton *star5;
    QPushButton *pushButton_6;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(978, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(0, 0, 971, 571));
        tabWidget->setMaximumSize(QSize(971, 571));
        tabWidget->setStyleSheet(QString::fromUtf8(""));
        tab = new QWidget();
        tab->setObjectName("tab");
        tableView = new QTableView(tab);
        tableView->setObjectName("tableView");
        tableView->setGeometry(QRect(80, 10, 791, 491));
        tableView->setStyleSheet(QString::fromUtf8("background-color: rgb(239, 255, 253);"));
        label = new QLabel(tab);
        label->setObjectName("label");
        label->setGeometry(QRect(140, 80, 151, 31));
        label->setStyleSheet(QString::fromUtf8("background-color: rgb(239, 255, 253);"));
        comboBox = new QComboBox(tab);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(290, 80, 76, 31));
        comboBox->setStyleSheet(QString::fromUtf8("\n"
"\n"
"QComboBox::drop-down {\n"
"    background-color: rgb(150, 200, 255);  /* drop-down button */\n"
"	border-color: rgb(85, 255, 255);\n"
"	gridline-color: rgb(85, 255, 255);\n"
"    border-left: 1px solid gray;\n"
"    width: 25px;\n"
"}\n"
"\n"
"QComboBox QAbstractItemView {\n"
"    background-color: rgb(220, 240, 255);  /* drop-down list background */\n"
"    selection-background-color: rgb(100, 150, 200); /* highlight selection */\n"
"}\n"
""));
        lineEdit = new QLineEdit(tab);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(370, 80, 201, 31));
        pushButton = new QPushButton(tab);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(580, 80, 93, 31));
        pushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	color: rgb(255, 255, 255);\n"
"    \n"
"	background-color: rgb(0, 85, 127);\n"
"    border: 2px solid black;\n"
"    border-radius: 5px;\n"
"}\n"
""));
        label_2 = new QLabel(tab);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(170, 120, 101, 31));
        comboBox_2 = new QComboBox(tab);
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->setObjectName("comboBox_2");
        comboBox_2->setGeometry(QRect(260, 120, 76, 31));
        comboBox_2->setStyleSheet(QString::fromUtf8("\n"
"\n"
"QComboBox::drop-down {\n"
"    background-color: rgb(150, 200, 255);  /* drop-down button */\n"
"	border-color: rgb(85, 255, 255);\n"
"	gridline-color: rgb(85, 255, 255);\n"
"    border-left: 1px solid gray;\n"
"    width: 25px;\n"
"}\n"
"\n"
"QComboBox QAbstractItemView {\n"
"    background-color: rgb(220, 240, 255);  /* drop-down list background */\n"
"    selection-background-color: rgb(100, 150, 200); /* highlight selection */\n"
"}\n"
"\n"
""));
        comboBox_3 = new QComboBox(tab);
        comboBox_3->addItem(QString());
        comboBox_3->addItem(QString());
        comboBox_3->setObjectName("comboBox_3");
        comboBox_3->setGeometry(QRect(340, 120, 111, 31));
        comboBox_3->setStyleSheet(QString::fromUtf8("\n"
"\n"
"QComboBox::drop-down {\n"
"    background-color: rgb(150, 200, 255);  /* drop-down button */\n"
"	border-color: rgb(85, 255, 255);\n"
"	gridline-color: rgb(85, 255, 255);\n"
"    border-left: 1px solid gray;\n"
"    width: 25px;\n"
"}\n"
"\n"
"QComboBox QAbstractItemView {\n"
"    background-color: rgb(220, 240, 255);  /* drop-down list background */\n"
"    selection-background-color: rgb(100, 150, 200); /* highlight selection */\n"
"}\n"
""));
        pushButton_2 = new QPushButton(tab);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(460, 120, 93, 31));
        pushButton_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	color: rgb(255, 255, 255);\n"
"    \n"
"	background-color: rgb(0, 85, 127);\n"
"    border: 2px solid black;\n"
"    border-radius: 5px;\n"
"}\n"
""));
        tableWidget = new QTableWidget(tab);
        if (tableWidget->columnCount() < 6)
            tableWidget->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(90, 160, 751, 281));
        tableWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 85, 127);"));
        verticalScrollBar = new QScrollBar(tab);
        verticalScrollBar->setObjectName("verticalScrollBar");
        verticalScrollBar->setGeometry(QRect(840, 160, 16, 281));
        verticalScrollBar->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 85, 127);"));
        verticalScrollBar->setOrientation(Qt::Orientation::Vertical);
        pushButton_3 = new QPushButton(tab);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(460, 30, 93, 29));
        pushButton_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	color: rgb(255, 255, 255);\n"
"    \n"
"	background-color: rgb(0, 85, 127);\n"
"    border: 2px solid black;\n"
"    border-radius: 5px;\n"
"}\n"
"\n"
""));
        lineEdit_2 = new QLineEdit(tab);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(270, 30, 181, 31));
        label_3 = new QLabel(tab);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(120, 30, 141, 31));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        tableView_2 = new QTableView(tab_2);
        tableView_2->setObjectName("tableView_2");
        tableView_2->setGeometry(QRect(260, 20, 421, 481));
        tableView_2->setStyleSheet(QString::fromUtf8("background-color: rgb(239, 255, 253);"));
        label_4 = new QLabel(tab_2);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(300, 120, 101, 41));
        lineEdit_3 = new QLineEdit(tab_2);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(300, 160, 221, 26));
        label_5 = new QLabel(tab_2);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(300, 200, 81, 31));
        lineEdit_4 = new QLineEdit(tab_2);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setGeometry(QRect(300, 230, 221, 26));
        label_6 = new QLabel(tab_2);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(300, 270, 81, 31));
        label_7 = new QLabel(tab_2);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(300, 340, 81, 31));
        lineEdit_5 = new QLineEdit(tab_2);
        lineEdit_5->setObjectName("lineEdit_5");
        lineEdit_5->setGeometry(QRect(300, 300, 221, 26));
        lineEdit_6 = new QLineEdit(tab_2);
        lineEdit_6->setObjectName("lineEdit_6");
        lineEdit_6->setGeometry(QRect(300, 370, 221, 26));
        label_8 = new QLabel(tab_2);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(300, 400, 71, 41));
        comboBox_4 = new QComboBox(tab_2);
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->setObjectName("comboBox_4");
        comboBox_4->setGeometry(QRect(300, 450, 141, 31));
        comboBox_4->setStyleSheet(QString::fromUtf8("\n"
"\n"
"QComboBox::drop-down {\n"
"    background-color: rgb(150, 200, 255);  /* drop-down button */\n"
"	border-color: rgb(85, 255, 255);\n"
"	gridline-color: rgb(85, 255, 255);\n"
"    border-left: 1px solid gray;\n"
"    width: 25px;\n"
"}\n"
"\n"
"QComboBox QAbstractItemView {\n"
"    background-color: rgb(220, 240, 255);  /* drop-down list background */\n"
"    selection-background-color: rgb(100, 150, 200); /* highlight selection */\n"
"}\n"
"\n"
"\n"
""));
        pushButton_4 = new QPushButton(tab_2);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(550, 430, 91, 31));
        pushButton_4->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	color: rgb(255, 255, 255);\n"
"    \n"
"	background-color: rgb(0, 85, 127);\n"
"    border: 2px solid black;\n"
"    border-radius: 5px;\n"
"}\n"
""));
        label_14 = new QLabel(tab_2);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(400, 50, 141, 61));
        pushButton_7 = new QPushButton(tab_2);
        pushButton_7->setObjectName("pushButton_7");
        pushButton_7->setGeometry(QRect(550, 388, 93, 31));
        pushButton_7->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	color: rgb(255, 255, 255);\n"
"    \n"
"	background-color: rgb(0, 85, 127);\n"
"    border: 2px solid black;\n"
"    border-radius: 5px;\n"
"}\n"
""));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        tableView_4 = new QTableView(tab_3);
        tableView_4->setObjectName("tableView_4");
        tableView_4->setGeometry(QRect(90, 10, 791, 491));
        tableView_4->setStyleSheet(QString::fromUtf8("background-color: rgb(239, 255, 253);"));
        label_10 = new QLabel(tab_3);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(350, 40, 271, 81));
        label_11 = new QLabel(tab_3);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(300, 140, 171, 41));
        lineEdit_7 = new QLineEdit(tab_3);
        lineEdit_7->setObjectName("lineEdit_7");
        lineEdit_7->setGeometry(QRect(300, 180, 291, 26));
        label_12 = new QLabel(tab_3);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(300, 220, 161, 31));
        comboBox_5 = new QComboBox(tab_3);
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->setObjectName("comboBox_5");
        comboBox_5->setGeometry(QRect(300, 250, 101, 31));
        comboBox_5->setStyleSheet(QString::fromUtf8("\n"
"\n"
"QComboBox::drop-down {\n"
"    background-color: rgb(150, 200, 255);  /* drop-down button */\n"
"	border-color: rgb(85, 255, 255);\n"
"	gridline-color: rgb(85, 255, 255);\n"
"    border-left: 1px solid gray;\n"
"    width: 25px;\n"
"}\n"
"\n"
"QComboBox QAbstractItemView {\n"
"    background-color: rgb(220, 240, 255);  /* drop-down list background */\n"
"    selection-background-color: rgb(100, 150, 200); /* highlight selection */\n"
"}\n"
"\n"
""));
        pushButton_5 = new QPushButton(tab_3);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(510, 340, 93, 29));
        pushButton_5->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	color: rgb(255, 255, 255);\n"
"    \n"
"	background-color: rgb(0, 85, 127);\n"
"    border: 2px solid black;\n"
"    border-radius: 5px;\n"
"}\n"
""));
        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName("tab_4");
        tableView_3 = new QTableView(tab_4);
        tableView_3->setObjectName("tableView_3");
        tableView_3->setGeometry(QRect(70, 20, 791, 491));
        tableView_3->setStyleSheet(QString::fromUtf8("background-color: rgb(239, 255, 253);"));
        label_9 = new QLabel(tab_4);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(290, 40, 341, 111));
        plainTextEdit = new QPlainTextEdit(tab_4);
        plainTextEdit->setObjectName("plainTextEdit");
        plainTextEdit->setGeometry(QRect(270, 260, 351, 171));
        label_13 = new QLabel(tab_4);
        label_13->setObjectName("label_13");
        label_13->setGeometry(QRect(350, 160, 181, 31));
        lineEdit_8 = new QLineEdit(tab_4);
        lineEdit_8->setObjectName("lineEdit_8");
        lineEdit_8->setGeometry(QRect(340, 200, 211, 26));
        star1 = new QRadioButton(tab_4);
        star1->setObjectName("star1");
        star1->setGeometry(QRect(360, 230, 21, 24));
        star2 = new QRadioButton(tab_4);
        star2->setObjectName("star2");
        star2->setGeometry(QRect(440, 230, 21, 24));
        star3 = new QRadioButton(tab_4);
        star3->setObjectName("star3");
        star3->setGeometry(QRect(400, 230, 21, 24));
        star4 = new QRadioButton(tab_4);
        star4->setObjectName("star4");
        star4->setGeometry(QRect(480, 230, 21, 24));
        star5 = new QRadioButton(tab_4);
        star5->setObjectName("star5");
        star5->setGeometry(QRect(520, 230, 21, 24));
        pushButton_6 = new QPushButton(tab_4);
        pushButton_6->setObjectName("pushButton_6");
        pushButton_6->setGeometry(QRect(390, 450, 121, 31));
        pushButton_6->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	color: rgb(255, 255, 255);\n"
"    \n"
"	background-color: rgb(0, 85, 127);\n"
"    border: 2px solid black;\n"
"    border-radius: 5px;\n"
"}\n"
""));
        tabWidget->addTab(tab_4, QString());
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 978, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:11pt; font-weight:700; font-style:italic;\">Rechercher Par :</span></p></body></html>", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Nom", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "ID", nullptr));

        pushButton->setText(QCoreApplication::translate("MainWindow", "Recherher", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:11pt; font-weight:700; font-style:italic;\">Trier Par :</span></p></body></html>", nullptr));
        comboBox_2->setItemText(0, QCoreApplication::translate("MainWindow", "Score", nullptr));
        comboBox_2->setItemText(1, QCoreApplication::translate("MainWindow", "Nom", nullptr));

        comboBox_3->setItemText(0, QCoreApplication::translate("MainWindow", "Decroissant", nullptr));
        comboBox_3->setItemText(1, QCoreApplication::translate("MainWindow", "Croissant", nullptr));

        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Trier", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Prenom", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "N\302\260Tel", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Type", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:11pt; font-weight:700; font-style:italic;\">ID \303\240 supprimer :</span></p><p><br/></p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Acceuil", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:700;\">Prenom :</span></p></body></html>", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:700;\">Nom :</span></p></body></html>", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:700;\">N\302\260Tel :</span></p></body></html>", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:700;\">Email :</span></p></body></html>", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:700;\">Type :</span></p></body></html>", nullptr));
        comboBox_4->setItemText(0, QCoreApplication::translate("MainWindow", "Lunette De Vue", nullptr));
        comboBox_4->setItemText(1, QCoreApplication::translate("MainWindow", "Lunette Soleil", nullptr));

        pushButton_4->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:22pt; font-weight:700; font-style:italic;\">Ajouter</span></p></body></html>", nullptr));
        pushButton_7->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:22pt; font-weight:700; font-style:italic;\">Ravitalement</span></p></body></html>", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:700;\">Nom du produit :</span></p></body></html>", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:700;\">Quantit\303\251 :</span></p></body></html>", nullptr));
        comboBox_5->setItemText(0, QCoreApplication::translate("MainWindow", "1", nullptr));
        comboBox_5->setItemText(1, QCoreApplication::translate("MainWindow", "2", nullptr));
        comboBox_5->setItemText(2, QCoreApplication::translate("MainWindow", "3", nullptr));
        comboBox_5->setItemText(3, QCoreApplication::translate("MainWindow", "4", nullptr));
        comboBox_5->setItemText(4, QCoreApplication::translate("MainWindow", "5", nullptr));
        comboBox_5->setItemText(5, QCoreApplication::translate("MainWindow", "6", nullptr));
        comboBox_5->setItemText(6, QCoreApplication::translate("MainWindow", "7", nullptr));
        comboBox_5->setItemText(7, QCoreApplication::translate("MainWindow", "8", nullptr));
        comboBox_5->setItemText(8, QCoreApplication::translate("MainWindow", "9", nullptr));
        comboBox_5->setItemText(9, QCoreApplication::translate("MainWindow", "10", nullptr));
        comboBox_5->setItemText(10, QCoreApplication::translate("MainWindow", "11", nullptr));
        comboBox_5->setItemText(11, QCoreApplication::translate("MainWindow", "12", nullptr));
        comboBox_5->setItemText(12, QCoreApplication::translate("MainWindow", "13", nullptr));
        comboBox_5->setItemText(13, QCoreApplication::translate("MainWindow", "14", nullptr));
        comboBox_5->setItemText(14, QCoreApplication::translate("MainWindow", "15", nullptr));
        comboBox_5->setItemText(15, QCoreApplication::translate("MainWindow", "16", nullptr));
        comboBox_5->setItemText(16, QCoreApplication::translate("MainWindow", "17", nullptr));
        comboBox_5->setItemText(17, QCoreApplication::translate("MainWindow", "18", nullptr));
        comboBox_5->setItemText(18, QCoreApplication::translate("MainWindow", "19", nullptr));
        comboBox_5->setItemText(19, QCoreApplication::translate("MainWindow", "20", nullptr));
        comboBox_5->setItemText(20, QCoreApplication::translate("MainWindow", "21", nullptr));
        comboBox_5->setItemText(21, QCoreApplication::translate("MainWindow", "22", nullptr));
        comboBox_5->setItemText(22, QCoreApplication::translate("MainWindow", "23", nullptr));
        comboBox_5->setItemText(23, QCoreApplication::translate("MainWindow", "24", nullptr));
        comboBox_5->setItemText(24, QCoreApplication::translate("MainWindow", "25", nullptr));
        comboBox_5->setItemText(25, QCoreApplication::translate("MainWindow", "26", nullptr));
        comboBox_5->setItemText(26, QCoreApplication::translate("MainWindow", "27", nullptr));
        comboBox_5->setItemText(27, QCoreApplication::translate("MainWindow", "28", nullptr));
        comboBox_5->setItemText(28, QCoreApplication::translate("MainWindow", "29", nullptr));
        comboBox_5->setItemText(29, QCoreApplication::translate("MainWindow", "30", nullptr));
        comboBox_5->setItemText(30, QCoreApplication::translate("MainWindow", "31", nullptr));
        comboBox_5->setItemText(31, QCoreApplication::translate("MainWindow", "32", nullptr));
        comboBox_5->setItemText(32, QCoreApplication::translate("MainWindow", "33", nullptr));
        comboBox_5->setItemText(33, QCoreApplication::translate("MainWindow", "34", nullptr));
        comboBox_5->setItemText(34, QCoreApplication::translate("MainWindow", "35", nullptr));
        comboBox_5->setItemText(35, QCoreApplication::translate("MainWindow", "36", nullptr));
        comboBox_5->setItemText(36, QCoreApplication::translate("MainWindow", "37", nullptr));
        comboBox_5->setItemText(37, QCoreApplication::translate("MainWindow", "38", nullptr));
        comboBox_5->setItemText(38, QCoreApplication::translate("MainWindow", "39", nullptr));
        comboBox_5->setItemText(39, QCoreApplication::translate("MainWindow", "40", nullptr));
        comboBox_5->setItemText(40, QCoreApplication::translate("MainWindow", "41", nullptr));
        comboBox_5->setItemText(41, QCoreApplication::translate("MainWindow", "42", nullptr));
        comboBox_5->setItemText(42, QCoreApplication::translate("MainWindow", "43", nullptr));
        comboBox_5->setItemText(43, QCoreApplication::translate("MainWindow", "44", nullptr));
        comboBox_5->setItemText(44, QCoreApplication::translate("MainWindow", "45", nullptr));
        comboBox_5->setItemText(45, QCoreApplication::translate("MainWindow", "46", nullptr));
        comboBox_5->setItemText(46, QCoreApplication::translate("MainWindow", "47", nullptr));
        comboBox_5->setItemText(47, QCoreApplication::translate("MainWindow", "48", nullptr));
        comboBox_5->setItemText(48, QCoreApplication::translate("MainWindow", "49", nullptr));
        comboBox_5->setItemText(49, QCoreApplication::translate("MainWindow", "50", nullptr));

        pushButton_5->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("MainWindow", "Ravitalement", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:22pt; font-weight:700; font-style:italic;\">Donner Votre Avis</span></p></body></html>", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:700;\">ID du fournisseur :</span></p></body></html>", nullptr));
        star1->setText(QString());
        star2->setText(QString());
        star3->setText(QString());
        star4->setText(QString());
        star5->setText(QString());
        pushButton_6->setText(QCoreApplication::translate("MainWindow", "Envoyer", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QCoreApplication::translate("MainWindow", "Feedback", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
