#include "produit.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Produit::Produit() {}

Produit::Produit(int code, int id_fournisseur, QString marque, QString type, int quantite_stock, float prix)
{
    this->code = code;
    this->id_fournisseur = id_fournisseur;
    this->marque = marque;
    this->type = type;
    this->quantite_stock = quantite_stock;
    this->prix = prix;
}

bool Produit::ajouter()
{
    QSqlQuery query;

    query.prepare("INSERT INTO PRODUIT (CODE_PRODUIT, ID_FOURNISSEUR, MARQUE, TYPE, QUANTITE_STOCK, PRIX) "
                  "VALUES (:code, :id_fournisseur, :marque, :type, :quantite_stock, :prix)");

    query.bindValue(":code", code);
    query.bindValue(":id_fournisseur", id_fournisseur);
    query.bindValue(":marque", marque);
    query.bindValue(":type", type);
    query.bindValue(":quantite_stock", quantite_stock);
    query.bindValue(":prix", prix);

    if(query.exec()) {
        qDebug() << "✅ Produit ajouté avec succès! Code:" << code;
        return true;
    } else {
        qDebug() << "❌ Erreur ajout:" << query.lastError().text();
        return false;
    }
}
bool Produit::verifierFournisseurExiste(int id_fournisseur)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM FOURNISSEUR WHERE ID_FOURNISSEUR = :id");
    query.bindValue(":id", id_fournisseur);

    if(query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}
bool Produit::supprimer(int code)
{
    QSqlQuery query;
    query.prepare("DELETE FROM PRODUIT WHERE CODE_PRODUIT = :code");
    query.bindValue(":code", code);

    qDebug() << "Tentative suppression - Code:" << code;

    if(query.exec()) {
        qDebug() << "✅ Produit supprimé avec succès! Code:" << code;
        return true;
    } else {
        qDebug() << "❌ Erreur suppression:" << query.lastError().text();
        return false;
    }
}

QSqlQueryModel* Produit::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();

    model->setQuery("SELECT CODE_PRODUIT, ID_FOURNISSEUR, MARQUE, TYPE, QUANTITE_STOCK, PRIX FROM PRODUIT");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Code Produit"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("ID Fournisseur"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Marque"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Quantité Stock"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Prix"));

    return model;
}
QSqlQueryModel* Produit::rechercherParCode(int code)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    QString queryStr = "SELECT CODE_PRODUIT, ID_FOURNISSEUR, MARQUE, TYPE, QUANTITE_STOCK, PRIX FROM PRODUIT WHERE CODE_PRODUIT = " + QString::number(code);
    model->setQuery(queryStr);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Code Produit"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("ID Fournisseur"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Marque"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Quantité Stock"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Prix"));

    return model;
}


bool Produit::modifier(int code, int id_fournisseur, QString marque, QString type, int quantite_stock, float prix)
{
    QSqlQuery query;

    if (!verifierFournisseurExiste(id_fournisseur)) {
        qDebug() << "❌ Fournisseur inexistant pour modification:" << id_fournisseur;
        return false;
    }

    query.prepare("UPDATE PRODUIT SET ID_FOURNISSEUR=:id_fournisseur, MARQUE=:marque, "
                  "TYPE=:type, QUANTITE_STOCK=:quantite_stock, PRIX=:prix WHERE CODE_PRODUIT=:code");

    query.bindValue(":code", code);
    query.bindValue(":id_fournisseur", id_fournisseur);
    query.bindValue(":marque", marque);
    query.bindValue(":type", type);
    query.bindValue(":quantite_stock", quantite_stock);
    query.bindValue(":prix", prix);

    if(query.exec()) {
        qDebug() << "✅ Produit modifié avec succès! Code:" << code;
        return true;
    } else {
        qDebug() << "❌ Erreur modification produit:" << query.lastError().text();
        return false;
    }
}
QList<Produit> Produit::getAllProduits()
{
    QList<Produit> produits;
    QSqlQuery query;

    query.prepare("SELECT CODE_PRODUIT, ID_FOURNISSEUR, MARQUE, TYPE FROM PRODUIT");

    if (query.exec()) {
        while (query.next()) {
            Produit p;
            p.setCode(query.value(0).toInt());
            p.setIdFournisseur(query.value(1).toInt());
            p.setMarque(query.value(2).toString());
            p.setType(query.value(3).toString());
            p.setQuantiteStock(query.value(0).toInt());;  // Valeur par défaut
            p.setPrix(query.value(0).toInt());;        // Valeur par défaut plus réaliste

            produits.append(p);
        }
    }

    return produits;
}
QSqlQueryModel* Produit::afficherTrie(const QString& orderBy)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryString = "SELECT CODE_PRODUIT, ID_FOURNISSEUR, MARQUE, TYPE, QUANTITE_STOCK, PRIX FROM PRODUIT ";

    // CONVERSION POUR ORACLE
    if (orderBy == "CODE_PRODUIT ASC") {
        queryString += "ORDER BY TO_NUMBER(CODE_PRODUIT) ASC";
    }
    else if (orderBy == "CODE_PRODUIT DESC") {
        queryString += "ORDER BY TO_NUMBER(CODE_PRODUIT) DESC";
    }
    else if (orderBy == "MARQUE ASC") {
        queryString += "ORDER BY MARQUE ASC";
    }
    else if (orderBy == "MARQUE DESC") {
        queryString += "ORDER BY MARQUE DESC";
    }
    else if (orderBy == "TYPE ASC") {
        queryString += "ORDER BY TYPE ASC";
    }
    else if (orderBy == "TYPE DESC") {
        queryString += "ORDER BY TYPE DESC";
    }
    else if (orderBy == "PRIX ASC") {
        queryString += "ORDER BY PRIX ASC";
    }
    else if (orderBy == "PRIX DESC") {
        queryString += "ORDER BY PRIX DESC";
    }
    else if (orderBy == "QUANTITE_STOCK ASC") {
        queryString += "ORDER BY QUANTITE_STOCK ASC";
    }
    else if (orderBy == "QUANTITE_STOCK DESC") {
        queryString += "ORDER BY QUANTITE_STOCK DESC";
    }
    else {
        queryString += "ORDER BY TO_NUMBER(CODE_PRODUIT) ASC";
    }

    QSqlQuery query;
    if (query.exec(queryString)) {
        model->setQuery(query);

        model->setHeaderData(0, Qt::Horizontal, QObject::tr("Code Produit"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("ID Fournisseur"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Marque"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Type"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Quantité Stock"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Prix"));

        qDebug() << "✅ Tri appliqué:" << orderBy << "- Lignes:" << model->rowCount();
    } else {
        qDebug() << "❌ Erreur tri:" << query.lastError().text();
        delete model;
        return afficher();
    }

    return model;
}
