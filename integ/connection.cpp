#include "connection.h"
Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("base_projet");
    db.setUserName("optic2");
    db.setPassword("fongolax2");
    if (db.open())
        test=true;





    return  test;
}


