#include "opticstor.h"
#include "ui_opticstor.h"
#include <QIntValidator>
#include <QDoubleValidator>
#include <QDebug>
#include <QDate>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QtCharts>
#include <QPrinter>
#include <QTextDocument>
#include <QTextCursor>
#include <QFileDialog>
#include <QDesktopServices>
#include <QDir>
#include <QMenu>
#include <QAction>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>
 //#include <QtCharts/QChartView>
 //#include <QtCharts/QPieSeries>
#include <QtCharts>

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
#include <QFileInfo>
#include <QDateTime>
#include <QSerialPort>  // ← AJOUTER
#include <QSerialPortInfo>  // ← AJOUTER
#include <QThread>
opticstor::opticstor(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui_opticstor)
{
    ui->setupUi(this);

    // ==================== INITIALISATION ARDUINO ====================
    qDebug() << "=== TENTATIVE DE CONNEXION ARDUINO ===";

    int ret = A.connect_arduino();

    qDebug() << "Résultat connexion Arduino:" << ret;

    switch(ret) {
    case 0:
        qDebug() << "✅ SUCCES: Arduino connectée sur port:" << A.getarduino_port_name();

        // Attendre un peu pour la stabilisation
        QThread::sleep(1);

        // Connecter le signal readyRead
        QObject::connect(A.getserial(), &QSerialPort::readyRead,
                         this, &opticstor::update_label);

        // Tester la communication
        A.write_to_arduino("HELLO\n");
        qDebug() << "Message 'HELLO' envoyé à Arduino";

        QMessageBox::information(this, "Arduino",
                                 "✅ Arduino connectée avec succès!\n"
                                 "Port: " + A.getarduino_port_name());
        break;

    case 1:
        qDebug() << "⚠️ ARDUINO DÉTECTÉ mais non connecté";
        QMessageBox::warning(this, "Arduino",
                             "Arduino détecté mais impossible d'ouvrir le port.\n"
                             "Vérifiez qu'aucun autre programme n'utilise le port série.");
        break;

    case -1:
        qDebug() << "❌ AUCUN ARDUINO DÉTECTÉ";

        // Lister tous les ports disponibles pour debug
        qDebug() << "Ports disponibles:";
        foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
            qDebug() << " - " << info.portName()
            << "(" << info.description() << ")"
            << "VID:" << info.vendorIdentifier()
            << "PID:" << info.productIdentifier();
        }

        QMessageBox::critical(this, "Arduino",
                              "Aucun Arduino détecté!\n\n"
                              "Vérifiez:\n"
                              "1. Que l'Arduino est bien branchée en USB\n"
                              "2. Que le driver est installé\n"
                              "3. Redémarrez l'application\n"
                              "4. Essayez un autre câble USB");
        break;
    }
    //-------------camerastuff---------.
    connect(ui->next, &QPushButton::clicked,ui->camera, &camerawidget::nextGlasses);
    //ui->camera->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    // After ui->setupUi(this);
    /* QVBoxLayout* layout = new QVBoxLayout(ui->camera->parentWidget()); // the QGroupBox
    layout->setContentsMargins(0, 0, 0, 0);        // remove extra spacing
    layout->addWidget(ui->camera);                 // add the camera widget to the layout
    layout->setAlignment(ui->camera, Qt::AlignCenter); // center it inside the layout*/
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->groupBox_13->layout());
    if (!layout) {
        layout = new QVBoxLayout(ui->groupBox_13);
        ui->groupBox_13->setLayout(layout);
    }
    /*// In MainWindow constructor
    qDebug() << "GUI initializing...";
    show();*/

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    ui->camera->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->camera->setFixedSize(231, 231);

    layout->addWidget(ui->camera, 0, Qt::AlignCenter);

    worker = new cameraworker(0, this);
    connect(worker, &cameraworker::frameReady, ui->camera, &camerawidget::onFrame);
    worker->start();
    //------------------------------------------

    prixtotal=0;
    rech=0;
    ui->commandsc->setCurrentIndex(1);
    connect(ui->ajout, &QPushButton::clicked, this, &opticstor::ajoutcommande);
    connect(ui->supp, &QPushButton::clicked, this, &opticstor::supprimcommande);
    connect(ui->update, &QPushButton::clicked, this, &opticstor::updatecommande);
    connect(ui->autre, &QPushButton::clicked, this, &opticstor::autrecmd);
    connect(ui->updatec, &QPushButton::clicked, this, &opticstor::updmen);
    connect(ui->stat, &QPushButton::clicked,
            this, &opticstor::on_showChartButton_clicked);
    connect(ui->tablecmd, &QTableWidget::cellClicked,this, &opticstor::on_commandSelected);
    // Dans le constructeur opticstor::opticstor() :
    connect(ui->boutonTestArduino, &QPushButton::clicked,
            this, &opticstor::on_boutonTestArduino_clicked);
    connect(ui->pdff, &QPushButton::clicked,
            this, &opticstor::on_generatePDFButton_clicked);

    connect(ui->orderbox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(affichecommande()));
    loadtypesmarquesComboBox();
    loadclientComboBox();
    loademp();
    affichecommande();
    // Connexions pour la navigation
    connect(ui->commandebut, &QPushButton::clicked, this, &opticstor::onGestCommande);
    connect(ui->gest, &QPushButton::clicked, this, &opticstor::onGest);
    connect(ui->produitbut, &QPushButton::clicked, this, &opticstor::onprod);
    connect(ui->emplbut, &QPushButton::clicked, this, &opticstor::onemp);
    connect(ui->fournbut, &QPushButton::clicked, this, &opticstor::onfourn);
    // Ravitalement and Feedback buttons in the ravitaillement tab
    // Use findChild to be resilient to generated-ui member name changes
    {
        QPushButton* pb = findChild<QPushButton*>("pushButton");
        if (pb) connect(pb, &QPushButton::clicked, this, &opticstor::on_ajoutravitalement_clicked);
    }
    {
        QPushButton* pb2 = findChild<QPushButton*>("pushButton_2");
        if (pb2) connect(pb2, &QPushButton::clicked, this, &opticstor::on_feedback_clicked);
    }
    //connect(ui->feedback_btn, &QPushButton::clicked, this, &opticstor::onFeedbackPage);

    // Configuration des validateurs
    // Validateurs pour les tests d'ordonnance
    ui->id_client_test->setValidator(new QIntValidator(0, 999999, this));
    // ui->og_sphere->setValidator(new QDoubleValidator(-20.0, 20.0, 2, this));
    ui->og_cylindre->setValidator(new QDoubleValidator(-20.0, 20.0, 2, this));
    ui->og_axe->setValidator(new QIntValidator(0, 180, this));
    // ui->od_sphere->setValidator(new QDoubleValidator(-20.0, 20.0, 2, this));
    ui->od_cylindre->setValidator(new QDoubleValidator(-20.0, 20.0, 2, this));
    ui->od_axe->setValidator(new QIntValidator(0, 180, this));
    ui->id_test_supp->setValidator(new QIntValidator(0, 999999, this));
    ui->id_client_rech_test->setValidator(new QIntValidator(0, 999999, this));
    // Guides pour l'utilisateur
    ui->og_cylindre->setPlaceholderText("Ex: -5.00, +4.75");
    ui->od_cylindre->setPlaceholderText("Ex: -5.00, +4.75");

    // Clients
    ui->id_client->setValidator(new QIntValidator(0, 999999, this));
    ui->telephone->setValidator(new QIntValidator(0, 99999999, this));
    ui->id_client_3->setValidator(new QIntValidator(0, 999999, this));
    ui->age_client->setValidator(new QIntValidator(0, 120, this));
    ui->id_recherche_client->setValidator(new QIntValidator(0, 999999, this));

    // Configuration du combo box des statuts
    ui->statut_client->addItems({"saisir", "en cours", "terminé", "récupéré"});

    // Produits
    ui->code_produit->setValidator(new QIntValidator(0, 999999, this));
    ui->id_fournisseur_produit->setValidator(new QIntValidator(0, 999999, this));
    ui->quantite_produit->setValidator(new QIntValidator(0, 999999, this));
    ui->prix_produit->setValidator(new QDoubleValidator(0, 999999, 2, this));
    ui->code_recherche_produit->setValidator(new QIntValidator(0, 999999, this));

    // Fournisseurs
    ui->id_fournisseur->setValidator(new QIntValidator(0, 999999, this));
    ui->telephone_fournisseur->setValidator(new QIntValidator(0, 99999999, this));
    ui->id_fournisseur_supp->setValidator(new QIntValidator(0, 999999, this));

    // Commandes
   /* ui->id_commande->setValidator(new QIntValidator(0, 999999, this));
    ui->id_client_commande->setValidator(new QIntValidator(0, 999999, this));
    ui->id_employe_commande->setValidator(new QIntValidator(0, 999999, this));
    ui->prix_total_commande->setValidator(new QDoubleValidator(0, 999999, 2, this));
    ui->id_commande_supp->setValidator(new QIntValidator(0, 999999, this));
    ui->id_commande_rech->setValidator(new QIntValidator(0, 999999, this));*/

    // ==================== CONFIGURATION FEEDBACK ====================
    ui->id_feedback->setValidator(new QIntValidator(0, 999999, this));
    ui->client_feedback->setValidator(new QIntValidator(0, 999999, this));
    //ui->id_feedback_supp->setValidator(new QIntValidator(0, 999999, this));
    //ui->id_client_rech_feedback->setValidator(new QIntValidator(0, 999999, this));
    ui->date_feedback->setDate(QDate::currentDate());
    ui->type_feedback->addItems({"Suggestion", "Réclamation", "Question", "Problème", "Compliment"});
    ui->categorie_feedback->addItems({"Produit", "Service", "Livraison", "Site Web", "Employé"});
    ui->table_feedbacks->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table_feedbacks->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->table_feedbacks->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Employés
    ui->id_employe->setValidator(new QIntValidator(0, 99999999, this));
    ui->id_supp_employe->setValidator(new QIntValidator(0, 99999999, this));
    ui->id_rech_employe->setValidator(new QIntValidator(0, 99999999, this));

    // Configuration des tables
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->table_produits->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table_produits->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->table_produits->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableWidget_22->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_22->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_22->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Configuration QTableWidget pour commandes
   /* ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setColumnCount(5);
    ui->tableWidget_2->setHorizontalHeaderLabels(QStringList() << "ID Commande" << "ID Client" << "ID Employé" << "Date" << "Prix Total");
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
*/
    // Configuration QTableWidget pour clients
    /*ui->tabcl_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tabcl_2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tabcl_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabcl_2->setColumnCount(3);
    ui->tabcl_2->setHorizontalHeaderLabels(QStringList() << "ID Client" << "Email" << "Code Produit");
    ui->tabcl_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);*/

    // Configuration de la table des employés
    ui->table_employes->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table_employes->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->table_employes->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Configuration de la table des tests d'ordonnance
    ui->table_tests_ordonnance->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table_tests_ordonnance->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->table_tests_ordonnance->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Afficher les données initiales
    ui->tableView->setModel(tmpClient.afficher());
    ui->table_produits->setModel(tmpProduit.afficher());
    ui->tableWidget_22->setModel(tmpFournisseur.afficher());
    //remplirTableCommandes();
    remplirTableTestsOrdonnance();

    // Connexions pour les boutons
    // Clients
    connect(ui->ajouter, &QPushButton::clicked, this, &opticstor::on_ajouter_clicked);
    connect(ui->supprimer, &QPushButton::clicked, this, &opticstor::on_supprimer_clicked);
    connect(ui->modifier, &QPushButton::clicked, this, &opticstor::on_modifier_clicked);
    connect(ui->tableView, &QTableView::clicked, this, &opticstor::on_tableView_clicked);
    connect(ui->rechercher_client, &QPushButton::clicked, this, &opticstor::on_rechercher_client_clicked);
    connect(ui->statistique_age, &QPushButton::clicked, this, &opticstor::on_statistique_age_clicked);
    connect(ui->exporter_pdf, &QPushButton::clicked, this, &opticstor::on_exporter_pdf_clicked);
    connect(ui->combo_tri, &QComboBox::currentTextChanged, this, &opticstor::on_combo_tri_currentIndexChanged);

    // Connexions pour la gestion des statuts
    connect(ui->changer_statut_btn, &QPushButton::clicked, this, &opticstor::on_changer_statut_clicked);
   // connect(ui->rechercher_statut_btn, &QPushButton::clicked, this, &opticstor::on_rechercher_statut_clicked);
    connect(ui->statistiques_statut_btn, &QPushButton::clicked, this, &opticstor::on_statistiques_statut_clicked);

    // Produits
    connect(ui->ajouter_produit, &QPushButton::clicked, this, &opticstor::on_ajouter_produit_clicked);
    connect(ui->supprimer_produit, &QPushButton::clicked, this, &opticstor::on_supprimer_produit_clicked);
    connect(ui->modifier_produit, &QPushButton::clicked, this, &opticstor::on_modifier_produit_clicked);
    connect(ui->rechercher_produit, &QPushButton::clicked, this, &opticstor::on_rechercher_produit_clicked);

    // Fournisseurs
    connect(ui->ajouter_fournisseur, &QPushButton::clicked, this, &opticstor::on_ajouter_fournisseur_clicked);
    connect(ui->supprimer_fournisseur, &QPushButton::clicked, this, &opticstor::on_supprimer_fournisseur_clicked);
    connect(ui->modifier_fournisseur, &QPushButton::clicked, this, &opticstor::on_modifier_fournisseur_clicked);
    // The UI uses 'recherchebtn' for the fournisseur search button
    if (ui->recherchebtn) connect(ui->recherchebtn, &QPushButton::clicked, this, &opticstor::on_rechercher_fournisseur_clicked);
    
    // Connexions optionnelles pour ravitaillement / feedback / statistiques (si les boutons existent dans l'UI)
    // Ces connexions seront actives si les boutons correspondants sont ajoutés à l'interface
    QPushButton* ravitalementBtn = findChild<QPushButton*>("ravitalement");
    if (ravitalementBtn) {
        connect(ravitalementBtn, &QPushButton::clicked, this, &opticstor::on_ajoutravitalement_clicked);
    }
    QPushButton* feedbackBtn = findChild<QPushButton*>("feedback");
    if (feedbackBtn) {
        connect(feedbackBtn, &QPushButton::clicked, this, &opticstor::on_feedback_clicked);
    }
    QPushButton* triparnomBtn = findChild<QPushButton*>("triparnom");
    if (triparnomBtn) {
        connect(triparnomBtn, &QPushButton::clicked, this, &opticstor::on_triparnom_clicked);
    }
    QPushButton* exportexelBtn = findChild<QPushButton*>("exportexel");
    if (exportexelBtn) {
        connect(exportexelBtn, &QPushButton::clicked, this, &opticstor::on_exportexel_clicked);
    }
    QPushButton* statBtn = findChild<QPushButton*>("stat");
    if (statBtn) {
        connect(statBtn, &QPushButton::clicked, this, &opticstor::on_stat_fournisseur_clicked);
    }
    
    // Initialiser ravitTable à nullptr par défaut
    ravitTable = nullptr;;

    // Commandes
   /* connect(ui->ajouter_commande, &QPushButton::clicked, this, &opticstor::on_ajouter_commande_clicked);
    connect(ui->supprimer_commande, &QPushButton::clicked, this, &opticstor::on_supprimer_commande_clicked);
    connect(ui->modifier_commande, &QPushButton::clicked, this, &opticstor::on_modifier_commande_clicked);
    connect(ui->rechercher_commande, &QPushButton::clicked, this, &opticstor::on_rechercher_commande_clicked);
    // connect(ui->actualiser_commande, &QPushButton::clicked, this, &opticstor::on_actualiser_commande_clicked);
    // connect(ui->trier_date_commande, &QPushButton::clicked, this, &opticstor::on_trier_date_commande_clicked);
    // connect(ui->trier_prix_commande, &QPushButton::clicked, this, &opticstor::on_trier_prix_commande_clicked);
*/
    // Employés
    connect(ui->ajouter_employe, &QPushButton::clicked, this, &opticstor::on_ajouter_employe_clicked);
    connect(ui->supprimer_employe, &QPushButton::clicked, this, &opticstor::on_supprimer_employe_clicked);
    connect(ui->modifier_employe, &QPushButton::clicked, this, &opticstor::on_modifier_employe_clicked);
    // connect(ui->rechercher_employe, &QPushButton::clicked, this, &opticstor::on_rechercher_employe_clicked);
    // connect(ui->actualiser_employe, &QPushButton::clicked, this, &opticstor::on_actualiser_employe_clicked);

    // Tests d'ordonnance
    connect(ui->ajouter_test_ordonnance, &QPushButton::clicked, this, &opticstor::on_ajouter_test_ordonnance_clicked);
    connect(ui->supprimer_test_ordonnance, &QPushButton::clicked, this, &opticstor::on_supprimer_test_ordonnance_clicked);
    connect(ui->afficher_tests_ordonnance, &QPushButton::clicked, this, &opticstor::on_afficher_tests_ordonnance_clicked);
    connect(ui->rechercher_tests_client, &QPushButton::clicked, this, &opticstor::on_rechercher_tests_client_clicked);

    // Feedbacks
    connect(ui->ajouter_feedback_btn, &QPushButton::clicked, this, &opticstor::on_ajouter_feedback_clicked);
    connect(ui->annuler_feedback_btn, &QPushButton::clicked, this, &opticstor::on_annuler_feedback_clicked);
    connect(ui->afficher_feedbacks_btn, &QPushButton::clicked, this, &opticstor::on_afficher_feedbacks_clicked);
    //connect(ui->supprimer_feedback_btn, &QPushButton::clicked, this, &opticstor::on_supprimer_feedback_clicked);
    //connect(ui->rechercher_feedback_client_btn, &QPushButton::clicked, this, &opticstor::on_rechercher_feedback_client_clicked);

    // Fonctions existantes
    // connect(ui->submitbut_2, &QPushButton::clicked, this, &opticstor::onSubmitClicked);
   /* connect(ui->cancelbut_2, &QPushButton::clicked, this, &opticstor::onCancelClicked);
    // connect(ui->deletebut_2, &QPushButton::clicked, this, &opticstor::onDeleteClicked);
    connect(ui->submitbutt_2, &QPushButton::clicked, this, &opticstor::onSubmitClickedd);
    connect(ui->cancelbutt_2, &QPushButton::clicked, this, &opticstor::onCancelClickedd);
    connect(ui->deletebutt_2, &QPushButton::clicked, this, &opticstor::onDeleteClickedd);
    // connect(ui->editbut_3, &QPushButton::clicked, this, &opticstor::onEditClicked);*/
    // connect(ui->sortCombo_2, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &opticstor::onSortChanged);
    // connect(ui->searchbut_2, &QPushButton::clicked, this, &opticstor::onSearchClicked);
    setupTriComboBox();
    setupTendances();

    // CONFIGURE TON TABLEAU ICI
    QTableWidget *tableStock = ui->tableStock_2; // Vérifie que ça s'appelle comme ça!
    if (tableStock) {
        tableStock->setColumnCount(6);
        tableStock->setHorizontalHeaderLabels(
            QStringList() << "Code" << "Marque" << "Sexe" << "Type" << "Quantité" << "Prix"
            );
        tableStock->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        // Affiche tous les produits au démarrage
        afficherStockSelonStatut("TOUS");
    } else {
        qDebug() << "ERREUR: tableStock non trouvé dans l'interface!";
    }


}



