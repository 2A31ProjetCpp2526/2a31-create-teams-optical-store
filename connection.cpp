#include "connection.h"
Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("opticstor");//inserer le nom de la source de données
    db.setUserName("opticstores");//inserer nom de l'utilisateur
    db.setPassword("mohamed123");//inserer mot de passe de cet utilisateur

    if (db.open())
        test=true;





    return  test;
}


