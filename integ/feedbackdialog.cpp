#include "feedbackdialog.h"
#include "fournisseur.h"
#include "metier.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QPushButton>

FeedbackDialog::FeedbackDialog(QWidget *parent) : QDialog(parent)
{
    // Create widgets programmatically to avoid using a .ui file
    tableView = new QTableView(this);
    messageEdit = new QTextEdit(this);
    sendButton = new QPushButton(tr("Envoyer"), this);
    // create star buttons (1..5)
    for (int i = 0; i < 5; ++i) {
        QPushButton *b = new QPushButton(QStringLiteral("☆"), this);
        b->setFlat(true);
        b->setProperty("starIndex", i+1);
        b->setStyleSheet("font-size:24px;color:gold;padding:2px;border:none;");
        starButtons.append(b);
    }
    mainLayout = new QVBoxLayout(this);

    // stars start at 0 (none selected)
    currentRating = 0;
    messageEdit->setPlaceholderText(tr("Votre message..."));

    // Basic layout: table on top, message and rating below and send button
    mainLayout->addWidget(tableView);
    mainLayout->addWidget(messageEdit);
    QHBoxLayout *starsLay = new QHBoxLayout();
    for (auto *sb : starButtons) starsLay->addWidget(sb);
    mainLayout->addLayout(starsLay);
    mainLayout->addWidget(sendButton);
    setLayout(mainLayout);

    // Load fournisseurs into the table
    Fournisseur tmp;
    QSqlQueryModel* model = tmp.afficher();
    tableView->setModel(model);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(sendButton, &QPushButton::clicked, this, &FeedbackDialog::on_sendButton_clicked);
    // connect star buttons so they behave like rating
    for (auto *sb : starButtons) {
        connect(sb, &QPushButton::clicked, this, [this, sb]() {
            int idx = sb->property("starIndex").toInt();
            currentRating = idx;
            // update visuals
            for (auto *s : starButtons) {
                int si = s->property("starIndex").toInt();
                s->setText(si <= currentRating ? QStringLiteral("★") : QStringLiteral("☆"));
            }
        });
    }
}

FeedbackDialog::~FeedbackDialog()
{
    // Qt smart pointers: children will be deleted by QObject hierarchy
}

void FeedbackDialog::on_sendButton_clicked()
{
    QModelIndexList sel = tableView->selectionModel()->selectedRows();
    if (sel.isEmpty()) {
        QMessageBox::warning(this, tr("Feedback"), tr("Veuillez choisir un fournisseur"));
        return;
    }

    int row = sel.first().row();
    QSqlQueryModel* m = qobject_cast<QSqlQueryModel*>(tableView->model());
    if (!m) return;

    int id = m->data(m->index(row,0)).toInt();
    QString msg = messageEdit->toPlainText();
    int rating = currentRating;

    Metier met;
    QString err;
    if (!met.envoyerFeedback(id, msg, rating, err)) {
        QMessageBox::critical(this, tr("Erreur"), tr("Feedback non envoyé: %1").arg(err));
        return;
    }

    QMessageBox::information(this, tr("Envoyé"), tr("Feedback envoyé avec succès"));
    accept();
}
