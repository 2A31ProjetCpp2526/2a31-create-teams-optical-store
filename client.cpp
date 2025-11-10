#include "client.h"
#include <QSqlQuery>
#include <QVariant>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSqlError> // ← AJOUTER CET INCLUDE
#include <QDebug>    // ← AJOUTER CET INCLUDE

Client::Client() {
    id_client = 0;
    nom = prenom = email = telephone = "";
}

Client::Client(int id, QString n, QString p, QString e, QString t) {
    id_client = id;
    nom = n;
    prenom = p;
    email = e;
    telephone = t;
}

int Client::getId() { return id_client; }
QString Client::getNom() { return nom; }
QString Client::getPrenom() { return prenom; }
QString Client::getEmail() { return email; }
QString Client::getTelephone() { return telephone; }

// Validation du téléphone (uniquement des chiffres, 8-15 chiffres)
bool Client::estTelephoneValide(const QString& telephone) {
    QRegularExpression regex("^[0-9]{8,15}$");
    return regex.match(telephone).hasMatch();
}

// Validation de l'email
bool Client::estEmailValide(const QString& email) {
    QRegularExpression regex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    return regex.match(email).hasMatch();
}

bool Client::ajouter()
{
    // Validation des données avec messages détaillés
    if (id_client <= 0) {
        qDebug() << "Erreur: ID client invalide:" << id_client;
        return false;
    }

    if (nom.isEmpty()) {
        qDebug() << "Erreur: Nom vide";
        return false;
    }

    if (prenom.isEmpty()) {
        qDebug() << "Erreur: Prénom vide";
        return false;
    }

    if (!estEmailValide(email)) {
        qDebug() << "Erreur: Email invalide:" << email;
        return false;
    }

    if (!estTelephoneValide(telephone)) {
        qDebug() << "Erreur: Téléphone invalide:" << telephone;
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO client (id_client, nom, prenom, email, telephone) "
                  "VALUES (:id_client, :nom, :prenom, :email, :telephone)");
    query.bindValue(":id_client", id_client);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);

    if (!query.exec()) {
        qDebug() << "Erreur SQL lors de l'ajout:" << query.lastError().text();
        return false;
    }

    return true;
}

QSqlQueryModel* Client::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM client");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Téléphone"));
    return model;
}

bool Client::supprimer(int id)
{
    if (id <= 0) {
        qDebug() << "Erreur: ID invalide pour suppression:" << id;
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM client WHERE id_client = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur SQL lors de la suppression:" << query.lastError().text();
        return false;
    }

    // Vérifier si une ligne a été supprimée
    if (query.numRowsAffected() <= 0) {
        qDebug() << "Aucun client trouvé avec ID:" << id;
        return false;
    }

    return true;
}

bool Client::modifier()
{
    // Validation des données avec messages détaillés
    if (id_client <= 0) {
        qDebug() << "Erreur: ID client invalide pour modification:" << id_client;
        return false;
    }

    if (nom.isEmpty()) {
        qDebug() << "Erreur: Nom vide pour modification";
        return false;
    }

    if (prenom.isEmpty()) {
        qDebug() << "Erreur: Prénom vide pour modification";
        return false;
    }

    if (!estEmailValide(email)) {
        qDebug() << "Erreur: Email invalide pour modification:" << email;
        return false;
    }

    if (!estTelephoneValide(telephone)) {
        qDebug() << "Erreur: Téléphone invalide pour modification:" << telephone;
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE client SET nom = :nom, prenom = :prenom, email = :email, telephone = :telephone "
                  "WHERE id_client = :id_client");
    query.bindValue(":id_client", id_client);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);

    if (!query.exec()) {
        qDebug() << "Erreur SQL lors de la modification:" << query.lastError().text();
        return false;
    }

    // Vérifier si une ligne a été modifiée
    if (query.numRowsAffected() <= 0) {
        qDebug() << "Aucun client trouvé avec ID pour modification:" << id_client;
        return false;
    }

    return true;
}
