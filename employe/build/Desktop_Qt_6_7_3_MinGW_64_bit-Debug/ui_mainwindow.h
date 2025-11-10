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
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tab_4;
    QFrame *frame_2;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QPushButton *pushButton_8;
    QPushButton *pushButton_9;
    QFrame *frame_3;
    QLabel *label_6;
    QLabel *label_7;
    QWidget *tab;
    QTableWidget *tableWidget;
    QDateEdit *date;
    QPushButton *ajouter;
    QPushButton *annuler;
    QLabel *label_14;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_18;
    QLineEdit *prenom;
    QLineEdit *nom;
    QLineEdit *salaire;
    QLabel *label_19;
    QLabel *label_5;
    QLineEdit *type;
    QTableView *tableView;
    QPushButton *afficher;
    QWidget *tab_5;
    QTableWidget *tableWidget_2;
    QLabel *label_22;
    QLineEdit *id_supp;
    QLabel *label_23;
    QTextEdit *textEdit;
    QPushButton *supprimer;
    QCalendarWidget *calendarWidget;
    QLabel *label_24;
    QLabel *label_4;
    QWidget *tab_6;
    QLabel *label_8;
    QTableWidget *tableWidget_4;
    QLineEdit *id_mod;
    QLineEdit *nom_mod;
    QLineEdit *prenom_mod;
    QLineEdit *type_mod;
    QLabel *label_20;
    QLabel *label_21;
    QLabel *label_25;
    QLabel *label_26;
    QLabel *label_27;
    QDateEdit *date_mod;
    QPushButton *modifier;
    QWidget *tab_2;
    QLabel *label;
    QLabel *label_2;
    QTableWidget *tableWidget_3;
    QWidget *tab_3;
    QLabel *label_3;
    QTextEdit *textEdit_2;
    QLineEdit *lineEdit;
    QFrame *frame;
    QPushButton *pushButton_3;
    QLabel *label_11;
    QMenuBar *menubar;
    QMenu *menuajouter;
    QMenu *menuperfomance;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        MainWindow->setMaximumSize(QSize(800, 16777215));
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setStyleSheet(QString::fromUtf8("* {\n"
"    font-family: \"Segoe UI\";\n"
"    font-size: 12pt;\n"
"}\n"
""));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(-20, 30, 701, 361));
        tabWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(216, 217, 255);\n"
"\n"
"font: 700 12pt \"Segoe UI\";\n"
"color: rgb(0, 0, 0);"));
        tab_4 = new QWidget();
        tab_4->setObjectName("tab_4");
        frame_2 = new QFrame(tab_4);
        frame_2->setObjectName("frame_2");
        frame_2->setGeometry(QRect(10, 10, 120, 311));
        frame_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        frame_2->setFrameShape(QFrame::Shape::StyledPanel);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        pushButton_6 = new QPushButton(frame_2);
        pushButton_6->setObjectName("pushButton_6");
        pushButton_6->setGeometry(QRect(20, 140, 81, 31));
        pushButton_6->setStyleSheet(QString::fromUtf8("font: 600 italic 8pt \"Segoe UI\";"));
        pushButton_7 = new QPushButton(frame_2);
        pushButton_7->setObjectName("pushButton_7");
        pushButton_7->setGeometry(QRect(20, 190, 81, 31));
        pushButton_7->setStyleSheet(QString::fromUtf8("font: 600 italic 8pt \"Segoe UI\";"));
        pushButton_8 = new QPushButton(frame_2);
        pushButton_8->setObjectName("pushButton_8");
        pushButton_8->setGeometry(QRect(20, 240, 81, 31));
        pushButton_8->setStyleSheet(QString::fromUtf8("font: 600 italic 8pt \"Segoe UI\";"));
        pushButton_9 = new QPushButton(frame_2);
        pushButton_9->setObjectName("pushButton_9");
        pushButton_9->setGeometry(QRect(20, 90, 81, 31));
        pushButton_9->setStyleSheet(QString::fromUtf8("font: 600 italic 8pt \"Segoe UI\";"));
        frame_3 = new QFrame(frame_2);
        frame_3->setObjectName("frame_3");
        frame_3->setGeometry(QRect(20, 10, 81, 61));
        frame_3->setFrameShape(QFrame::Shape::StyledPanel);
        frame_3->setFrameShadow(QFrame::Shadow::Raised);
        label_6 = new QLabel(tab_4);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(-150, -20, 701, 361));
        label_6->setPixmap(QPixmap(QString::fromUtf8("../../../Downloads/c9d03170-aced-4d4f-8414-d7da4814c40d.jpg")));
        label_7 = new QLabel(tab_4);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(290, 130, 371, 51));
        label_7->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: rgb(230, 234, 235);\n"
