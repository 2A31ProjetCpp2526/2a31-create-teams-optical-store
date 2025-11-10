#ifndef PRODUIT_H
#define PRODUIT_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Produit
{
private:
    int code;
    int id_fournisseur;
    QString marque;
    QString type;
    int quantite_stock;
    float prix;

public:
    Produit();
    Produit(int, int, QString, QString, int, float);

    int getCode() { return code; }
    int getIdFournisseur() { return id_fournisseur; }
    QString getMarque() { return marque; }
    QString getType() { return type; }
    int getQuantiteStock() { return quantite_stock; }
    float getPrix() { return prix; }

    void setCode(int c) { code = c; }
    void setIdFournisseur(int id) { id_fournisseur = id; }
    void setMarque(QString m) { marque = m; }
    void setType(QString t) { type = t; }
    void setQuantiteStock(int q) { quantite_stock = q; }
    void setPrix(float p) { prix = p; }

    bool ajouter();
    bool supprimer(int code);
    QSqlQueryModel* afficher();
    QSqlQueryModel* rechercherParCode(int code);
    bool modifier(int code, int id_fournisseur, QString marque, QString type, int quantite_stock, float prix);
    bool verifierCodeExiste(int code);
    bool verifierFournisseurExiste(int id_fournisseur);
};

#endif // PRODUIT_H
