#include "connection.h"
Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("base_projet");//inserer le nom de la source de données
    db.setUserName("optic2");//inserer nom de l'utilisateur
    db.setPassword("fongolax2");//inserer mot de passe de cet utilisateur

    if (db.open())
        test=true;





    return  test;
}


