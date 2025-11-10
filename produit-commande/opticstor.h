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
    QString type;
    QString marque;
    //int client;
    double prixtotal;

private slots:
    // Slots existants
   /* void onSubmitClicked();
    void onCancelClicked();
    void onDeleteClicked();
    void onEditClicked();
    void onSubmitClickedd();
    void onCancelClickedd();
    void onDeleteClickedd();
    void onEditClickedd();
    void onSearchClicked();
    void onSortChanged(int index);*/
    void onGestCommande();
    void onGest();
    void onprod();
    void onemp();
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
    //slots pour la gestion des commandes
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

};
#endif // OPTICSTOR_H
