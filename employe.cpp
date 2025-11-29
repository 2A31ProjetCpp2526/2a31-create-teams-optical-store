#include "employe.h"
#include <QSqlError>

employe::employe()
{
    id_employe = 0;
    nom = "";
    prenom = "";
    date_naissance = QDate::currentDate();
    departement = "";
}

employe::employe(int id_employe, QString nom, QString prenom, QDate date_naissance, QString departement)
{
    this->id_employe = id_employe;
    this->nom = nom;
    this->prenom = prenom;
    this->date_naissance = date_naissance;
    this->departement = departement;
}

bool employe::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO employe (id_employe, nom, prenom, date_naissance, departement) "
                  "VALUES (:id_employe, :nom, :prenom, :date_naissance, :departement)");

    query.bindValue(":id_employe", id_employe);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":date_naissance", date_naissance);
    query.bindValue(":departement", departement);

    return query.exec();
}

bool employe::supprimer(int id_employe)
{
    QSqlQuery query;
    query.prepare("DELETE FROM employe WHERE id_employe = :id_employe");
    query.bindValue(":id_employe", id_employe);

    return query.exec();
}

bool employe::modifier(int id_employe, QString nom, QString prenom, QDate date_naissance, QString departement)
{
    QSqlQuery query;
    query.prepare("UPDATE employe SET nom = :nom, prenom = :prenom, "
                  "date_naissance = :date_naissance, departement = :departement "
                  "WHERE id_employe = :id_employe");

    query.bindValue(":id_employe", id_employe);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":date_naissance", date_naissance);
    query.bindValue(":departement", departement);

    return query.exec();
}

QSqlQueryModel* employe::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT id_employe, nom, prenom, date_naissance, departement FROM employe");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id_employe"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date Naissance"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Département"));

    return model;
}

QSqlQueryModel* employe::rechercherParCIN(int id_employe)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT id_employe, nom, prenom, date_naissance, departement FROM employe WHERE id = :id");
    query.bindValue(":id_employe", id_employe);
    query.exec();

    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id_employe"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date Naissance"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Département"));

    return model;
}

bool employe::recuperer(int id_employe)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM employe WHERE id_employe = :id_employe");
    query.bindValue(":id_employe", id_employe);

    if (query.exec() && query.next()) {
        this->id_employe = query.value("id_employe").toInt();
        this->nom = query.value("nom").toString();
        this->prenom = query.value("prenom").toString();
        this->date_naissance = query.value("date_naissance").toDate();
        this->departement = query.value("departement").toString();
        return true;
    }
    return false;
}
