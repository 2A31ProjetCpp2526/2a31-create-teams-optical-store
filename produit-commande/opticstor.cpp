#include "opticstor.h"
#include "ui_opticstor.h"
#include <QIntValidator>
#include <QDoubleValidator>
#include <QDebug>
#include "connection.h";
opticstor::opticstor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::opticstor)
{
    ui->setupUi(this);
    prixtotal=0;
    ui->commandsc->setCurrentIndex(1);
    connect(ui->ajout, &QPushButton::clicked, this, &opticstor::ajoutcommande);
    connect(ui->supp, &QPushButton::clicked, this, &opticstor::supprimcommande);
    connect(ui->update, &QPushButton::clicked, this, &opticstor::updatecommande);
    connect(ui->autre, &QPushButton::clicked, this, &opticstor::autrecmd);
    connect(ui->updatec, &QPushButton::clicked, this, &opticstor::updmen);
    loadtypesmarquesComboBox();
    loadclientComboBox();
    loademp();
    affichecommande();
    // Connexions existantes...
    /*connect(ui->submitbut_2, &QPushButton::clicked, this, &opticstor::onSubmitClicked);
    connect(ui->cancelbut_2, &QPushButton::clicked, this, &opticstor::onCancelClicked);
    connect(ui->deletebut_2, &QPushButton::clicked, this, &opticstor::onDeleteClicked);
    connect(ui->submitbutt_2, &QPushButton::clicked, this, &opticstor::onSubmitClickedd);
    connect(ui->cancelbutt_2, &QPushButton::clicked, this, &opticstor::onCancelClickedd);
    connect(ui->deletebutt_2, &QPushButton::clicked, this, &opticstor::onDeleteClickedd);
    //connect(ui->editbut_3, &QPushButton::clicked, this, &opticstor::onEditClicked);
    connect(ui->sortCombo_2, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &opticstor::onSortChanged);
    connect(ui->searchbut_2, &QPushButton::clicked, this, &opticstor::onSearchClicked);*/
    connect(ui->commandebut, &QPushButton::clicked, this, &opticstor::onGestCommande);
    connect(ui->gest, &QPushButton::clicked, this, &opticstor::onGest);
    connect(ui->produitbut, &QPushButton::clicked, this, &opticstor::onprod);
    connect(ui->emplbut, &QPushButton::clicked, this, &opticstor::onemp);
    connect(ui->fournbut, &QPushButton::clicked, this, &opticstor::onfourn);

    // Configuration des tables existantes...
    //ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    //ui->tableWidget_2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tabcl_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tabcl_2->setSelectionMode(QAbstractItemView::SingleSelection);

    /*ui->tableWidget_2->setColumnCount(5);
    ui->tableWidget_2->setHorizontalHeaderLabels(QStringList() << "opticstorID" << "IDclient" << "IDemployee" << "Date \n opticstor" << "prix total");
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);*/
    ui->tabcl_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabcl_2->setColumnCount(3);
    ui->tabcl_2->setHorizontalHeaderLabels(QStringList() << "IDclient" << "email" << "codeprod");
    ui->tabcl_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Configuration pour la gestion des clients
    ui->id_client->setValidator(new QIntValidator(0, 999999, this));
    ui->telephone->setValidator(new QIntValidator(0, 99999999, this));
    ui->id_client_3->setValidator(new QIntValidator(0, 999999, this));

    // Configuration pour la gestion des produits
    ui->code_produit->setValidator(new QIntValidator(0, 999999, this));
    ui->id_fournisseur_produit->setValidator(new QIntValidator(0, 999999, this));
    ui->quantite_produit->setValidator(new QIntValidator(0, 999999, this));
    ui->prix_produit->setValidator(new QDoubleValidator(0, 999999, 2, this));
    ui->code_recherche_produit->setValidator(new QIntValidator(0, 999999, this));

    // Afficher les clients et produits dans les tables
    ui->tableView->setModel(tmpClient.afficher());
    ui->table_produits->setModel(tmpProduit.afficher());

    // Connexions pour les produits
    connect(ui->ajouter_produit, &QPushButton::clicked, this, &opticstor::on_ajouter_produit_clicked);
    connect(ui->supprimer_produit, &QPushButton::clicked, this, &opticstor::on_supprimer_produit_clicked);
    connect(ui->modifier_produit, &QPushButton::clicked, this, &opticstor::on_modifier_produit_clicked);
    //connect(ui->rechercher_produit, &QPushButton::clicked, this, &opticstor::on_rechercher_produit_clicked);
   // connect(ui->actualiser_produit, &QPushButton::clicked, this, &opticstor::on_actualiser_produit_clicked);
}

// ==================== FONCTIONS POUR LA GESTION DES PRODUITS ====================

