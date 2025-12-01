#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employe.h"
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QHeaderView>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QFont>
#include <QSqlQuery>
#include <QTableWidget>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    ui->setupUi(this);
    connect(ui->sendchat, &QPushButton::clicked, this, &MainWindow::sendMessage);
    ui->tableView->horizontalHeader()->setSectionsClickable(true);
    ui->tableView->horizontalHeader()->setHighlightSections(true);

    // Let clicking headers select the whole column (visual feedback)
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectColumns);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(ui->btnPerformance, &QPushButton::clicked, this, [=](){
        int totalWorkingDays = 20; // replace with number of working days for the month
        afficherPerformancePresence(totalWorkingDays);
    });


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
        afficherTableemployes(); // refresh after adding
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
        afficherTableemployes();  // refresh tableView if you display employees
        ui->id_supp->clear();     // clear input after deleting
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression. Vérifiez l'ID !");
    }
}


// =====================================
// === Afficher les employés ===========
// =====================================
void MainWindow::afficherTableemployes()
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
        afficherTableemployes();  // Refresh the table after update
    } else {
        QMessageBox::critical(this, "Erreur", "La modification a échoué !");
    }
}

// =====================================
// === Bouton Afficher =================
// =====================================
void MainWindow::on_afficher_clicked()
{
    afficherTableemployes();
}
void MainWindow::on_rechercher_clicked()
{
    QString idText = ui->id_rech->text().trimmed();

    if (idText.isEmpty()) {
        QMessageBox::warning(this, "Champ manquant", "Veuillez entrer un ID.");
        return;
    }

    int id = idText.toInt();
    if (id <= 0) {
        QMessageBox::warning(this, "ID invalide", "L'ID doit être un nombre positif.");
        return;
    }

    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery(
        "SELECT ID, NOM, PRENOM, TYPE, SALAIRE, DATE_EMB "
        "FROM EMPLOYE WHERE ID = " + QString::number(id)
        );

    ui->tableView->setModel(model);

    if (model->rowCount() == 0) {
        QMessageBox::information(this, "Introuvable", "Aucun employé trouvé avec cet ID.");
    }
}




void MainWindow::on_trie_clicked()
{
    int columnIndex = ui->tableView->currentIndex().column();

    if (columnIndex < 0) {
        QMessageBox::warning(this, "Tri impossible", "Veuillez d'abord sélectionner une colonne dans le tableau.");
        return;
    }

    QString columnName = ui->tableView->model()->headerData(columnIndex, Qt::Horizontal).toString();

    // Mapping headers -> real SQL column names
    QMap<QString, QString> map;
    map["ID"] = "ID";
    map["NOM"] = "NOM";
    map["PRENOM"] = "PRENOM";
    map["TYPE"] = "TYPE";
    map["SALAIRE"] = "SALAIRE";
    map["DATE_EMB"] = "DATE_EMB";

    QString dbColumn = map.value(columnName);

    if (dbColumn.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Impossible d'associer l'en-tête à une colonne SQL.");
        return;
    }

    // Correct table name !!!
    QString queryStr = QString("SELECT * FROM EMPLOYE ORDER BY %1 ASC").arg(dbColumn);

    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(queryStr);

    if (model->lastError().isValid()) {
        QMessageBox::critical(this, "Erreur SQL", model->lastError().text());
        return;
    }

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();

    QMessageBox::information(this, "Tri effectué",
                             QString("Tri effectué par la colonne '%1'.").arg(columnName));
}

void MainWindow::on_statistique_clicked()
{
    // --- Count employees by salary ranges ---
    int low = 0, medium = 0, high = 0;

    QSqlQuery query("SELECT SALAIRE FROM EMPLOYE");
    while (query.next()) {
        double salaire = query.value(0).toDouble();
        if (salaire < 1000)
            low++;
        else if (salaire <= 2000)
            medium++;
        else
            high++;
    }

    // --- Create Pie Series ---
    QPieSeries *series = new QPieSeries();
    series->append("< 1000", low);
    series->append("1000-2000", medium);
    series->append("> 2000", high);

    for (QPieSlice *slice : series->slices()) {
        slice->setLabelVisible(true);
        slice->setLabelFont(QFont("Arial", 10));
        slice->setExploded(true);
        slice->setPen(QPen(Qt::black, 1));
    }

    // --- Create Chart ---
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des employés par salaire");
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // --- Create ChartView ---
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(400, 300); // adjust size

    // --- Insert into the first cell of your tableWidget ---
    ui->tableWidget_s->setRowCount(1);
    ui->tableWidget_s->setColumnCount(1);
    ui->tableWidget_s->setCellWidget(0, 0, chartView);
}



//CHAT BOTT ************************

