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
    int id_fournisseur;
    QString nom_entreprise;
    QString telephone;
    QString email;

public:
    // Constructeurs
    Fournisseur();
    Fournisseur(int id, QString nom_entreprise, QString telephone, QString email);
    Fournisseur(QString nom_entreprise, QString telephone, QString email);

    // Getters
    int getId() const { return id_fournisseur; }
    QString getNomEntreprise() const { return nom_entreprise; }
    QString getTelephone() const { return telephone; }
    QString getEmail() const { return email; }

    // Setters
    void setId(int id) { id_fournisseur = id; }
    void setNomEntreprise(const QString& nom) { nom_entreprise = nom; }
    void setTelephone(const QString& tel) { telephone = tel; }
    void setEmail(const QString& em) { email = em; }

    // Fonctions CRUD
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int id);
    bool modifier(int id);
    QSqlQueryModel* rechercher(const QString& mot_cle, const QString& colonne);

    // Vérifier si un fournisseur existe
    static bool verifierExistence(int id);
};

#endif // FOURNISSEUR_H