void opticstor::on_ajouter_produit_clicked()
{
    QString codeText = ui->code_produit->text();
    QString idFournisseurText = ui->id_fournisseur_produit->text();
    QString marque = ui->marque_produit->text();
    QString type = ui->type_produit->currentText();
    QString quantiteText = ui->quantite_produit->text();
    QString prixText = ui->prix_produit->text();

    // Validation des champs obligatoires
    if (codeText.isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Erreur - Champ manquant"),
                             QObject::tr("❌ Le champ <b>Code produit</b> est vide!"), QMessageBox::Ok);
        ui->code_produit->setFocus();
        return;
    }

    int code = codeText.toInt();
    if (code <= 0) {
        QMessageBox::warning(this, QObject::tr("Erreur - Code invalide"),
                             QObject::tr("❌ Le <b>code produit</b> est invalide!"), QMessageBox::Ok);
        ui->code_produit->setFocus();
        return;
    }

    if (idFournisseurText.isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Erreur - Champ manquant"),
                             QObject::tr("❌ Le champ <b>ID Fournisseur</b> est vide!"), QMessageBox::Ok);
        ui->id_fournisseur_produit->setFocus();
        return;
    }

    int id_fournisseur = idFournisseurText.toInt();
    if (id_fournisseur <= 0) {
        QMessageBox::warning(this, QObject::tr("Erreur - ID invalide"),
                             QObject::tr("❌ L'<b>ID fournisseur</b> est invalide!"), QMessageBox::Ok);
        ui->id_fournisseur_produit->setFocus();
        return;
    }

    // ============ DÉBUT - AJOUTEZ CES LIGNES ICI ============
    // Vérifier si le fournisseur existe
    if (!tmpProduit.verifierFournisseurExiste(id_fournisseur)) {
        QMessageBox::warning(this, QObject::tr("Erreur - Fournisseur inexistant"),
                             QObject::tr("❌ L'<b>ID fournisseur %1</b> n'existe pas!\n\n"
                                         "Veuillez d'abord créer ce fournisseur dans la gestion des fournisseurs.").arg(id_fournisseur), QMessageBox::Ok);
        ui->id_fournisseur_produit->setFocus();
        return;
    }
    // ============ FIN - AJOUTEZ CES LIGNES ICI ============

    // Le reste de votre code existant continue ici...
    if (marque.isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Erreur - Champ manquant"),
                             QObject::tr("❌ Le champ <b>Marque</b> est vide!"), QMessageBox::Ok);
        ui->marque_produit->setFocus();
        return;
    }

    if (quantiteText.isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Erreur - Champ manquant"),
                             QObject::tr("❌ Le champ <b>Quantité</b> est vide!"), QMessageBox::Ok);
        ui->quantite_produit->setFocus();
        return;
    }

    int quantite = quantiteText.toInt();
    if (quantite <= 0) {
        QMessageBox::warning(this, QObject::tr("Erreur - Quantité invalide"),
                             QObject::tr("❌ La <b>quantité</b> est invalide!"), QMessageBox::Ok);
        ui->quantite_produit->setFocus();
        return;
    }

    if (prixText.isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Erreur - Champ manquant"),
                             QObject::tr("❌ Le champ <b>Prix</b> est vide!"), QMessageBox::Ok);
        ui->prix_produit->setFocus();
        return;
    }

    float prix = prixText.toFloat();
    if (prix <= 0) {
        QMessageBox::warning(this, QObject::tr("Erreur - Prix invalide"),
                             QObject::tr("❌ Le <b>prix</b> est invalide!"), QMessageBox::Ok);
        ui->prix_produit->setFocus();
        return;
    }

    Produit p(code, id_fournisseur, marque, type, quantite, prix);
    bool test = p.ajouter();

    if (test) {
        QMessageBox::information(this, QObject::tr("✅ Succès"),
                                 QObject::tr("Produit <b>ajouté</b> avec succès!\n\n"
                                             "Code: <b>%1</b>\n"
                                             "Marque: <b>%2</b>\n"
                                             "Type: <b>%3</b>\n"
                                             "Quantité: <b>%4</b>").arg(code).arg(marque).arg(type).arg(quantite), QMessageBox::Ok);
        ui->table_produits->setModel(tmpProduit.afficher());

        // Vider les champs après ajout
        ui->code_produit->clear();
        ui->id_fournisseur_produit->clear();
        ui->marque_produit->clear();
        ui->quantite_produit->clear();
        ui->prix_produit->clear();
        ui->type_produit->setCurrentIndex(0);
    } else {
        QMessageBox::critical(this, QObject::tr("❌ Erreur d'ajout"),
                              QObject::tr("Échec de l'ajout du produit!\n\n"
                                          "Raisons possibles:\n"
                                          "• Le code <b>%1</b> existe déjà\n"
                                          "• L'ID fournisseur n'existe pas\n"
                                          "• Problème de connexion à la base de données").arg(code), QMessageBox::Ok);
    }
}
void opticstor::on_supprimer_produit_clicked()
{
    QString codeText = ui->code_recherche_produit->text();

    if (codeText.isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Erreur - Champ manquant"),
                             QObject::tr("❌ Le champ <b>Code à supprimer</b> est vide!\n\n"
                                         "Veuillez entrer le code du produit à supprimer."), QMessageBox::Ok);
        ui->code_recherche_produit->setFocus();
        return;
    }

    int code = codeText.toInt();
    if (code <= 0) {
        QMessageBox::warning(this, QObject::tr("Erreur - Code invalide"),
                             QObject::tr("❌ Le <b>code</b> est invalide!\n"
                                         "Le code doit être un nombre positif."), QMessageBox::Ok);
        ui->code_recherche_produit->setFocus();
        return;
    }

    // Demander confirmation avant suppression
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, QObject::tr("Confirmation de suppression"),
                                  QObject::tr("⚠️ Voulez-vous vraiment supprimer le produit avec le code <b>%1</b>?\n\n"
                                              "Cette action est irréversible!").arg(code),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return;
    }

    bool test = tmpProduit.supprimer(code);

    if (test) {
        QMessageBox::information(this, QObject::tr("✅ Succès"),
                                 QObject::tr("Produit avec le code <b>%1</b> a été <b>supprimé</b> avec succès!").arg(code), QMessageBox::Ok);
        ui->table_produits->setModel(tmpProduit.afficher());
        ui->code_recherche_produit->clear();
    } else {
        QMessageBox::critical(this, QObject::tr("❌ Erreur de suppression"),
                              QObject::tr("Échec de la suppression!\n\n"
                                          "Raisons possibles:\n"
                                          "• Aucun produit trouvé avec le code <b>%1</b>\n"
                                          "• Problème de connexion à la base de données\n"
                                          "• Erreur système").arg(code), QMessageBox::Ok);
    }
}

