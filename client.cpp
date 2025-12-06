#include "client.h"
#include <QSqlQuery>
#include <QVariant>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSqlError>
#include <QDebug>
#include <QMap>
#include <QList>
#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>
Client::Client() {
    id_client = 0;
    nom = prenom = email = telephone = "";
    age = 0;
    statut = "saisir";
    date_saisir = QDate::currentDate();
}

Client::Client(int id, QString n, QString p, QString e, QString t, int a, QString s) {
    id_client = id;
    nom = n;
    prenom = p;
    email = e;
    telephone = t;
    age = a;
    statut = s;
    date_saisir = QDate::currentDate();
}

int Client::getId() { return id_client; }
QString Client::getNom() { return nom; }
QString Client::getPrenom() { return prenom; }
QString Client::getEmail() { return email; }
QString Client::getTelephone() { return telephone; }
int Client::getAge() { return age; }
QString Client::getStatut() { return statut; }

// Getters pour les dates
QDate Client::getDateSaisir() const { return date_saisir; }
QDate Client::getDateEnCours() const { return date_en_cours; }
QDate Client::getDateTermine() const { return date_termine; }
QDate Client::getDateRecupere() const { return date_recupere; }

// Validation du téléphone
bool Client::estTelephoneValide(const QString& telephone) {
    QRegularExpression regex("^[0-9]{8,15}$");
    return regex.match(telephone).hasMatch();
}

// Validation de l'email
bool Client::estEmailValide(const QString& email) {
    QRegularExpression regex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    return regex.match(email).hasMatch();
}

// Validation de l'âge
bool Client::estAgeValide(int age) {
    return (age >= 0 && age <= 120);
}

// Validation du statut
bool Client::estStatutValide(const QString& statut) {
    return (statut == "saisir" || statut == "en cours" || statut == "terminé" || statut == "récupéré");
}

bool Client::ajouter()
{
    // Validation des données
    if (id_client <= 0) {
        qDebug() << "Erreur: ID client invalide:" << id_client;
        return false;
    }

    if (nom.isEmpty()) {
        qDebug() << "Erreur: Nom vide";
        return false;
    }

    if (prenom.isEmpty()) {
        qDebug() << "Erreur: Prénom vide";
        return false;
    }

    if (!estEmailValide(email)) {
        qDebug() << "Erreur: Email invalide:" << email;
        return false;
    }

    if (!estTelephoneValide(telephone)) {
        qDebug() << "Erreur: Téléphone invalide:" << telephone;
        return false;
    }

    if (!estAgeValide(age)) {
        qDebug() << "Erreur: Age invalide:" << age;
        return false;
    }

    if (!estStatutValide(statut)) {
        qDebug() << "Erreur: Statut invalide:" << statut;
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO client (id_client, nom, prenom, email, telephone, age, statut, date_saisir) "
                  "VALUES (:id_client, :nom, :prenom, :email, :telephone, :age, :statut, :date_saisir)");
    query.bindValue(":id_client", id_client);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":age", age);
    query.bindValue(":statut", statut);
    query.bindValue(":date_saisir", date_saisir);

    if (!query.exec()) {
        qDebug() << "Erreur SQL lors de l'ajout:" << query.lastError().text();
        return false;
    }

    return true;
}

QSqlQueryModel* Client::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT id_client, nom, prenom, email, telephone, age, statut FROM client");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Âge"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Statut"));
    return model;
}

bool Client::supprimer(int id)
{
    if (id <= 0) {
        qDebug() << "Erreur: ID invalide pour suppression:" << id;
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM client WHERE id_client = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur SQL lors de la suppression:" << query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() <= 0) {
        qDebug() << "Aucun client trouvé avec ID:" << id;
        return false;
    }

    return true;
}