void MainWindow::sendMessage() {
    QString userMsg = ui->chat->text().trimmed();
    if (userMsg.isEmpty()) return;

    ui->boxchat->append("You: " + userMsg);
    ui->chat->clear();

    // --- Requête réseau ---
    QNetworkRequest request(QUrl("https://generativelanguage.googleapis.com/v1beta/openai/chat/completions"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Remplacer par votre clé API Gemini free
    QString apiKey = "AIzaSyBcbj-OSUWzpojKSlpaUDGRK_hEiN9uQE4";
    request.setRawHeader("Authorization", ("Bearer " + apiKey).toUtf8());

    // SSL
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());



    // --- JSON ---
    QJsonObject json;
    json["model"] = "gemini-2.5-flash"; // Modèle free

    QJsonArray messages;

    // ✅ System prompt for Optic Store specialization
    messages.append(QJsonObject{
        {"role", "system"},
        {"content", "You are an assistant specialized in Optic Store. Only answer questions related to eyeglasses, lenses, frames, and optical products. If the question is not related to the Optic Store, politely say: 'I can only answer questions about the Optic Store.'"}
    });

    // User message
    messages.append(QJsonObject{
        {"role", "user"},
        {"content", userMsg}
    });

    // Add messages array to JSON
    json["messages"] = messages;

    // --- Envoyer ---
    static QNetworkAccessManager manager;
    connect(&manager, &QNetworkAccessManager::finished, this, &MainWindow::onResponse);
    manager.post(request, QJsonDocument(json).toJson());
}

void MainWindow::onResponse(QNetworkReply *reply) {
    if (reply->error()) {
        ui->boxchat->append("Error: " + reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    QJsonArray choices = obj["choices"].toArray();
    if (!choices.isEmpty()) {
        QString answer = choices[0].toObject()["message"].toObject()["content"].toString();
        ui->boxchat->append("AI: " + answer);
    } else {
        ui->boxchat->append("AI: (no response)");
    }

    reply->deleteLater();
}





void MainWindow::on_pdf_clicked()
{
    // 1️⃣ Ask user where to save the PDF
    QString filePath = QFileDialog::getSaveFileName(this, "Exporter la liste des employés en PDF", "", "PDF Files (*.pdf)");
    if (filePath.isEmpty()) return;

    // 2️⃣ Configure PDF writer
    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300);
    pdfWriter.setTitle("Liste des employés");

    QPainter painter(&pdfWriter);

    // 3️⃣ Margins
    int marginLeft = 50;
    int marginTop = 100;
    int x = marginLeft;
    int y = marginTop;

    // 4️⃣ Title
    QString title = "Liste des employés";
    QRect rectTitle(marginLeft, y, pdfWriter.width() - (marginLeft * 2), 80);
    painter.setFont(QFont("Helvetica", 20, QFont::Bold));
    painter.setPen(Qt::darkBlue);
    painter.drawText(rectTitle, Qt::AlignCenter, title);
    y += 100;

    // 5️⃣ Table settings
    int rowHeight = 40;
    int maxColsPerPage = 7;
    int colCount = ui->tableView->model()->columnCount();

    // If you have any "photo" column, skip it
    int photoIndex = -1;
    for (int i = 0; i < colCount; ++i) {
        if (ui->tableView->model()->headerData(i, Qt::Horizontal).toString().toLower().contains("pdp")) {
            photoIndex = i;
            break;
        }
    }

    int adjustedColCount = (photoIndex != -1) ? colCount - 1 : colCount;
    int colsToShow = qMin(maxColsPerPage, adjustedColCount);
    int colWidth = (pdfWriter.width() - 2*marginLeft - (colsToShow * 10)) / colsToShow;

    // 6️⃣ Header
    painter.setFont(QFont("Helvetica", 12, QFont::Bold));
    painter.setPen(Qt::black);
    painter.setBrush(QColor(230, 230, 230));
    for (int i = 0, j = 0; i < colCount; ++i) {
        if (i == photoIndex) continue;
        if (j >= colsToShow) break;

        painter.drawRect(x, y, colWidth, rowHeight);
        painter.drawText(QRect(x + 5, y, colWidth - 10, rowHeight),
                         Qt::AlignCenter,
                         ui->tableView->model()->headerData(i, Qt::Horizontal).toString());
        x += colWidth + 10;
        j++;
    }
    y += rowHeight + 10;
    x = marginLeft;

    // 7️⃣ Table content
    painter.setFont(QFont("Helvetica", 10));
    painter.setPen(Qt::black);
    int rowCount = ui->tableView->model()->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        QColor rowColor = (row % 2 == 0) ? QColor(245, 245, 245) : QColor(255, 255, 255);
        painter.setBrush(rowColor);

        for (int col = 0, j = 0; col < colCount; ++col) {
            if (col == photoIndex) continue;
            if (j >= colsToShow) break;

            painter.setPen(QColor(150, 150, 150));
            painter.drawRect(x, y, colWidth, rowHeight);

            QString data = ui->tableView->model()->data(ui->tableView->model()->index(row, col)).toString();
            data = data.left(colWidth / 10) + (data.length() > colWidth / 10 ? "..." : "");

            Qt::Alignment alignment = (data.toDouble() || data.toInt())
                                          ? Qt::AlignRight | Qt::AlignVCenter
                                          : Qt::AlignLeft | Qt::AlignVCenter;

            painter.drawText(QRect(x + 5, y, colWidth - 10, rowHeight), alignment, data);
            x += colWidth + 10;
            j++;
        }
        x = marginLeft;
        y += rowHeight;

        // 8️⃣ Page break
        if (y > pdfWriter.height() - marginTop) {
            pdfWriter.newPage();
            y = marginTop;

            painter.setFont(QFont("Helvetica", 12, QFont::Bold));
            painter.setPen(Qt::black);
            painter.setBrush(QColor(230, 230, 230));
            x = marginLeft;
            for (int i = 0, j = 0; i < colCount; ++i) {
                if (i == photoIndex) continue;
                if (j >= colsToShow) break;

                painter.drawRect(x, y, colWidth, rowHeight);
                painter.drawText(QRect(x + 5, y, colWidth - 10, rowHeight),
                                 Qt::AlignCenter,
                                 ui->tableView->model()->headerData(i, Qt::Horizontal).toString());
                x += colWidth + 10;
                j++;
            }
            y += rowHeight + 10;
            x = marginLeft;
        }
    }

    painter.end();

    // 9️⃣ Open PDF automatically
    if (!QDesktopServices::openUrl(QUrl::fromLocalFile(filePath))) {
        QMessageBox::warning(this, "Avertissement", "Le PDF a été généré mais n'a pas pu être ouvert automatiquement.");
    } else {
        QMessageBox::information(this, "Succès", "Le fichier PDF des employés a été généré et ouvert avec succès !");
    }

}
// --- Check-in ---
void MainWindow::on_btn_checkin_clicked()
{
    int id = ui->lineEdit_id_emp->text().toInt();
    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID valide !");
        return;
    }

    QSqlQuery query;
    QDateTime now = QDateTime::currentDateTime();

    // Prevent multiple check-ins for today
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM presence "
                       "WHERE id_employe=:id AND date_jour BETWEEN :start AND :end");
    checkQuery.bindValue(":id", id);
    checkQuery.bindValue(":start", QDateTime(now.date(), QTime(0,0,0)));
    checkQuery.bindValue(":end", QDateTime(now.date(), QTime(23,59,59)));
    checkQuery.exec();
    checkQuery.next();
    if (checkQuery.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Erreur", "Check-in déjà effectué aujourd'hui !");
        return;
    }

    query.prepare("INSERT INTO presence (id, id_employe, date_jour, heure_entree) "
                  "VALUES (PRESENCE_SEQ.NEXTVAL, :id_emp, :date, :heure)");
    query.bindValue(":id_emp", id);      // Employee ID
    query.bindValue(":date", now.date());
    query.bindValue(":heure", now.time());

    if (query.exec()) {
        QMessageBox::information(this, "Check-in", "Check-in effectué à " + now.toString());
        afficherTablePresence();
    } else {
        QMessageBox::critical(this, "Erreur", query.lastError().text());
    }
}

