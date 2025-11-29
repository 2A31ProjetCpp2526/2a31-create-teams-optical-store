#ifndef OPTICSTOR_H
#define OPTICSTOR_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include "client.h"
#include "produit.h"
#include "fournisseur.h"
#include "commande.h"
#include "employe.h"
#include <QChartView>
#include <QPieSeries>
#include <QTableWidget>          // ← AJOUTER
#include <QTableWidgetItem>      // ← AJOUTER
#include <QHeaderView>           // ← AJOUTER
#include <QTextDocument>
#include <QTextCursor>
#include <QPrinter>
#include <QFileDialog>
#include <QDesktopServices>

#include <QMessageBox>
#include <QSqlQuery>


using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
class opticstor;
}
QT_END_NAMESPACE

class opticstor : public QMainWindow
{
    Q_OBJECT

public:
    opticstor(QWidget *parent = nullptr);
    ~opticstor();



private slots:
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

    // Gestion des commandes
    void on_ajouter_commande_clicked();
    void on_supprimer_commande_clicked();
    void on_modifier_commande_clicked();
    void on_rechercher_commande_clicked();
    void on_actualiser_commande_clicked();
    void on_trier_date_commande_clicked();
    void on_trier_prix_commande_clicked();

    // Gestion des employés
    void on_ajouter_employe_clicked();
    void on_supprimer_employe_clicked();
    void on_modifier_employe_clicked();
    void on_rechercher_employe_clicked();
    void on_actualiser_employe_clicked();

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

    // Fonctions existantes
    void onSubmitClicked();
    void onCancelClicked();
    void onDeleteClicked();
    void onSubmitClickedd();
    void onCancelClickedd();
    void onDeleteClickedd();
    void onSearchClicked();
    void onEditClicked();
    void onEditClickedd();
    void onSortChanged(int index);
private:
    Ui::opticstor *ui;
    Client tmpClient;
    Produit tmpProduit;
    Fournisseur tmpFournisseur;
    Commande tmpCommande;
    employe tmpEmploye;
    QChart *chart;
    QChartView *chartView;

    QString genererHTMLClients(const QList<Client>& clients);

    void remplirTableCommandes();
    void remplirTableClients();
    void remplirTableEmployes();
    void remplirTableTestsOrdonnance();
    void afficherStatistiquesAvecDurees();
    void setupTriComboBox();
    void setupTendances();
    void peuplerComboBox();
    void chargerTableFournisseurs();

    // MÉTHODES PRODUITS ET STATISTIQUES
    QString genererHTMLProduits(const QList<Produit>& produits);
    void afficherStockSelonStatut(const QString& statut);
    QString determinerSexe(const QString& marque);
    void calculerEtAfficherTendances();
    void analyserVentesParModele();
    void analyserVentesParForme();// Nouvelle méthode pour afficher les stats avec durées
};

#endif // OPTICSTOR_H