bool Client::modifier()
{
    if (id_client <= 0) {
        qDebug() << "Erreur: ID client invalide pour modification:" << id_client;
        return false;
    }

    if (nom.isEmpty()) {
        qDebug() << "Erreur: Nom vide pour modification";
        return false;
    }

    if (prenom.isEmpty()) {
        qDebug() << "Erreur: Prénom vide pour modification";
        return false;
    }

    if (!estEmailValide(email)) {
        qDebug() << "Erreur: Email invalide pour modification:" << email;
        return false;
    }

    if (!estTelephoneValide(telephone)) {
        qDebug() << "Erreur: Téléphone invalide pour modification:" << telephone;
        return false;
    }

    if (!estAgeValide(age)) {
        qDebug() << "Erreur: Age invalide pour modification:" << age;
        return false;
    }

    if (!estStatutValide(statut)) {
        qDebug() << "Erreur: Statut invalide pour modification:" << statut;
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE client SET nom = :nom, prenom = :prenom, email = :email, telephone = :telephone, age = :age, statut = :statut "
                  "WHERE id_client = :id_client");
    query.bindValue(":id_client", id_client);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":age", age);
    query.bindValue(":statut", statut);

    if (!query.exec()) {
        qDebug() << "Erreur SQL lors de la modification:" << query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() <= 0) {
        qDebug() << "Aucun client trouvé avec ID pour modification:" << id_client;
        return false;
    }

    return true;
}

// Calculer la durée dans un statut spécifique
int Client::getDureeDansStatut(const QString& statut) const {
    QDate dateDebut;

    if (statut == "saisir") {
        dateDebut = date_saisir;
    } else if (statut == "en cours") {
        dateDebut = date_en_cours;
    } else if (statut == "terminé") {
        dateDebut = date_termine;
    } else if (statut == "récupéré") {
        dateDebut = date_recupere;
    }

    if (!dateDebut.isValid()) return 0;

    // Calculer la différence en jours
    return dateDebut.daysTo(QDate::currentDate());
}

// Obtenir toutes les durées
QMap<QString, int> Client::getDureesTousStatuts() const {
    QMap<QString, int> durees;
    durees["saisir"] = getDureeDansStatut("saisir");
    durees["en cours"] = getDureeDansStatut("en cours");
    durees["terminé"] = getDureeDansStatut("terminé");
    durees["récupéré"] = getDureeDansStatut("récupéré");
    return durees;
}

// Mettre à jour les dates lors du changement de statut
bool Client::mettreAJourDatesStatut(const QString& nouveauStatut) {
    QDate aujourdhui = QDate::currentDate();

    if (nouveauStatut == "en cours" && !date_en_cours.isValid()) {
        date_en_cours = aujourdhui;
    } else if (nouveauStatut == "terminé" && !date_termine.isValid()) {
        date_termine = aujourdhui;
    } else if (nouveauStatut == "récupéré" && !date_recupere.isValid()) {
        date_recupere = aujourdhui;
    }

    QSqlQuery query;
    query.prepare("UPDATE client SET date_en_cours = :date_en_cours, "
                  "date_termine = :date_termine, date_recupere = :date_recupere "
                  "WHERE id_client = :id_client");

    query.bindValue(":date_en_cours", date_en_cours.isValid() ? date_en_cours : QVariant(QVariant::Date));
    query.bindValue(":date_termine", date_termine.isValid() ? date_termine : QVariant(QVariant::Date));
    query.bindValue(":date_recupere", date_recupere.isValid() ? date_recupere : QVariant(QVariant::Date));
    query.bindValue(":id_client", id_client);

    return query.exec();
}

bool Client::modifierStatut(int id, const QString& nouveauStatut)
{
    if (id <= 0) {
        qDebug() << "Erreur: ID client invalide pour modification du statut:" << id;
        return false;
    }

    if (!estStatutValide(nouveauStatut)) {
        qDebug() << "Erreur: Statut invalide:" << nouveauStatut;
        return false;
    }

    // Récupérer le client pour avoir ses dates actuelles
    QSqlQuery selectQuery;
    selectQuery.prepare("SELECT date_saisir, date_en_cours, date_termine, date_recupere FROM client WHERE id_client = :id_client");
    selectQuery.bindValue(":id_client", id);

    if (!selectQuery.exec() || !selectQuery.next()) {
        qDebug() << "Erreur: Impossible de récupérer le client";
        return false;
    }

    // Mettre à jour le statut ET les dates
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE client SET statut = :statut, "
                        "date_en_cours = COALESCE(date_en_cours, CASE WHEN :statut IN ('en cours', 'terminé', 'récupéré') THEN CURRENT_DATE ELSE NULL END), "
                        "date_termine = COALESCE(date_termine, CASE WHEN :statut IN ('terminé', 'récupéré') THEN CURRENT_DATE ELSE NULL END), "
                        "date_recupere = COALESCE(date_recupere, CASE WHEN :statut = 'récupéré' THEN CURRENT_DATE ELSE NULL END) "
                        "WHERE id_client = :id_client");

    updateQuery.bindValue(":statut", nouveauStatut);
    updateQuery.bindValue(":id_client", id);

    if (!updateQuery.exec()) {
        qDebug() << "Erreur SQL lors de la modification du statut:" << updateQuery.lastError().text();
        return false;
    }

    return updateQuery.numRowsAffected() > 0;
}

