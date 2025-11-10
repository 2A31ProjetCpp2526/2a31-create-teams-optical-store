#ifndef OPTICSTOR_H
#define OPTICSTOR_H

#include <QMainWindow>
#include <QMessageBox>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QBrush>

#include "client.h"
#include "produit.h"
#include "fournisseur.h"
#include "employes.h"   // ✅ using your plural class name

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

private:
    Ui::opticstor *ui;
    Client tmpClient;
    Produit tmpProduit;
    Fournisseur tmpFournisseur;
    employe tmpEmploye;   // ✅ using the plural class name

private slots:
    // Slots existants
    void onSubmitClicked();
    void onCancelClicked();
    void onDeleteClicked();
    void onEditClicked();
    void onSubmitClickedd();
    void onCancelClickedd();
    void onDeleteClickedd();
    void onEditClickedd();
    void onSearchClicked();
    void onSortChanged(int index);
    void onGestCommande();
    void onGest();
    void onprod();
    void onemp();   // ✅ existing, used to show employee section
    void onfourn();

    // Slots pour la gestion des clients
    void on_ajouter_clicked();
    void on_supprimer_clicked();
    void on_modifier_clicked();

    // Slots pour la gestion des produits
    void on_ajouter_produit_clicked();
    void on_supprimer_produit_clicked();
    void on_modifier_produit_clicked();
    void on_rechercher_produit_clicked();
    void on_actualiser_produit_clicked();

    // Slots pour la gestion des fournisseurs
    void on_ajouter_fournisseur_clicked();
    void on_supprimer_fournisseur_clicked();
    void on_modifier_fournisseur_clicked();
    void on_rechercher_fournisseur_clicked();
    void on_actualiser_fournisseur_clicked();



    // ✅ Fonction utilitaire pour charger les employés dans la table
    void chargerTableEmployes();

    // Fonction utilitaire pour charger les fournisseurs
    void chargerTableFournisseurs();

    // focntions pour employes
    void on_ajouter_employe_clicked();
    void on_supprimer_employe_clicked();
    void on_modifier_employe_clicked();


};

#endif // OPTICSTOR_H
