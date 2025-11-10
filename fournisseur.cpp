#include "fournisseur.h"
#include "connection.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>

// ===== CONSTRUCTEURS =====

Fournisseur::Fournisseur()
    : ID_FOURNISSEUR(0), NOM(""), PRENOM(""), TELEPHONE(""), EMAIL(""), TYPE("")
{
}

Fournisseur::Fournisseur(QString nom, QString prenom, QString telephone, QString email, QString type)
    : ID_FOURNISSEUR(0), NOM(nom), PRENOM(prenom), TELEPHONE(telephone), EMAIL(email), TYPE(type)
{
}

Fournisseur::Fournisseur(int id, QString nom, QString prenom, QString telephone, QString email, QString type)
    : ID_FOURNISSEUR(id), NOM(nom), PRENOM(prenom), TELEPHONE(telephone), EMAIL(email), TYPE(type)
{
}


// ===== AJOUTER =====
bool Fournisseur::ajouter()
{
    // Validation des données
    if (NOM.isEmpty() || PRENOM.isEmpty() || TELEPHONE.isEmpty() || EMAIL.isEmpty() || TYPE.isEmpty()) {
        QMessageBox::warning(nullptr, QObject::tr("Validation"),
                           QObject::tr("Tous les champs sont obligatoires !"));
        qDebug() << "❌ Erreur: Champs vides";
        return false;
    }

    // Validation email simple
    if (!EMAIL.contains("@")) {
        QMessageBox::warning(nullptr, QObject::tr("Email invalide"),
                           QObject::tr("L'email doit contenir '@'"));
        return false;
    }

    // Validation téléphone (doit contenir au moins 8 caractères)
    if (TELEPHONE.length() < 8) {
        QMessageBox::warning(nullptr, QObject::tr("Téléphone invalide"),
                           QObject::tr("Le téléphone doit contenir au moins 8 caractères"));
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur Connexion"),
                            QObject::tr("La base de données n'est pas connectée"));
        qDebug() << "❌ Erreur: BD non connectée";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO FOURNISSEUR (NOM, PRENOM, TELEPHONE, EMAIL, TYPE) "
                  "VALUES (:nom, :prenom, :tel, :email, :type)");

    query.bindValue(":nom", NOM);
    query.bindValue(":prenom", PRENOM);
    query.bindValue(":tel", TELEPHONE);
    query.bindValue(":email", EMAIL);
    query.bindValue(":type", TYPE);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur SQL"),
                            QObject::tr("Erreur lors de l'ajout : %1").arg(query.lastError().text()));
        qDebug() << "❌ Erreur SQL (ajouter):" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ Fournisseur ajouté avec succès";
    QMessageBox::information(nullptr, QObject::tr("Succès"),
                           QObject::tr("Fournisseur ajouté avec succès !"));
    return true;
}

// ===== AFFICHER =====
QSqlQueryModel* Fournisseur::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isOpen()) {
        qDebug() << "❌ Erreur: BD non connectée pour affichage";
        return model;
    }

    QSqlQuery query(db);
    query.prepare("SELECT ID_FOURNISSEUR, NOM, PRENOM, TELEPHONE, EMAIL, TYPE "
                  "FROM FOURNISSEUR "
                  "ORDER BY ID_FOURNISSEUR DESC");

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL (afficher):" << query.lastError().text();
        return model;
    }

    // Utiliser std::move pour éviter les avertissements de dépréciations
    model->setQuery(std::move(query));

    // En-têtes des colonnes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Type"));

    qDebug() << "✅ Affichage réussi, lignes:" << model->rowCount();

    return model;
}

