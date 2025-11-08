#include "connection.h"
#include <QMessageBox>
#include "opticstor.h"   // ✅ à la place de "mainwindow.h"
#include <QApplication>
#include "client.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Connection c;

    bool test = c.createconnect();
    if (test) {
        QMessageBox::information(nullptr, QObject::tr("Connexion réussie"),
                                 QObject::tr("Connexion établie avec succès."));
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Échec de connexion"),
                              QObject::tr("Impossible d'établir la connexion."));
    }

    opticstor w;   // ✅ à la place de MainWindow
    w.show();

    return a.exec();
}