void opticstor::on_modifier_produit_clicked()
{
    QString codeText = ui->code_produit->text();
    QString idFournisseurText = ui->id_fournisseur_produit->text();
    QString marque = ui->marque_produit->text();
    QString type = ui->type_produit->currentText();
    QString quantiteText = ui->quantite_produit->text();
    QString prixText = ui->prix_produit->text();

    // Validation des champs obligatoires
    if (codeText.isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Erreur - Champ manquant"),
                             QObject::tr("❌ Le champ <b>Code produit</b> est vide!\n"
                                         "Veuillez entrer le code du produit à modifier."), QMessageBox::Ok);
        ui->code_produit->setFocus();
        return;
    }

    int code = codeText.toInt();
    if (code <= 0) {
        QMessageBox::warning(this, QObject::tr("Erreur - Code invalide"),
                             QObject::tr("❌ Le <b>code produit</b> est invalide!\n"
                                         "Le code doit être un nombre positif."), QMessageBox::Ok);
        ui->code_produit->setFocus();
        return;
    }

    if (idFournisseurText.isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Erreur - Champ manquant"),
                             QObject::tr("❌ Le champ <b>ID Fournisseur</b> est vide!"), QMessageBox::Ok);
        ui->id_fournisseur_produit->setFocus();
        return;
    }

    int id_fournisseur = idFournisseurText.toInt();
    if (id_fournisseur <= 0) {
        QMessageBox::warning(this, QObject::tr("Erreur - ID invalide"),
                             QObject::tr("❌ L'<b>ID fournisseur</b> est invalide!"), QMessageBox::Ok);
        ui->id_fournisseur_produit->setFocus();
        return;
    }

    if (marque.isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Erreur - Champ manquant"),
                             QObject::tr("❌ Le champ <b>Marque</b> est vide!"), QMessageBox::Ok);
        ui->marque_produit->setFocus();
        return;
    }

    if (quantiteText.isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Erreur - Champ manquant"),
                             QObject::tr("❌ Le champ <b>Quantité</b> est vide!"), QMessageBox::Ok);
        ui->quantite_produit->setFocus();
        return;
    }

    int quantite = quantiteText.toInt();
    if (quantite <= 0) {
        QMessageBox::warning(this, QObject::tr("Erreur - Quantité invalide"),
                             QObject::tr("❌ La <b>quantité</b> est invalide!"), QMessageBox::Ok);
        ui->quantite_produit->setFocus();
        return;
    }

    if (prixText.isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Erreur - Champ manquant"),
                             QObject::tr("❌ Le champ <b>Prix</b> est vide!"), QMessageBox::Ok);
        ui->prix_produit->setFocus();
        return;
    }

    float prix = prixText.toFloat();
    if (prix <= 0) {
        QMessageBox::warning(this, QObject::tr("Erreur - Prix invalide"),
                             QObject::tr("❌ Le <b>prix</b> est invalide!"), QMessageBox::Ok);
        ui->prix_produit->setFocus();
        return;
    }

    bool test = tmpProduit.modifier(code, id_fournisseur, marque, type, quantite, prix);

    if (test) {
        QMessageBox::information(this, QObject::tr("✅ Succès"),
                                 QObject::tr("Produit avec le code <b>%1</b> a été <b>modifié</b> avec succès!\n\n"
                                             "Nouveaux détails:\n"
                                             "• Marque: <b>%2</b>\n"
                                             "• Type: <b>%3</b>\n"
                                             "• Quantité: <b>%4</b>\n"
                                             "• Prix: <b>%5 DT</b>").arg(code).arg(marque).arg(type).arg(quantite).arg(prix), QMessageBox::Ok);
        ui->table_produits->setModel(tmpProduit.afficher());

        // Vider les champs après modification
        ui->code_produit->clear();
        ui->id_fournisseur_produit->clear();
        ui->marque_produit->clear();
        ui->quantite_produit->clear();
        ui->prix_produit->clear();
        ui->type_produit->setCurrentIndex(0);
    } else {
        QMessageBox::critical(this, QObject::tr("❌ Erreur de modification"),
                              QObject::tr("Échec de la modification!\n\n"
                                          "Raisons possibles:\n"
                                          "• Aucun produit trouvé avec le code <b>%1</b>\n"
                                          "• Problème de connexion à la base de données\n"
                                          "• Erreur système").arg(code), QMessageBox::Ok);
    }
}

void opticstor::on_rechercher_produit_clicked()
{
    QString codeText = ui->code_recherche_produit->text();

    if (codeText.isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Erreur - Champ manquant"),
                             QObject::tr("❌ Le champ <b>Code à rechercher</b> est vide!\n\n"
                                         "Veuillez entrer le code du produit à rechercher."), QMessageBox::Ok);
        ui->code_recherche_produit->setFocus();
        return;
    }

    int code = codeText.toInt();
    if (code <= 0) {
        QMessageBox::warning(this, QObject::tr("Erreur - Code invalide"),
                             QObject::tr("❌ Le <b>code</b> est invalide!\n"
                                         "Le code doit être un nombre positif."), QMessageBox::Ok);
        ui->code_recherche_produit->setFocus();
        return;
    }

    QSqlQueryModel* model = tmpProduit.rechercherParCode(code);

    if (model->rowCount() == 0) {
        QMessageBox::information(this, QObject::tr("Recherche"),
                                 QObject::tr("Aucun produit trouvé avec le code <b>%1</b>.").arg(code), QMessageBox::Ok);
        ui->table_produits->setModel(tmpProduit.afficher());
    } else {
        ui->table_produits->setModel(model);
        QMessageBox::information(this, QObject::tr("✅ Produit trouvé"),
                                 QObject::tr("Produit avec le code <b>%1</b> a été trouvé!").arg(code), QMessageBox::Ok);
    }
}

void opticstor::on_actualiser_produit_clicked()
{
    ui->table_produits->setModel(tmpProduit.afficher());
    QMessageBox::information(this, QObject::tr("✅ Actualisation"),
                             QObject::tr("Liste des produits <b>actualisée</b> avec succès!"), QMessageBox::Ok);
}

