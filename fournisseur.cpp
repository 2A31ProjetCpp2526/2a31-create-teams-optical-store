#include "fournisseur.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>

// ===== CONSTRUCTEURS =====
Fournisseur::Fournisseur()
    : id_fournisseur(0), nom_entreprise(""), telephone(""), email("")
{
}

Fournisseur::Fournisseur(int id, QString nom, QString tel, QString em)
    : id_fournisseur(id), nom_entreprise(nom), telephone(tel), email(em)
{
}

Fournisseur::Fournisseur(QString nom, QString tel, QString em)
    : id_fournisseur(0), nom_entreprise(nom), telephone(tel), email(em)
{
}

// ===== VÉRIFIER EXISTENCE =====
bool Fournisseur::verifierExistence(int id)
{
    QSqlQuery query;
    query.prepare("SELECT id_fournisseur FROM fournisseur WHERE id_fournisseur = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        return true;
    }
    return false;
}

// ===== AJOUTER =====
bool Fournisseur::ajouter()
{
    // Validation des données
    if (nom_entreprise.isEmpty() || telephone.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(nullptr, QObject::tr("Validation"),
                             QObject::tr("Tous les champs sont obligatoires !"));
        return false;
    }

    // Validation email
    if (!email.contains("@")) {
        QMessageBox::warning(nullptr, QObject::tr("Email invalide"),
                             QObject::tr("L'email doit contenir '@'"));
        return false;
    }

    QSqlQuery query;

    if (id_fournisseur == 0) {
        // Auto-incrément si ID non spécifié
        query.prepare("INSERT INTO fournisseur (nom_entreprise, num_telephone, email) "
                      "VALUES (:nom, :tel, :email)");
    } else {
        // ID spécifié
        query.prepare("INSERT INTO fournisseur (id_fournisseur, nom_entreprise, num_telephone, email) "
                      "VALUES (:id, :nom, :tel, :email)");
        query.bindValue(":id", id_fournisseur);
    }

    query.bindValue(":nom", nom_entreprise);
    query.bindValue(":tel", telephone);
    query.bindValue(":email", email);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur SQL"),
                              QObject::tr("Erreur lors de l'ajout : %1").arg(query.lastError().text()));
        qDebug() << "❌ Erreur SQL (ajouter):" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ Fournisseur ajouté avec succès";
    return true;
}

// ===== AFFICHER =====
QSqlQueryModel* Fournisseur::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();

    QSqlQuery query;
    query.prepare("SELECT id_fournisseur, nom_entreprise, num_telephone, email "
                  "FROM fournisseur "
                  "ORDER BY id_fournisseur DESC");

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL (afficher):" << query.lastError().text();
        return model;
    }

    model->setQuery(std::move(query));

    // En-têtes des colonnes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom Entreprise"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));

    return model;
}

// ===== SUPPRIMER =====
bool Fournisseur::supprimer(int id)
{
    if (id <= 0) {
        QMessageBox::warning(nullptr, QObject::tr("Erreur"), QObject::tr("ID invalide"));
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM fournisseur WHERE id_fournisseur = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur SQL"),
                              QObject::tr("Erreur lors de la suppression : %1").arg(query.lastError().text()));
        return false;
    }

    if (query.numRowsAffected() == 0) {
        QMessageBox::warning(nullptr, QObject::tr("Non trouvé"),
                             QObject::tr("Aucun fournisseur trouvé avec cet ID"));
        return false;
    }

    qDebug() << "✅ Fournisseur supprimé ID:" << id;
    return true;
}

// ===== MODIFIER =====
bool Fournisseur::modifier(int id)
{
    if (id <= 0) {
        QMessageBox::warning(nullptr, QObject::tr("Erreur"), QObject::tr("ID invalide"));
        return false;
    }

    if (nom_entreprise.isEmpty() || telephone.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(nullptr, QObject::tr("Validation"),
                             QObject::tr("Tous les champs sont obligatoires !"));
        return false;
    }

    if (!email.contains("@")) {
        QMessageBox::warning(nullptr, QObject::tr("Email invalide"),
                             QObject::tr("L'email doit contenir '@'"));
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE fournisseur "
                  "SET nom_entreprise = :nom, num_telephone = :tel, email = :email "
                  "WHERE id_fournisseur = :id");

    query.bindValue(":nom", nom_entreprise);
    query.bindValue(":tel", telephone);
    query.bindValue(":email", email);
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur SQL"),
                              QObject::tr("Erreur lors de la modification : %1").arg(query.lastError().text()));
        return false;
    }

    if (query.numRowsAffected() == 0) {
        QMessageBox::warning(nullptr, QObject::tr("Non trouvé"),
                             QObject::tr("Aucun fournisseur trouvé avec cet ID"));
        return false;
    }

    qDebug() << "✅ Fournisseur modifié ID:" << id;
    return true;
}

// ===== RECHERCHER =====
QSqlQueryModel* Fournisseur::rechercher(const QString& mot_cle, const QString& colonne)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    QString colonneName;
    if (colonne == "ID") colonneName = "id_fournisseur";
    else if (colonne == "Nom") colonneName = "nom_entreprise";
    else colonneName = "nom_entreprise"; // par défaut

    QSqlQuery query;
    query.prepare(QString("SELECT id_fournisseur, nom_entreprise, num_telephone, email "
                          "FROM fournisseur "
                          "WHERE %1 LIKE :mot_cle "
                          "ORDER BY id_fournisseur DESC").arg(colonneName));

    query.bindValue(":mot_cle", "%" + mot_cle + "%");

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL (rechercher):" << query.lastError().text();
        return model;
    }

    model->setQuery(std::move(query));

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom Entreprise"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));

    return model;
}