// ==================== GESTION DES STATUTS ====================

void opticstor::on_changer_statut_clicked()
{
    QString idText = ui->id_client_statut->text();
    QString nouveauStatut = ui->statut_client->currentText();

    if (idText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID client!");
        return;
    }

    int id = idText.toInt();

    if (tmpClient.modifierStatut(id, nouveauStatut)) {
        QMessageBox::information(this, "Succès",
                                 QString("Statut du client %1 changé à: %2").arg(id).arg(nouveauStatut));
        ui->tableView->setModel(tmpClient.afficher()); // Actualiser l'affichage principal

        // ICI - Vous pourriez ajouter une logique pour actualiser automatiquement
        // la fenêtre des statistiques si elle est ouverte
        QMessageBox::information(this, "Succès",
                                 QString("Statut mis à jour! \nOuvrez à nouveau les statistiques pour voir les changements."));

        ui->id_client_statut->clear();
    } else {
        QMessageBox::critical(this, "Erreur",
                              "Erreur lors du changement de statut!\nVérifiez que l'ID existe.");
    }
}

void opticstor::on_rechercher_statut_clicked()
{
    QString statutRecherche = ui->statut_client->currentText();

    QSqlQueryModel* model = Client::rechercherParStatut(statutRecherche);
    ui->tableView->setModel(model);

    if (model->rowCount() == 0) {
        QMessageBox::information(this, "Recherche",
                                 QString("Aucun client avec le statut: %1").arg(statutRecherche));
    } else {
        QMessageBox::information(this, "Succès",
                                 QString("%1 client(s) trouvé(s) avec le statut: %2")
                                     .arg(model->rowCount()).arg(statutRecherche));
    }
}

void opticstor::on_statistiques_statut_clicked()
{
    afficherStatistiquesAvecDurees();
}

void opticstor::afficherStatistiquesAvecDurees()
{
    QWidget *statsWindow = new QWidget();
    statsWindow->setWindowTitle("Progression des Statuts des Clients avec Durées");
    statsWindow->setMinimumSize(1400, 800);
    statsWindow->setStyleSheet("background-color: #f5f6fa;");

    QVBoxLayout *mainLayout = new QVBoxLayout(statsWindow);

    // Titre
    QLabel *title = new QLabel("📊 PROGRESSION DES STATUTS DES CLIENTS AVEC DURÉES");
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #2c3e50; padding: 20px; background-color: white; border-radius: 10px; margin: 10px;");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    // Tableau avec 10 colonnes
    QTableWidget *table = new QTableWidget();
    table->setColumnCount(10);
    table->setHorizontalHeaderLabels(QStringList()
                                     << "ID Client"
                                     << "Nom"
                                     << "SAISIR"
                                     << "Durée"
                                     << "EN COURS"
                                     << "Durée"
                                     << "TERMINÉ"
                                     << "Durée"
                                     << "RÉCUPÉRÉ"
                                     << "Durée");

    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setStyleSheet("QTableWidget { background-color: white; border-radius: 8px; margin: 10px; }"
                         "QHeaderView::section { background-color: #34495e; color: white; font-weight: bold; padding: 10px; }"
                         "QTableWidget::item { padding: 8px; text-align: center; }");

    // Récupérer tous les clients
    QList<Client> clients = Client::getAllClients();
    table->setRowCount(clients.size());

    // Couleurs pour chaque statut
    QMap<QString, QString> couleursStatut = {
        {"saisir", "#3498db"},
        {"en cours", "#3498db"},
        {"terminé", "#3498db"},
        {"récupéré", "#3498db"}
    };

    // Remplir le tableau
    for (int i = 0; i < clients.size(); i++) {
        Client client = clients[i];
        QString statutActuel = client.getStatut().toLower();
        QMap<QString, int> durees = client.getDureesTousStatuts();

        // Colonne 1: ID Client
        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(client.getId()));
        idItem->setTextAlignment(Qt::AlignCenter);
        idItem->setFont(QFont("Arial", 10, QFont::Bold));
        idItem->setBackground(QColor("#ecf0f1"));
        table->setItem(i, 0, idItem);

        // Colonne 2: Nom Complet
        QTableWidgetItem *nomItem = new QTableWidgetItem(client.getNom() + " " + client.getPrenom());
        nomItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 1, nomItem);

        // SAISIR
        QTableWidgetItem *saisirItem = new QTableWidgetItem("█████");
        saisirItem->setTextAlignment(Qt::AlignCenter);
        saisirItem->setFont(QFont("Arial", 11, QFont::Bold));
        saisirItem->setForeground(QColor(couleursStatut["saisir"]));
        table->setItem(i, 2, saisirItem);

        QTableWidgetItem *saisirDureeItem = new QTableWidgetItem(QString("%1 j").arg(durees["saisir"]));
        saisirDureeItem->setTextAlignment(Qt::AlignCenter);
        saisirDureeItem->setBackground(QColor("#e8f4fc"));
        table->setItem(i, 3, saisirDureeItem);

        // EN COURS
        QTableWidgetItem *enCoursItem = new QTableWidgetItem();
        QTableWidgetItem *enCoursDureeItem = new QTableWidgetItem();

        // TERMINÉ
        QTableWidgetItem *termineItem = new QTableWidgetItem();
        QTableWidgetItem *termineDureeItem = new QTableWidgetItem();

        // RÉCUPÉRÉ
        QTableWidgetItem *recupereItem = new QTableWidgetItem();
        QTableWidgetItem *recupereDureeItem = new QTableWidgetItem();

        // Appliquer la progression selon le statut
        if (statutActuel == "saisir") {
            enCoursItem->setText("");
            enCoursDureeItem->setBackground(QColor("#f8f9fa"));
            termineItem->setText("");
            termineDureeItem->setBackground(QColor("#f8f9fa"));
            recupereItem->setText("");
        }
        else if (statutActuel == "en cours") {
            enCoursItem->setText("████████");
            enCoursItem->setForeground(QColor(couleursStatut["en cours"]));
            enCoursDureeItem->setText(QString("%1 j").arg(durees["en cours"]));
            enCoursDureeItem->setBackground(QColor("#fef9e7"));
            termineItem->setText("");
            termineDureeItem->setBackground(QColor("#f8f9fa"));
            recupereItem->setText("");
        }
        else if (statutActuel == "terminé") {
            enCoursItem->setText("████████");
            enCoursItem->setForeground(QColor(couleursStatut["en cours"]));
            enCoursDureeItem->setText(QString("%1 j").arg(durees["en cours"]));
            enCoursDureeItem->setBackground(QColor("#fef9e7"));
            termineItem->setText("██████");
            termineItem->setForeground(QColor(couleursStatut["terminé"]));
            termineDureeItem->setText(QString("%1 j").arg(durees["terminé"]));
            termineDureeItem->setBackground(QColor("#e8f6f3"));
            recupereItem->setText("");
        }
        else if (statutActuel == "récupéré") {
            enCoursItem->setText("████████");
            enCoursItem->setForeground(QColor(couleursStatut["en cours"]));
            enCoursDureeItem->setText(QString("%1 j").arg(durees["en cours"]));
            enCoursDureeItem->setBackground(QColor("#fef9e7"));
            termineItem->setText("██████");
            termineItem->setForeground(QColor(couleursStatut["terminé"]));
            termineDureeItem->setText(QString("%1 j").arg(durees["terminé"]));
            termineDureeItem->setBackground(QColor("#e8f6f3"));
            recupereItem->setText("████████");
            recupereItem->setForeground(QColor(couleursStatut["récupéré"]));
            recupereDureeItem->setText(QString("%1 j").arg(durees["récupéré"]));
        }

        table->setItem(i, 4, enCoursItem);
        table->setItem(i, 5, enCoursDureeItem);
        table->setItem(i, 6, termineItem);
        table->setItem(i, 7, termineDureeItem);
        table->setItem(i, 8, recupereItem);
        table->setItem(i, 9, recupereDureeItem);
    }

    // Ajuster les largeurs
    table->setColumnWidth(0, 80);
    table->setColumnWidth(1, 150);
    table->setColumnWidth(2, 70);
    table->setColumnWidth(3, 90);
    table->setColumnWidth(4, 100);
    table->setColumnWidth(5, 90);
    table->setColumnWidth(6, 70);
    table->setColumnWidth(7, 90);
    table->setColumnWidth(8, 90);

    mainLayout->addWidget(table);

    // Légende
    QWidget *legendeWidget = new QWidget();
    legendeWidget->setStyleSheet("background-color: white; border-radius: 8px; padding: 15px; margin: 10px;");
    QVBoxLayout *legendeLayout = new QVBoxLayout(legendeWidget);

    QLabel *legendeTitle = new QLabel("🎨 LÉGENDE - BARRE DE PROGRESSION ET DURÉES");
    legendeTitle->setStyleSheet("font-weight: bold; color: #2c3e50; font-size: 14px; margin-bottom: 10px;");

    QHBoxLayout *couleursLayout = new QHBoxLayout();
    QStringList statuts = {"saisir", "en cours", "terminé", "récupéré"};
    for (const QString& statut : statuts) {
        QLabel *legendeItem = new QLabel("███ " + statut.toUpper() + " (jours)");
        legendeItem->setStyleSheet(QString(
                                       "color: %1; padding: 8px 12px; border-radius: 5px; margin: 0 5px; font-weight: bold; background-color: #ecf0f1;"
                                       ).arg(couleursStatut[statut]));
        couleursLayout->addWidget(legendeItem);
    }
    couleursLayout->addStretch();

    QLabel *explication = new QLabel("💡 La barre s'étend jusqu'au statut actuel du client - Les durées sont en jours");
    explication->setStyleSheet("color: #7f8c8d; font-style: italic; margin-top: 10px;");

    legendeLayout->addWidget(legendeTitle);
    legendeLayout->addLayout(couleursLayout);
    legendeLayout->addWidget(explication);
    mainLayout->addWidget(legendeWidget);

    // Calcul des durées moyennes
    int totalSaisir = 0, totalEnCours = 0, totalTermine = 0, totalRecupere = 0;
    int countSaisir = 0, countEnCours = 0, countTermine = 0, countRecupere = 0;

    for (const Client& client : clients) {
        QMap<QString, int> durees = client.getDureesTousStatuts();
        if (durees["saisir"] > 0) { totalSaisir += durees["saisir"]; countSaisir++; }
        if (durees["en cours"] > 0) { totalEnCours += durees["en cours"]; countEnCours++; }
        if (durees["terminé"] > 0) { totalTermine += durees["terminé"]; countTermine++; }
        if (durees["récupéré"] > 0) { totalRecupere += durees["récupéré"]; countRecupere++; }
    }

    double moyenneSaisir = countSaisir > 0 ? totalSaisir / double(countSaisir) : 0;
    double moyenneEnCours = countEnCours > 0 ? totalEnCours / double(countEnCours) : 0;
    double moyenneTermine = countTermine > 0 ? totalTermine / double(countTermine) : 0;
    double moyenneRecupere = countRecupere > 0 ? totalRecupere / double(countRecupere) : 0;

    QLabel *statsDurees = new QLabel(
        QString("📈 DURÉES MOYENNES: Saisir: %1j • En cours: %2j • Terminé: %3j • Récupéré: %4j")
            .arg(moyenneSaisir, 0, 'f', 1)
            .arg(moyenneEnCours, 0, 'f', 1)
            .arg(moyenneTermine, 0, 'f', 1)
            .arg(moyenneRecupere, 0, 'f', 1)
        );
    statsDurees->setStyleSheet("font-size: 14px; font-weight: bold; color: #2c3e50; padding: 15px; background-color: #e8f4fc; border-radius: 8px; margin: 10px;");
    statsDurees->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(statsDurees);

    // Boutons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *refreshButton = new QPushButton("🔄 Actualiser");
    refreshButton->setFixedSize(120, 40);
    refreshButton->setStyleSheet("QPushButton { background-color: #3498db; color: white; font-weight: bold; border-radius: 6px; }"
                                 "QPushButton:hover { background-color: #2980b9; }");

    QPushButton *closeButton = new QPushButton("Fermer");
    closeButton->setFixedSize(100, 40);
    closeButton->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; font-weight: bold; border-radius: 6px; }"
                               "QPushButton:hover { background-color: #c0392b; }");

    QObject::connect(refreshButton, &QPushButton::clicked, statsWindow, [this]() {
        afficherStatistiquesAvecDurees();
    });

    QObject::connect(closeButton, &QPushButton::clicked, statsWindow, &QWidget::close);

    btnLayout->addWidget(refreshButton);
    btnLayout->addWidget(closeButton);
    btnLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addLayout(btnLayout);

    statsWindow->show();
}