// ==================== FONCTIONS POUR LA GESTION DES CLIENTS ====================

void opticstor::on_ajouter_clicked()
{
    QString idText = ui->id_client->text();
    QString nom = ui->nom->text();
    QString prenom = ui->prenom->text();
    QString email = ui->email->text();
    QString telephone = ui->telephone->text();

    // Validation des champs obligatoires avec messages spécifiques
    if (idText.isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Erreur - Champ manquant"),
                             QObject::tr("❌ Le champ <b>ID client</b> est vide!\n"
                                         "Veuillez entrer un ID."), QMessageBox::Ok);
        ui->id_client->setFocus();
        return;
    }

    int id = idText.toInt();
    if (id <= 0) {
        QMessageBox::warning(this, QObject::tr("Erreur - ID invalide"),
                             QObject::tr("❌ L'<b>ID client</b> est invalide!\n"
                                         "L'ID doit être un nombre positif."), QMessageBox::Ok);
        ui->id_client->setFocus();
        return;
    }

    if (nom.isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Erreur - Champ manquant"),
                             QObject::tr("❌ Le champ <b>Nom</b> est vide!"), QMessageBox::Ok);
        ui->nom->setFocus();
        return;
    }

    if (prenom.isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Erreur - Champ manquant"),
                             QObject::tr("❌ Le champ <b>Prénom</b> est vide!"), QMessageBox::Ok);
        ui->prenom->setFocus();
        return;
    }

    if (email.isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Erreur - Champ manquant"),
                             QObject::tr("❌ Le champ <b>Email</b> est vide!"), QMessageBox::Ok);
        ui->email->setFocus();
        return;
    }

    if (telephone.isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Erreur - Champ manquant"),
                             QObject::tr("❌ Le champ <b>Téléphone</b> est vide!"), QMessageBox::Ok);
        ui->telephone->setFocus();
        return;
    }

    // Validation du téléphone
    if (!Client::estTelephoneValide(telephone)) {
        QMessageBox::warning(this, QObject::tr("Erreur - Téléphone invalide"),
                             QObject::tr("❌ Le <b>numéro de téléphone</b> est invalide!\n\n"
                                         "✓ Doit contenir <b>uniquement des chiffres</b>\n"
                                         "✓ Doit avoir au moins <b>8 chiffres</b>\n"
                                         "✗ Votre saisie: <b>%1</b>").arg(telephone), QMessageBox::Ok);
        ui->telephone->setFocus();
        ui->telephone->selectAll();
        return;
    }

    // Validation de l'email
    if (!Client::estEmailValide(email)) {
        QMessageBox::warning(this, QObject::tr("Erreur - Email invalide"),
                             QObject::tr("❌ L'<b>adresse email</b> est invalide!\n\n"
                                         "✓ Format valide: <b>exemple@domaine.com</b>\n"
                                         "✓ Doit contenir <b>@</b> et un <b>domaine</b>\n"
                                         "✗ Votre saisie: <b>%1</b>").arg(email), QMessageBox::Ok);
        ui->email->setFocus();
        ui->email->selectAll();
        return;
    }

    Client c(id, nom, prenom, email, telephone);
    bool test = c.ajouter();

    if (test) {
        QMessageBox::information(this, QObject::tr("✅ Succès"),
                                 QObject::tr("Client <b>ajouté</b> avec succès!\n\n"
                                             "ID: <b>%1</b>\n"
                                             "Nom: <b>%2 %3</b>").arg(id).arg(prenom).arg(nom), QMessageBox::Ok);
        ui->tableView->setModel(tmpClient.afficher());

        // Vider les champs après ajout
        ui->id_client->clear();
        ui->nom->clear();
        ui->prenom->clear();
        ui->email->clear();
        ui->telephone->clear();
    } else {
        QMessageBox::critical(this, QObject::tr("❌ Erreur d'ajout"),
                              QObject::tr("Échec de l'ajout du client!\n\n"
                                          "Raisons possibles:\n"
                                          "• L'ID <b>%1</b> existe déjà\n"
                                          "• Problème de connexion à la base de données\n"
                                          "• Erreur système").arg(id), QMessageBox::Ok);
    }
}

void opticstor::on_supprimer_clicked()
{
    QString idText = ui->id_client_3->text();

    if (idText.isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Erreur - Champ manquant"),
                             QObject::tr("❌ Le champ <b>ID à supprimer</b> est vide!\n\n"
                                         "Veuillez entrer l'ID du client à supprimer."), QMessageBox::Ok);
        ui->id_client_3->setFocus();
        return;
    }

    int id = idText.toInt();
    if (id <= 0) {
        QMessageBox::warning(this, QObject::tr("Erreur - ID invalide"),
                             QObject::tr("❌ L'<b>ID</b> est invalide!\n"
                                         "L'ID doit être un nombre positif."), QMessageBox::Ok);
        ui->id_client_3->setFocus();
        return;
    }

    // Demander confirmation avant suppression
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, QObject::tr("Confirmation de suppression"),
                                  QObject::tr("⚠️ Voulez-vous vraiment supprimer le client avec l'ID <b>%1</b>?\n\n"
                                              "Cette action est irréversible!").arg(id),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return;
    }

    bool test = tmpClient.supprimer(id);

    if (test) {
        QMessageBox::information(this, QObject::tr("✅ Succès"),
                                 QObject::tr("Client avec l'ID <b>%1</b> a été <b>supprimé</b> avec succès!").arg(id), QMessageBox::Ok);
        ui->tableView->setModel(tmpClient.afficher());
        ui->id_client_3->clear();
    } else {
        QMessageBox::critical(this, QObject::tr("❌ Erreur de suppression"),
                              QObject::tr("Échec de la suppression!\n\n"
                                          "Raisons possibles:\n"
                                          "• Aucun client trouvé avec l'ID <b>%1</b>\n"
                                          "• Problème de connexion à la base de données\n"
                                          "• Erreur système").arg(id), QMessageBox::Ok);
    }
}

