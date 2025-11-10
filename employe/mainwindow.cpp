#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employe.h"
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// =====================================
// === Ajouter un employé ==============
// =====================================
void MainWindow::on_ajouter_clicked()
{
    QString nom = ui->nom->text();
    QString prenom = ui->prenom->text();
    QString type = ui->type->text();
    float salaire = ui->salaire->text().toFloat();
    QDate date_emb = ui->date->date();

    employe e(nom, prenom, salaire, date_emb, type);

    if (e.ajouter()) {
        QMessageBox::information(this, "Succès", "Employé ajouté avec succès !");
        afficherTableEmployes(); // refresh after adding
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout !");
    }
}

void MainWindow::on_supprimer_clicked()
{
    int id = ui->id_supp->text().toInt();  // take the ID from the input

    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID valide !");
        return;
    }

    // Optional: confirm before deleting
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmer la suppression",
                                  "Voulez-vous vraiment supprimer cet employé ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No)
        return;

    employe e;
    if (e.supprimer(id)) {
        QMessageBox::information(this, "Succès", "L'employé a été supprimé avec succès !");
        afficherTableEmployes();  // refresh tableView if you display employees
        ui->id_supp->clear();     // clear input after deleting
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression. Vérifiez l'ID !");
    }
}


// =====================================
// === Afficher les employés ===========
// =====================================
void MainWindow::afficherTableEmployes()
{
    employe empTmp;
    QSqlQueryModel* model = empTmp.afficher();

    if (model) {
        ui->tableView->setModel(model);
        ui->tableView->setStyleSheet(
            "QTableView {"
            "   background-color: #f5f5f5;"
            "   border: 1px solid #ccc;"
            "   gridline-color: #ccc;"
            "   selection-background-color: #A3C1DA;"
            "   selection-color: white;"
            "   font: bold 12px;"
            "}"
            "QHeaderView::section {"
            "   background-color: #2e3d4e;"
            "   color: white;"
            "   padding: 5px;"
            "   border: 1px solid #aaa;"
            "}"
            );
        ui->tableView->resizeColumnsToContents();
        ui->tableView->horizontalHeader()->setStretchLastSection(true);
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de charger la liste des employés.");
    }
}
void MainWindow::on_modifier_clicked()
{
    int id = ui->id_mod->text().toInt();
    QString nom = ui->nom_mod->text().trimmed();
    QString prenom = ui->prenom_mod->text().trimmed();
    QString type = ui->type_mod->text().trimmed();
    QDate dateEmb = ui->date_mod->date();

    // --- Validation ---
    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID valide !");
        return;
    }

    if (nom.isEmpty() || prenom.isEmpty() || type.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs !");
        return;
    }

    // --- Load the existing employee and update fields ---
    employe e;
    if (!e.recuperer(id)) {
        QMessageBox::critical(this, "Erreur", "Aucun employé trouvé avec cet ID !");
        return;
    }

    e.setNOM(nom);
    e.setPRENOM(prenom);
    e.setDATE_EMB(dateEmb);
    e.setTYPE(type);


    if (e.modifier()) {
        QMessageBox::information(this, "Succès", "Employé modifié avec succès !");
        afficherTableEmployes();  // Refresh the table after update
    } else {
        QMessageBox::critical(this, "Erreur", "La modification a échoué !");
    }
}

// =====================================
// === Bouton Afficher =================
// =====================================
void MainWindow::on_afficher_clicked()
{
    afficherTableEmployes();
}