// ==================== GESTION DES FEEDBACKS ====================

void opticstor::on_ajouter_feedback_clicked()
{
    QString idFeedbackText = ui->id_feedback->text();
    QString idClientText = ui->client_feedback->text();
    QString type = ui->type_feedback->currentText();
    QString categorie = ui->categorie_feedback->currentText();
    QDate date = ui->date_feedback->date();
    QString message = ui->message_feedback->toPlainText();

    // Validation des champs obligatoires
    if (idFeedbackText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le champ ID feedback est vide!");
        ui->id_feedback->setFocus();
        return;
    }

    int id_feedback = idFeedbackText.toInt();
    if (id_feedback <= 0) {
        QMessageBox::warning(this, "Erreur", "ID feedback invalide!");
        ui->id_feedback->setFocus();
        return;
    }

    if (idClientText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le champ Client est vide!");
        ui->client_feedback->setFocus();
        return;
    }

    int id_client = idClientText.toInt();
    if (id_client <= 0) {
        QMessageBox::warning(this, "Erreur", "ID client invalide!");
        ui->client_feedback->setFocus();
        return;
    }

    if (message.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le champ Message est vide!");
        ui->message_feedback->setFocus();
        return;
    }

    // Vérifier la connexion à la base de données
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Erreur", "Base de données non connectée!");
        return;
    }

    qDebug() << "Base de données connectée:" << db.isOpen();

    // Appel CORRIGÉ avec la méthode statique
    bool test = Client::ajouterFeedback(id_feedback, id_client, type, categorie, date, message);

    if (test) {
        QMessageBox::information(this, "Succès",
                                 QString("Feedback ajouté avec succès!\n\n"
                                         "ID Feedback: %1\n"
                                         "ID Client: %2\n"
                                         "Type: %3\n"
                                         "Catégorie: %4").arg(id_feedback).arg(id_client).arg(type).arg(categorie));

        // Vider les champs après ajout
        ui->id_feedback->clear();
        ui->client_feedback->clear();
        ui->message_feedback->clear();
        ui->type_feedback->setCurrentIndex(0);
        ui->categorie_feedback->setCurrentIndex(0);
        ui->date_feedback->setDate(QDate::currentDate());

        // Actualiser l'affichage
        on_afficher_feedbacks_clicked();
    } else {
        QMessageBox::critical(this, "Erreur",
                              QString("Échec de l'ajout du feedback!\n\n"
                                      "Raisons possibles:\n"
                                      "• L'ID feedback %1 existe déjà\n"
                                      "• L'ID client %2 n'existe pas\n"
                                      "• Problème de connexion à la base de données").arg(id_feedback).arg(id_client));
    }
}

void opticstor::on_annuler_feedback_clicked()
{
    // Vider tous les champs du feedback
    ui->id_feedback->clear();
    ui->client_feedback->clear();
    ui->message_feedback->clear();
    //ui->piece_jointe_feedback->clear();
    ui->type_feedback->setCurrentIndex(0);
    ui->categorie_feedback->setCurrentIndex(0);
    ui->date_feedback->setDate(QDate::currentDate());

    QMessageBox::information(this, "Annulation",
                             "Formulaire de feedback annulé!\nTous les champs ont été réinitialisés.");
}

void opticstor::on_afficher_feedbacks_clicked()
{
    Client clientTemp;
    QSqlQueryModel* model = clientTemp.afficherTousLesFeedbacks();
    ui->table_feedbacks->setModel(model);

    // Ajuster la largeur des colonnes
    ui->table_feedbacks->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QMessageBox::information(this, "Actualisation",
                             "Liste des feedbacks actualisée avec succès!");
}

/*void opticstor::on_supprimer_feedback_clicked()
{
    QString idFeedbackText = ui->id_feedback_supp->text();

    if (idFeedbackText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le champ ID feedback à supprimer est vide!");
        ui->id_feedback_supp->setFocus();
        return;
    }

    int id_feedback = idFeedbackText.toInt();

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de suppression",
                                                              QString("Voulez-vous vraiment supprimer le feedback avec l'ID %1?\n\n"
                                                                      "Cette action est irréversible!").arg(id_feedback),
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return;
    }

    bool test = Client::supprimerFeedback(id_feedback);

    if (test) {
        QMessageBox::information(this, "Succès",
                                 QString("Feedback avec l'ID %1 a été supprimé avec succès!").arg(id_feedback));
        on_afficher_feedbacks_clicked(); // Actualiser l'affichage
        ui->id_feedback_supp->clear();
    } else {
        QMessageBox::critical(this, "Erreur de suppression",
                              QString("Échec de la suppression!\n\n"
                                      "Raisons possibles:\n"
                                      "• Aucun feedback trouvé avec l'ID %1\n"
                                      "• Problème de connexion à la base de données").arg(id_feedback));
    }
}

void opticstor::on_rechercher_feedback_client_clicked()
{
    QString idClientText = ui->id_client_rech_feedback->text();

    if (idClientText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le champ ID client est vide!");
        ui->id_client_rech_feedback->setFocus();
        return;
    }

    int id_client = idClientText.toInt();
    QSqlQueryModel* model = Client::afficherFeedbacksParClient(id_client);
    ui->table_feedbacks->setModel(model);

    if (model->rowCount() == 0) {
        QMessageBox::information(this, "Recherche",
                                 QString("Aucun feedback trouvé pour le client avec l'ID %1.").arg(id_client));
    } else {
        QMessageBox::information(this, "Feedback trouvé",
                                 QString("%1 feedback(s) trouvé(s) pour le client ID %2!").arg(model->rowCount()).arg(id_client));
    }
}
*/

void opticstor::onFeedbackPage()
{
    ui->opticstack->setCurrentIndex(5); // Index de la page feedback
    on_afficher_feedbacks_clicked(); // Actualiser automatiquement
}

// ==================== GESTION DES CLIENTS (MODIFIÉ POUR L'AGE ET STATUT) ====================

void opticstor::on_ajouter_clicked()
{
    QString idText = ui->id_client->text();
    QString nom = ui->nom->text();
    QString prenom = ui->prenom->text();
    QString email = ui->email->text();
    QString telephone = ui->telephone->text();
    QString ageText = ui->age_client->text();
    QString statut = ui->statut_client->currentText(); // ← RÉCUPÉRER LE STATUT

    if (idText.isEmpty() || nom.isEmpty() || prenom.isEmpty() || email.isEmpty() || telephone.isEmpty() || ageText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis!");
        return;
    }

    int id = idText.toInt();
    int age = ageText.toInt();

    // Validation de l'âge
    if (!Client::estAgeValide(age)) {
        QMessageBox::warning(this, "Erreur", "Âge invalide! Doit être entre 0 et 120 ans.");
        return;
    }

    // Validation du statut
    if (!Client::estStatutValide(statut)) {
        QMessageBox::warning(this, "Erreur", "Statut invalide!");
        return;
    }

    Client c(id, nom, prenom, email, telephone, age, statut); // ← AJOUT DU PARAMÈTRE STATUT

    if (c.ajouter()) {
        QMessageBox::information(this, "Succès", "Client ajouté avec succès!");
        ui->tableView->setModel(tmpClient.afficher());

        // Vider les champs
        ui->id_client->clear();
        ui->nom->clear();
        ui->prenom->clear();
        ui->email->clear();
        ui->telephone->clear();
        ui->age_client->clear();
        ui->statut_client->setCurrentIndex(0); // ← REMETTRE À "saisir"
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout du client!");
    }
}

void opticstor::on_supprimer_clicked()
{
    QString idText = ui->id_client_3->text();

    if (idText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID de client!");
        return;
    }

    int id = idText.toInt();

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation",
                                                              "Voulez-vous vraiment supprimer ce client?",
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (tmpClient.supprimer(id)) {
            QMessageBox::information(this, "Succès", "Client supprimé avec succès!");
            ui->tableView->setModel(tmpClient.afficher());
            ui->id_client_3->clear();
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression!");
        }
    }
}

void opticstor::on_modifier_clicked()
{
    QString idText = ui->id_client->text();
    QString nom = ui->nom->text();
    QString prenom = ui->prenom->text();
    QString email = ui->email->text();
    QString telephone = ui->telephone->text();
    QString ageText = ui->age_client->text();
    QString statut = ui->statut_client->currentText(); // ← RÉCUPÉRER LE STATUT

    if (idText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Aucun client sélectionné! Veuillez sélectionner un client dans le tableau.");
        return;
    }

    int id = idText.toInt();
    int age = ageText.toInt();

    // Validation de l'âge
    if (!Client::estAgeValide(age)) {
        QMessageBox::warning(this, "Erreur", "Âge invalide! Doit être entre 0 et 120 ans.");
        return;
    }

    // Validation du statut
    if (!Client::estStatutValide(statut)) {
        QMessageBox::warning(this, "Erreur", "Statut invalide!");
        return;
    }

    Client c(id, nom, prenom, email, telephone, age, statut);

    if (c.modifier()) {
        QMessageBox::information(this, "Succès", "Client modifié avec succès!");
        ui->tableView->setModel(tmpClient.afficher());
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la modification!");
    }
}

void opticstor::on_tableView_clicked(const QModelIndex &index)
{
    // Récupérer la ligne sélectionnée
    int row = index.row();

    // Récupérer le modèle
    QSqlQueryModel *model = static_cast<QSqlQueryModel*>(ui->tableView->model());

    // Remplir les champs avec les données de la ligne sélectionnée
    ui->id_client->setText(model->data(model->index(row, 0)).toString());        // ID
    ui->nom->setText(model->data(model->index(row, 1)).toString());              // Nom
    ui->prenom->setText(model->data(model->index(row, 2)).toString());           // Prénom
    ui->email->setText(model->data(model->index(row, 3)).toString());            // Email
    ui->telephone->setText(model->data(model->index(row, 4)).toString());        // Téléphone
    ui->age_client->setText(model->data(model->index(row, 5)).toString());       // Âge

    // Récupérer et sélectionner le statut ← AJOUT DU STATUT
    QString statut = model->data(model->index(row, 6)).toString();
    int indexStatut = ui->statut_client->findText(statut);
    if (indexStatut >= 0) {
        ui->statut_client->setCurrentIndex(indexStatut);
    }

    // Message de débogage (optionnel)
    qDebug() << "Ligne sélectionnée - ID:" << model->data(model->index(row, 0)).toString();
}

// ==================== EXPORTATION DES CLIENTS EN PDF ====================

void opticstor::on_exporter_pdf_clicked()
{
    // Demander où sauvegarder le fichier
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Exporter les clients en PDF",
                                                    QDir::homePath() + "/liste_clients.pdf",
                                                    "Fichiers PDF (*.pdf)");

    if (fileName.isEmpty()) {
        return; // L'utilisateur a annulé
    }

    // S'assurer que le fichier a l'extension .pdf
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
        fileName += ".pdf";
    }

    // Récupérer tous les clients
    QList<Client> clients = Client::getAllClients();

    if (clients.isEmpty()) {
        QMessageBox::information(this, "Export PDF", "Aucun client à exporter !");
        return;
    }

    // Créer le document HTML
    QString htmlContent = genererHTMLClients(clients);

    // Créer le document texte
    QTextDocument document;
    document.setHtml(htmlContent);

    // Configurer l'imprimante PDF
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    // Exporter en PDF
    document.print(&printer);

    // Message de confirmation
    QMessageBox::information(this,
                             "Export PDF Réussi",
                             QString("Liste des clients exportée avec succès!\n\n"
                                     "Fichier: %1\n"
                                     "Nombre de clients: %2")
                                 .arg(fileName)
                                 .arg(clients.count()));

    // Optionnel: Ouvrir le PDF après export
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}

QString opticstor::genererHTMLClients(const QList<Client>& clients)
{
    QString html;

    // HTML de base
    html = "<html><body>";
    html += "<h1>Liste des Clients</h1>";
    html += "<table border='1'>";
    html += "<tr><th>ID</th><th>Nom</th><th>Prénom</th><th>Email</th><th>Téléphone</th><th>Age</th><th>Statut</th></tr>";

    // Ajouter chaque client
    for (int i = 0; i < clients.size(); i++) {
        Client client = clients[i];

        html += "<tr>";
        html += "<td>" + QString::number(client.getId()) + "</td>";
        html += "<td>" + client.getNom() + "</td>";
        html += "<td>" + client.getPrenom() + "</td>";
        html += "<td>" + client.getEmail() + "</td>";
        html += "<td>" + client.getTelephone() + "</td>";
        html += "<td>" + QString::number(client.getAge()) + "</td>";
        html += "<td>" + client.getStatut() + "</td>";
        html += "</tr>";
    }

    html += "</table></body></html>";
    return html;
}

// ==================== TRI AUTOMATIQUE PAR LISTE DÉROULANTE ====================

void opticstor::on_combo_tri_currentIndexChanged(const QString &text)
{
    // Ne rien faire si c'est l'élément "Trier par..."
    if (text == "Trier par...") {
        return;
    }

    QString critere;

    if (text == "ID") {
        critere = "id";
    } else if (text == "Nom") {
        critere = "nom";
    } else if (text == "Prénom") {
        critere = "prenom";
    } else if (text == "Âge") {
        critere = "age";
    } else {
        critere = "id"; // Par défaut
    }

    // Appliquer le tri
    QSqlQueryModel* model = Client::trier(critere);
    ui->tableView->setModel(model);
}

// ==================== RECHERCHE CLIENT PAR ID ====================

void opticstor::on_rechercher_client_clicked()
{
    QString idText = ui->id_recherche_client->text();

    if (idText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID client à rechercher!");
        return;
    }

    int id = idText.toInt();

    QSqlQueryModel* model = Client::rechercherParId(id);
    ui->tableView->setModel(model);

    if (model->rowCount() == 0) {
        QMessageBox::information(this, "Recherche", "Aucun client trouvé avec cet ID!");
    } else {
        QMessageBox::information(this, "Succès", "Client trouvé!");

        // Remplir automatiquement les champs avec le client trouvé
        if (model->rowCount() > 0) {
            ui->id_client->setText(model->data(model->index(0, 0)).toString());
            ui->nom->setText(model->data(model->index(0, 1)).toString());
            ui->prenom->setText(model->data(model->index(0, 2)).toString());
            ui->email->setText(model->data(model->index(0, 3)).toString());
            ui->telephone->setText(model->data(model->index(0, 4)).toString());
            ui->age_client->setText(model->data(model->index(0, 5)).toString());

            // Récupérer et sélectionner le statut
            QString statut = model->data(model->index(0, 6)).toString();
            int indexStatut = ui->statut_client->findText(statut);
            if (indexStatut >= 0) {
                ui->statut_client->setCurrentIndex(indexStatut);
            }
        }
    }
}

// ==================== STATISTIQUES PAR ÂGE ====================