"	font: 700 28pt \"Segoe UI\";\n"
"    color: #800000; /* dark red like your other slides */\n"
"    font-family: \"Georgia\";\n"
"}\n"
""));
        tabWidget->addTab(tab_4, QString());
        label_6->raise();
        frame_2->raise();
        label_7->raise();
        tab = new QWidget();
        tab->setObjectName("tab");
        tableWidget = new QTableWidget(tab);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(20, 30, 221, 251));
        tableWidget->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgb(216, 217, 255);"));
        date = new QDateEdit(tab);
        date->setObjectName("date");
        date->setGeometry(QRect(119, 140, 111, 31));
        date->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"background-color: rgb(204, 202, 255);"));
        ajouter = new QPushButton(tab);
        ajouter->setObjectName("ajouter");
        ajouter->setGeometry(QRect(130, 250, 80, 18));
        ajouter->setStyleSheet(QString::fromUtf8("font: 700 italic 9pt \"Segoe UI\";\n"
"background-color:rgb(175, 161, 255);\n"
"color: rgb(0, 0, 0);\n"
""));
        annuler = new QPushButton(tab);
        annuler->setObjectName("annuler");
        annuler->setGeometry(QRect(30, 250, 80, 18));
        annuler->setStyleSheet(QString::fromUtf8("font: 700 italic 9pt \"Segoe UI\";\n"
"background-color:rgb(175, 161, 255);\n"
"color: rgb(0, 0, 0);\n"
""));
        label_14 = new QLabel(tab);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(30, 140, 81, 20));
        label_14->setMinimumSize(QSize(0, 20));
        label_14->setStyleSheet(QString::fromUtf8("font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(216, 217, 255);\n"
"font: 8pt \"Segoe UI\";\n"
"color: rgb(0, 0, 0);\n"
"font: 700 9pt \"Segoe UI\";\n"
""));
        label_16 = new QLabel(tab);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(30, 100, 51, 20));
        label_16->setMinimumSize(QSize(0, 20));
        label_16->setStyleSheet(QString::fromUtf8("font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(216, 217, 255);\n"
"font: 8pt \"Segoe UI\";\n"
"color: rgb(0, 0, 0);\n"
"font: 700 9pt \"Segoe UI\";\n"
""));
        label_17 = new QLabel(tab);
        label_17->setObjectName("label_17");
        label_17->setGeometry(QRect(30, 60, 41, 20));
        label_17->setMinimumSize(QSize(0, 20));
        label_17->setStyleSheet(QString::fromUtf8("font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(216, 217, 255);\n"
"font: 8pt \"Segoe UI\";\n"
"color: rgb(0, 0, 0);\n"
"font: 700 9pt \"Segoe UI\";\n"
""));
        label_18 = new QLabel(tab);
        label_18->setObjectName("label_18");
        label_18->setGeometry(QRect(30, 180, 41, 20));
        label_18->setMinimumSize(QSize(0, 20));
        label_18->setStyleSheet(QString::fromUtf8("font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(216, 217, 255);\n"
"font: 8pt \"Segoe UI\";\n"
"color: rgb(0, 0, 0);\n"
"font: 700 9pt \"Segoe UI\";\n"
""));
        prenom = new QLineEdit(tab);
        prenom->setObjectName("prenom");
        prenom->setGeometry(QRect(120, 100, 91, 20));
        nom = new QLineEdit(tab);
        nom->setObjectName("nom");
        nom->setGeometry(QRect(120, 60, 91, 20));
        salaire = new QLineEdit(tab);
        salaire->setObjectName("salaire");
        salaire->setGeometry(QRect(120, 180, 91, 20));
        label_19 = new QLabel(tab);
        label_19->setObjectName("label_19");
        label_19->setGeometry(QRect(30, 220, 41, 20));
        label_19->setMinimumSize(QSize(0, 20));
        label_19->setStyleSheet(QString::fromUtf8("font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(216, 217, 255);\n"
"font: 8pt \"Segoe UI\";\n"
"color: rgb(0, 0, 0);\n"
"font: 700 9pt \"Segoe UI\";\n"
""));
        label_5 = new QLabel(tab);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(240, 10, 371, 31));
        label_5->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	\n"
