#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ajouter_clicked();
    void on_afficher_clicked();
    void on_supprimer_clicked();
    void on_modifier_clicked();
    void on_rechercher_clicked();

    // ← connect this to your "Afficher" button

    void on_trie_clicked();

    void on_statistique_clicked();

    void on_pdf_clicked();
    void sendMessage();            // envoyer le message
    void onResponse(QNetworkReply* reply);
    // Check-in / Check-out
    void on_btn_checkin_clicked();
    void on_btn_checkout_clicked();
    void afficherTablePresence();


private:
    Ui::MainWindow *ui;
    void afficherTableemployes(); // function to load tableView
     void afficherPerformancePresence(int totalWorkingDays);



};

#endif // MAINWINDOW_H