// === METHODES POUR TESTS ORDONNANCE ===

bool Client::ajouterTestOrdonnance(QDate date,
                                   double ogCylindre, int ogAxe,
                                   double odCylindre, int odAxe,
                                   QString commentaire)
{
    if (id_client <= 0) {
        qDebug() << "Erreur: ID client invalide pour test d'ordonnance";
        return false;
    }

    if (!date.isValid()) {
        qDebug() << "Erreur: Date de test invalide";
        return false;
    }

    if (ogAxe < 0 || ogAxe > 180 || odAxe < 0 || odAxe > 180) {
        qDebug() << "Erreur: Axe doit être entre 0 et 180 degrés";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO test_ordonnance (id_test, id_client, date_test, "
                  "oeil_gauche_cylindre, oeil_gauche_axe, "
                  "oeil_droit_cylindre, oeil_droit_axe, commentaire) "
                  "VALUES (seq_test_ordonnance.NEXTVAL, :id_client, :date_test, "
                  ":og_cylindre, :og_axe, "
                  ":od_cylindre, :od_axe, :commentaire)");

    query.bindValue(":id_client", id_client);
    query.bindValue(":date_test", date);
    query.bindValue(":og_cylindre", ogCylindre);
    query.bindValue(":og_axe", ogAxe);
    query.bindValue(":od_cylindre", odCylindre);
    query.bindValue(":od_axe", odAxe);
    query.bindValue(":commentaire", commentaire);

    if (!query.exec()) {
        qDebug() << "Erreur SQL lors de l'ajout du test d'ordonnance:" << query.lastError().text();
        return false;
    }

    return true;
}

QSqlQueryModel* Client::afficherTestsOrdonnance()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT id_test, date_test, "
                  "oeil_gauche_cylindre, oeil_gauche_axe, "
                  "oeil_droit_cylindre, oeil_droit_axe, commentaire "
                  "FROM test_ordonnance "
                  "WHERE id_client = :id_client "
                  "ORDER BY date_test DESC");
    query.bindValue(":id_client", id_client);
    query.exec();

    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Test"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("OG Cylindre"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("OG Axe"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("OD Cylindre"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("OD Axe"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Commentaire"));

    return model;
}

