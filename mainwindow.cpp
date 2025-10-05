#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Feuille de style pour transformer les RadioButton en étoiles
    QString starStyle = R"(
        QRadioButton::indicator {
            width: 32px;
            height: 32px;
        }
        QRadioButton::indicator::unchecked {
            image: url(:/icons/star_empty.png);
        }
        QRadioButton::indicator::checked {
            image: url(:/icons/star_full.png);
        }
    )";

    // Appliquer le style aux 5 étoiles
    ui->star1->setStyleSheet(starStyle);
    ui->star2->setStyleSheet(starStyle);
    ui->star3->setStyleSheet(starStyle);
    ui->star4->setStyleSheet(starStyle);
    ui->star5->setStyleSheet(starStyle);

    // Connecter chaque étoile à la fonction de mise à jour
    connect(ui->star1, &QRadioButton::toggled, this, [=](bool checked){ if(checked) setRating(1); });
    connect(ui->star2, &QRadioButton::toggled, this, [=](bool checked){ if(checked) setRating(2); });
    connect(ui->star3, &QRadioButton::toggled, this, [=](bool checked){ if(checked) setRating(3); });
    connect(ui->star4, &QRadioButton::toggled, this, [=](bool checked){ if(checked) setRating(4); });
    connect(ui->star5, &QRadioButton::toggled, this, [=](bool checked){ if(checked) setRating(5); });

    // Initialisation : pas d’étoiles sélectionnées
    setRating(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Fonction pour mettre à jour l’affichage des étoiles
void MainWindow::setRating(int rating)
{
    QList<QRadioButton*> stars = {ui->star1, ui->star2, ui->star3, ui->star4, ui->star5};

    for (int i = 0; i < stars.size(); i++) {
        if (i < rating)
            stars[i]->setChecked(true);
        else
            stars[i]->setChecked(false);
    }
}
