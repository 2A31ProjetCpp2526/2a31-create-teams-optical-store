#ifndef FOURNISSEUR_H
#define FOURNISSEUR_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

class Fournisseur
{
private:
    int ID_FOURNISSEUR;
    QString NOM;
    QString PRENOM;
    QString TELEPHONE;
    QString EMAIL;
    QString TYPE;

public:
    // Constructeurs
    Fournisseur();  // Constructeur par défaut
    Fournisseur(QString nom, QString prenom, QString telephone, QString email, QString type);  // Avec paramètres
    Fournisseur(int id, QString nom, QString prenom, QString telephone, QString email, QString type);  // Avec ID

    // Getters
    int getId() const { return ID_FOURNISSEUR; }
    QString getNom() const { return NOM; }
    QString getPrenom() const { return PRENOM; }
    QString getTelephone() const { return TELEPHONE; }
    QString getEmail() const { return EMAIL; }
    QString getType() const { return TYPE; }

    // Setters
    void setId(int id) { ID_FOURNISSEUR = id; }
    void setNom(const QString& nom) { NOM = nom; }
    void setPrenom(const QString& prenom) { PRENOM = prenom; }
    void setTelephone(const QString& tel) { TELEPHONE = tel; }
    void setEmail(const QString& email) { EMAIL = email; }
    void setType(const QString& type) { TYPE = type; }

    // Fonctions CRUD
    bool ajouter();
    QSqlQueryModel* afficher();
    QSqlQueryModel* afficherTrie(const QString& orderBy);
    bool supprimer(int id);
    bool modifier(int id);
    QSqlQueryModel* rechercher(const QString& mot_cle, const QString& colonne);
};

#endif // FOURNISSEUR_H