void opticstor::on_statistique_age_clicked()
{
    // Créer une nouvelle fenêtre pour les statistiques
    QWidget *statsWindow = new QWidget();
    statsWindow->setWindowTitle("Statistiques des Clients par Âge");
    statsWindow->setMinimumSize(800, 600);
    statsWindow->setStyleSheet("background-color: white;");

    // Layout principal - CORRECT
    QVBoxLayout *mainLayout = new QVBoxLayout(statsWindow);

    // Titre
    QLabel *title = new QLabel("📊 STATISTIQUES DES CLIENTS PAR ÂGE");
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #2c3e50; padding: 15px;");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    // Récupérer les statistiques
    QMap<QString, int> stats = Client::getStatistiquesAge();
    int total = stats["Moins de 20 ans"] + stats["20-50 ans"] + stats["Plus de 50 ans"];

    if (total == 0) {
        QLabel *noDataLabel = new QLabel("Aucun client dans la base de données !");
        noDataLabel->setStyleSheet("font-size: 16px; color: #7f8c8d; padding: 50px;");
        noDataLabel->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(noDataLabel);
    } else {
        // Créer le camembert
        QPieSeries *series = new QPieSeries();

        // Définir les tranches avec couleurs
        QStringList tranches = {"Moins de 20 ans", "20-50 ans", "Plus de 50 ans"};
        QList<QColor> couleurs = {
            QColor(41, 128, 185),   // Bleu
            QColor(39, 174, 96),    // Vert
            QColor(231, 76, 60)     // Rouge
        };

        for (int i = 0; i < tranches.size(); ++i) {
            QString tranche = tranches[i];
            int count = stats[tranche];
            double percentage = (count * 100.0) / total;

            // Créer la tranche
            QPieSlice *slice = series->append(tranche, count);

            // Configurer l'apparence
            slice->setColor(couleurs[i]);
            slice->setLabelVisible(true);
            slice->setExploded(true);

            // Formater le label avec pourcentage
            QString label = QString("%1\n%2 clients\n%3%")
                                .arg(tranche)
                                .arg(count)
                                .arg(QString::number(percentage, 'f', 1));
            slice->setLabel(label);
        }

        // Créer le chart
        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Répartition par Tranche d'Âge");
        chart->setAnimationOptions(QChart::AllAnimations);

        // Créer la vue du chart
        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        // Ajouter le graphique au layout
        mainLayout->addWidget(chartView);
    }

    // Bouton fermer
    QPushButton *closeButton = new QPushButton("Fermer");
    closeButton->setFixedSize(100, 40);
    connect(closeButton, &QPushButton::clicked, statsWindow, &QWidget::close);

    mainLayout->addWidget(closeButton, 0, Qt::AlignCenter);

    // Afficher la fenêtre
    statsWindow->show();
}

// ==================== GESTION DES TESTS ORDONNANCE ====================

void opticstor::on_ajouter_test_ordonnance_clicked()
{
    QString idClientText = ui->id_client_test->text();
    QString dateText = ui->date_test->text();
    // QString ogSphereText = ui->og_sphere->currentText();
    QString ogCylindreText = ui->og_cylindre->text();
    QString ogAxeText = ui->og_axe->text();
    // QString odSphereText = ui->od_sphere->currentText();
    QString odCylindreText = ui->od_cylindre->text();
    QString odAxeText = ui->od_axe->text();

    // Validation des champs
    if (idClientText.isEmpty() || dateText.isEmpty() ||
        ogCylindreText.isEmpty() || ogAxeText.isEmpty() ||
        odCylindreText.isEmpty() || odAxeText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs obligatoires doivent être remplis!");
        return;
    }

    int idClient = idClientText.toInt();
    QDate date = QDate::fromString(dateText, "dd/MM/yyyy");

    if (!date.isValid()) {
        QMessageBox::warning(this, "Erreur", "Date invalide! Format: JJ/MM/AAAA");
        return;
    }

    //double ogSphere = ogSphereText.toDouble();
    double ogCylindre = ogCylindreText.toDouble();
    int ogAxe = ogAxeText.toInt();
    //double odSphere = odSphereText.toDouble();
    double odCylindre = odCylindreText.toDouble();
    int odAxe = odAxeText.toInt();

    // Validation des axes
    if (ogAxe < 0 || ogAxe > 180 || odAxe < 0 || odAxe > 180) {
        QMessageBox::warning(this, "Erreur", "L'axe doit être entre 0 et 180 degrés!");
        return;
    }

    // Créer un client temporaire avec l'ID seulement
    Client client(idClient, "", "", "", "", 0, "saisir"); // ← AJOUT DU STATUT PAR DÉFAUT

    if (client.ajouterTestOrdonnance(date, ogCylindre, ogAxe,
                                     odCylindre, odAxe)) {
        QMessageBox::information(this, "Succès", "Test d'ordonnance ajouté avec succès!");

        // Vider les champs
        ui->id_client_test->clear();
        ui->date_test->clear();
        // ui->og_sphere->clear();
        ui->og_cylindre->clear();
        ui->og_axe->clear();
        // ui->od_sphere->clear();
        ui->od_cylindre->clear();
        ui->od_axe->clear();

        // Actualiser l'affichage
        remplirTableTestsOrdonnance();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout du test d'ordonnance!");
    }
}

void opticstor::on_supprimer_test_ordonnance_clicked()
{
    QString idTestText = ui->id_test_supp->text();

    if (idTestText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID de test!");
        return;
    }

    int idTest = idTestText.toInt();

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation",
                                                              "Voulez-vous vraiment supprimer ce test d'ordonnance?",
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (Client::supprimerTestOrdonnance(idTest)) {
            QMessageBox::information(this, "Succès", "Test d'ordonnance supprimé avec succès!");
            remplirTableTestsOrdonnance();
            ui->id_test_supp->clear();
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression!");
        }
    }
}

void opticstor::on_afficher_tests_ordonnance_clicked()
{
    remplirTableTestsOrdonnance();
    QMessageBox::information(this, "Succès", "Liste des tests actualisée!");
}

void opticstor::on_rechercher_tests_client_clicked()
{
    QString idClientText = ui->id_client_rech_test->text();

    if (idClientText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID client!");
        return;
    }

    int idClient = idClientText.toInt();

    // Créer un client temporaire avec l'ID seulement
    Client client(idClient, "", "", "", "", 0, "saisir"); // ← AJOUT DU STATUT PAR DÉFAUT

    QSqlQueryModel* model = client.afficherTestsOrdonnance();
    ui->table_tests_ordonnance->setModel(model);

    if (model->rowCount() == 0) {
        QMessageBox::information(this, "Recherche", "Aucun test trouvé pour ce client!");
    }
}

void opticstor::remplirTableTestsOrdonnance()
{
    QSqlQueryModel* model = Client::afficherTousTestsOrdonnance();
    ui->table_tests_ordonnance->setModel(model);

    // Configuration de la table
    ui->table_tests_ordonnance->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table_tests_ordonnance->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->table_tests_ordonnance->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_tests_ordonnance->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

// ==================== GESTION DES EMPLOYÉS ====================

void opticstor::on_ajouter_employe_clicked()
{
    QString id_employeText = ui->id_employe->text();
    QString nom = ui->nom_employe->text();
    QString prenom = ui->prenom_employe->text();
    QString dateNaissanceText = ui->date_naissance_employe->text();
    QString departement = ui->departement_employe->text();

    // Validation des champs
    if (id_employeText.isEmpty() || nom.isEmpty() || prenom.isEmpty() ||
        dateNaissanceText.isEmpty() || departement.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis!");
        return;
    }

    int id_employe = id_employeText.toInt();
    QDate dateNaissance = QDate::fromString(dateNaissanceText, "dd-MM-yyyy");

    if (!dateNaissance.isValid()) {
        QMessageBox::warning(this, "Erreur", "Date de naissance invalide! Format: JJ-MM-AAAA");
        return;
    }

    employe emp(id_employe, nom, prenom, dateNaissance, departement);

    if (emp.ajouter()) {
        QMessageBox::information(this, "Succès", "Employé ajouté avec succès!");
        remplirTableEmployes();

        // Vider les champs
        ui->id_employe->clear();
        ui->nom_employe->clear();
        ui->prenom_employe->clear();
        ui->date_naissance_employe->clear();
        ui->departement_employe->clear();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout de l'employé!");
    }
}

void opticstor::on_supprimer_employe_clicked()
{
    QString id_employeText = ui->id_supp_employe->text();

    if (id_employeText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un id d'employé!");
        return;
    }

    int id_employe = id_employeText.toInt();

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation",
                                                              "Voulez-vous vraiment supprimer cet employé?",
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (tmpEmploye.supprimer(id_employe)) {
            QMessageBox::information(this, "Succès", "Employé supprimé avec succès!");
            remplirTableEmployes();
            ui->id_supp_employe->clear();
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression!");
        }
    }
}

void opticstor::on_modifier_employe_clicked()
{
    QString id_employeText = ui->id_employe->text();
    QString nom = ui->nom_employe->text();
    QString prenom = ui->prenom_employe->text();
    QString dateNaissanceText = ui->date_naissance_employe->text();
    QString departement = ui->departement_employe->text();

    if (id_employeText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "id employé requis pour la modification!");
        return;
    }

    int id_employe = id_employeText.toInt();
    QDate dateNaissance = QDate::fromString(dateNaissanceText, "yyyy-MM-dd");

    if (tmpEmploye.modifier(id_employe, nom, prenom, dateNaissance, departement)) {
        QMessageBox::information(this, "Succès", "Employé modifié avec succès!");
        remplirTableEmployes();

        ui->id_employe->clear();
        ui->nom_employe->clear();
        ui->prenom_employe->clear();
        ui->date_naissance_employe->clear();
        ui->departement_employe->clear();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la modification!");
    }
}

void opticstor::on_rechercher_employe_clicked()
{
    QString id_employeText = ui->id_rech_employe->text();

    if (id_employeText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un CIN à rechercher!");
        return;
    }

    int id_employe = id_employeText.toInt();
    QSqlQueryModel* model = tmpEmploye.rechercherParCIN(id_employe);

    if (model->rowCount() > 0) {
        ui->table_employes->setModel(model);
        QMessageBox::information(this, "Succès", "Employé trouvé!");
    } else {
        QMessageBox::information(this, "Recherche", "Aucun employé trouvé!");
        remplirTableEmployes();
    }
}

void opticstor::on_actualiser_employe_clicked()
{
    remplirTableEmployes();
    QMessageBox::information(this, "Succès", "Liste actualisée!");
}

void opticstor::remplirTableEmployes()
{
    QSqlQueryModel* model = tmpEmploye.afficher();
    ui->table_employes->setModel(model);
}

// ==================== FONCTIONS POUR REMPLIR LES TABLES ====================

/*void opticstor::remplirTableCommandes()
{
    ui->tableWidget_2->setRowCount(0);
    QSqlQuery query;

    if (query.exec("SELECT * FROM COMMANDES")) {
        while (query.next()) {
            int row = ui->tableWidget_2->rowCount();
            ui->tableWidget_2->insertRow(row);

            ui->tableWidget_2->setItem(row, 0, new QTableWidgetItem(query.value("ID_COMMANDE").toString()));
            ui->tableWidget_2->setItem(row, 1, new QTableWidgetItem(query.value("ID_CLIENT").toString()));
            ui->tableWidget_2->setItem(row, 2, new QTableWidgetItem(query.value("ID_EMPLOYE").toString()));
            ui->tableWidget_2->setItem(row, 3, new QTableWidgetItem(query.value("DATE_COMMANDE").toString()));
            ui->tableWidget_2->setItem(row, 4, new QTableWidgetItem(query.value("PRIX_TOTAL").toString()));
        }
    } else {
        qDebug() << "Erreur lors du remplissage des commandes:" << query.lastError().text();
    }
}*/

/*void opticstor::remplirTableClients()
{
    ui->tabcl_2->setRowCount(0);
    QSqlQuery query;

    if (query.exec("SELECT ID_CLIENT, EMAIL FROM CLIENTS")) {
        while (query.next()) {
            int row = ui->tabcl_2->rowCount();
            ui->tabcl_2->insertRow(row);

            ui->tabcl_2->setItem(row, 0, new QTableWidgetItem(query.value("ID_CLIENT").toString()));
            ui->tabcl_2->setItem(row, 1, new QTableWidgetItem(query.value("EMAIL").toString()));
            ui->tabcl_2->setItem(row, 2, new QTableWidgetItem(""));
        }
    }
}*/

// ==================== GESTION DES COMMANDES ====================
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
    ui->tablecmd->setEditTriggers(QAbstractItemView::NoEditTriggers);

    if(rech==0){
        QString orderBy = ui->orderbox->currentText();
        QString sql = "SELECT * FROM COMMANDE ORDER BY " + orderBy;
        QSqlQuery query(sql);  // Direct execution
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
    }
    else{
        QSqlQuery query;
        query.prepare("SELECT * FROM COMMANDE WHERE ID_CLIENT=:client");
        query.bindValue(":client",ui->search->text());
        ui->tablecmd->insertRow(0);
        QTableWidgetItem *item = new QTableWidgetItem(query.value(0).toString());
        ui->tablecmd->setItem(0,0, item);


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
    //qDebug() << "Success! Loaded" << pow << "rows";
}
void opticstor::rechcomd(){
    if(ui->search->text().isEmpty()){
        QMessageBox::warning(this,"recherche bar vide","choisir un client ID pour rechercher");
        return;
    }
    rech=1;

}
void opticstor::on_showChartButton_clicked() {
    // Step 1: Query sold glasses counts
    QSqlQuery query;
    int vueCount = 0;
    int solaireCount = 0;

    if(!query.exec("SELECT p.TYPE, COUNT(*) FROM CONTIENT c "
                    "JOIN PRODUIT p ON c.CODE_PRODUIT = p.CODE_PRODUIT "
                    "GROUP BY p.TYPE")) {
        qDebug() << "Query failed:" << query.lastError().text();
        return;
    }

    while(query.next()) {
        QString type = query.value(0).toString();
        int count = query.value(1).toInt();
        if(type == "vue") vueCount = count;
        else if(type == "solaire") solaireCount = count;
    }

    // Step 2: Create pie chart WITHOUT QtCharts:: prefix
    QPieSeries *series = new QPieSeries();
    series->append("Vue", vueCount);
    series->append("Solaire", solaireCount);

    // Optional: highlight the first slice
    series->slices().at(0)->setExploded();
    series->slices().at(0)->setLabelVisible();
    series->slices().at(1)->setLabelVisible();

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des verres vendus");
    chart->legend()->setAlignment(Qt::AlignRight);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Step 3: Show chart in a modal dialog
    QDialog dialog(this);
    QVBoxLayout layout(&dialog);
    layout.addWidget(chartView);
    dialog.setLayout(&layout);
    dialog.setWindowTitle("Statistiques de ventes");
    dialog.resize(500, 400);
    dialog.exec();
}

void opticstor::createPDFReceipt(const QString &pdfPath,
                                 const QString &customerName,
                                 const QVector<QPair<QString,double>> &productList,
                                 double totalPrice)
{

    //QString pdfPath = QDir::currentPath() + "/receipt_" + commandeId + ".pdf"; // PDF location

    QPdfWriter pdf(pdfPath);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setPageMargins(QMarginsF(15, 15, 15, 15));

    QPainter painter(&pdf);

    int y = 100;            // starting vertical position
    int lineHeight = 500;   // spacing between lines

    painter.setFont(QFont("Arial", 12));

    // --- HEADER ---
    painter.drawText(50, y, "Optic Store");
    y += lineHeight;
    painter.drawText(50, y, "Receipt / Facture");
    y += lineHeight;

    painter.drawLine(50, y, 550, y);
    y += lineHeight;

    // --- INFO ---
    painter.drawText(50, y, "Commande ID: " + QFileInfo(pdfPath).baseName()); // use filename as ID
    y += lineHeight;
    painter.drawText(50, y, "Client: " + customerName);
    y += lineHeight;
    painter.drawText(50, y, "Date: " + QDate::currentDate().toString("dd/MM/yyyy"));
    y += lineHeight;

    painter.drawLine(50, y, 550, y);
    y += lineHeight;

    // --- PRODUCTS ---
    painter.drawText(50, y, "Produit");
    painter.drawText(4000, y, "Prix");
    y += lineHeight;

    painter.drawLine(50, y, 550, y);
    y += lineHeight;

    for (auto &item : productList) {
        painter.drawText(50, y, item.first); // product name
        painter.drawText(4000, y, QString::number(item.second, 'f', 2) + " TND"); // price
        y += lineHeight;
    }

    painter.drawLine(50, y, 550, y);
    y += lineHeight;

    // --- TOTAL ---
    painter.setFont(QFont("Arial", 14, QFont::Bold));
    painter.drawText(50, y, "Total: " + QString::number(totalPrice, 'f', 2) + " TND");

    painter.end();
}

