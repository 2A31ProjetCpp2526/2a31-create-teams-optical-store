#include "mainwindow.h"
#include "connexion.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Connection c;
    if (!c.CreateConnexion()) {
        QMessageBox::critical(nullptr, QObject::tr("Database Connection"),
                              QObject::tr("Connection failed.\nThe application will exit."),
                              QMessageBox::Ok);
        return -1; // exit app if DB fails
    }

    QMessageBox::information(nullptr, QObject::tr("Database Connection"),
                             QObject::tr("Connection successful."),
                             QMessageBox::Ok);

    MainWindow w;
    w.show();
    return a.exec();
}
