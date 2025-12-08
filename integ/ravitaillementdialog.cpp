#include "ravitaillementdialog.h"
#include "fournisseur.h"
#include "metier.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QHeaderView>

RavitaillementDialog::RavitaillementDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Ajouter un ravitaillement"));

    productEdit = new QLineEdit(this);
    supplierCombo = new QComboBox(this);
    quantitySpin = new QSpinBox(this);
    messageEdit = new QTextEdit(this);
    noteSpin = new QSpinBox(this);

    quantitySpin->setRange(1, 10000000);
    noteSpin->setRange(0, 5);
    messageEdit->setPlaceholderText(tr("Message optionnel"));

    sendBtn = new QPushButton(tr("Ajouter"), this);
    cancelBtn = new QPushButton(tr("Annuler"), this);

    QFormLayout *form = new QFormLayout(this);
    form->addRow(tr("Nom ou Code produit"), productEdit);
    form->addRow(tr("Fournisseur (optionnel)"), supplierCombo);
    form->addRow(tr("Quantité"), quantitySpin);
    form->addRow(tr("Message"), messageEdit);
    form->addRow(tr("Note (0-5)"), noteSpin);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(sendBtn);
    btnLayout->addWidget(cancelBtn);
    form->addRow(btnLayout);

    connect(sendBtn, &QPushButton::clicked, this, &RavitaillementDialog::onSendClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &RavitaillementDialog::reject);

    loadSuppliers();
}

RavitaillementDialog::~RavitaillementDialog()
{
}

void RavitaillementDialog::loadSuppliers()
{
    // Use Fournisseur model to populate combo
    Fournisseur f;
    QSqlQueryModel* m = f.afficher();
    supplierCombo->addItem(tr("-- Aucun --"), -1);
    for (int r = 0; r < m->rowCount(); ++r) {
        QString id = m->data(m->index(r, 0)).toString();
        QString nom = m->data(m->index(r, 1)).toString();
        QString prenom = m->data(m->index(r, 2)).toString();
        int iid = id.toInt();
        supplierCombo->addItem(QString("%1 - %2 %3").arg(id).arg(nom).arg(prenom), iid);
    }
}

int RavitaillementDialog::currentSelectedSupplierId() const
{
    return supplierCombo->currentData().toInt();
}

void RavitaillementDialog::onSendClicked()
{
    QString prod = productEdit->text().trimmed();
    int quant = quantitySpin->value();
    int idf = currentSelectedSupplierId();
    QString msg = messageEdit->toPlainText();
    int note = noteSpin->value();

    if (prod.isEmpty()) {
        QMessageBox::warning(this, tr("Erreur"), tr("Le champ produit est requis (nom ou code)."));
        productEdit->setFocus();
        return;
    }

    // If product text is numeric use code, else code=-1 and rely on matching by marque in autre method if needed
    bool isNum;
    int code = prod.toInt(&isNum);
    if (!isNum) code = -1;

    Metier met;
    QString err;
    bool ok = met.ajouterRavitaillementStandalone(code, (idf > 0 ? idf : -1), quant, msg, note, err);
    if (!ok) {
        QMessageBox::critical(this, tr("Erreur ajout"), tr("Echec du ravitaillement: %1").arg(err));
        return;
    }

    QMessageBox::information(this, tr("Succès"), tr("Ravitaillement ajouté"));
    emit ravitaillementAdded();
    accept();
}