void opticstor::on_modifier_clicked()
{
    QString idText = ui->id_client->text();
    QString nom = ui->nom->text();
    QString prenom = ui->prenom->text();
    QString email = ui->email->text();
    QString telephone = ui->telephone->text();

    // Validation des champs obligatoires avec messages spécifiques
    if (idText.isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Erreur - Champ manquant"),
                             QObject::tr("❌ Le champ <b>ID client</b> est vide!\n"
                                         "Veuillez entrer l'ID du client à modifier."), QMessageBox::Ok);
        ui->id_client->setFocus();
        return;
    }

    int id = idText.toInt();
    if (id <= 0) {
        QMessageBox::warning(this, QObject::tr("Erreur - ID invalide"),
                             QObject::tr("❌ L'<b>ID client</b> est invalide!\n"
                                         "L'ID doit être un nombre positif."), QMessageBox::Ok);
        ui->id_client->setFocus();
        return;
    }

    if (nom.isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Erreur - Champ manquant"),
                             QObject::tr("❌ Le champ <b>Nom</b> est vide!"), QMessageBox::Ok);
        ui->nom->setFocus();
        return;
    }

    if (prenom.isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Erreur - Champ manquant"),
                             QObject::tr("❌ Le champ <b>Prénom</b> est vide!"), QMessageBox::Ok);
        ui->prenom->setFocus();
        return;
    }

    if (email.isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Erreur - Champ manquant"),
                             QObject::tr("❌ Le champ <b>Email</b> est vide!"), QMessageBox::Ok);
        ui->email->setFocus();
        return;
    }

    if (telephone.isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Erreur - Champ manquant"),
                             QObject::tr("❌ Le champ <b>Téléphone</b> est vide!"), QMessageBox::Ok);
        ui->telephone->setFocus();
        return;
    }

    // Validation du téléphone
    if (!Client::estTelephoneValide(telephone)) {
        QMessageBox::warning(this, QObject::tr("Erreur - Téléphone invalide"),
                             QObject::tr("❌ Le <b>numéro de téléphone</b> est invalide!\n\n"
                                         "✓ Doit contenir <b>uniquement des chiffres</b>\n"
                                         "✓ Doit avoir au moins <b>8 chiffres</b>\n"
                                         "✗ Votre saisie: <b>%1</b>").arg(telephone), QMessageBox::Ok);
        ui->telephone->setFocus();
        ui->telephone->selectAll();
        return;
    }

    // Validation de l'email
    if (!Client::estEmailValide(email)) {
        QMessageBox::warning(this, QObject::tr("Erreur - Email invalide"),
                             QObject::tr("❌ L'<b>adresse email</b> est invalide!\n\n"
                                         "✓ Format valide: <b>exemple@domaine.com</b>\n"
                                         "✓ Doit contenir <b>@</b> et un <b>domaine</b>\n"
                                         "✗ Votre saisie: <b>%1</b>").arg(email), QMessageBox::Ok);
        ui->email->setFocus();
        ui->email->selectAll();
        return;
    }

    Client c(id, nom, prenom, email, telephone);
    bool test = c.modifier();

    if (test) {
        QMessageBox::information(this, QObject::tr("✅ Succès"),
                                 QObject::tr("Client avec l'ID <b>%1</b> a été <b>modifié</b> avec succès!\n\n"
                                             "Nouveaux détails:\n"
                                             "• Nom: <b>%2 %3</b>\n"
                                             "• Email: <b>%4</b>\n"
                                             "• Téléphone: <b>%5</b>").arg(id).arg(prenom).arg(nom).arg(email).arg(telephone), QMessageBox::Ok);
        ui->tableView->setModel(tmpClient.afficher());

        // Vider les champs après modification
        ui->id_client->clear();
        ui->nom->clear();
        ui->prenom->clear();
        ui->email->clear();
        ui->telephone->clear();
    } else {
        QMessageBox::critical(this, QObject::tr("❌ Erreur de modification"),
                              QObject::tr("Échec de la modification!\n\n"
                                          "Raisons possibles:\n"
                                          "• Aucun client trouvé avec l'ID <b>%1</b>\n"
                                          "• Problème de connexion à la base de données\n"
                                          "• Erreur système").arg(id), QMessageBox::Ok);
    }
}

// ==================== FONCTIONS EXISTANTES ====================

