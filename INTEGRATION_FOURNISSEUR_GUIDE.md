# Guide d'intégration du module Fournisseur

## État actuel de l'intégration

### ✅ Complété :
1. ✅ Import de `fournisseur.h` dans `opticstor.h`
2. ✅ Ajout de l'instance `Fournisseur tmpFournisseur` dans `opticstor.h`
3. ✅ Ajout des slots pour les fournisseurs dans `opticstor.h`
4. ✅ Implémentation des 5 fonctions de gestion des fournisseurs dans `opticstor.cpp`:
   - `on_ajouter_fournisseur_clicked()`
   - `on_supprimer_fournisseur_clicked()`
   - `on_modifier_fournisseur_clicked()`
   - `on_rechercher_fournisseur_clicked()`
   - `on_actualiser_fournisseur_clicked()`

## ⏳ À faire : Ajouter les widgets UI

Les fonctions attendront les widgets suivants dans le fichier `opticstor.ui` :

### Pour l'ajout de fournisseur :
- `lineEdit_nom_fournisseur` - Champ pour le Nom
- `lineEdit_prenom_fournisseur` - Champ pour le Prénom
- `lineEdit_tel_fournisseur` - Champ pour le Téléphone
- `lineEdit_email_fournisseur` - Champ pour l'Email
- `comboBox_type_fournisseur` - ComboBox pour le Type
- `table_fournisseurs` - QTableView pour afficher les fournisseurs

### Pour la suppression de fournisseur :
- `lineEdit_id_fournisseur` - Champ pour l'ID à supprimer

### Pour la modification de fournisseur :
- `lineEdit_id_modifier_fournisseur` - Champ pour l'ID à modifier
- `lineEdit_nom_modifier_fournisseur` - Champ pour le Nom
- `lineEdit_prenom_modifier_fournisseur` - Champ pour le Prénom
- `lineEdit_tel_modifier_fournisseur` - Champ pour le Téléphone
- `lineEdit_email_modifier_fournisseur` - Champ pour l'Email
- `comboBox_type_modifier_fournisseur` - ComboBox pour le Type

### Pour la recherche de fournisseur :
- `lineEdit_recherche_fournisseur` - Champ pour le mot clé de recherche
- `comboBox_recherche_fournisseur` - ComboBox pour choisir la colonne de recherche

### Boutons :
- `ajouter_fournisseur` - QPushButton pour ajouter
- `supprimer_fournisseur` - QPushButton pour supprimer
- `modifier_fournisseur` - QPushButton pour modifier
- `rechercher_fournisseur` - QPushButton pour rechercher
- `actualiser_fournisseur` - QPushButton pour actualiser

## Prochaines étapes dans le code

Une fois les widgets ajoutés à l'UI, connecter-les dans le constructeur d'`opticstor` en ajoutant :

```cpp
// Dans opticstor::opticstor - Initialiser la table des fournisseurs
ui->table_fournisseurs->setModel(tmpFournisseur.afficher());

// Connexions pour les fournisseurs
connect(ui->ajouter_fournisseur, &QPushButton::clicked, this, &opticstor::on_ajouter_fournisseur_clicked);
connect(ui->supprimer_fournisseur, &QPushButton::clicked, this, &opticstor::on_supprimer_fournisseur_clicked);
connect(ui->modifier_fournisseur, &QPushButton::clicked, this, &opticstor::on_modifier_fournisseur_clicked);
connect(ui->rechercher_fournisseur, &QPushButton::clicked, this, &opticstor::on_rechercher_fournisseur_clicked);
connect(ui->actualiser_fournisseur, &QPushButton::clicked, this, &opticstor::on_actualiser_fournisseur_clicked);

// Validateurs pour les champs du fournisseur
ui->lineEdit_tel_fournisseur->setValidator(new QIntValidator(0, 99999999, this));
ui->lineEdit_id_fournisseur->setValidator(new QIntValidator(0, 999999, this));
ui->lineEdit_id_modifier_fournisseur->setValidator(new QIntValidator(0, 999999, this));
```

## Structure CRUD des fournisseurs

Le module est basé sur les opérations CRUD complètes :
- **Create** (Ajouter) : Ajoute un nouveau fournisseur avec validation
- **Read** (Afficher) : Affiche tous les fournisseurs dans une table
- **Update** (Modifier) : Modifie un fournisseur existant
- **Delete** (Supprimer) : Supprime un fournisseur avec confirmation

Plus une fonction de recherche pour filtrer les fournisseurs.

## Classe Fournisseur utilisée

La classe `Fournisseur` gère :
- **Propriétés** : ID, Nom, Prénom, Téléphone, Email, Type
- **Méthodes CRUD** : ajouter(), afficher(), modifier(), supprimer(), rechercher()
- **Validation** : Email (doit contenir @), Téléphone (min 8 caractères)