QSqlQueryModel* Client::afficherTousTestsOrdonnance()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT t.id_test, c.nom, c.prenom, t.date_test, "
                    "t.oeil_gauche_cylindre, t.oeil_gauche_axe, "
                    "t.oeil_droit_cylindre, t.oeil_droit_axe, t.commentaire "
                    "FROM test_ordonnance t "
                    "JOIN client c ON t.id_client = c.id_client "
                    "ORDER BY t.date_test DESC");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Test"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date Test"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("OG Cylindre"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("OG Axe"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("OD Cylindre"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("OD Axe"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Commentaire"));

    return model;
}

bool Client::supprimerTestOrdonnance(int idTest)
{
    if (idTest <= 0) {
        qDebug() << "Erreur: ID test invalide";
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM test_ordonnance WHERE id_test = :id");
    query.bindValue(":id", idTest);

    if (!query.exec()) {
        qDebug() << "Erreur SQL lors de la suppression du test:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

QSqlQueryModel* Client::rechercherParId(int id)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    if (id <= 0) {
        qDebug() << "Erreur: ID invalide pour la recherche";
        return model;
    }

    QSqlQuery query;
    query.prepare("SELECT id_client, nom, prenom, email, telephone, age, statut FROM client WHERE id_client = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur SQL lors de la recherche:" << query.lastError().text();
        return model;
    }

    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Âge"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Statut"));

    return model;
}

QMap<QString, int> Client::getStatistiquesAge()
{
    QMap<QString, int> stats;

    QSqlQuery query;

    query.prepare("SELECT COUNT(*) FROM client WHERE age < 20");
    if (query.exec() && query.next()) {
        stats["Moins de 20 ans"] = query.value(0).toInt();
    }

    query.prepare("SELECT COUNT(*) FROM client WHERE age BETWEEN 20 AND 50");
    if (query.exec() && query.next()) {
        stats["20-50 ans"] = query.value(0).toInt();
    }

    query.prepare("SELECT COUNT(*) FROM client WHERE age > 50");
    if (query.exec() && query.next()) {
        stats["Plus de 50 ans"] = query.value(0).toInt();
    }

    return stats;
}

QList<Client> Client::getAllClients()
{
    QList<Client> clients;

    QSqlQuery query("SELECT id_client, nom, prenom, email, telephone, age, statut, "
                    "date_saisir, date_en_cours, date_termine, date_recupere "
                    "FROM client ORDER BY nom, prenom");

    while (query.next()) {
        Client client;
        client.id_client = query.value(0).toInt();
        client.nom = query.value(1).toString();
        client.prenom = query.value(2).toString();
        client.email = query.value(3).toString();
        client.telephone = query.value(4).toString();
        client.age = query.value(5).toInt();
        client.statut = query.value(6).toString();
        client.date_saisir = query.value(7).toDate();
        client.date_en_cours = query.value(8).toDate();
        client.date_termine = query.value(9).toDate();
        client.date_recupere = query.value(10).toDate();
        clients.append(client);
    }

    return clients;
}

QSqlQueryModel* Client::trier(const QString& critere)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QString requete;

    if (critere == "id") {
        requete = "SELECT id_client, nom, prenom, email, telephone, age, statut FROM client ORDER BY id_client ASC";
    } else if (critere == "nom") {
        requete = "SELECT id_client, nom, prenom, email, telephone, age, statut FROM client ORDER BY nom ASC, prenom ASC";
    } else if (critere == "prenom") {
        requete = "SELECT id_client, nom, prenom, email, telephone, age, statut FROM client ORDER BY prenom ASC, nom ASC";
    } else if (critere == "age") {
        requete = "SELECT id_client, nom, prenom, email, telephone, age, statut FROM client ORDER BY age ASC";
    } else if (critere == "statut") {
        requete = "SELECT id_client, nom, prenom, email, telephone, age, statut FROM client ORDER BY statut ASC";
    } else {
        requete = "SELECT id_client, nom, prenom, email, telephone, age, statut FROM client ORDER BY id_client ASC";
    }

    model->setQuery(requete);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Âge"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Statut"));

    return model;
}

QMap<QString, int> Client::getStatistiquesStatut()
{
    QMap<QString, int> stats;

    QSqlQuery query;
    QStringList statuts = {"saisir", "en cours", "terminé", "récupéré"};

    for (const QString& statut : statuts) {
        query.prepare("SELECT COUNT(*) FROM client WHERE statut = :statut");
        query.bindValue(":statut", statut);

        if (query.exec() && query.next()) {
            stats[statut] = query.value(0).toInt();
        }
    }

    return stats;
}

QSqlQueryModel* Client::rechercherParStatut(const QString& statutRecherche)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    if (!estStatutValide(statutRecherche)) {
        qDebug() << "Erreur: Statut invalide pour la recherche";
        return model;
    }

    QSqlQuery query;
    query.prepare("SELECT id_client, nom, prenom, email, telephone, age, statut FROM client WHERE statut = :statut");
    query.bindValue(":statut", statutRecherche);

    if (!query.exec()) {
        qDebug() << "Erreur SQL lors de la recherche par statut:" << query.lastError().text();
        return model;
    }

    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Âge"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Statut"));

    return model;
}

// ==================== MÉTHODES POUR LE FEEDBACK ====================

bool Client::estTypeFeedbackValide(const QString& type) {
    return (type == "Suggestion" || type == "Réclamation" || type == "Question" ||
            type == "Problème" || type == "Compliment");
}

bool Client::estCategorieFeedbackValide(const QString& categorie) {
    return (categorie == "Produit" || categorie == "Service" || categorie == "Livraison" ||
            categorie == "Site Web" || categorie == "Employé");
}

