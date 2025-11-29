#include "commande.h"
#include "connection.h"

// ===== CONSTRUCTEURS =====
Commande::Commande()
    : id_commande(0), id_client(0), id_employe(0), prix_totale(0)
{
}

Commande::Commande(int id,  int id_cl, int id_emp, QDate date, double prix)
    : id_commande(id), id_client(id_cl), id_employe(id_emp),
    date_commande(date), prix_totale(prix)
{
}

Commande::Commande( int id_cl, int id_emp, QDate date, double prix)
    : id_commande(0),  id_client(id_cl), id_employe(id_emp),
    date_commande(date), prix_totale(prix)
{
}

// ===== VÉRIFIER CLIENT EXISTE =====
bool Commande::verifierClientExiste(int id)
{
    QSqlQuery query;
    query.prepare("SELECT id_client FROM client WHERE id_client = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        return true;
    }
    return false;
}

// ===== VÉRIFIER EMPLOYE EXISTE =====
bool Commande::verifierEmployeExiste(int id_employe)
{
    QSqlQuery query;
    query.prepare("SELECT id_employe FROM employe WHERE id_employe = :id_employe");
    query.bindValue(":id_employe", id_employe);

    if (query.exec() && query.next()) {
        return true;
    }
    return false;
}

// ===== AJOUTER =====
bool Commande::ajouter()
{
    // Validation des données
    if (id_commande <= 0) {
        QMessageBox::warning(nullptr, QObject::tr("Validation"),
                             QObject::tr("ID commande invalide !"));
        return false;
    }

    if (!verifierClientExiste(id_client)) {
        QMessageBox::warning(nullptr, QObject::tr("Validation"),
                             QObject::tr("Le client n'existe pas !"));
        return false;
    }

    if (!verifierEmployeExiste(id_employe)) {
        QMessageBox::warning(nullptr, QObject::tr("Validation"),
                             QObject::tr("L'employé n'existe pas !"));
        return false;
    }

    if (prix_totale <= 0) {
        QMessageBox::warning(nullptr, QObject::tr("Validation"),
                             QObject::tr("Prix total invalide !"));
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO commande (id_commande,  id_client, id_employe, date_commande, prix_totale) "
                  "VALUES (:id,  :id_cl, :id_emp, :date, :prix)");

    query.bindValue(":id", id_commande);
    query.bindValue(":id_cl", id_client);
    query.bindValue(":id_emp", id_employe);
    query.bindValue(":date", date_commande);
    query.bindValue(":prix", prix_totale);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur SQL"),
                              QObject::tr("Erreur lors de l'ajout : %1").arg(query.lastError().text()));
        qDebug() << "❌ Erreur SQL (ajouter commande):" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ Commande ajoutée avec succès - ID:" << id_commande;
    return true;
}

// ===== AFFICHER =====
QSqlQueryModel* Commande::afficher(int id, int id_cl, int id_emp, QDate date, double prix)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    QSqlQuery query;
    query.prepare("SELECT id_commande, id_client, id_employe, date_commande, prix_totale "
                  "FROM commande "
                  "ORDER BY id_commande DESC");

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL (afficher commande):" << query.lastError().text();
        return model;
    }

    model->setQuery(std::move(query));

    // En-têtes des colonnes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Commande"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("ID Client"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("ID Employé"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date Commande"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Prix Total"));

    return model;
}

// ===== SUPPRIMER =====
bool Commande::supprimer(int id)
{
    if (id <= 0) {
        QMessageBox::warning(nullptr, QObject::tr("Erreur"), QObject::tr("ID invalide"));
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM commande WHERE id_commande = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur SQL"),
                              QObject::tr("Erreur lors de la suppression : %1").arg(query.lastError().text()));
        return false;
    }

    if (query.numRowsAffected() == 0) {
        QMessageBox::warning(nullptr, QObject::tr("Non trouvé"),
                             QObject::tr("Aucune commande trouvée avec cet ID"));
        return false;
    }

    qDebug() << "✅ Commande supprimée ID:" << id;
    return true;
}

// ===== MODIFIER =====
bool Commande::modifier(int id, int id_cl, int id_emp, QDate date, double prix)
{
    if (id <= 0) {
        QMessageBox::warning(nullptr, QObject::tr("Erreur"), QObject::tr("ID invalide"));
        return false;
    }

    if (!verifierClientExiste(id_cl)) {
        QMessageBox::warning(nullptr, QObject::tr("Validation"),
                             QObject::tr("Le client n'existe pas !"));
        return false;
    }

    if (!verifierEmployeExiste(id_employe)) {
        QMessageBox::warning(nullptr, QObject::tr("Validation"),
                             QObject::tr("L'employé n'existe pas !"));
        return false;
    }

    if (prix <= 0) {
        QMessageBox::warning(nullptr, QObject::tr("Validation"),
                             QObject::tr("Prix total invalide !"));
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE commande "
                  "SET  id_client = :id_cl, id_employe = :id_emp, "
                  "date_commande = :date, prix_totale = :prix "
                  "WHERE id_commande = :id");

    query.bindValue(":id_cl", id_cl);
    query.bindValue(":id_emp", id_emp);
    query.bindValue(":date", date);
    query.bindValue(":prix", prix);
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur SQL"),
                              QObject::tr("Erreur lors de la modification : %1").arg(query.lastError().text()));
        return false;
    }

    if (query.numRowsAffected() == 0) {
        QMessageBox::warning(nullptr, QObject::tr("Non trouvé"),
                             QObject::tr("Aucune commande trouvée avec cet ID"));
        return false;
    }

    qDebug() << "✅ Commande modifiée ID:" << id;
    return true;
}

// ===== RECHERCHER PAR ID =====
QSqlQueryModel* Commande::rechercherParId(int id)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    QSqlQuery query;
    query.prepare("SELECT id_commande, id_client, id_employe, date_commande, prix_totale "
                  "FROM commande "
                  "WHERE id_commande = :id");

    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL (rechercher commande):" << query.lastError().text();
        return model;
    }

    model->setQuery(std::move(query));

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Commande"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("ID Client"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("ID Employé"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date Commande"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Prix Total"));

    return model;
}

// ===== TRIER PAR DATE =====
QSqlQueryModel* Commande::trierParDate()
{
    QSqlQueryModel* model = new QSqlQueryModel();

    QSqlQuery query;
    query.prepare("SELECT id_commande, id_client, id_employe, date_commande, prix_totale "
                  "FROM commande "
                  "ORDER BY date_commande DESC");

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL (trier par date):" << query.lastError().text();
        return model;
    }

    model->setQuery(std::move(query));

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Commande"));    
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("ID Client"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("ID Employé"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date Commande"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Prix Total"));

    return model;
}

// ===== TRIER PAR PRIX =====
QSqlQueryModel* Commande::trierParPrix()
{
    QSqlQueryModel* model = new QSqlQueryModel();

    QSqlQuery query;
    query.prepare("SELECT id_commande, id_client, id_employe, date_commande, prix_totale "
                  "FROM commande "
                  "ORDER BY prix_totale DESC");

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL (trier par prix):" << query.lastError().text();
        return model;
    }

    model->setQuery(std::move(query));

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Commande"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("ID Client"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("ID Employé"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date Commande"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Prix Total"));

    return model;
}
