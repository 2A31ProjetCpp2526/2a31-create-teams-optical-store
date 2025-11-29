#ifndef COMMANDE_H
#define COMMANDE_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

class Commande
{
private:
    int id_commande;
    int id_client;
    int id_employe;
    QDate date_commande;
    double prix_totale;

public:
    // Constructeurs
    Commande();
    Commande(int id, int id_cl, int id_emp, QDate date, double prix);
    Commande( int id_cl, int id_emp, QDate date, double prix);

    // Getters
    int getId() const { return id_commande; }
    int getIdClient() const { return id_client; }
    int getIdEmploye() const { return id_employe; }
    QDate getDateCommande() const { return date_commande; }
    double getPrixTotale() const { return prix_totale; }

    // Setters
    void setId(int id) { id_commande = id; }
    void setIdClient(int id_cl) { id_client = id_cl; }
    void setIdEmploye(int id_emp) { id_employe = id_emp; }
    void setDateCommande(QDate date) { date_commande = date; }
    void setPrixTotale(double prix) { prix_totale = prix; }

    // Fonctions CRUD
    bool ajouter();
    QSqlQueryModel* afficher(int id, int id_cl, int id_emp, QDate date, double prix);
    bool supprimer(int id);
    bool modifier(int id, int id_cl, int id_emp, QDate date, double prix);
    QSqlQueryModel* rechercherParId(int id);
    QSqlQueryModel* trierParDate();
    QSqlQueryModel* trierParPrix();

    // Vérifications
    static bool verifierClientExiste(int id_employe);
    static bool verifierEmployeExiste(int id_employe);
};

#endif // COMMANDE_H
