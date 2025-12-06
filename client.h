#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>
#include <QMap>
#include <QList>

class Client
{
private:
    int id_client;
    QString nom, prenom, email, telephone;
    int age;
    QString statut;
    QDate date_saisir;
    QDate date_en_cours;
    QDate date_termine;
    QDate date_recupere;

public:
    Client();
    Client(int, QString, QString, QString, QString, int, QString statut = "saisir");

    int getId();
    QString getNom();
    QString getPrenom();
    QString getEmail();
    QString getTelephone();
    int getAge();
    QString getStatut();

    // Getters pour les dates
    QDate getDateSaisir() const;
    QDate getDateEnCours() const;
    QDate getDateTermine() const;
    QDate getDateRecupere() const;

    // Setters
    void setId(int id) { id_client = id; }
    void setNom(QString n) { nom = n; }
    void setPrenom(QString p) { prenom = p; }
    void setEmail(QString e) { email = e; }
    void setTelephone(QString t) { telephone = t; }
    void setAge(int a) { age = a; }
    void setStatut(QString s) { statut = s; }

    // Méthodes de validation
    static bool estTelephoneValide(const QString& telephone);
    static bool estEmailValide(const QString& email);
    static bool estAgeValide(int age);
    static bool estStatutValide(const QString& statut);

    // Méthodes CRUD
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool modifier();
    bool modifierStatut(int id, const QString& nouveauStatut);

    // Méthodes pour les durées de statut
    int getDureeDansStatut(const QString& statut) const;
    QMap<QString, int> getDureesTousStatuts() const;
    bool mettreAJourDatesStatut(const QString& nouveauStatut);

    // Tests ordonnance
    bool ajouterTestOrdonnance(QDate date,
                               double ogCylindre, int ogAxe,
                               double odCylindre, int odAxe,
                               QString commentaire = "");

    QSqlQueryModel* afficherTestsOrdonnance();
    static QSqlQueryModel* afficherTousTestsOrdonnance();
    static bool supprimerTestOrdonnance(int idTest);
    static QSqlQueryModel* rechercherParId(int id);
    static QMap<QString, int> getStatistiquesAge();
    static QList<Client> getAllClients();
    static QSqlQueryModel* trier(const QString& critere);

    // Méthodes pour les statistiques de statut
    static QMap<QString, int> getStatistiquesStatut();
    static QSqlQueryModel* rechercherParStatut(const QString& statut);

    // Méthodes pour le feedback
    static bool ajouterFeedback(int id_feedback, int id_client, const QString& type,
                                const QString& categorie, const QDate& date,
                                const QString& message);
    static void annulerFeedback();
    static bool estTypeFeedbackValide(const QString& type);
    static bool estCategorieFeedbackValide(const QString& categorie);
    static QSqlQueryModel* afficherTousLesFeedbacks();
    static QSqlQueryModel* afficherFeedbacksParClient(int idClient);
    static bool supprimerFeedback(int id_feedback);
};

#endif // CLIENT_H