// --- Check-out ---
void MainWindow::on_btn_checkout_clicked()
{
    int id = ui->lineEdit_id_emp->text().toInt();
    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID valide !");
        return;
    }

    QDate today = QDate::currentDate();
    QDateTime start(today, QTime(0,0,0));
    QDateTime end(today, QTime(23,59,59));

    // Find today’s check-in
    QSqlQuery query;
    query.prepare("SELECT id, heure_entree FROM presence "
                  "WHERE id_employe=:id AND date_jour BETWEEN :start AND :end "
                  "ORDER BY heure_entree DESC FETCH FIRST 1 ROWS ONLY");
    query.bindValue(":id", id);
    query.bindValue(":start", start);
    query.bindValue(":end", end);

    if (!query.exec() || !query.next()) {
        QMessageBox::warning(this, "Erreur", "Aucun check-in trouvé pour aujourd'hui !");
        return;
    }

    int presenceId = query.value("id").toInt();
    QTime checkinTime = query.value("heure_entree").toTime();
    QDateTime dtCheckin(today, checkinTime);
    QDateTime dtCheckout = QDateTime::currentDateTime();

    double hoursWorked = dtCheckin.secsTo(dtCheckout) / 3600.0; // heures travaillées

    // Update check-out
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE presence SET heure_sortie=:sortie, heures_travail=:heures "
                        "WHERE id=:id");
    updateQuery.bindValue(":sortie", dtCheckout.time());
    updateQuery.bindValue(":heures", hoursWorked);
    updateQuery.bindValue(":id", presenceId);

    if (updateQuery.exec()) {
        QMessageBox::information(this, "Check-out",
                                 QString("Check-out effectué à %1\nHeures travaillées: %2")
                                     .arg(dtCheckout.toString())
                                     .arg(hoursWorked, 0, 'f', 2));
        afficherTablePresence();
    } else {
        QMessageBox::critical(this, "Erreur", updateQuery.lastError().text());
    }
}