"	background-color: rgb(214, 215, 253);\n"
"	font: 700 18pt \"Segoe UI\";\n"
"    color: #800000; /* dark red like your other slides */\n"
"    font-family: \"Georgia\";\n"
"}\n"
""));
        type = new QLineEdit(tab);
        type->setObjectName("type");
        type->setGeometry(QRect(110, 220, 113, 20));
        tableView = new QTableView(tab);
        tableView->setObjectName("tableView");
        tableView->setGeometry(QRect(250, 80, 321, 141));
        afficher = new QPushButton(tab);
        afficher->setObjectName("afficher");
        afficher->setGeometry(QRect(370, 50, 80, 18));
        afficher->setStyleSheet(QString::fromUtf8("font: 700 italic 9pt \"Segoe UI\";\n"
"background-color:rgb(175, 161, 255);\n"
"color: rgb(0, 0, 0);\n"
""));
        tabWidget->addTab(tab, QString());
        label_5->raise();
        tableWidget->raise();
        date->raise();
        ajouter->raise();
        annuler->raise();
        label_14->raise();
        label_16->raise();
        label_17->raise();
        label_18->raise();
        prenom->raise();
        nom->raise();
        salaire->raise();
        label_19->raise();
        type->raise();
        tableView->raise();
        afficher->raise();
        tab_5 = new QWidget();
        tab_5->setObjectName("tab_5");
        tableWidget_2 = new QTableWidget(tab_5);
        tableWidget_2->setObjectName("tableWidget_2");
        tableWidget_2->setGeometry(QRect(80, 60, 311, 231));
        tableWidget_2->setStyleSheet(QString::fromUtf8("background-color: rgb(215, 216, 254);"));
        label_22 = new QLabel(tab_5);
        label_22->setObjectName("label_22");
        label_22->setGeometry(QRect(100, 90, 81, 20));
        label_22->setMinimumSize(QSize(0, 20));
        label_22->setStyleSheet(QString::fromUtf8("font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(216, 217, 255);\n"
"font: 8pt \"Segoe UI\";\n"
"color: rgb(0, 0, 0);\n"
"font: 700 9pt \"Segoe UI\";\n"
""));
        id_supp = new QLineEdit(tab_5);
        id_supp->setObjectName("id_supp");
        id_supp->setGeometry(QRect(170, 90, 171, 20));
        label_23 = new QLabel(tab_5);
        label_23->setObjectName("label_23");
        label_23->setGeometry(QRect(90, 130, 81, 20));
        label_23->setMinimumSize(QSize(0, 20));
        label_23->setStyleSheet(QString::fromUtf8("font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(216, 217, 255);\n"
"font: 8pt \"Segoe UI\";\n"
"color: rgb(0, 0, 0);\n"
"font: 700 9pt \"Segoe UI\";\n"
""));
        textEdit = new QTextEdit(tab_5);
        textEdit->setObjectName("textEdit");
        textEdit->setGeometry(QRect(170, 130, 171, 101));
        supprimer = new QPushButton(tab_5);
        supprimer->setObjectName("supprimer");
        supprimer->setGeometry(QRect(290, 250, 80, 18));
        supprimer->setStyleSheet(QString::fromUtf8("font: 700 italic 9pt \"Segoe UI\";\n"
"background-color:rgb(175, 161, 255);\n"
"color: rgb(0, 0, 0);\n"
""));
        calendarWidget = new QCalendarWidget(tab_5);
        calendarWidget->setObjectName("calendarWidget");
        calendarWidget->setGeometry(QRect(410, 80, 231, 161));
        calendarWidget->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"background-color: rgb(216, 217, 255);"));
        label_24 = new QLabel(tab_5);
        label_24->setObjectName("label_24");
        label_24->setGeometry(QRect(410, 70, 231, 31));
        label_24->setMinimumSize(QSize(0, 20));
        label_24->setStyleSheet(QString::fromUtf8("font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(216, 217, 255);\n"
"font: 8pt \"Segoe UI\";\n"
"color: rgb(0, 0, 0);\n"
"font: 700 9pt \"Segoe UI\";\n"
""));
        label_4 = new QLabel(tab_5);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(230, 10, 371, 31));
        label_4->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: rgb(216, 217, 255);\n"