void opticstor::generatePDFForCommande(const QString &commandeId)
{
    QString customerName;
    QVector<QPair<QString,double>> items;
    double totalPrice = 0.0;

    // Get customer name
    QSqlQuery query;
    query.prepare("SELECT NOM FROM CLIENT WHERE ID_CLIENT = "
                  "(SELECT ID_CLIENT FROM COMMANDE WHERE ID_COMMANDE = :clmd)");
    query.bindValue(":clmd", commandeId);

    if(query.exec() && query.next()) {
        customerName = query.value(0).toString();
    } else {
        customerName = "Unknown";
    }

    // Get products for this commande
    QSqlQuery prodQuery;
    prodQuery.prepare("SELECT p.CODE_PRODUIT, p.PRIX FROM CONTIENT c "
                      "JOIN PRODUIT p ON c.CODE_PRODUIT = p.CODE_PRODUIT "
                      "WHERE c.ID_COMMANDE = :clmd");
    prodQuery.bindValue(":clmd", commandeId);

    if(prodQuery.exec()) {
        while(prodQuery.next()) {
            QString productName = prodQuery.value(0).toString();
            double price = prodQuery.value(1).toDouble();
            items.push_back({productName, price});
            totalPrice += price;
        }
    }

    QString pdfFileName = QString("receipt_%1_%2.pdf")
                              .arg(commandeId)
                              .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));

    createPDFReceipt(pdfFileName, customerName, items, totalPrice);
}

/*void opticstor::on_generatePDFButton_clicked()
{
    QString commandeId = ui->IDcommande->text();
    QString customerName;
    QVector<QPair<QString,double>> items;
    double totalPrice = 0.0;

    // --- Get customer name ---
    QSqlQuery query;
    query.prepare("SELECT NOM FROM CLIENT WHERE ID_CLIENT = "
                  "(SELECT ID_CLIENT FROM COMMANDE WHERE ID_COMMANDE = :clmd)");
    query.bindValue(":clmd", commandeId);
    if(query.exec() && query.next()) {
        customerName = query.value(0).toString();
    } else {
        qDebug() << "Failed to get customer name:" << query.lastError().text();
        customerName = "Unknown";
    }

    // --- Get products and prices for this command ---
    QSqlQuery prodQuery;
    prodQuery.prepare("SELECT p.NOM, p.PRIX FROM CONTIENT c "
                      "JOIN PRODUIT p ON c.CODE_PRODUIT = p.CODE_PRODUIT "
                      "WHERE c.ID_COMMANDE = :clmd");
    prodQuery.bindValue(":clmd", commandeId);

    if(prodQuery.exec()) {
        while(prodQuery.next()) {
            QString productName = prodQuery.value(0).toString();
            double price = prodQuery.value(1).toDouble();
            items.push_back({productName, price});
            totalPrice += price;
        }
    } else {
        qDebug() << "Failed to get products:" << prodQuery.lastError().text();
    }
    QString pdfFileName = QString("receipt_%1_%2.pdf")
                              .arg(commandeId)
                              .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    QString pdfPath = QDir::homePath() + "/Documents/" + pdfFileName;
    // --- Create PDF with dynamic data ---
    createPDFReceipt(commandeId, customerName, items, totalPrice);
    QMessageBox::information(this, "PDF Generated",  "PDF saved as: " + pdfFileName);
}*/
void opticstor::on_generatePDFButton_clicked()
{
    qDebug() << "PDF button clicked! Function is executing...";

    QString commandeId = ui->IDcommande->text().trimmed();

    // Check if commandeId is empty
    if(commandeId.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter a commande ID");
        return;
    }

    QString customerName;
    QVector<QPair<QString,double>> items;
    double totalPrice = 0.0;

    // --- Get customer name ---
    QSqlQuery query;
    query.prepare("SELECT NOM FROM CLIENT WHERE ID_CLIENT = "
                  "(SELECT ID_CLIENT FROM COMMANDE WHERE ID_COMMANDE = :clmd)");
    query.bindValue(":clmd", commandeId);
    if(query.exec() && query.next()) {
        customerName = query.value(0).toString();
    } else {
        qDebug() << "Failed to get customer name:" << query.lastError().text();
        QMessageBox::warning(this, "Error", "No commande found with ID: " + commandeId);
        return;
    }

    // --- Get products and prices for this command ---
    QSqlQuery prodQuery;
    prodQuery.prepare("SELECT p.CODE_PRODUIT, p.PRIX FROM CONTIENT c "  // ← FIXED: p.CODE_PRODUIT instead of p.NOM
                      "JOIN PRODUIT p ON c.CODE_PRODUIT = p.CODE_PRODUIT "
                      "WHERE c.ID_COMMANDE = :clmd");
    prodQuery.bindValue(":clmd", commandeId);

    if(prodQuery.exec()) {
        while(prodQuery.next()) {
            QString productName = prodQuery.value(0).toString();
            double price = prodQuery.value(1).toDouble();
            items.push_back({productName, price});
            totalPrice += price;
        }
    } else {
        qDebug() << "Failed to get products:" << prodQuery.lastError().text();
        QMessageBox::warning(this, "Error", "Failed to retrieve products");
        return;
    }

    // Check if we have any products
    if(items.isEmpty()) {
        QMessageBox::warning(this, "Error", "No products found for this commande");
        return;
    }

    QString pdfFileName = QString("receipt_%1_%2.pdf")
                              .arg(commandeId)
                              .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));

    // Create Documents directory if it doesn't exist
    QString documentsDir = QDir::homePath() + "/Documents/";
    QDir dir;
    if (!dir.exists(documentsDir)) {
        dir.mkpath(documentsDir);
    }

    QString pdfPath = documentsDir + pdfFileName;

    qDebug() << "Attempting to create PDF at:" << pdfPath;

    // --- Create PDF with dynamic data ---
    createPDFReceipt(pdfPath, customerName, items, totalPrice);  // ← FIXED: pass pdfPath instead of commandeId

    // Check if PDF was actually created
    if(QFile::exists(pdfPath)) {
        QMessageBox::information(this, "Success", "PDF saved as: " + pdfFileName + "\nLocation: " + documentsDir);
        qDebug() << "PDF successfully created at:" << pdfPath;
    } else {
        QMessageBox::critical(this, "Error", "Failed to create PDF file!");
        qDebug() << "PDF file was not created!";
    }
}
void opticstor::on_commandSelected(int row, int column)
{
    Q_UNUSED(column);

    // Assuming the first column (0) is the Commande ID
    QString commandeId = ui->tablecmd->item(row, 0)->text(); // assuming column 0 holds the ID

    generatePDFForCommande(commandeId);
}

void opticstor::updmen(){
    ui->commandsc->setCurrentIndex(0);
}


// ==================== GESTION DES FOURNISSEURS ====================

void opticstor::on_ajouter_fournisseur_clicked()
{
    QString idText = ui->id_fournisseur->text();
    QString nom_entreprise = ui->nom_entreprise->text();
    QString telephone = ui->telephone_fournisseur->text();
    QString email = ui->email_fournisseur->text();

    if (nom_entreprise.isEmpty() || telephone.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis!");
        return;
    }

    // Utiliser le nom_entreprise comme nom et laisser prénom vide, type par défaut "Local"
    // Si l'utilisateur a saisi un ID, l'utiliser pour construire le fournisseur
    Fournisseur f;
    if (!idText.isEmpty()) {
        bool ok = false;
        int id = idText.toInt(&ok);
        if (ok && id > 0) {
            f = Fournisseur(id, nom_entreprise, "", telephone, email, "Local");
        } else {
            QMessageBox::warning(this, "Erreur", "ID fournisseur invalide!");
            return;
        }
    } else {
        f = Fournisseur(nom_entreprise, "", telephone, email, "Local");
    }

    if (f.ajouter()) {
        QMessageBox::information(this, "Succès", "Fournisseur ajouté avec succès!");
        ui->tableWidget_22->setModel(tmpFournisseur.afficher());

        ui->id_fournisseur->clear();
        ui->nom_entreprise->clear();
        ui->telephone_fournisseur->clear();
        ui->email_fournisseur->clear();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout du fournisseur!");
    }
}

void opticstor::on_supprimer_fournisseur_clicked()
{
    QString idText = ui->id_fournisseur_supp->text();

    if (idText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID de fournisseur!");
        return;
    }

    int id = idText.toInt();

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation",
                                                              "Voulez-vous vraiment supprimer ce fournisseur?",
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (tmpFournisseur.supprimer(id)) {
            QMessageBox::information(this, "Succès", "Fournisseur supprimé avec succès!");
            ui->tableWidget_22->setModel(tmpFournisseur.afficher());
            ui->id_fournisseur_supp->clear();
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression!");
        }
    }
}

void opticstor::on_modifier_fournisseur_clicked()
{
    QString idText = ui->id_fournisseur->text();
    QString nom_entreprise = ui->nom_entreprise->text();
    QString telephone = ui->telephone_fournisseur->text();
    QString email = ui->email_fournisseur->text();

    if (idText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "ID fournisseur requis pour la modification!");
        return;
    }

    int id = idText.toInt();
    
    // Set the values in tmpFournisseur and call modifier
    tmpFournisseur.setNom(nom_entreprise);
    tmpFournisseur.setPrenom("");
    tmpFournisseur.setTelephone(telephone);
    tmpFournisseur.setEmail(email);
    tmpFournisseur.setType("Local");

    if (tmpFournisseur.modifier(id)) {
        QMessageBox::information(this, "Succès", "Fournisseur modifié avec succès!");
        ui->tableWidget_22->setModel(tmpFournisseur.afficher());

        ui->id_fournisseur->clear();
        ui->nom_entreprise->clear();
        ui->telephone_fournisseur->clear();
        ui->email_fournisseur->clear();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la modification!");
    }
}

void opticstor::on_rechercher_fournisseur_clicked()
{
    QString mot_cle = ui->recherchelabel->text();
    QString colonne = "NOM";  // Default search column

    if (mot_cle.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un terme de recherche!");
        return;
    }

    QSqlQueryModel* model = tmpFournisseur.rechercher(mot_cle, colonne);
    ui->tableWidget_22->setModel(model);

    if (model->rowCount() == 0) {
        QMessageBox::information(this, "Recherche", "Aucun résultat trouvé!");
    }
}

void opticstor::on_actualiser_fournisseur_clicked()
{
    ui->tableWidget_22->setModel(tmpFournisseur.afficher());
    QMessageBox::information(this, "Succès", "Liste actualisée!");
}

// ==================== GESTION DES PRODUITS ====================

void opticstor::on_ajouter_produit_clicked()
{
    QString codeText = ui->code_produit->text();
    QString idFournisseurText = ui->id_fournisseur_produit->text();
    QString marque = ui->marque_produit->text();
    QString type = ui->type_produit->currentText();
    QString quantiteText = ui->quantite_produit->text();
    QString prixText = ui->prix_produit->text();

    if (codeText.isEmpty() || idFournisseurText.isEmpty() || marque.isEmpty() ||
        quantiteText.isEmpty() || prixText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis!");
        return;
    }

    int code = codeText.toInt();
    int id_fournisseur = idFournisseurText.toInt();
    int quantite = quantiteText.toInt();
    float prix = prixText.toFloat();

    Produit p(code, id_fournisseur, marque, type, quantite, prix);

    if (p.ajouter()) {
        QMessageBox::information(this, "Succès", "Produit ajouté avec succès!");
        ui->table_produits->setModel(tmpProduit.afficher());

        ui->code_produit->clear();
        ui->id_fournisseur_produit->clear();
        ui->marque_produit->clear();
        ui->quantite_produit->clear();
        ui->prix_produit->clear();
        ui->type_produit->setCurrentIndex(0);
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout du produit!");
    }
}

void opticstor::on_supprimer_produit_clicked()
{
    QString codeText = ui->code_recherche_produit->text();

    if (codeText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un code de produit!");
        return;
    }

    int code = codeText.toInt();

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation",
                                                              "Voulez-vous vraiment supprimer ce produit?",
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (tmpProduit.supprimer(code)) {
            QMessageBox::information(this, "Succès", "Produit supprimé avec succès!");
            ui->table_produits->setModel(tmpProduit.afficher());
            ui->code_recherche_produit->clear();
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression!");
        }
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
void opticstor::setupTriComboBox()
{
    ui->combo_tri_produit_2->clear();

    ui->combo_tri_produit_2->addItem("Code produit ▲", "CODE_PRODUIT ASC");
    ui->combo_tri_produit_2->addItem("Code produit ▼", "CODE_PRODUIT DESC");
    ui->combo_tri_produit_2->addItem("Marque A-Z", "MARQUE ASC");
    ui->combo_tri_produit_2->addItem("Marque Z-A", "MARQUE DESC");
    ui->combo_tri_produit_2->addItem("Type A-Z", "TYPE ASC");
    ui->combo_tri_produit_2->addItem("Type Z-A", "TYPE DESC");
    ui->combo_tri_produit_2->addItem("Prix ▲", "PRIX ASC");
    ui->combo_tri_produit_2->addItem("Prix ▼", "PRIX DESC");
    ui->combo_tri_produit_2->addItem("Stock élevé", "QUANTITE_STOCK DESC");
    ui->combo_tri_produit_2->addItem("Stock faible", "QUANTITE_STOCK ASC");

    connect(ui->combo_tri_produit_2, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &opticstor::on_combo_tri_produit_currentIndexChanged);
}

void opticstor::on_combo_tri_produit_currentIndexChanged(int index)
{
    if (index < 0) return;

    QString orderBy = ui->combo_tri_produit_2->itemData(index).toString();
    qDebug() << "Tri demandé:" << orderBy;

    QSqlQueryModel* model = tmpProduit.afficherTrie(orderBy);

    if (model) {
        ui->table_produits->setModel(model);

        // Configuration des en-têtes
        model->setHeaderData(0, Qt::Horizontal, "Code Produit");
        model->setHeaderData(1, Qt::Horizontal, "Fournisseur");
        model->setHeaderData(2, Qt::Horizontal, "Marque");
        model->setHeaderData(3, Qt::Horizontal, "Type");
        model->setHeaderData(4, Qt::Horizontal, "Stock");
        model->setHeaderData(5, Qt::Horizontal, "Prix");

        // DEBUG
        qDebug() << "=== RÉSULTATS DU TRI ===";
        qDebug() << "Critère:" << orderBy;
        qDebug() << "Nombre total:" << model->rowCount();
        qDebug() << "Erreur:" << model->lastError().text();
    } else {
        qDebug() << "ERREUR: Modèle null";
    }
}
// ==================== FONCTIONS POUR LES TENDANCES ====================



void opticstor::on_tendances_btn_clicked()
{
    qDebug() << "Bouton Tendances cliqué!";
    calculerEtAfficherTendances();
}

void opticstor::setupTendances()
{
    qDebug() << "Initialisation du tableau des tendances...";

    QTableWidget *tableResultats = ui->tableResultats_2;
    if (!tableResultats) {
        qDebug() << "ERREUR: Tableau des résultats null!";
        return;
    }

    // Configurer uniquement le tableau des RÉSULTATS
    tableResultats->setRowCount(2);
    tableResultats->setColumnCount(2);
    tableResultats->setHorizontalHeaderLabels(QStringList() << "Catégorie" << "Tendance");
    tableResultats->setItem(0, 0, new QTableWidgetItem("Forme la plus vendue"));
    tableResultats->setItem(1, 0, new QTableWidgetItem("Modèle le plus vendu"));
    tableResultats->setItem(0, 1, new QTableWidgetItem("Cliquez sur Actualiser"));
    tableResultats->setItem(1, 1, new QTableWidgetItem("Cliquez sur Actualiser"));

    tableResultats->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    qDebug() << "Initialisation des tendances terminée!";
}

void opticstor::calculerEtAfficherTendances()
{
    qDebug() << "=== CALCUL TENDANCES PAR OCCURRENCES ===";

    QTableWidget *tableResultats = ui->tableResultats_2;
    if (!tableResultats) {
        QMessageBox::critical(this, "Erreur", "Tableau des résultats non initialisé!");
        return;
    }

    // Vérifier la connexion à la base de données
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Erreur", "Base de données non connectée!");
        return;
    }

    // ========== FORMES AVEC NOMBRE D'OCCURRENCES ==========
    qDebug() << "=== FORMES - NOMBRE D'OCCURRENCES ===";
    QSqlQuery formesQuery;
    if (formesQuery.exec("SELECT FORME_LUNETTES, COUNT(*) FROM COMMANDE WHERE FORME_LUNETTES IS NOT NULL GROUP BY FORME_LUNETTES ORDER BY COUNT(*) DESC")) {

        QString formeTop;
        int maxOccurrences = 0;
        bool hasResults = false;

        while (formesQuery.next()) {
            hasResults = true;
            QString forme = formesQuery.value(0).toString();
            int occurrences = formesQuery.value(1).toInt();
            qDebug() << "Forme:" << forme << "| Occurrences:" << occurrences;

            if (occurrences > maxOccurrences) {
                maxOccurrences = occurrences;
                formeTop = forme;
            }
        }

        if (hasResults && maxOccurrences > 0) {
            qDebug() << "→ FORME TENDANCE:" << formeTop << "avec" << maxOccurrences << "occurrences";
            tableResultats->setItem(0, 1, new QTableWidgetItem(QString("%1 (%2 commandes)").arg(formeTop).arg(maxOccurrences)));
        } else {
            tableResultats->setItem(0, 1, new QTableWidgetItem("Aucune donnée"));
        }
    } else {
        qDebug() << "Erreur formes:" << formesQuery.lastError().text();
    }

    // ========== MODÈLES AVEC NOMBRE D'OCCURRENCES ==========
    qDebug() << "=== MODÈLES - NOMBRE D'OCCURRENCES ===";
    QSqlQuery modelesQuery;
    if (modelesQuery.exec("SELECT MODELE_LUNETTES, COUNT(*) FROM COMMANDE WHERE MODELE_LUNETTES IS NOT NULL GROUP BY MODELE_LUNETTES ORDER BY COUNT(*) DESC")) {

        QString modeleTop;
        int maxOccurrences = 0;
        bool hasResults = false;

        while (modelesQuery.next()) {
            hasResults = true;
            QString modele = modelesQuery.value(0).toString();
            int occurrences = modelesQuery.value(1).toInt();
            qDebug() << "Modèle:" << modele << "| Occurrences:" << occurrences;

            if (occurrences > maxOccurrences) {
                maxOccurrences = occurrences;
                modeleTop = modele;
            }
        }

        if (hasResults && maxOccurrences > 0) {
            qDebug() << "→ MODÈLE TENDANCE:" << modeleTop << "avec" << maxOccurrences << "occurrences";
            tableResultats->setItem(1, 1, new QTableWidgetItem(QString("%1 (%2 commandes)").arg(modeleTop).arg(maxOccurrences)));
        } else {
            tableResultats->setItem(1, 1, new QTableWidgetItem("Aucune donnée"));
        }
    } else {
        qDebug() << "Erreur modèles:" << modelesQuery.lastError().text();
    }

    // Rafraîchissement
    tableResultats->viewport()->update();
    tableResultats->repaint();

    qDebug() << "=== CALCUL PAR OCCURRENCES TERMINÉ ===";
}
void opticstor::on_exporter_pdf_produits_clicked()
{
    // Récupérer tous les produits D'ABORD
    QList<Produit> produits = Produit::getAllProduits();

    // Affiche un message debug
    qDebug() << "Produits à exporter :" << produits.count();

    if (produits.isEmpty()) {
        QMessageBox::information(this, "Export PDF", "Aucun produit à exporter !");
        return;
    }

    // Demander où sauvegarder le fichier
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Exporter les produits en PDF",
                                                    QDir::homePath() + "/liste_produits.pdf",
                                                    "Fichiers PDF (*.pdf)");

    if (fileName.isEmpty()) {
        return;
    }

    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
        fileName += ".pdf";
    }

    // Créer le document HTML
    QString htmlContent = genererHTMLProduits(produits);

    // Créer le document texte
    QTextDocument document;
    document.setHtml(htmlContent);

    // Configurer l'imprimante PDF
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    // Exporter en PDF
    document.print(&printer);

    QMessageBox::information(this,
                             "Export PDF Réussi",
                             QString("Liste des produits exportée avec succès!\n\n"
                                     "Fichier: %1\n"
                                     "Nombre de produits: %2")
                                 .arg(fileName)
                                 .arg(produits.count()));

    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}
