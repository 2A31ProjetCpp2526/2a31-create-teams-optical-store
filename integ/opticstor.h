#ifndef OPTICSTOR_H
#define OPTICSTOR_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include "client.h"
#include "produit.h"
#include "fournisseur.h"
//#include "commande.h"
#include "employe.h"
#include "metier.h"
#include "ravitaillementdialog.h"
#include "feedbackdialog.h"
#include <QChartView>
#include <QPieSeries>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QTextDocument>
#include <QTextCursor>
#include <QPrinter>
#include <QFileDialog>
#include <QDesktopServices>
#include "camerawidget.h"
#include "cameraworker.h"
#include "ui_opticstor.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QNetworkReply>
#include "arduino.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include <QTimer>                   // Add this
#include <QJsonObject>              // Add this if using JSON
#include <QJsonArray>               // Add this if using JSON
#include <QJsonDocument>            // Add this if using JSON
#include <QNetworkAccessManager>    // Add this if using network
#include <QNetworkReply>            // Add this if using network
//using namespace std;

class Ui_opticstor;

class opticstor : public QMainWindow
{
    Q_OBJECT

public:
    opticstor(QWidget *parent = nullptr);
    ~opticstor();

    // PUBLIC METHODS FROM FIRST FILE:
    int getMaxAge();           // ← ADD THIS
    int getAverageAge();       // ← ADD THIS
    int getClientCount();      // ← ADD THIS
    int getTestCount();        // ← ADD THIS

private slots:
    // ARDUINO SLOTS FROM FIRST FILE:
    void update_label();                         // ← ADD THIS
    void showAgeMaxWindow(int ageMax);           // ← ADD THIS
    void showClientCountWindow(int nbClients);   // ← ADD THIS
    void showTestCountWindow(int nbTests);       // ← ADD THIS
    void on_boutonTestArduino_clicked();         // ← ADD THIS

    // Gestion des clients
    void on_ajouter_clicked();
    void on_supprimer_clicked();
    void on_modifier_clicked();
    void on_tableView_clicked(const QModelIndex &index);
    void on_rechercher_client_clicked();
    void on_statistique_age_clicked();
    void on_exporter_pdf_clicked();
    void on_combo_tri_currentIndexChanged(const QString &text);

    // Gestion des produits
    void on_ajouter_produit_clicked();
    void on_supprimer_produit_clicked();
    void on_modifier_produit_clicked();
    void on_rechercher_produit_clicked();
    void on_actualiser_produit_clicked();
    void on_exporter_pdf_produits_clicked();
    void on_combo_tri_produit_currentIndexChanged(int index);

    // SLOTS STOCK ET STATISTIQUES
    void on_btnRupture_clicked();
    void on_btnFaible_clicked();
    void on_btnNormal_clicked();
    void on_btnTous_clicked();
    void on_statistique_stock_clicked();
    void on_tendances_btn_clicked();

    // Gestion des fournisseurs
    void on_ajouter_fournisseur_clicked();
    void on_supprimer_fournisseur_clicked();
    void on_modifier_fournisseur_clicked();
    void on_rechercher_fournisseur_clicked();
    void on_actualiser_fournisseur_clicked();

    // Ravitaillement et Feedback fournisseur
    void on_ajoutravitalement_clicked();
    void on_feedback_clicked();
    void on_triparnom_clicked();
    void on_exportexel_clicked();
    void on_stat_fournisseur_clicked();
    void chargerRavitaillementTable();
    void showRavContextMenu(const QPoint &pos);
    void chargerTableFournisseurs();

    // Gestion des commandes
    void ajoutcommande();
    void supprimcommande();
    void affichecommande();
    void updatecommande();
    void autrecmd();
    void loadtypesmarquesComboBox();
    void loadclientComboBox();
    void loademp();
    bool commandExists(int);
    void updmen();
    void rechcomd();
    void on_showChartButton_clicked();
    void createPDFReceipt(const QString &pdfPath,
                          const QString &customerName,
                          const QVector<QPair<QString,double>> &productList,
                          double totalPrice);
    void generatePDFForCommande(const QString &commandeId);
    void on_generatePDFButton_clicked();
    void on_commandSelected(int row, int column);

    // Gestion des employés - KEEP BOTH SETS (or choose one)
    // From first file:
    /*void on_ajouter_employe_clicked();
    void on_supprimer_employe_clicked();
    void on_modifier_employe_clicked();
    void on_rechercher_employe_clicked();
    void on_actualiser_employe_clicked();*/

    // From second file (different names):
    void on_ajouter_3_clicked();
    void on_afficher_3_clicked();
    void on_supprimer_3_clicked();
    void on_modifier_3_clicked();
    void on_rechercher_2_clicked();
    void on_trie_2_clicked();
    void on_statistique_2_clicked();
    void on_pdf_2_clicked();
    void sendMessage();
    void onResponse(QNetworkReply* reply);
    void on_btn_checkin_clicked();
    void on_btn_checkout_clicked();
    void afficherTablePresence();

    // Gestion des tests d'ordonnance
    void on_ajouter_test_ordonnance_clicked();
    void on_supprimer_test_ordonnance_clicked();
    void on_afficher_tests_ordonnance_clicked();
    void on_rechercher_tests_client_clicked();

    // Gestion des feedbacks
    void on_ajouter_feedback_clicked();
    void on_annuler_feedback_clicked();
    void on_afficher_feedbacks_clicked();

    // statut
    void on_changer_statut_clicked();
    void on_rechercher_statut_clicked();
    void on_statistiques_statut_clicked();

    // Navigation
    void onGestCommande();
    void onGest();
    void onprod();
    void onemp();
    void onfourn();
    void onFeedbackPage();

private:
    Ui_opticstor *ui;
    Client tmpClient;
    Produit tmpProduit;
    Fournisseur tmpFournisseur;
    employe tmpEmploye;
    Metier tmpMetier;
    QChart *chart;
    QChartView *chartView;
    QTableWidget *ravitTable;
    QString type;
    QString marque;
    double prixtotal;
    int rech;
    cameraworker *worker;

    // ARDUINO FROM FIRST FILE:
    Arduino A;                 // ← ADD THIS
    QByteArray data;           // ← ADD THIS

    QString genererHTMLClients(const QList<Client>& clients);
    void remplirTableClients();
    void remplirTableEmployes();
    void remplirTableTestsOrdonnance();
    void afficherStatistiquesAvecDurees();
    void setupTriComboBox();
    void setupTendances();
    void peuplerComboBox();

    // MÉTHODES PRODUITS ET STATISTIQUES
    QString genererHTMLProduits(const QList<Produit>& produits);
    void afficherStockSelonStatut(const QString& statut);
    void afficherStockSelonStatut(const QString& statut, int nombreProduits);  // ← OVERLOAD ADD
    QString determinerSexe(const QString& marque);
    void calculerEtAfficherTendances();
    void analyserVentesParModele();
    void analyserVentesParForme();

    // ARDUINO METHOD FROM FIRST FILE:
    void envoyerMessageArduino(const QString& statut, int nombreProduits);  // ← ADD THIS

    // FROM SECOND FILE:
    void afficherTableemployes();
    void afficherPerformancePresence(int totalWorkingDays);
};

#endif // OPTICSTOR_H