"	font: 700 18pt \"Segoe UI\";\n"
"    color: #800000; /* dark red like your other slides */\n"
"    font-family: \"Georgia\";\n"
"}\n"
""));
        tabWidget->addTab(tab_5, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName("tab_6");
        label_8 = new QLabel(tab_6);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(200, 20, 371, 31));
        label_8->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: rgb(216, 217, 255);\n"
"	font: 700 18pt \"Segoe UI\";\n"
"    color: #800000; /* dark red like your other slides */\n"
"    font-family: \"Georgia\";\n"
"}\n"
""));
        tableWidget_4 = new QTableWidget(tab_6);
        tableWidget_4->setObjectName("tableWidget_4");
        tableWidget_4->setGeometry(QRect(170, 60, 221, 251));
        tableWidget_4->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgb(216, 217, 255);"));
        id_mod = new QLineEdit(tab_6);
        id_mod->setObjectName("id_mod");
        id_mod->setGeometry(QRect(280, 80, 91, 20));
        nom_mod = new QLineEdit(tab_6);
        nom_mod->setObjectName("nom_mod");
        nom_mod->setGeometry(QRect(280, 120, 91, 20));
        prenom_mod = new QLineEdit(tab_6);
        prenom_mod->setObjectName("prenom_mod");
        prenom_mod->setGeometry(QRect(280, 160, 91, 20));
        type_mod = new QLineEdit(tab_6);
        type_mod->setObjectName("type_mod");
        type_mod->setGeometry(QRect(280, 240, 91, 20));
        label_20 = new QLabel(tab_6);
        label_20->setObjectName("label_20");
        label_20->setGeometry(QRect(200, 80, 41, 20));
        label_20->setMinimumSize(QSize(0, 20));
        label_20->setStyleSheet(QString::fromUtf8("font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(216, 217, 255);\n"
"font: 8pt \"Segoe UI\";\n"
"color: rgb(0, 0, 0);\n"
"font: 700 9pt \"Segoe UI\";\n"
""));
        label_21 = new QLabel(tab_6);
        label_21->setObjectName("label_21");
        label_21->setGeometry(QRect(200, 120, 41, 20));
        label_21->setMinimumSize(QSize(0, 20));
        label_21->setStyleSheet(QString::fromUtf8("font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(216, 217, 255);\n"
"font: 8pt \"Segoe UI\";\n"
"color: rgb(0, 0, 0);\n"
"font: 700 9pt \"Segoe UI\";\n"
""));
        label_25 = new QLabel(tab_6);
        label_25->setObjectName("label_25");
        label_25->setGeometry(QRect(200, 160, 41, 20));
        label_25->setMinimumSize(QSize(0, 20));
        label_25->setStyleSheet(QString::fromUtf8("font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(216, 217, 255);\n"
"font: 8pt \"Segoe UI\";\n"
"color: rgb(0, 0, 0);\n"
"font: 700 9pt \"Segoe UI\";\n"
""));
        label_26 = new QLabel(tab_6);
        label_26->setObjectName("label_26");
        label_26->setGeometry(QRect(200, 200, 41, 20));
        label_26->setMinimumSize(QSize(0, 20));
        label_26->setStyleSheet(QString::fromUtf8("font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(216, 217, 255);\n"
"font: 8pt \"Segoe UI\";\n"
"color: rgb(0, 0, 0);\n"
"font: 700 9pt \"Segoe UI\";\n"
""));
        label_27 = new QLabel(tab_6);
        label_27->setObjectName("label_27");
        label_27->setGeometry(QRect(200, 240, 41, 20));
        label_27->setMinimumSize(QSize(0, 20));
        label_27->setStyleSheet(QString::fromUtf8("font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(216, 217, 255);\n"
"font: 8pt \"Segoe UI\";\n"
"color: rgb(0, 0, 0);\n"
"font: 700 9pt \"Segoe UI\";\n"
""));
        date_mod = new QDateEdit(tab_6);
        date_mod->setObjectName("date_mod");
        date_mod->setGeometry(QRect(280, 200, 91, 22));
        date_mod->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"background-color: rgb(204, 202, 255);"));
        modifier = new QPushButton(tab_6);
        modifier->setObjectName("modifier");
        modifier->setGeometry(QRect(240, 280, 80, 18));
        modifier->setStyleSheet(QString::fromUtf8("font: 700 italic 9pt \"Segoe UI\";\n"
"background-color:rgb(175, 161, 255);\n"
"color: rgb(0, 0, 0);\n"
""));
        tabWidget->addTab(tab_6, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        label = new QLabel(tab_2);
        label->setObjectName("label");
        label->setGeometry(QRect(210, 20, 371, 31));
        label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: rgb(216, 217, 255);\n"
"	font: 700 18pt \"Segoe UI\";\n"
"    color: #800000; /* dark red like your other slides */\n"
"    font-family: \"Georgia\";\n"
"}\n"
""));
        label_2 = new QLabel(tab_2);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(40, 75, 491, 31));
        label_2->setStyleSheet(QString::fromUtf8("background-color: rgb(231, 236, 239);"));
        tableWidget_3 = new QTableWidget(tab_2);
        if (tableWidget_3->columnCount() < 3)
            tableWidget_3->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        if (tableWidget_3->rowCount() < 4)
            tableWidget_3->setRowCount(4);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_3->setVerticalHeaderItem(0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_3->setVerticalHeaderItem(1, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_3->setVerticalHeaderItem(2, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_3->setVerticalHeaderItem(3, __qtablewidgetitem6);
        tableWidget_3->setObjectName("tableWidget_3");
        tableWidget_3->setGeometry(QRect(10, 110, 631, 192));
        tableWidget_3->setMinimumSize(QSize(301, 192));
        tableWidget_3->setBaseSize(QSize(200, 300));
        QFont font;
        font.setFamilies({QString::fromUtf8("Segoe UI")});
        font.setPointSize(12);
        font.setBold(true);
        font.setItalic(false);
        tableWidget_3->setFont(font);
        tableWidget_3->setStyleSheet(QString::fromUtf8("QTableWidget {\n"
"    border: 1px solid #aaa;\n"
"    border-radius: 6px;\n"
"    gridline-color: #ccc;\n"
"    background: #f9f9f9;\n"
"}\n"
"QHeaderView::section {\n"
"    background-color: #d9d9d9;\n"
"    font-weight: bold;\n"
"}\n"
""));
        tableWidget_3->setDragDropMode(QAbstractItemView::DragDropMode::DragDrop);
        tableWidget_3->setDefaultDropAction(Qt::DropAction::MoveAction);
        tableWidget_3->setAlternatingRowColors(false);
        tableWidget_3->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
        tableWidget_3->setShowGrid(true);
        tableWidget_3->setGridStyle(Qt::PenStyle::SolidLine);
        tableWidget_3->setSortingEnabled(false);
        tableWidget_3->setWordWrap(true);
        tableWidget_3->setCornerButtonEnabled(false);
        tableWidget_3->setColumnCount(3);
        tableWidget_3->horizontalHeader()->setVisible(true);
        tableWidget_3->horizontalHeader()->setCascadingSectionResizes(true);
        tableWidget_3->horizontalHeader()->setDefaultSectionSize(200);
        tableWidget_3->horizontalHeader()->setStretchLastSection(true);
        tableWidget_3->verticalHeader()->setCascadingSectionResizes(true);
        tableWidget_3->verticalHeader()->setHighlightSections(true);
        tableWidget_3->verticalHeader()->setProperty("showSortIndicator", QVariant(true));
        tableWidget_3->verticalHeader()->setStretchLastSection(false);
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        label_3 = new QLabel(tab_3);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(0, 40, 391, 61));
        label_3->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: rgb(233, 237, 240);\n"
"	font: 700 18pt \"Segoe UI\";\n"
"    color: #800000; /* dark red like your other slides */\n"
"    font-family: \"Georgia\";\n"
"}\n"
""));
        textEdit_2 = new QTextEdit(tab_3);
        textEdit_2->setObjectName("textEdit_2");
        textEdit_2->setGeometry(QRect(20, 100, 491, 121));
        textEdit_2->setStyleSheet(QString::fromUtf8("QTextEdit { border: 1px solid #aaa; border-radius: 6px; background: #f9f9f9; }\n"
""));
        textEdit_2->setReadOnly(true);
        lineEdit = new QLineEdit(tab_3);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(30, 230, 381, 21));
        lineEdit->setStyleSheet(QString::fromUtf8("QLineEdit { border: 1px solid #aaa; border-radius: 6px; padding: 4px; }\n"
""));
        frame = new QFrame(tab_3);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(0, 80, 571, 191));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        pushButton_3 = new QPushButton(frame);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(420, 150, 81, 20));
        pushButton_3->setStyleSheet(QString::fromUtf8("QPushButton { background-color: #0078d7; color: white; border-radius: 6px; padding: 4px 8px; }\n"
"QPushButton:hover { background-color: #005a9e; }\n"
""));
        label_11 = new QLabel(tab_3);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(0, -10, 701, 361));
        label_11->setPixmap(QPixmap(QString::fromUtf8("../../../Downloads/c9d03170-aced-4d4f-8414-d7da4814c40d.jpg")));
        tabWidget->addTab(tab_3, QString());
        label_11->raise();
        label_3->raise();
        frame->raise();
        textEdit_2->raise();
        lineEdit->raise();
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 17));
        menuajouter = new QMenu(menubar);
        menuajouter->setObjectName("menuajouter");
        menuperfomance = new QMenu(menubar);
        menuperfomance->setObjectName("menuperfomance");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        QWidget::setTabOrder(tableWidget, ajouter);
        QWidget::setTabOrder(ajouter, annuler);
        QWidget::setTabOrder(annuler, date);
        QWidget::setTabOrder(date, prenom);
        QWidget::setTabOrder(prenom, nom);
        QWidget::setTabOrder(nom, salaire);
        QWidget::setTabOrder(salaire, tabWidget);

        menubar->addAction(menuajouter->menuAction());
        menubar->addAction(menuperfomance->menuAction());
        menuajouter->addSeparator();
        menuperfomance->addSeparator();
        menuperfomance->addSeparator();

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton_6->setText(QCoreApplication::translate("MainWindow", "\360\237\223\235client", nullptr));
        pushButton_7->setText(QCoreApplication::translate("MainWindow", "\360\237\217\252fournisseur", nullptr));
        pushButton_8->setText(QCoreApplication::translate("MainWindow", "\360\237\233\222produit", nullptr));
        pushButton_9->setText(QCoreApplication::translate("MainWindow", "\360\237\221\245employe", nullptr));
        label_6->setText(QString());
        label_7->setText(QCoreApplication::translate("MainWindow", "WELCOME", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QCoreApplication::translate("MainWindow", "acceuil", nullptr));
        ajouter->setText(QCoreApplication::translate("MainWindow", "ajouter", nullptr));
        annuler->setText(QCoreApplication::translate("MainWindow", "annuler", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "date d'aumbauche", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "prenom", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "nom", nullptr));
        label_18->setText(QCoreApplication::translate("MainWindow", "salaire", nullptr));
        label_19->setText(QCoreApplication::translate("MainWindow", "type", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\360\237\223\212 Ajouter", nullptr));
        afficher->setText(QCoreApplication::translate("MainWindow", "afficher", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "ajouter", nullptr));
        label_22->setText(QCoreApplication::translate("MainWindow", "id_employe", nullptr));
        label_23->setText(QCoreApplication::translate("MainWindow", "raison d'exclu :", nullptr));
        supprimer->setText(QCoreApplication::translate("MainWindow", "supprimer", nullptr));
        label_24->setText(QCoreApplication::translate("MainWindow", "date d'exclu", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QCoreApplication::translate("MainWindow", "supprimer", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "modifier", nullptr));
        label_20->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        label_21->setText(QCoreApplication::translate("MainWindow", "nom", nullptr));
        label_25->setText(QCoreApplication::translate("MainWindow", "prenom", nullptr));
        label_26->setText(QCoreApplication::translate("MainWindow", "date d'au", nullptr));
        label_27->setText(QCoreApplication::translate("MainWindow", "type", nullptr));
        modifier->setText(QCoreApplication::translate("MainWindow", "modifier", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_6), QCoreApplication::translate("MainWindow", "modifier", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\360\237\223\212 Performance", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Les indicateurs de performance :", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_3->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Indicateur", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_3->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Am\303\251lioration du rendement", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_3->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Suivi KPI en temps r\303\251el", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "discipline", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\360\237\222\254 Communication interne\n"
"", nullptr));
        lineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\303\211crire un message...", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "envoyer", nullptr));
        label_11->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("MainWindow", "performance", nullptr));
        menuajouter->setTitle(QCoreApplication::translate("MainWindow", "ajouter", nullptr));
        menuperfomance->setTitle(QCoreApplication::translate("MainWindow", "perfomance", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