/*void opticstor::onSubmitClicked()
{
    int cmdId = ui->cmid_2->text().toInt();
    int prix = ui->prt_2->text().toInt();
    QString text2 = ui->idcl_2->text();
    QString text3 = ui->idemp_2->text();
    QString text4 = ui->datcmd_2->text();

    int newRow = ui->tableWidget_2->rowCount();
    ui->tableWidget_2->insertRow(newRow);

    ui->tableWidget_2->setItem(newRow, 1, new QTableWidgetItem(text2));
    ui->tableWidget_2->setItem(newRow, 2, new QTableWidgetItem(text3));
    ui->tableWidget_2->setItem(newRow, 3, new QTableWidgetItem(text4));
    ui->tableWidget_2->setItem(newRow, 0, new QTableWidgetItem(QString::number(cmdId)));
    ui->tableWidget_2->item(newRow, 0)->setData(Qt::EditRole, cmdId);

    ui->tableWidget_2->setItem(newRow, 4, new QTableWidgetItem(QString::number(prix)));
    ui->tableWidget_2->item(newRow, 4)->setData(Qt::EditRole, prix);

    ui->cmid_2->clear();
    ui->idcl_2->clear();
    ui->idemp_2->clear();
    ui->datcmd_2->clear();
    ui->prt_2->clear();
}

void opticstor::onSubmitClickedd()
{
    int cmdId = ui->cdprod_2->text().toInt();
    QString text2 = ui->idcll_2->text();
    QString text3 = ui->email_2->text();

    int newRow = ui->tabcl_2->rowCount();
    ui->tabcl_2->insertRow(newRow);

    ui->tabcl_2->setItem(newRow, 0, new QTableWidgetItem(text2));
    ui->tabcl_2->setItem(newRow, 1, new QTableWidgetItem(text3));
    ui->tabcl_2->setItem(newRow, 2, new QTableWidgetItem(QString::number(cmdId)));
    ui->tabcl_2->item(newRow, 2)->setData(Qt::EditRole, cmdId);

    ui->idcll_2->clear();
    ui->email_2->clear();
    ui->cdprod_2->clear();
}

void opticstor::onCancelClickedd() {
    ui->idcll_2->clear();
    ui->email_2->clear();
    ui->cdprod_2->clear();
}

void opticstor::onCancelClicked() {
    ui->cmid_2->clear();
    ui->idcl_2->clear();
    ui->idemp_2->clear();
    ui->datcmd_2->clear();
    ui->prt_2->clear();
}

void opticstor::onDeleteClicked()
{
    QModelIndexList selected = ui->tableWidget_2->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Delete", "Please select a row to delete.");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Delete",
                                  "Are you sure you want to delete the selected row?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        int row = selected.first().row();
        ui->tableWidget_2->removeRow(row);
    }
}

void opticstor::onDeleteClickedd()
{
    QModelIndexList selected = ui->tabcl_2->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Delete", "Please select a row to delete.");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Delete",
                                  "Are you sure you want to delete the selected row?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        int row = selected.first().row();
        ui->tabcl_2->removeRow(row);
    }
}

void opticstor::onSearchClicked()
{
    QString text = ui->searchLineEdit_2->text();
    if (text.isEmpty())
        return;

    for (int row = 0; row < ui->tableWidget_2->rowCount(); ++row) {
        for (int col = 0; col < ui->tableWidget_2->columnCount(); ++col) {
            QTableWidgetItem *item = ui->tableWidget_2->item(row, col);
            if (item && item->text().contains(text, Qt::CaseInsensitive)) {
                ui->tableWidget_2->setCurrentCell(row, col);
                return;
            }
        }
    }

    QMessageBox::information(this, "Not found", "No matching result in the table.");
}

void opticstor::onEditClicked()
{
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
    QModelIndex currentIndex = ui->tableWidget_2->currentIndex();
    if (currentIndex.isValid()) {
        ui->tableWidget_2->edit(currentIndex);
    }
}

void opticstor::onEditClickedd()
{
    ui->tabcl_2->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
    QModelIndex currentIndex = ui->tabcl_2->currentIndex();
    if (currentIndex.isValid()) {
        ui->tabcl_2->edit(currentIndex);
    }
}
*/
void opticstor::onGestCommande(){
    ui->opticstack->setCurrentIndex(1);
}

void opticstor::onGest(){
    ui->opticstack->setCurrentIndex(2);
}

/*void opticstor::onSortChanged(int index)
{
    ui->tabcl_2->sortItems(index, Qt::AscendingOrder);
}*/

void opticstor:: onprod(){
    ui->opticstack->setCurrentIndex(3);
}

void opticstor::onemp(){
    ui->opticstack->setCurrentIndex(4);
}

void opticstor::onfourn(){
    ui->opticstack->setCurrentIndex(0);
}
// ==================== FONCTIONS POUR LA GESTION DES COMMANDES ====================
void opticstor::updatecommande(){
    QList<QTableWidgetItem*> selected = ui->tablecmd->selectedItems();
    if (selected.isEmpty()) return;

    int selectedRow = selected.first()->row();
    QTableWidgetItem* idItem = ui->tablecmd->item(selectedRow, 0);
    if (!idItem) return;

    int actualId = idItem->text().toInt();

    QSqlQuery query;
    if(ui->idclientc->currentIndex()!=0){
        query.prepare("UPDATE    COMMANDE SET ID_CLIENT=:client WHERE ID_COMMANDE=:idcmd");
        query.bindValue(":client",ui->idclientc->currentText());
        query.bindValue(":idcmd",actualId);
        query.exec();
    }
    if(ui->idemployec->currentIndex()!=0){
        query.prepare("UPDATE COMMANDE SET CIN=:cin WHERE ID_COMMANDE=:idcmd");
        query.bindValue(":cin",ui->idemployec->currentText());
        query.bindValue(":idcmd",actualId);
        query.exec();
    }
    if(ui->typec->currentIndex()!=0){
        QSqlQuery tqrt;
        tqrt.prepare("SELECT CODE_PRODUIT FROM PRODUIT WHERE TYPE = :type AND MARQUE = :marque AND QUANTITE_STOCK > 0 AND CODE_PRODUIT NOT IN (SELECT CODE_PRODUIT FROM CONTIENT)");
        tqrt.bindValue(":type",ui->typec->currentText());

        query.prepare("UPDATE CONTIENT c SET c.CODE_PRODUIT=:cin WHERE c.ID_COMMANDE=:idcmd AND PRODUIT p");
        query.bindValue(":cin",ui->idemployec->currentText());
        query.bindValue(":idcmd",actualId);
        query.exec();
    }
    ui->commandsc->setCurrentIndex(1);

    affichecommande();
}

