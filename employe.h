#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>

class employe
{
public:
    employe();
    employe(int id_employe, QString nom, QString prenom, QDate date_naissance, QString departement);

    // Getters
    int getCIN() { return id_employe; }
    QString getNom() { return nom; }
    QString getPrenom() { return prenom; }
    QDate getDateNaissance() { return date_naissance; }
    QString getDepartement() { return departement; }

    // Setters
    void setCIN(int id_employe) { id_employe = id_employe; }
    void setNom(QString n) { nom = n; }
    void setPrenom(QString p) { prenom = p; }
    void setDateNaissance(QDate d) { date_naissance = d; }
    void setDepartement(QString d) { departement = d; }

    // Méthodes CRUD
    bool ajouter();
    bool supprimer(int id_employe);
    bool modifier(int id_employe, QString nom, QString prenom, QDate date_naissance, QString departement);
    QSqlQueryModel* afficher();
    QSqlQueryModel* rechercherParCIN(int id_employe);
    bool recuperer(int id_employe);

private:
    int id_employe;
    QString nom;
    QString prenom;
    QDate date_naissance;
    QString departement;
};

#endif // EMPLOYE_H
