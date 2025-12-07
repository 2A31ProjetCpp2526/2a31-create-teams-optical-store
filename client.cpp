#include "client.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>

// Constructor
Client::Client(int id, QString nom, QString prenom, QString email, QString telephone, int age, QString statut)
    : id(id), nom(nom), prenom(prenom), email(email), telephone(telephone), age(age), statut(statut)
{
}

// Getters
int Client::getId() const { return id; }
QString Client::getNom() const { return nom; }
QString Client::getPrenom() const { return prenom; }
QString Client::getEmail() const { return email; }
QString Client::getTelephone() const { return telephone; }
int Client::getAge() const { return age; }
QString Client::getStatut() const { return statut; }

// Setters
void Client::setId(int newId) { id = newId; }
void Client::setNom(QString newNom) { nom = newNom; }
void Client::setPrenom(QString newPrenom) { prenom = newPrenom; }
void Client::setEmail(QString newEmail) { email = newEmail; }
void Client::setTelephone(QString newTelephone) { telephone = newTelephone; }
void Client::setAge(int newAge) { age = newAge; }
void Client::setStatut(QString newStatut) { statut = newStatut; }

// CRUD Operations
bool Client::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO CLIENTS (ID_CLIENT, NOM, PRENOM, EMAIL, TELEPHONE, AGE, STATUT) "
                  "VALUES (:id, :nom, :prenom, :email, :telephone, :age, :statut)");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":age", age);
    query.bindValue(":statut", statut);

    if (!query.exec()) {
        qDebug() << "Erreur SQL:" << query.lastError().text();  // <--- Log SQL error here
        return false;
    }
    return true;
    //return query.exec();
}

bool Client::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM CLIENTS WHERE ID_CLIENT = :id");
    query.bindValue(":id", id);
    return query.exec();
}

bool Client::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE CLIENTS SET NOM=:nom, PRENOM=:prenom, EMAIL=:email, "
                  "TELEPHONE=:telephone, AGE=:age, STATUT=:statut WHERE ID_CLIENT=:id");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":age", age);
    query.bindValue(":statut", statut);

    return query.exec();
}

QSqlQueryModel* Client::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID_CLIENT, NOM, PRENOM, EMAIL, TELEPHONE, AGE, STATUT FROM CLIENTS");
    return model;
}

// Status Management
bool Client::modifierStatut(int id, QString nouveauStatut)
{
    QSqlQuery query;
    query.prepare("UPDATE CLIENTS SET STATUT = :statut WHERE ID_CLIENT = :id");
    query.bindValue(":statut", nouveauStatut);
    query.bindValue(":id", id);
    return query.exec();
}

QSqlQueryModel* Client::rechercherParStatut(QString statut)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM CLIENTS WHERE STATUT = :statut");
    query.bindValue(":statut", statut);
    query.exec();
    model->setQuery(query);
    return model;
}

// Get all clients
QList<Client> Client::getAllClients()
{
    QList<Client> clients;
    QSqlQuery query("SELECT ID_CLIENT, NOM, PRENOM, EMAIL, TELEPHONE, AGE, STATUT FROM CLIENTS");

    while (query.next()) {
        Client client(
            query.value(0).toInt(),
            query.value(1).toString(),
            query.value(2).toString(),
            query.value(3).toString(),
            query.value(4).toString(),
            query.value(5).toInt(),
            query.value(6).toString()
            );
        clients.append(client);
    }
    return clients;
}

// Duration calculations (simplified - you'll need to implement based on your database structure)
QMap<QString, int> Client::getDureesTousStatuts() const
{
    QMap<QString, int> durees;
    // This is a simplified implementation
    // You'll need to implement based on your actual database structure
    durees["saisir"] = 2;
    durees["en cours"] = 3;
    durees["terminé"] = 1;
    durees["récupéré"] = 0;
    return durees;
}

// Feedback Management
bool Client::ajouterFeedback(int id_feedback, int id_client, QString type, QString categorie, QDate date, QString message)
{
    QSqlQuery query;
    query.prepare("INSERT INTO FEEDBACKS (ID_FEEDBACK, ID_CLIENT, TYPE, CATEGORIE, DATE_FEEDBACK, MESSAGE) "
                  "VALUES (:id_feedback, :id_client, :type, :categorie, :date, :message)");
    query.bindValue(":id_feedback", id_feedback);
    query.bindValue(":id_client", id_client);
    query.bindValue(":type", type);
    query.bindValue(":categorie", categorie);
    query.bindValue(":date", date);
    query.bindValue(":message", message);

    return query.exec();
}

QSqlQueryModel* Client::afficherTousLesFeedbacks()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM FEEDBACKS");
    return model;
}

// Validation methods
bool Client::estAgeValide(int age)
{
    return (age >= 0 && age <= 120);
}

bool Client::estStatutValide(QString statut)
{
    QStringList statutsValides = {"saisir", "en cours", "terminé", "récupéré"};
    return statutsValides.contains(statut.toLower());
}

// Sorting
QSqlQueryModel* Client::trier(QString critere)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryStr = "SELECT * FROM CLIENTS ORDER BY " + critere;
    model->setQuery(queryStr);
    return model;
}

// Search by ID
QSqlQueryModel* Client::rechercherParId(int id)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM CLIENTS WHERE ID_CLIENT = :id");
    query.bindValue(":id", id);
    query.exec();
    model->setQuery(query);
    return model;
}

// Age statistics
QMap<QString, int> Client::getStatistiquesAge()
{
    QMap<QString, int> stats;

    QSqlQuery query;
    query.exec("SELECT AGE FROM CLIENTS");

    int moins20 = 0, entre20et50 = 0, plus50 = 0;

    while (query.next()) {
        int age = query.value(0).toInt();
        if (age < 20) moins20++;
        else if (age <= 50) entre20et50++;
        else plus50++;
    }

    stats["Moins de 20 ans"] = moins20;
    stats["20-50 ans"] = entre20et50;
    stats["Plus de 50 ans"] = plus50;

    return stats;
}

// Prescription tests
bool Client::ajouterTestOrdonnance(QDate date, double ogCylindre, int ogAxe, double odCylindre, int odAxe)
{
    QSqlQuery query;
    query.prepare("INSERT INTO TESTS_ORDONNANCE (ID_CLIENT, DATE_TEST, OG_CYLINDRE, OG_AXE, OD_CYLINDRE, OD_AXE) "
                  "VALUES (:id_client, :date_test, :og_cylindre, :og_axe, :od_cylindre, :od_axe)");
    query.bindValue(":id_client", id);
    query.bindValue(":date_test", date);
    query.bindValue(":og_cylindre", ogCylindre);
    query.bindValue(":og_axe", ogAxe);
    query.bindValue(":od_cylindre", odCylindre);
    query.bindValue(":od_axe", odAxe);

    return query.exec();
}

bool Client::supprimerTestOrdonnance(int idTest)
{
    QSqlQuery query;
    query.prepare("DELETE FROM TESTS_ORDONNANCE WHERE ID_TEST = :id_test");
    query.bindValue(":id_test", idTest);
    return query.exec();
}

QSqlQueryModel* Client::afficherTestsOrdonnance()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM TESTS_ORDONNANCE WHERE ID_CLIENT = :id_client");
    query.bindValue(":id_client", id);
    query.exec();
    model->setQuery(query);
    return model;
}

QSqlQueryModel* Client::afficherTousTestsOrdonnance()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM TESTS_ORDONNANCE");
    return model;
}