QString opticstor::genererHTMLProduits(const QList<Produit>& produits)
{
    QString html;

    // HTML de base
    html = "<html><body>";
    html += "<h1>Liste des Produits</h1>";
    html += "<table border='1' style='width:100%; border-collapse:collapse;'>";
    html += "<tr style='background-color:#f2f2f2;'>";
    html += "<th style='padding:8px;'>Code</th>";
    html += "<th style='padding:8px;'>ID Fournisseur</th>";
    html += "<th style='padding:8px;'>Marque</th>";
    html += "<th style='padding:8px;'>Type</th>";
    html += "<th style='padding:8px;'>Quantité</th>";
    html += "<th style='padding:8px;'>Prix (€)</th>";
    html += "</tr>";

    // Ajouter chaque produit
    for (int i = 0; i < produits.size(); i++) {
        Produit produit = produits[i];

        // Alterner les couleurs des lignes pour une meilleure lisibilité
        QString rowColor = (i % 2 == 0) ? "#ffffff" : "#f9f9f9";

        html += "<tr style='background-color:" + rowColor + ";'>";
        html += "<td style='padding:8px;'>" + QString::number(produit.getCode()) + "</td>";
        html += "<td style='padding:8px;'>" + QString::number(produit.getIdFournisseur()) + "</td>";
        html += "<td style='padding:8px;'>" + produit.getMarque() + "</td>";
        html += "<td style='padding:8px;'>" + produit.getType() + "</td>";
        html += "<td style='padding:8px;'>" + QString::number(produit.getQuantiteStock()) + "</td>";
        html += "<td style='padding:8px;'>" + QString::number(produit.getPrix(), 'f', 2) + " €</td>";
        html += "</tr>";
    }

    html += "</table>";

    // Ajouter des statistiques
    html += "<div style='margin-top:20px; padding:10px; background-color:#f2f2f2;'>";
    html += "<h3>Statistiques</h3>";
    html += "<p>Nombre total de produits: <strong>" + QString::number(produits.count()) + "</strong></p>";

    // Calculer la valeur totale du stock
    double valeurTotale = 0;
    int totalQuantite = 0;
    for (const Produit& produit : produits) {
        valeurTotale += produit.getPrix() * produit.getQuantiteStock();
        totalQuantite += produit.getQuantiteStock();
    }

    html += "<p>Valeur totale du stock: <strong>" + QString::number(valeurTotale, 'f', 2) + " €</strong></p>";
    html += "<p>Quantité totale en stock: <strong>" + QString::number(totalQuantite) + "</strong></p>";
    html += "</div>";

    html += "</body></html>";
    return html;
}
// FONCTION PLEINE POUR DÉTERMINER LE SEXE
QString opticstor::determinerSexe(const QString& marque)
{
    QString marqueUpper = marque.toUpper();

    if (marqueUpper.contains("DIOR") || marqueUpper.contains("FENDI") ||
        marqueUpper.contains("MIU") || marqueUpper.contains("CHANEL"))
        return "FEMME";
    else if (marqueUpper.contains("RAYBAN") || marqueUpper.contains("OAKLEY") ||
             marqueUpper.contains("POLAROID") || marqueUpper.contains("ARMANI"))
        return "HOMME";
    else
        return "MIXTE";
}

// FONCTION PLEINE POUR AFFICHER LE STOCK
void opticstor::afficherStockSelonStatut(const QString& statut)
{
    qDebug() << "=== STATUT REÇU:" << statut << "===";

    QTableWidget *tableStock = ui->tableStock_2;
    if (!tableStock) {
        qDebug() << "ERREUR: tableStock est NULL!";
        return;
    }

    // Vider le tableau
    tableStock->setRowCount(0);

    QSqlQuery query;
    QString requete;

    // TES NOUVELLES RÈGLES:
    if (statut == "RUPTURE") {
        requete = "SELECT CODE_PRODUIT, MARQUE, TYPE, QUANTITE_STOCK, PRIX FROM PRODUIT WHERE QUANTITE_STOCK < 3"; // 0,1,2
    }
    else if (statut == "FAIBLE") {
        requete = "SELECT CODE_PRODUIT, MARQUE, TYPE, QUANTITE_STOCK, PRIX FROM PRODUIT WHERE QUANTITE_STOCK BETWEEN 3 AND 10"; // 3-10
    }
    else if (statut == "NORMAL") {
        requete = "SELECT CODE_PRODUIT, MARQUE, TYPE, QUANTITE_STOCK, PRIX FROM PRODUIT WHERE QUANTITE_STOCK > 10"; // 11+
    }
    else if (statut == "TOUS") {
        requete = "SELECT CODE_PRODUIT, MARQUE, TYPE, QUANTITE_STOCK, PRIX FROM PRODUIT";
    }

    if (query.exec(requete)) {
        int row = 0;
        while (query.next()) {
            tableStock->insertRow(row);

            int code = query.value(0).toInt();
            QString marque = query.value(1).toString();
            QString type = query.value(2).toString();
            int quantite = query.value(3).toInt();
            double prix = query.value(4).toDouble();

            QString sexe = determinerSexe(marque);

            // Ajouter les données au tableau
            tableStock->setItem(row, 0, new QTableWidgetItem(QString::number(code)));
            tableStock->setItem(row, 1, new QTableWidgetItem(marque));
            tableStock->setItem(row, 2, new QTableWidgetItem(sexe));
            tableStock->setItem(row, 3, new QTableWidgetItem(type));
            tableStock->setItem(row, 4, new QTableWidgetItem(QString::number(quantite)));
            tableStock->setItem(row, 5, new QTableWidgetItem(QString::number(prix, 'f', 2) + " DT"));

            // 🎨 COULEURS AVEC TES NOUVELLES RÈGLES
            if (quantite < 3) {
                // ROUGE FONCÉ pour rupture (< 3)
                for (int col = 0; col < 6; col++) {
                    if (tableStock->item(row, col)) {
                        tableStock->item(row, col)->setBackground(QColor(255, 150, 150));
                        tableStock->item(row, col)->setForeground(QColor(0, 0, 0));
                    }
                }
            } else if (quantite <= 10) {
                // ORANGE FONCÉ pour faible (3-10)
                for (int col = 0; col < 6; col++) {
                    if (tableStock->item(row, col)) {
                        tableStock->item(row, col)->setBackground(QColor(255, 200, 100));
                        tableStock->item(row, col)->setForeground(QColor(0, 0, 0));
                    }
                }
            } else {
                // VERT FONCÉ pour normal (11+)
                for (int col = 0; col < 6; col++) {
                    if (tableStock->item(row, col)) {
                        tableStock->item(row, col)->setBackground(QColor(150, 255, 150));
                        tableStock->item(row, col)->setForeground(QColor(0, 0, 0));
                    }
                }
            }

            row++;
        }

        qDebug() << row << "lignes ajoutées pour statut:" << statut;
        tableStock->resizeColumnsToContents();

    } else {
        qDebug() << "Erreur requête:" << query.lastError().text();
    }
}
// TES 4 BOUTONS - FONCTIONS COMPLÈTES
void opticstor::on_btnRupture_clicked()
{
    afficherStockSelonStatut("RUPTURE");
}

void opticstor::on_btnFaible_clicked()
{
    afficherStockSelonStatut("FAIBLE");
}

void opticstor::on_btnNormal_clicked()
{
    afficherStockSelonStatut("NORMAL");
}

void opticstor::on_btnTous_clicked()
{
    afficherStockSelonStatut("TOUS");
}
void opticstor::on_statistique_stock_clicked()
{
    // Créer une nouvelle fenêtre pour les statistiques de stock
    QWidget *statsWindow = new QWidget();
    statsWindow->setWindowTitle("📊 Statistiques des Niveaux de Stock");
    statsWindow->setMinimumSize(800, 600);
    statsWindow->setStyleSheet("background-color: white;");

    // Layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(statsWindow);

    // Titre
    QLabel *title = new QLabel("📦 STATISTIQUES DES NIVEAUX DE STOCK");
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #2c3e50; padding: 15px;");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    // Analyser le stock depuis la base de données - VERSION OPTIMISÉE
    int stockFaibleCount = 0;
    int ruptureCount = 0;
    int stockOKCount = 0;
    int totalProduits = 0;

    QSqlQuery query;

    // Version optimisée avec comptage direct en SQL - NOUVEAUX SEUILS
    QString requeteComptage =
        "SELECT "
        "SUM(CASE WHEN QUANTITE_STOCK < 3 THEN 1 ELSE 0 END) as rupture, "  // 0, 1, 2
        "SUM(CASE WHEN QUANTITE_STOCK >= 3 AND QUANTITE_STOCK < 10 THEN 1 ELSE 0 END) as faible, "  // 3-9
        "SUM(CASE WHEN QUANTITE_STOCK >= 10 THEN 1 ELSE 0 END) as ok, "  // 10+
        "COUNT(*) as total "
        "FROM PRODUIT";

    if (query.exec(requeteComptage) && query.next()) {
        ruptureCount = query.value("rupture").toInt();
        stockFaibleCount = query.value("faible").toInt();
        stockOKCount = query.value("ok").toInt();
        totalProduits = query.value("total").toInt();
    } else {
        // Gestion d'erreur SQL
        QLabel *errorLabel = new QLabel("Erreur lors de la lecture de la base de données: " + query.lastError().text());
        errorLabel->setStyleSheet("font-size: 16px; color: #e74c3c; padding: 20px;");
        errorLabel->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(errorLabel);
    }

    if (totalProduits == 0) {
        QLabel *noDataLabel = new QLabel("Aucun produit trouvé dans la base de données !");
        noDataLabel->setStyleSheet("font-size: 16px; color: #7f8c8d; padding: 50px;");
        noDataLabel->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(noDataLabel);
    } else {
        // Créer le camembert
        QPieSeries *series = new QPieSeries();

        // Définir les statuts avec couleurs - NOUVEAUX LIBELLÉS
        QStringList statuts = {"✅ Stock NORMAL", "⚠ Stock FAIBLE", "🔴 RUPTURE"};
        QList<QColor> couleurs = {
            QColor(39, 174, 96),    // Vert
            QColor(243, 156, 18),   // Orange
            QColor(231, 76, 60)     // Rouge
        };

        QList<int> counts = {stockOKCount, stockFaibleCount, ruptureCount};

        for (int i = 0; i < statuts.size(); ++i) {
            int count = counts[i];
            if (count > 0) {
                double percentage = (count * 100.0) / totalProduits;

                // Créer la tranche
                QPieSlice *slice = series->append(statuts[i], count);

                // Configurer l'apparence
                slice->setColor(couleurs[i]);
                slice->setLabelVisible(true);
                slice->setExploded(true);

                // Formater le label avec pourcentage
                QString label = QString("%1\n%2 produit(s)\n%3%")
                                    .arg(statuts[i])
                                    .arg(count)
                                    .arg(QString::number(percentage, 'f', 1));
                slice->setLabel(label);
            }
        }

        // Créer le chart
        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Répartition des Niveaux de Stock");
        chart->setAnimationOptions(QChart::AllAnimations);
        chart->setTitleBrush(QBrush(QColor(44, 62, 80)));
        chart->setTitleFont(QFont("Arial", 14, QFont::Bold));

        // Créer la vue du chart
        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setStyleSheet("background-color: transparent;");

        // Ajouter le graphique au layout
        mainLayout->addWidget(chartView);

        // Ajouter un résumé textuel - NOUVELLES DESCRIPTIONS
        QLabel *summary = new QLabel(
            QString("📋 RÉSUMÉ:\n\n"
                    "• Total produits analysés: %1\n"
                    "• ✅ Stock NORMAL (10+ unités): %2 produit(s)\n"
                    "• ⚠ Stock FAIBLE (3-9 unités): %3 produit(s)\n"
                    "• 🔴 RUPTURE (0-2 unités): %4 produit(s)")
                .arg(totalProduits)
                .arg(stockOKCount)
                .arg(stockFaibleCount)
                .arg(ruptureCount)
            );
        summary->setStyleSheet("font-size: 14px; color: #2c3e50; padding: 15px; background-color: #f8f9fa; border-radius: 10px; margin: 10px;");
        summary->setAlignment(Qt::AlignLeft);
        mainLayout->addWidget(summary);
    }

    // Bouton fermer
    QPushButton *closeButton = new QPushButton("Fermer");
    closeButton->setFixedSize(100, 40);
    closeButton->setStyleSheet("QPushButton { background-color: #3498db; color: white; border: none; border-radius: 5px; font-weight: bold; }"
                               "QPushButton:hover { background-color: #2980b9; }");
    connect(closeButton, &QPushButton::clicked, statsWindow, &QWidget::close);

    mainLayout->addWidget(closeButton, 0, Qt::AlignCenter);

    // Afficher la fenêtre
    statsWindow->show();
}

