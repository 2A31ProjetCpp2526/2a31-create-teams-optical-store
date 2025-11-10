#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Client
{
private:
    int id_client;
    QString nom, prenom, email, telephone;

public:
    Client();
    Client(int, QString, QString, QString, QString);

    int getId();
    QString getNom();
    QString getPrenom();
    QString getEmail();
    QString getTelephone();

    // Méthodes de validation
    static bool estTelephoneValide(const QString& telephone);
    static bool estEmailValide(const QString& email);

    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool modifier();
};

#endif // CLIENT_H