// ===== SUPPRIMER =====
bool Fournisseur::supprimer(int id)
{
    if (id <= 0) {
        QMessageBox::warning(nullptr, QObject::tr("Erreur"),
                           QObject::tr("ID invalide"));
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur Connexion"),
                            QObject::tr("La base de données n'est pas connectée"));
        return false;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM FOURNISSEURS WHERE ID_FOURNISSEUR = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur SQL"),
                            QObject::tr("Erreur lors de la suppression : %1").arg(query.lastError().text()));
        qDebug() << "❌ Erreur SQL (supprimer):" << query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() == 0) {
        QMessageBox::warning(nullptr, QObject::tr("Non trouvé"),
                           QObject::tr("Aucun fournisseur trouvé avec cet ID"));
        return false;
    }

    qDebug() << "✅ Fournisseur supprimé ID:" << id;
    QMessageBox::information(nullptr, QObject::tr("Succès"),
                           QObject::tr("Fournisseur supprimé avec succès !"));
    return true;
}

// ===== MODIFIER =====
bool Fournisseur::modifier(int id)
{
    // Validation
    if (id <= 0) {
        QMessageBox::warning(nullptr, QObject::tr("Erreur"),
                           QObject::tr("ID invalide"));
        return false;
    }

    if (NOM.isEmpty() || PRENOM.isEmpty() || TELEPHONE.isEmpty() || EMAIL.isEmpty() || TYPE.isEmpty()) {
        QMessageBox::warning(nullptr, QObject::tr("Validation"),
                           QObject::tr("Tous les champs sont obligatoires !"));
        return false;
    }

    if (!EMAIL.contains("@")) {
        QMessageBox::warning(nullptr, QObject::tr("Email invalide"),
                           QObject::tr("L'email doit contenir '@'"));
        return false;
    }

    if (TELEPHONE.length() < 8) {
        QMessageBox::warning(nullptr, QObject::tr("Téléphone invalide"),
                           QObject::tr("Le téléphone doit contenir au moins 8 caractères"));
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur Connexion"),
                            QObject::tr("La base de données n'est pas connectée"));
        return false;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE FOURNISSEUR "
                  "SET NOM = :nom, "
                  "    PRENOM = :prenom, "
                  "    TELEPHONE = :tel, "
                  "    EMAIL = :email, "
                  "    TYPE = :type "
                  "WHERE ID_FOURNISSEUR = :id");

    query.bindValue(":nom", NOM);
    query.bindValue(":prenom", PRENOM);
    query.bindValue(":tel", TELEPHONE);
    query.bindValue(":email", EMAIL);
    query.bindValue(":type", TYPE);
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur SQL"),
                            QObject::tr("Erreur lors de la modification : %1").arg(query.lastError().text()));
        qDebug() << "❌ Erreur SQL (modifier):" << query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() == 0) {
        QMessageBox::warning(nullptr, QObject::tr("Non trouvé"),
                           QObject::tr("Aucun fournisseur trouvé avec cet ID"));
        return false;
    }

    qDebug() << "✅ Fournisseur modifié ID:" << id;
    QMessageBox::information(nullptr, QObject::tr("Succès"),
                           QObject::tr("Fournisseur modifié avec succès !"));
    return true;
}

// ===== RECHERCHER =====
QSqlQueryModel* Fournisseur::rechercher(const QString& mot_cle, const QString& colonne)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isOpen()) {
        qDebug() << "❌ Erreur: BD non connectée pour recherche";
        return model;
    }

    QSqlQuery query(db);

    QString colonneName = (colonne == "ID") ? "ID_FOURNISSEUR" : "NOM";

    query.prepare(QString("SELECT ID_FOURNISSEUR, NOM, PRENOM, TELEPHONE, EMAIL, TYPE "
                         "FROM FOURNISSEURS "
                         "WHERE %1 LIKE :mot_cle "
                         "ORDER BY ID_FOURNISSEUR DESC").arg(colonneName));

    query.bindValue(":mot_cle", "%" + mot_cle + "%");

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL (rechercher):" << query.lastError().text();
        return model;
    }

    // Utiliser std::move pour éviter les avertissements de dépréciations
    model->setQuery(std::move(query));

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Type"));

    qDebug() << "✅ Recherche réussie, résultats:" << model->rowCount();

    return model;
}