// --- Afficher la table de présence ---
void MainWindow::afficherTablePresence()
{
    // 1. Updated SQL Query: Removed 'heure_sortie'
    QSqlQuery query("SELECT id_employe, date_jour, heure_entree, heures_travail "
                    "FROM presence ORDER BY date_jour DESC");

    // 2. USE QTableWidget METHODS (e.g., clear, setRowCount)
    // NOTE: If ui->tableView_presence is truly a QTableView, this will crash.
    // Assuming ui->tableView_presence is a QTableWidget that was misnamed.
    ui->tableView_presence->clearContents();
    ui->tableView_presence->setRowCount(0);

    // 3. Updated Headers and Column Count (4 columns instead of 5)
    QStringList headers = {"ID Employé", "Date", "Heure Entrée", "Heures Travail"};
    ui->tableView_presence->setColumnCount(headers.size()); // Size is now 4
    ui->tableView_presence->setHorizontalHeaderLabels(headers);

    int row = 0;
    while (query.next()) {
        ui->tableView_presence->insertRow(row);

        // Data retrieval
        QVariant hEntree = query.value("heure_entree");
        QVariant hTravail = query.value("heures_travail");

        // 4. Populate items (Indexes are adjusted)
        // Column 0: ID Employé
        ui->tableView_presence->setItem(row, 0, new QTableWidgetItem(query.value("id_employe").toString()));

        // Column 1: Date
        ui->tableView_presence->setItem(row, 1, new QTableWidgetItem(query.value("date_jour").toDate().toString("yyyy-MM-dd")));

        // Column 2: Heure Entrée
        ui->tableView_presence->setItem(row, 2, new QTableWidgetItem(hEntree.isNull() ? "" : hEntree.toTime().toString("HH:mm:ss")));

        // Column 3: Heures Travail (Moved from index 4)
        ui->tableView_presence->setItem(row, 3, new QTableWidgetItem(hTravail.isNull() ? "" : QString::number(hTravail.toDouble(), 'f', 2)));

        row++;
    }

    ui->tableView_presence->resizeColumnsToContents();
}
void MainWindow::afficherPerformancePresence(int totalWorkingDays)
{
    QSqlQuery query;
    query.prepare(R"(
        SELECT e.id, e.nom, e.prenom, COUNT(p.id) AS jours_present
        FROM employe e
        LEFT JOIN presence p ON e.id = p.id_employe
        GROUP BY e.id, e.nom, e.prenom
        ORDER BY e.id
    )");

    if(!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL", query.lastError().text());
        return;
    }

    ui->tableWidget_performance->clear();
    QStringList headers = {"ID", "Nom", "Prénom", "Jours Présent", "Total Jours", "Score (%)"};
    ui->tableWidget_performance->setColumnCount(headers.size());
    ui->tableWidget_performance->setHorizontalHeaderLabels(headers);
    ui->tableWidget_performance->setRowCount(0);

    int row = 0;
    while(query.next()) {
        int jours_present = query.value("jours_present").toInt();
        double score = ((double)jours_present / totalWorkingDays) * 100.0;

        ui->tableWidget_performance->insertRow(row);
        ui->tableWidget_performance->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        ui->tableWidget_performance->setItem(row, 1, new QTableWidgetItem(query.value("nom").toString()));
        ui->tableWidget_performance->setItem(row, 2, new QTableWidgetItem(query.value("prenom").toString()));
        ui->tableWidget_performance->setItem(row, 3, new QTableWidgetItem(QString::number(jours_present)));
        ui->tableWidget_performance->setItem(row, 4, new QTableWidgetItem(QString::number(totalWorkingDays)));
        ui->tableWidget_performance->setItem(row, 5, new QTableWidgetItem(QString::number(score, 'f', 2) + "%"));
        row++;
    }

    ui->tableWidget_performance->resizeColumnsToContents();
}
