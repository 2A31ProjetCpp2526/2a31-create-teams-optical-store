#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QSqlQueryModel>
#include <QDate>
#include <QMap>
#include <QList>

class Client
{
private:
    int id;
    QString nom;
    QString prenom;
    QString email;
    QString telephone;
    int age;
    QString statut;

public:
    Client(int id = 0, QString nom = "", QString prenom = "", QString email = "",
           QString telephone = "", int age = 0, QString statut = "saisir");

    // Getters
    int getId() const;
    QString getNom() const;
    QString getPrenom() const;
    QString getEmail() const;
    QString getTelephone() const;
    int getAge() const;
    QString getStatut() const;

    // Setters
    void setId(int newId);
    void setNom(QString newNom);
    void setPrenom(QString newPrenom);
    void setEmail(QString newEmail);
    void setTelephone(QString newTelephone);
    void setAge(int newAge);
    void setStatut(QString newStatut);

    // CRUD Operations
    bool ajouter();
    bool supprimer(int id);
    bool modifier();
    static QSqlQueryModel* afficher();

    // Status Management
    static bool modifierStatut(int id, QString nouveauStatut);
    static QSqlQueryModel* rechercherParStatut(QString statut);
    static QList<Client> getAllClients();
    QMap<QString, int> getDureesTousStatuts() const;

    // Feedback Management
    static bool ajouterFeedback(int id_feedback, int id_client, QString type,
                                QString categorie, QDate date, QString message);
    static QSqlQueryModel* afficherTousLesFeedbacks();

    // Validation
    static bool estAgeValide(int age);
    static bool estStatutValide(QString statut);

    // Sorting and Searching
    static QSqlQueryModel* trier(QString critere);
    static QSqlQueryModel* rechercherParId(int id);
    static QMap<QString, int> getStatistiquesAge();

    // Prescription Tests
    bool ajouterTestOrdonnance(QDate date, double ogCylindre, int ogAxe,
                               double odCylindre, int odAxe);
    static bool supprimerTestOrdonnance(int idTest);
    QSqlQueryModel* afficherTestsOrdonnance();
    static QSqlQueryModel* afficherTousTestsOrdonnance();
};

#endif // CLIENT_H
