#include "connexion.h"
#include <QDebug>
#include <QSqlError>

Connection::Connection() {

    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("opticstore");
    db.setUserName("mohamed");
    db.setPassword("mohamed17");

    if (!db.open()) {
        qDebug() << "Erreur de connexion BD:" << db.lastError().text();
    } else {
        qDebug() << "Connexion BD réussie ✅";
    }
}

bool Connection::CreateConnexion() {
    bool test = false;
    if (db.open()) {
        test = true;
    } else {
        qDebug() << "Database Error:" << db.lastError().text();
    }
    return test;
}

void Connection::FermerConnexion() {
    if (db.isOpen()) {
        db.close();
    }
}

bool Connection::OuvrirConnexion() {
    return db.open();
}