void opticstor::supprimcommande(){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Voulez-vous vraiment supprimer cette commande?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply != QMessageBox::Yes) return;
    QList<QTableWidgetItem*> selected = ui->tablecmd->selectedItems();
    if (selected.isEmpty()) return;

    int selectedRow = selected.first()->row();
    QTableWidgetItem* idItem = ui->tablecmd->item(selectedRow, 0);
    if (!idItem) return;

    int actualId = idItem->text().toInt();
    QSqlQuery deleteContenir;
    deleteContenir.prepare("DELETE FROM CONTIENT WHERE ID_COMMANDE=:id");
    deleteContenir.bindValue(":id", actualId);
    if(deleteContenir.exec()){
        QSqlQuery query;
        query.prepare("DELETE FROM COMMANDE WHERE ID_COMMANDE=:id");
        query.bindValue(":id", actualId);

        if(query.exec()){
            qDebug() << "deleted row";
            ui->tablecmd->removeRow(selectedRow);
            affichecommande();
        } else {
            qDebug() << "failed to delete:" << query.lastError().text();
        }
    }else{
        qDebug() << "failed to delete from contenir:" << deleteContenir.lastError().text();
    }
}

bool opticstor::commandExists(int commandId) {
    QSqlQuery query;
    query.prepare("SELECT 1 FROM COMMANDE WHERE ID_COMMANDE = :id");
    query.bindValue(":id", commandId);
    return (query.exec() && query.next());
}

void opticstor::loadtypesmarquesComboBox() {

    QSqlQuery query;

    query.exec("SELECT DISTINCT TYPE FROM PRODUIT ORDER BY TYPE");
    ui->type->addItem("Choisir type");
    ui->typec->addItem("Choisir type");
    while(query.next()) {
        ui->type->addItem(query.value(0).toString());
        //ui->typec->addItem(query.value(0).toString());
    }

    query.exec("SELECT DISTINCT MARQUE FROM PRODUIT ORDER BY MARQUE");
    ui->marque->addItem("Choisir marque");
    ui->marquec->addItem("Choisir marque");
    while(query.next()) {
        ui->marque->addItem(query.value(0).toString());
        ui->marque->addItem(query.value(0).toString());
    }

    ui->typec->setCurrentIndex(0);
    ui->marquec->setCurrentIndex(0);
    ui->type->setCurrentIndex(0);
    ui->marque->setCurrentIndex(0);
}

void opticstor::loademp(){
    QSqlQuery query;
    query.exec("SELECT CIN FROM EMPLOYE");
    ui->idemploye->addItem("Choisir employe");
    ui->idemployec->addItem("Choisir employe");
    while(query.next()){
        ui->idemploye->addItem(query.value(0).toString());
        ui->idemployec->addItem(query.value(0).toString());
    }
    ui->idemploye->setCurrentIndex(0);
    ui->idemployec->setCurrentIndex(0);
}

void opticstor::loadclientComboBox()
{
    QSqlQuery query;
    query.exec("SELECT ID_CLIENT FROM CLIENT");
    ui->idclient->addItem("Choisir client");
    ui->idclientc->addItem("Choisir client");
    while(query.next()){
        ui->idclient->addItem(query.value(0).toString());
        ui->idclientc->addItem(query.value(0).toString());
    }
    ui->idclient->setCurrentIndex(0);
    ui->idclientc->setCurrentIndex(0);
}

void opticstor::autrecmd(){
    if (ui->IDcommande->text().isEmpty()  || ui->idclient->currentIndex()==-1 || ui->idemploye->currentIndex()==-1 || ui->type->currentIndex()==-1 || ui->marque->currentIndex()==-1) {
        QMessageBox::warning(this, "Champs manquants", "Veuillez remplir tous les champs!");
        return;
    }
    if(prixtotal==0 && commandExists(ui->IDcommande->text().toInt())){
        QMessageBox::warning(this, "ID existant", "choisir un ID inexistant!");
        return;
    }

    QSqlQuery query;
    type=ui->type->currentText();
    marque=ui->marque->currentText();
    query.prepare("SELECT CODE_PRODUIT, PRIX FROM PRODUIT WHERE TYPE = :type AND MARQUE = :marque AND QUANTITE_STOCK > 0 AND CODE_PRODUIT NOT IN (SELECT CODE_PRODUIT FROM CONTIENT)");
    query.bindValue(":type", type);
    query.bindValue(":marque", marque);

    if (query.exec() && query.next()) {
        QString id = query.value(0).toString();
        double price = query.value(1).toDouble();
        QSqlQuery queryy;
        if(prixtotal==0){
            QSqlQuery cmd;
            cmd.prepare("INSERT INTO COMMANDE(ID_COMMANDE) VALUES(:cmd)");
            cmd.bindValue(":cmd",ui->IDcommande->text().toInt());
            if(cmd.exec()){qDebug()<<"success";}
            else{qDebug()<<"failed";}
        }
        queryy.prepare("INSERT INTO CONTIENT(ID_COMMANDE,CODE_PRODUIT) VALUES (:cmd,:id)");
        queryy.bindValue(":cmd",ui->IDcommande->text().toInt());
        queryy.bindValue(":id",id);
        qDebug() << "Available product:" << id << "Price:" << price;
        if(queryy.exec()){
            qDebug()<<"inserted";
            QSqlQuery dquerry;
            dquerry.prepare("UPDATE PRODUIT SET QUANTITE_STOCK=QUANTITE_STOCK-1 WHERE CODE_PRODUIT=:id");
            dquerry.bindValue(":id",id);
            if(dquerry.exec()){qDebug()<<"deleted";}
            else{qDebug() <<"failed to delete";}
            ui->cmdlabel->setText(ui->IDcommande->text());
            ui->clientlabel->setText(ui->idclient->currentText());
            ui->emplabel->setText(ui->idemploye->currentText());
            ui->commands->setCurrentIndex(1);
            prixtotal += price;
            ui->type->setCurrentIndex(0);
            ui->marque->setCurrentIndex(0);
            qDebug() << "Total price:" << prixtotal;
        }else {
            qDebug() << "Failed to insert into contenir:" << queryy.lastError().text();
            qDebug() << "Last query:" << queryy.lastQuery();
            qDebug() << "Bound values - cmd:" << ui->IDcommande->text().toInt()
                     << "id:" << id << "price:" << price;
            QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout du produit à la commande!");
        }

    } else {
        qDebug() << "No available products found";
        QMessageBox::information(this, "Non disponible",
                                 "Aucun produit trouvé avec type: " + type +
                                     " et marque: " + marque);
    }
}

