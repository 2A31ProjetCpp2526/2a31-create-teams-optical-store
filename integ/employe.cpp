#include "employe.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

employe::employe() {}

employe::employe(QString nom, QString prenom, float salaire, QDate date_emb, QString type)
{
    NOM = nom;
    PRENOM = prenom;
    SALAIRE = salaire;
    DATE_EMB = date_emb;
    TYPE = type;
}

bool employe::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO EMPLOYE (NOM, PRENOM, DATE_EMB, SALAIRE, TYPE) "
                  "VALUES (:nom, :prenom, TO_DATE(:date_emb, 'YYYY-MM-DD'), :salaire, :type)");

    query.bindValue(":nom", NOM);
    query.bindValue(":prenom", PRENOM);
    query.bindValue(":date_emb", DATE_EMB.toString("yyyy-MM-dd"));
    query.bindValue(":salaire", SALAIRE);
    query.bindValue(":type", TYPE);

    if (query.exec()) {
        qDebug() << "✅ Employé ajouté avec succès.";
        return true;
    } else {
        return false;
    }
}

QSqlQueryModel* employe::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID, NOM, PRENOM, TO_CHAR(DATE_EMB, 'YYYY-MM-DD') AS DATE_EMB, SALAIRE, TYPE FROM EMPLOYE");
    return model;
}

bool employe::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM EMPLOYE WHERE ID = :id");
    query.bindValue(":id", id);
    return query.exec();
}

bool employe::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYE SET NOM = :nom, PRENOM = :prenom, DATE_EMB = TO_DATE(:date_emb, 'YYYY-MM-DD'), SALAIRE = :salaire, TYPE = :type WHERE ID = :id");

    query.bindValue(":nom", NOM);
    query.bindValue(":prenom", PRENOM);
    query.bindValue(":date_emb", DATE_EMB.toString("yyyy-MM-dd"));
    query.bindValue(":salaire", SALAIRE);
    query.bindValue(":type", TYPE);
    query.bindValue(":id", ID_EMP);

    return query.exec();
}

bool employe::recuperer(int id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYE WHERE ID = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        ID_EMP = query.value("ID").toInt();
        NOM = query.value("NOM").toString();
        PRENOM = query.value("PRENOM").toString();
        SALAIRE = query.value("SALAIRE").toFloat();
        DATE_EMB = query.value("DATE_EMB").toDate();
        TYPE = query.value("TYPE").toString();
        return true;
    }
    return false;
}
