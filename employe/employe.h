#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>

class employe
{
private:
    int ID_EMP;
    QString NOM;
    QString PRENOM;
    float SALAIRE;
    QDate DATE_EMB;
    QString TYPE;

public:
    employe();
    employe( QString nom, QString prenom, float salaire, QDate date_emb, QString type);


    int getID_EMP() const { return ID_EMP; }
    QString getNOM() const { return NOM; }
    QString getPRENOM() const { return PRENOM; }
    float getSALAIRE() const { return SALAIRE; }
    QDate getDATE_EMB() const { return DATE_EMB; }
    QString getTYPE() const { return TYPE; }


    void setID_EMP(int id) { ID_EMP = id; }
    void setNOM(QString nom) { NOM = nom; }
    void setPRENOM(QString prenom) { PRENOM = prenom; }
    void setSALAIRE(float salaire) { SALAIRE = salaire; }
    void setDATE_EMB(QDate date) { DATE_EMB = date; }
    void setTYPE(QString type) { TYPE = type; }


    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int id);
    bool modifier();
    bool recuperer(int id);
};

#endif // EMPLOYE_H