bool Client::ajouterFeedback(int id_feedback, int id_client, const QString& type,
                             const QString& categorie, const QDate& date,
                             const QString& message) {

    qDebug() << "=== DÉBUT AJOUT FEEDBACK ===";
    qDebug() << "ID Feedback:" << id_feedback;
    qDebug() << "ID Client:" << id_client;
    qDebug() << "Type:" << type;
    qDebug() << "Catégorie:" << categorie;
    qDebug() << "Date:" << date;
    qDebug() << "Message:" << message;

    if (id_feedback <= 0) {
        qDebug() << "❌ Erreur: ID feedback invalide";
        return false;
    }

    if (id_client <= 0) {
        qDebug() << "❌ Erreur: ID client invalide";
        return false;
    }

    if (!estTypeFeedbackValide(type)) {
        qDebug() << "❌ Erreur: Type de feedback invalide";
        return false;
    }

    if (!estCategorieFeedbackValide(categorie)) {
        qDebug() << "❌ Erreur: Catégorie de feedback invalide";
        return false;
    }

    if (!date.isValid()) {
        qDebug() << "❌ Erreur: Date invalide";
        return false;
    }

    if (message.isEmpty()) {
        qDebug() << "❌ Erreur: Message vide";
        return false;
    }

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT id_client, nom, prenom FROM client WHERE id_client = :id_client");
    checkQuery.bindValue(":id_client", id_client);

    if (!checkQuery.exec()) {
        qDebug() << "❌ Erreur vérification client:" << checkQuery.lastError().text();
        return false;
    }

    if (!checkQuery.next()) {
        qDebug() << "❌ Client NON trouvé avec ID:" << id_client;
        return false;
    }

    qDebug() << "✅ Client trouvé:" << checkQuery.value(0).toInt()
             << checkQuery.value(1).toString() << checkQuery.value(2).toString();

    QSqlQuery checkFeedbackQuery;
    checkFeedbackQuery.prepare("SELECT id_feedback FROM feedback WHERE id_feedback = :id_feedback");
    checkFeedbackQuery.bindValue(":id_feedback", id_feedback);

    if (!checkFeedbackQuery.exec()) {
        qDebug() << "❌ Erreur vérification feedback:" << checkFeedbackQuery.lastError().text();
        return false;
    }

    if (checkFeedbackQuery.next()) {
        qDebug() << "❌ Feedback existe déjà avec ID:" << id_feedback;
        return false;
    }

    qDebug() << "✅ ID Feedback disponible:" << id_feedback;

    QSqlQuery query;
    query.prepare("INSERT INTO feedback (id_feedback, id_client, type_feedback, categorie, message, date_feedback) "
                  "VALUES (:id_feedback, :id_client, :type, :categorie, :message, :date_feedback)");

    query.bindValue(":id_feedback", id_feedback);
    query.bindValue(":id_client", id_client);
    query.bindValue(":type", type);
    query.bindValue(":categorie", categorie);
    query.bindValue(":message", message);
    query.bindValue(":date_feedback", date);

    if (!query.exec()) {
        qDebug() << "❌ Erreur INSERT feedback:" << query.lastError().text();
        qDebug() << "Requête:" << query.lastQuery();
        return false;
    }

    qDebug() << "✅ Feedback ajouté avec succès!";
    qDebug() << "=== FIN AJOUT FEEDBACK ===";
    return true;
}

void Client::annulerFeedback() {
    qDebug() << "Feedback annulé";
}

QSqlQueryModel* Client::afficherTousLesFeedbacks() {
    QSqlQueryModel* model = new QSqlQueryModel();

    QString queryStr = "SELECT "
                       "f.id_feedback, "
                       "f.id_client, "
                       "c.nom, "
                       "c.prenom, "
                       "f.type_feedback, "
                       "f.categorie, "
                       "f.date_feedback, "
                       "f.message "
                       "FROM feedback f "
                       "JOIN client c ON f.id_client = c.id_client "
                       "ORDER BY f.date_feedback DESC, f.id_feedback DESC";

    model->setQuery(queryStr);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Feedback"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("ID Client"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Catégorie"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Message"));

    return model;
}

QSqlQueryModel* Client::afficherFeedbacksParClient(int idClient) {
    QSqlQueryModel* model = new QSqlQueryModel();

    QSqlQuery query;
    query.prepare("SELECT "
                  "f.id_feedback, "
                  "f.type_feedback, "
                  "f.categorie, "
                  "f.date_feedback, "
                  "f.message "
                  "FROM feedback f "
                  "WHERE f.id_client = :id_client "
                  "ORDER BY f.date_feedback DESC");

    query.bindValue(":id_client", idClient);

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'affichage des feedbacks:" << query.lastError().text();
        return model;
    }

    model->setQuery(query);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Feedback"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Catégorie"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Message"));

    return model;
}

bool Client::supprimerFeedback(int id_feedback) {
    if (id_feedback <= 0) {
        qDebug() << "Erreur: ID feedback invalide pour suppression";
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM feedback WHERE id_feedback = :id_feedback");
    query.bindValue(":id_feedback", id_feedback);

    if (!query.exec()) {
        qDebug() << "Erreur SQL lors de la suppression du feedback:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}
