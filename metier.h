#ifndef METIER_H
#define METIER_H

#include <QString>
#include <QSqlQueryModel>

class Metier
{
public:
    Metier();

    // ===== RAVITAILLEMENT =====
    // Ajouter un ravitaillement (sans produit associé obligatoirement)
    bool ajouterRavitaillementStandalone(int codeProduit, int idFournisseur, int quantite, 
                                          const QString& message, int note, QString& erreur);
    
    // Afficher tous les ravitaillements
    QSqlQueryModel* afficherRavitaillement();
    
    // Modifier un ravitaillement
    bool modifierRavitaillement(int id, int codeProduit, int idFournisseur, int quantite,
                                 const QString& message, int note, QString& erreur);
    
    // Supprimer un ravitaillement
    bool supprimerRavitaillement(int id, QString& erreur);

    // ===== FEEDBACK =====
    // Envoyer un feedback pour un fournisseur
    bool envoyerFeedback(int idFournisseur, const QString& message, int note, QString& erreur);

    // ===== STATISTIQUES =====
    // Statistiques par fournisseur (si id=-1, tous les fournisseurs)
    QSqlQueryModel* statsFournisseur(int idFournisseur);
    
    // Statistiques groupées par type de fournisseur
    QSqlQueryModel* statsParType();
};

#endif // METIER_H