// FONCTION POUR REMPLIR LE COMBOBOX



// ==================== FONCTIONS EXISTANTES ====================
void opticstor::onGestCommande(){
    ui->opticstack->setCurrentIndex(1);
}

void opticstor::onGest(){
    ui->opticstack->setCurrentIndex(2);
}

/*void opticstor::onSortChanged(int index)
{
    ui->tabcl_2->sortItems(index, Qt::AscendingOrder);
}
*/
void opticstor:: onprod(){
    ui->opticstack->setCurrentIndex(3);
}

void opticstor::onemp(){
    ui->opticstack->setCurrentIndex(4);
}

void opticstor::onfourn(){
    ui->opticstack->setCurrentIndex(0);
}

// ==================== RAVITAILLEMENT / FEEDBACK / STATISTIQUES FOURNISSEURS ====================

void opticstor::on_ajoutravitalement_clicked()
{
    // Open the Ravitaillement dialog so user can fill all ravitaillement data in a single place
    RavitaillementDialog dlg(this);
    connect(&dlg, &RavitaillementDialog::ravitaillementAdded, this, [&](){
        // Refresh related views after an add
        ui->table_produits->setModel(tmpProduit.afficher());
        chargerRavitaillementTable();
    });
    dlg.exec();
}

void opticstor::on_feedback_clicked()
{
    FeedbackDialog dlg(this);
    dlg.exec();
}

void opticstor::on_triparnom_clicked()
{
    // Use the Fournisseur model ordering instead of relying on the view
    QSqlQueryModel* model = tmpFournisseur.afficherTrie("NOM_ENTREPRISE ASC");
    if (model && model->rowCount() >= 0) {
        ui->tableWidget_22->setModel(model);
        QMessageBox::information(this, tr("✅ Tri effectué"), tr("Fournisseurs triés par nom!"));
    } else {
        QMessageBox::warning(this, tr("Erreur"), tr("Impossible de trier les fournisseurs"));
    }
}


void opticstor::on_exportexel_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Exporter CSV"), QString(), tr("CSV files (*.csv);;All Files (*)"));
    if (path.isEmpty()) return;

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Erreur"), tr("Impossible d'ouvrir le fichier en écriture"));
        return;
    }

    QTextStream out(&file);

    // Get model from tableWidget_22
    QAbstractItemModel* model = ui->tableWidget_22->model();
    if (!model) {
        QMessageBox::warning(this, tr("Erreur"), tr("Aucune donnée à exporter"));
        file.close();
        return;
    }

    // header
    QStringList headers;
    for (int c = 0; c < model->columnCount(); ++c) {
        headers << model->headerData(c, Qt::Horizontal).toString();
    }
    out << headers.join(',') << '\n';

    for (int r = 0; r < model->rowCount(); ++r) {
        QStringList row;
        for (int c = 0; c < model->columnCount(); ++c) {
            QString data = model->data(model->index(r, c)).toString();
            row << data.replace(',', ';');
        }
        out << row.join(',') << '\n';
    }

    file.close();
    QMessageBox::information(this, tr("Exporté"), tr("CSV exporté avec succès à:\n%1").arg(path));
}

void opticstor::on_stat_fournisseur_clicked()
{
    // Ask whether to show stats per supplier or grouped by type
    QMessageBox msg(this);
    msg.setWindowTitle(tr("Choisir type de statistique"));
    msg.setText(tr("Afficher les statistiques par :"));
    msg.addButton(tr("Fournisseur"), QMessageBox::AcceptRole);
    QPushButton *byT = msg.addButton(tr("Type de fournisseur"), QMessageBox::AcceptRole);
    QPushButton *cancel = msg.addButton(QMessageBox::Cancel);
    msg.exec();

    if (msg.clickedButton() == cancel) return;

    if (msg.clickedButton() == byT) {
        // Show stats aggregated by type
        QSqlQueryModel* byType = tmpMetier.statsParType();
        if (!byType || byType->rowCount() == 0) {
            QMessageBox::information(this, tr("Statistiques"), tr("Aucune statistique de type trouvée."));
            return;
        }

        QString out;
        for (int r = 0; r < byType->rowCount(); ++r) {
            QString type = byType->data(byType->index(r,0)).toString();
            QString nb = byType->data(byType->index(r,1)).toString();
            QString totalQ = byType->data(byType->index(r,2)).toString();
            QString avg = byType->data(byType->index(r,3)).toString();
            out += QString("Type: %1  -  #Rav: %2  -  TotalQté: %3  -  NoteAvg: %4\n").arg(type, nb, totalQ, avg);
        }
        QMessageBox::information(this, tr("Statistiques par type"), out);
        return;
    }

    // default: by fournisseur
    QSqlQueryModel* all = tmpMetier.statsFournisseur(-1);
    if (!all || all->rowCount() == 0) {
        QMessageBox::information(this, "Statistiques", "Aucune statistique trouvée.");
        return;
    }

    QString out;
    for (int r = 0; r < all->rowCount(); ++r) {
        QString id = all->data(all->index(r,0)).toString();
        QString nb = all->data(all->index(r,1)).toString();
        QString totalQ = all->data(all->index(r,2)).toString();
        QString avg = all->data(all->index(r,3)).toString();
        out += QString("ID: %1  -  #Rav: %2  -  TotalQté: %3  -  NoteAvg: %4\n").arg(id, nb, totalQ, avg);
    }
    QMessageBox::information(this, "Statistiques fournisseurs", out);
}

void opticstor::chargerRavitaillementTable()
{
    // Clear the QTableWidget and refill from metier model
    QSqlQueryModel* model = tmpMetier.afficherRavitaillement();
    if (!ravitTable) return;
    ravitTable->setRowCount(0);
    ravitTable->setColumnCount(model->columnCount());

    // Set headers if available
    for (int c = 0; c < model->columnCount(); ++c) {
        QString header = model->headerData(c, Qt::Horizontal).toString();
        ravitTable->setHorizontalHeaderItem(c, new QTableWidgetItem(header));
    }

    for (int r = 0; r < model->rowCount(); ++r) {
        ravitTable->insertRow(r);
        for (int c = 0; c < model->columnCount(); ++c) {
            QModelIndex idx = model->index(r, c);
            QString value = model->data(idx).toString();
            ravitTable->setItem(r, c, new QTableWidgetItem(value));
        }
    }
}

void opticstor::showRavContextMenu(const QPoint &pos)
{
    if (!ravitTable) return;
    QTableWidgetItem* item = ravitTable->itemAt(pos);
    if (!item) return; // not on a row

    int row = item->row();

    QMenu menu(this);
    QAction *edit = menu.addAction(tr("Modifier"));
    QAction *del = menu.addAction(tr("Supprimer"));

    QAction *a = menu.exec(ravitTable->viewport()->mapToGlobal(pos));
    if (!a) return;

    // ID is in column 0 according to afficherRavitaillement()
    QString idStr = ravitTable->item(row, 0)->text();
    int id = idStr.toInt();

    if (a == del) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Confirmer"), tr("Supprimer ce ravitaillement ?"), QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            QString err;
            if (tmpMetier.supprimerRavitaillement(id, err)) {
                QMessageBox::information(this, tr("Supprimé"), tr("Ravitaillement supprimé"));
                chargerRavitaillementTable();
            } else {
                QMessageBox::critical(this, tr("Erreur"), tr("Impossible de supprimer : %1").arg(err));
            }
        }
        return;
    }

    if (a == edit) {
        // Open inline dialog with fields
        QDialog dlg(this);
        dlg.setWindowTitle(tr("Modifier Ravitaillement"));
        QFormLayout *form = new QFormLayout(&dlg);

        QLineEdit *codeEdit = new QLineEdit(&dlg);
        QLineEdit *fournEdit = new QLineEdit(&dlg);
        QSpinBox *qte = new QSpinBox(&dlg);
        QTextEdit *msgEdit = new QTextEdit(&dlg);
        QSpinBox *note = new QSpinBox(&dlg);

        qte->setRange(0, 10000000);
        note->setRange(0,5);

        // Fill current values from table columns
        QTableWidgetItem* itCode = ravitTable->item(row,1);
        QTableWidgetItem* itFourn = ravitTable->item(row,2);
        QTableWidgetItem* itQte = ravitTable->item(row,3);
        QTableWidgetItem* itMsg = ravitTable->item(row,4);
        QTableWidgetItem* itNote = ravitTable->item(row,5);

        if (itCode) codeEdit->setText(itCode->text());
        if (itFourn) fournEdit->setText(itFourn->text());
        if (itQte) qte->setValue(itQte->text().toInt());
        if (itMsg) msgEdit->setPlainText(itMsg->text());
        if (itNote) note->setValue(itNote->text().toInt());

        form->addRow(tr("Code Produit (laisser vide pour NULL)"), codeEdit);
        form->addRow(tr("ID Fournisseur (laisser vide pour NULL)"), fournEdit);
        form->addRow(tr("Quantité"), qte);
        form->addRow(tr("Message"), msgEdit);
        form->addRow(tr("Note (0-5)"), note);

        QHBoxLayout *btnLayout = new QHBoxLayout();
        QPushButton *save = new QPushButton(tr("Enregistrer"), &dlg);
        QPushButton *cancelBtn = new QPushButton(tr("Annuler"), &dlg);
        btnLayout->addWidget(save);
        btnLayout->addWidget(cancelBtn);
        form->addRow(btnLayout);

        connect(cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
        connect(save, &QPushButton::clicked, &dlg, [&]() {
            // gather data and call modifier
            bool okCode=false, okF=false;
            int cval = codeEdit->text().toInt(&okCode);
            int fval = fournEdit->text().toInt(&okF);
            int qval = qte->value();
            QString messageText = msgEdit->toPlainText();
            int noteVal = note->value();

            int codeParam = okCode ? cval : -1;
            int fournParam = okF ? fval : -1;

            QString err;
            bool ok2 = tmpMetier.modifierRavitaillement(id, codeParam, fournParam, qval, messageText, noteVal, err);
            if (!ok2) {
                QMessageBox::critical(&dlg, tr("Erreur"), tr("La modification a échoué: %1").arg(err));
            } else {
                QMessageBox::information(&dlg, tr("OK"), tr("Ravitaillement modifié"));
                dlg.accept();
                chargerRavitaillementTable();
            }
        });

        dlg.exec();
    }
}

void opticstor::chargerTableFournisseurs()
{
    ui->tableWidget_22->setModel(tmpFournisseur.afficher());
}