void opticstor::ajoutcommande(){
    if (ui->IDcommande->text().isEmpty()  || ui->idclient->currentIndex()==-1 || ui->idemploye->currentIndex()==-1 || ui->type->currentIndex()==-1 || ui->marque->currentIndex()==-1) {
        QMessageBox::warning(this, "Champs manquants", "Veuillez remplir tous les champs!");
        return;
    }
    if(prixtotal==0 && commandExists(ui->IDcommande->text().toInt())){
        QMessageBox::warning(this, "ID existant", "choisir un ID inexistant!");
        return;
    }
    type=ui->type->currentText();
    marque=ui->marque->currentText();
    QSqlQuery querry;
    querry.prepare("SELECT CODE_PRODUIT, PRIX FROM PRODUIT WHERE TYPE = :type AND MARQUE = :marque AND QUANTITE_STOCK > 0 AND CODE_PRODUIT NOT IN (SELECT CODE_PRODUIT FROM CONTIENT)");
    querry.bindValue(":type", type);
    querry.bindValue(":marque", marque);
    if (querry.exec() && querry.next()) {
        QString id = querry.value(0).toString();
        double price = querry.value(1).toDouble();
        QDate date = QDate::currentDate();
        QSqlQuery query,squery;
        if(prixtotal==0){
            query.prepare("INSERT INTO COMMANDE (ID_COMMANDE,ID_CLIENT,CIN, DATE_COMMANDE,PRIX_TOTALE) VALUES (:comd,:client,:cin, TO_DATE(:date, 'DD/MM/YYYY'), :prixtot)");
            query.bindValue(":prixtot",price);
        }else{
            query.prepare("UPDATE COMMANDE SET PRIX_TOTALE = :prixtot, CIN=:cin, ID_CLIENT=:client,DATE_COMMANDE=TO_DATE(:date, 'DD/MM/YYYY')  WHERE ID_COMMANDE = :comd");
            query.bindValue(":prixtot",prixtotal+price);
        }
        query.bindValue(":comd", ui->IDcommande->text().toInt());
        query.bindValue(":client", ui->idclient->currentText());
        query.bindValue(":cin", ui->idemploye->currentText());
        query.bindValue(":date",  date.toString("dd/MM/yyyy"));
        squery.prepare("INSERT INTO CONTIENT (ID_COMMANDE,CODE_PRODUIT) VALUES(:cmd,:cdprod)");
        squery.bindValue(":cmd",ui->IDcommande->text().toInt());
        squery.bindValue(":cdprod",id);
        if (query.exec() && squery.exec()) {
            qDebug() << "Person saved to database!";
            qDebug()<<"inserted";
            QSqlQuery dquerry;
            dquerry.prepare("UPDATE PRODUIT SET QUANTITE_STOCK=QUANTITE_STOCK-1 WHERE CODE_PRODUIT=:id");
            dquerry.bindValue(":id",id);
            if(dquerry.exec()){qDebug()<<"deleted";}
            else{qDebug() <<"failed to delete";}
            ui->type->setCurrentIndex(-1);
            ui->marque->setCurrentIndex(-1);
            qDebug() << "Total price:" << prixtotal;

            ui->IDcommande->clear();
            ui->idclient->setCurrentIndex(0);
            ui->idemploye->setCurrentIndex(0);
            ui->type->setCurrentIndex(0);
            ui->marque->setCurrentIndex(0);
            prixtotal=0;
            ui->commands->setCurrentIndex(0);
            affichecommande();
        } else {
            qDebug() << "Main query error:" << query.lastError().text();
            qDebug() << "Main query last query:" << query.lastQuery();
            qDebug() << "Main query bound values:" << query.boundValues();
            qDebug() << "Contenir query error:" << squery.lastError().text();
            qDebug() << "Contenir query last query:" << squery.lastQuery();
            qDebug() << "Contenir query bound values:" << squery.boundValues();
            qDebug() << "Error:" << query.lastError().text();
            QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout du produit à la commande!");
        }
    }else {
        qDebug() << "No available products found";
        QMessageBox::information(this, "Non disponible",
                                 "Aucun produit trouvé avec type: " + type +
                                     " et marque: " + marque);
    }
}

void opticstor::affichecommande(){
    QSqlQuery query("SELECT * FROM COMMANDE ORDER BY ID_COMMANDE");
    ui->tablecmd->setRowCount(0);
    QStringList headers;
    headers << "ID Commande" << "ID Client" << "CIN" << "Date" << "Prix Total";
    ui->tablecmd->setColumnCount(headers.size());
    ui->tablecmd->setHorizontalHeaderLabels(headers);

    int row = 0;
    while (query.next()) {
        ui->tablecmd->insertRow(row);
        for (int col = 0; col < headers.size(); col++) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            ui->tablecmd->setItem(row, col, item);
        }
        row++;
    }
    if (ui->tablecmd->rowCount() == 0) {
        int tableWidth = ui->tablecmd->width();
        int columnWidth = tableWidth / 5;
        for (int i = 0; i < 5; ++i) {
            ui->tablecmd->setColumnWidth(i, columnWidth);
        }
    }
    else{ui->tablecmd->resizeColumnsToContents();}
    ui->tablecmd->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    qDebug() << "Success! Loaded" << row << "rows";
}
void opticstor::updmen(){
    ui->commandsc->setCurrentIndex(0);
}
opticstor::~opticstor()
{
    delete ui;
}