opticstor::~opticstor()
{
    delete ui;
}
///////////////////////////////////////////
void opticstor::update_label()
{
    while (A.getserial() && A.getserial()->bytesAvailable() > 0) {
        data = A.read_from_arduino();
        QString receivedData = QString(data).trimmed();

        if (receivedData.isEmpty()) continue;

        qDebug() << "=== COMMANDE REÇUE ===" << receivedData;

        if (receivedData == "AGE_MAX") {
            qDebug() << "Appui court -> Fenêtre Âge";
            showAgeMaxWindow(getMaxAge());
        }
        else if (receivedData == "NB_CLIENTS") {
            qDebug() << "Appui moyen -> Fenêtre Clients";
            QTimer::singleShot(100, this, [this]() {
                showClientCountWindow(getClientCount());
            });
        }
        else if (receivedData == "NB_TEST") {
            qDebug() << "Appui long -> Fenêtre Tests";
            QTimer::singleShot(100, this, [this]() {
                showTestCountWindow(getTestCount());
            });
        }
    }
}
///////////////////////////////////////////////////
void opticstor::showAgeMaxWindow(int ageMax)
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("👴 Âge Maximum");
    dialog->setFixedSize(450, 350);

    // Style moderne comme les autres fenêtres
    dialog->setStyleSheet(
        "QDialog {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "                                stop:0 #3498db, stop:1 #2c3e50);"
        "    border-radius: 15px;"
        "    border: 3px solid #2980b9;"
        "}"
        "QLabel { color: white; font-family: 'Segoe UI', Arial; }"
        "QPushButton {"
        "    background-color: #e74c3c;"
        "    color: white;"
        "    padding: 12px 30px;"
        "    border-radius: 8px;"
        "    font-weight: bold;"
        "    border: none;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover { background-color: #c0392b; }"
        );

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->setSpacing(15);
    layout->setContentsMargins(25, 25, 25, 25);

    // 1. Icône
    QLabel *icon = new QLabel("👴");
    icon->setAlignment(Qt::AlignCenter);
    icon->setStyleSheet("font-size: 48px;");

    // 2. Titre
    QLabel *title = new QLabel("ÂGE MAXIMUM");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "font-size: 22px;"
        "font-weight: bold;"
        "color: #f1c40f;"
        "text-transform: uppercase;"
        "margin-bottom: 10px;"
        );

    // 3. Cadre pour l'âge
    QFrame *frame = new QFrame();
    frame->setStyleSheet(
        "QFrame {"
        "    background-color: rgba(255, 255, 255, 0.15);"
        "    border-radius: 10px;"
        "    border: 2px solid rgba(255, 255, 255, 0.3);"
        "}"
        );

    QVBoxLayout *frameLayout = new QVBoxLayout(frame);
    frameLayout->setContentsMargins(20, 20, 20, 20);

    QLabel *ageLabel = new QLabel(QString::number(ageMax));
    ageLabel->setAlignment(Qt::AlignCenter);
    ageLabel->setStyleSheet(
        "font-size: 72px;"
        "font-weight: bold;"
        "color: white;"
        "text-shadow: 2px 2px 4px rgba(0,0,0,0.5);"
        );

    QLabel *unitLabel = new QLabel("ans");
    unitLabel->setAlignment(Qt::AlignCenter);
    unitLabel->setStyleSheet(
        "font-size: 18px;"
        "color: #ecf0f1;"
        "font-weight: bold;"
        );

    frameLayout->addWidget(ageLabel);
    frameLayout->addWidget(unitLabel);

    // 4. Information supplémentaire
    QLabel *info = new QLabel(
        "📊 Cette statistique montre l'âge du client\n"
        "le plus âgé dans votre base de données.\n"
        "Données actualisées en temps réel."
        );
    info->setAlignment(Qt::AlignCenter);
    info->setWordWrap(true);
    info->setStyleSheet(
        "background-color: rgba(0, 0, 0, 0.2);"
        "border-radius: 8px;"
        "padding: 12px;"
        "color: #ecf0f1;"
        "font-size: 13px;"
        "margin-top: 10px;"
        );

    // 5. Bouton Fermer
    QPushButton *btnClose = new QPushButton("Fermer la fenêtre");
    btnClose->setCursor(Qt::PointingHandCursor);
    btnClose->setFixedWidth(200);
    connect(btnClose, &QPushButton::clicked, dialog, &QDialog::accept);

    // 6. Assemblage
    layout->addWidget(icon);
    layout->addWidget(title);
    layout->addWidget(frame);
    layout->addWidget(info);
    layout->addWidget(btnClose, 0, Qt::AlignCenter);

    dialog->exec();
    delete dialog;

    qDebug() << "Fenêtre âge max affichée:" << ageMax << "ans";
}
////////////////////////////////////////////////////
void opticstor::showClientCountWindow(int nbClients)
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("👥 Nombre de Clients");
    dialog->setFixedSize(400, 300);

    // Style
    dialog->setStyleSheet(
        "QDialog {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "                                stop:0 #8e44ad, stop:1 #3498db);"
        "    border-radius: 15px;"
        "    border: 3px solid #2980b9;"
        "}"
        "QLabel { color: white; font-family: 'Segoe UI'; }"
        "QPushButton {"
        "    background-color: #e67e22;"
        "    color: white;"
        "    padding: 12px 24px;"
        "    border-radius: 8px;"
        "    font-weight: bold;"
        "    border: none;"
        "}"
        "QPushButton:hover { background-color: #d35400; }"
        );

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->setSpacing(20);
    layout->setContentsMargins(30, 30, 30, 30);

    // Icône
    QLabel *icon = new QLabel("👥");
    icon->setAlignment(Qt::AlignCenter);
    icon->setStyleSheet("font-size: 50px;");

    // Titre
    QLabel *title = new QLabel("NOMBRE TOTAL DE CLIENTS");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #f1c40f;");

    // Valeur
    QLabel *countLabel = new QLabel(QString::number(nbClients));
    countLabel->setAlignment(Qt::AlignCenter);
    countLabel->setStyleSheet(
        "font-size: 72px;"
        "font-weight: bold;"
        "color: white;"
        "text-shadow: 3px 3px 6px rgba(0,0,0,0.5);"
        );

    // Sous-titre
    QLabel *subtitle = new QLabel("clients enregistrés");
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setStyleSheet("font-size: 18px; color: #ecf0f1;");

    // Info
    QLabel *info = new QLabel(
        "Cette statistique est mise à jour en temps réel\n"
        "depuis votre base de données."
        );
    info->setAlignment(Qt::AlignCenter);
    info->setWordWrap(true);
    info->setStyleSheet(
        "background-color: rgba(255,255,255,0.1);"
        "border-radius: 8px;"
        "padding: 10px;"
        "color: #bdc3c7;"
        "font-size: 13px;"
        );

    // Bouton
    QPushButton *btnClose = new QPushButton("Fermer");
    btnClose->setCursor(Qt::PointingHandCursor);
    connect(btnClose, &QPushButton::clicked, dialog, &QDialog::accept);

    // Assemblage
    layout->addWidget(icon);
    layout->addWidget(title);
    layout->addWidget(countLabel);
    layout->addWidget(subtitle);
    layout->addWidget(info);
    layout->addWidget(btnClose, 0, Qt::AlignCenter);

    dialog->exec();
    delete dialog;

    qDebug() << "Fenêtre clients affichée:" << nbClients << "clients";
    QTimer::singleShot(500, this, []() {
        qDebug() << "Prêt pour nouveau clique";
    });
}
////////////////////////////////////////////////////
int opticstor::getMaxAge()
{
    qDebug() << "=== DÉBUT getMaxAge() ===";

    QSqlQuery query;
    int maxAge = 0;

    // 1. Essayer différentes requêtes
    QStringList requetesPossibles = {
        "SELECT MAX(age) FROM client",
        "SELECT MAX(Age) FROM client",
        "SELECT MAX(AGE) FROM client",
        "SELECT age FROM client ORDER BY age DESC LIMIT 1",
        "SELECT * FROM client WHERE age = (SELECT MAX(age) FROM client)"
    };

    for (const QString& requete : requetesPossibles) {
        qDebug() << "Essai requête:" << requete;

        if (query.exec(requete)) {
            if (query.next()) {
                maxAge = query.value(0).toInt();
                qDebug() << "  → Résultat:" << maxAge;

                if (maxAge > 0) {
                    qDebug() << "  ✓ Âge max trouvé:" << maxAge;
                    return maxAge;
                }
            } else {
                qDebug() << "  → Aucun résultat";
            }
        } else {
            qDebug() << "  → Erreur:" << query.lastError().text();
        }
    }

    // 2. Voir ce qu'il y a dans la table client
    qDebug() << "\n=== CONTENU TABLE client ===";
    if (query.exec("SELECT id, nom, prenom, age FROM client LIMIT 10")) {
        while (query.next()) {
            int id = query.value(0).toInt();
            QString nom = query.value(1).toString();
            QString prenom = query.value(2).toString();
            int age = query.value(3).toInt();
            qDebug() << "Client:" << id << nom << prenom << "Age:" << age;
        }
    } else {
        qDebug() << "Erreur lecture table:" << query.lastError().text();
    }

    // 3. Voir la structure de la table
    qDebug() << "\n=== STRUCTURE TABLE client ===";
    if (query.exec("PRAGMA table_info(client)")) {
        while (query.next()) {
            QString colName = query.value(1).toString();
            QString colType = query.value(2).toString();
            qDebug() << "Colonne:" << colName << "Type:" << colType;
        }
    }

    qDebug() << "=== FIN getMaxAge() ===";

    // Si on n'a rien trouvé, retourner une valeur de test
    if (maxAge <= 0) {
        qDebug() << "⚠ Aucun âge trouvé, retourne valeur test: 34";
        return 34; // Valeur de test
    }

    return maxAge;
}
////////////////////////////////////////////
int opticstor::getClientCount()
{
    QSqlQuery query;
    int count = 0;

    if (query.exec("SELECT COUNT(*) FROM client") && query.next()) {
        count = query.value(0).toInt();
        qDebug() << "Nombre clients dans base:" << count;
    } else {
        qDebug() << "Erreur count clients:" << query.lastError().text();
    }

    return count;
}
////////////////////////////////////////////
// opticstor.cpp
void opticstor::on_boutonTestArduino_clicked()
{
    qDebug() << "=== TEST ARDUINO (3 BOUTONS) ===";

    if (A.getserial() && A.getserial()->isOpen()) {
        QString message = QString("✅ Arduino connectée!\n\n"
                                  "📌 Port: %1\n"
                                  "⚡ Baud rate: %2\n\n"
                                  "🎮 Boutons disponibles:\n"
                                  "• Pin 2 → Âge maximum des clients\n"
                                  "• Pin 4 → Nombre de clients\n"
                                  "• Pin 7 → Nombre de tests")
                              .arg(A.getarduino_port_name())
                              .arg(A.getserial()->baudRate());

        QMessageBox::information(this, "Test Arduino", message);

    } else {
        QMessageBox::warning(this, "Test Arduino",
                             "❌ Arduino non connectée!\n\n"
                             "Vérifiez:\n"
                             "1. Câble USB\n"
                             "2. Alimentation Arduino\n"
                             "3. Port COM dans les paramètres");
    }
}
///////////////////////////////
void opticstor::showTestCountWindow(int nbTests)
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("🔬 Nombre de Tests");
    dialog->setFixedSize(450, 350);

    // Style scientifique/professionnel
    dialog->setStyleSheet(
        "QDialog {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "                                stop:0 #1a2980, stop:1 #26d0ce);"
        "    border-radius: 12px;"
        "    border: 3px solid #2c3e50;"
        "}"
        "QLabel { color: white; font-family: 'Segoe UI', Arial; }"
        "QPushButton {"
        "    background-color: #9b59b6;"
        "    color: white;"
        "    padding: 12px 30px;"
        "    border-radius: 8px;"
        "    font-weight: bold;"
        "    border: none;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover { background-color: #8e44ad; }"
        );

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->setSpacing(15);
    layout->setContentsMargins(25, 25, 25, 25);

    // Icône
    QLabel *icon = new QLabel("🔬");
    icon->setAlignment(Qt::AlignCenter);
    icon->setStyleSheet("font-size: 48px;");

    // Titre
    QLabel *title = new QLabel("TESTS RÉALISÉS");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "font-size: 22px;"
        "font-weight: bold;"
        "color: #f1c40f;"
        "text-transform: uppercase;"
        "margin-bottom: 10px;"
        );

    // Cadre pour le nombre
    QFrame *frame = new QFrame();
    frame->setStyleSheet(
        "QFrame {"
        "    background-color: rgba(255, 255, 255, 0.15);"
        "    border-radius: 10px;"
        "    border: 2px solid rgba(255, 255, 255, 0.3);"
        "}"
        );

    QVBoxLayout *frameLayout = new QVBoxLayout(frame);
    frameLayout->setContentsMargins(20, 20, 20, 20);

    QLabel *countLabel = new QLabel(QString::number(nbTests));
    countLabel->setAlignment(Qt::AlignCenter);
    countLabel->setStyleSheet(
        "font-size: 72px;"
        "font-weight: bold;"
        "color: white;"
        "text-shadow: 2px 2px 4px rgba(0,0,0,0.5);"
        );

    QLabel *unitLabel = new QLabel("tests");
    unitLabel->setAlignment(Qt::AlignCenter);
    unitLabel->setStyleSheet(
        "font-size: 18px;"
        "color: #ecf0f1;"
        "font-weight: bold;"
        );

    frameLayout->addWidget(countLabel);
    frameLayout->addWidget(unitLabel);

    // Information
    QLabel *info = new QLabel(
        "📊 Ce nombre représente le total des tests\n"
        "réalisés depuis le début de l'activité.\n"
        "Données actualisées en temps réel."
        );
    info->setAlignment(Qt::AlignCenter);
    info->setWordWrap(true);
    info->setStyleSheet(
        "background-color: rgba(0, 0, 0, 0.2);"
        "border-radius: 8px;"
        "padding: 12px;"
        "color: #ecf0f1;"
        "font-size: 13px;"
        "margin-top: 10px;"
        );

    // Bouton Fermer
    QPushButton *btnClose = new QPushButton("Fermer la fenêtre");
    btnClose->setCursor(Qt::PointingHandCursor);
    btnClose->setFixedWidth(200);
    connect(btnClose, &QPushButton::clicked, dialog, &QDialog::accept);

    // Assemblage
    layout->addWidget(icon);
    layout->addWidget(title);
    layout->addWidget(frame);
    layout->addWidget(info);
    layout->addWidget(btnClose, 0, Qt::AlignCenter);

    dialog->exec();
    delete dialog;

    qDebug() << "Fenêtre tests affichée:" << nbTests << "tests";
    QTimer::singleShot(500, this, []() {
        qDebug() << "Prêt pour nouveau clique";
    });
}
///////////////////////////////////////
int opticstor::getTestCount()
{
    QSqlQuery query;
    int count = 0;

    // Simple comptage de tous les enregistrements dans la table TEST_ORDONNANCE
    if (query.exec("SELECT COUNT(*) FROM TEST_ORDONNANCE")) {
        if (query.next()) {
            count = query.value(0).toInt();
            qDebug() << "✅ Nombre de tests d'ordonnance:" << count;
        }
    } else {
        qDebug() << "❌ Erreur:" << query.lastError().text();
    }

    return count;
}
// FONCTION PLEINE POUR AFFICHER LE STOCK
    void opticstor::afficherStockSelonStatut(const QString& statut, int nombreProduits)
{
    qDebug() << "=== STATUT REÇU:" << statut << "===";

    QTableWidget *tableStock = ui->tableStock_2;
    if (!tableStock) {
        qDebug() << "ERREUR: tableStock est NULL!";
        return;
    }

    // Vider le tableau
    tableStock->setRowCount(0);

    QSqlQuery query;
    QString requete;
    int count = 0;

    // TES NOUVELLES RÈGLES:
    if (statut == "RUPTURE") {
        requete = "SELECT CODE_PRODUIT, MARQUE, TYPE, QUANTITE_STOCK, PRIX FROM PRODUIT WHERE QUANTITE_STOCK < 3";
    }
    else if (statut == "FAIBLE") {
        requete = "SELECT CODE_PRODUIT, MARQUE, TYPE, QUANTITE_STOCK, PRIX FROM PRODUIT WHERE QUANTITE_STOCK BETWEEN 3 AND 10";
    }
    else if (statut == "NORMAL") {
        requete = "SELECT CODE_PRODUIT, MARQUE, TYPE, QUANTITE_STOCK, PRIX FROM PRODUIT WHERE QUANTITE_STOCK > 10";
    }
    else if (statut == "TOUS") {
        requete = "SELECT CODE_PRODUIT, MARQUE, TYPE, QUANTITE_STOCK, PRIX FROM PRODUIT";
    }

    if (query.exec(requete)) {
        int row = 0;
        while (query.next()) {
            tableStock->insertRow(row);

            int code = query.value(0).toInt();
            QString marque = query.value(1).toString();
            QString type = query.value(2).toString();
            int quantite = query.value(3).toInt();
            double prix = query.value(4).toDouble();

            QString sexe = determinerSexe(marque);

            // Ajouter les données au tableau
            tableStock->setItem(row, 0, new QTableWidgetItem(QString::number(code)));
            tableStock->setItem(row, 1, new QTableWidgetItem(marque));
            tableStock->setItem(row, 2, new QTableWidgetItem(sexe));
            tableStock->setItem(row, 3, new QTableWidgetItem(type));
            tableStock->setItem(row, 4, new QTableWidgetItem(QString::number(quantite)));
            tableStock->setItem(row, 5, new QTableWidgetItem(QString::number(prix, 'f', 2) + " DT"));

            // 🎨 COULEURS AVEC TES NOUVELLES RÈGLES
            if (quantite < 3) {
                // ROUGE FONCÉ pour rupture (< 3)
                for (int col = 0; col < 6; col++) {
                    if (tableStock->item(row, col)) {
                        tableStock->item(row, col)->setBackground(QColor(255, 150, 150));
                        tableStock->item(row, col)->setForeground(QColor(0, 0, 0));
                    }
                }
            } else if (quantite <= 10) {
                // ORANGE FONCÉ pour faible (3-10)
                for (int col = 0; col < 6; col++) {
                    if (tableStock->item(row, col)) {
                        tableStock->item(row, col)->setBackground(QColor(255, 200, 100));
                        tableStock->item(row, col)->setForeground(QColor(0, 0, 0));
                    }
                }
            } else {
                // VERT FONCÉ pour normal (11+)
                for (int col = 0; col < 6; col++) {
                    if (tableStock->item(row, col)) {
                        tableStock->item(row, col)->setBackground(QColor(150, 255, 150));
                        tableStock->item(row, col)->setForeground(QColor(0, 0, 0));
                    }
                }
            }

            row++;
            count++;
        }

        qDebug() << row << "lignes ajoutées pour statut:" << statut;
        tableStock->resizeColumnsToContents();

        // Envoyer le message à l'Arduino pour l'écran LCD
        envoyerMessageArduino(statut, count);

    } else {
        qDebug() << "Erreur requête:" << query.lastError().text();
    }
}

// Fonction pour envoyer un message à l'Arduino avec les données RÉELLES de la base
void opticstor::envoyerMessageArduino(const QString& statut, int nombreProduits)
{
    // Vérifier si l'Arduino est connecté
    if (!A.getserial() || !A.getserial()->isOpen()) {
        qDebug() << "Arduino non connecté!";
        return;
    }

    // DEBUG CONFIRMATION
    qDebug() << "=== CONFIRMATION DONNÉES BASE ===";
    qDebug() << "Statut:" << statut;
    qDebug() << "Nombre produits (de la base):" << nombreProduits;
    qDebug() << "Source: Requête SQL exécutée avec succès";

    QString message;

    if (statut == "RUPTURE") {
        message = "RUPTURE: " + QString::number(nombreProduits);
        qDebug() << "Message formé: RUPTURE: " << nombreProduits << "produits";
    }
    else if (statut == "FAIBLE") {
        message = "FAIBLE: " + QString::number(nombreProduits);
        qDebug() << "Message formé: FAIBLE: " << nombreProduits << "produits";
    }
    else if (statut == "NORMAL") {
        message = "NORMAL: " + QString::number(nombreProduits);
        qDebug() << "Message formé: NORMAL: " << nombreProduits << "produits";
    }
    else if (statut == "TOUS") {
        message = "TOTAL: " + QString::number(nombreProduits);
        qDebug() << "Message formé: TOTAL: " << nombreProduits << "produits";
    }

    // AJOUTER UN SAUT DE LIGNE À LA FIN
    message += "\n";

    // Envoie avec TA méthode write_to_arduino
    A.write_to_arduino(message.toUtf8());

    qDebug() << "✅ Message envoyé à Arduino avec données RÉELLES de la base";
    qDebug() << "📊 Données: " << message.trimmed();
}


