#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->submitbut, &QPushButton::clicked, this, &MainWindow::onSubmitClicked);
    connect(ui->cancelbut, &QPushButton::clicked, this, &MainWindow::onCancelClicked);
    connect(ui->deletebut, &QPushButton::clicked, this, &MainWindow::onDeleteClicked);
    connect(ui->submitbutt, &QPushButton::clicked, this, &MainWindow::onSubmitClickedd);
    connect(ui->cancelbutt, &QPushButton::clicked, this, &MainWindow::onCancelClickedd);
    connect(ui->deletebutt, &QPushButton::clicked, this, &MainWindow::onDeleteClickedd);
    connect(ui->editbut, &QPushButton::clicked, this, &MainWindow::onEditClicked);
    connect(ui->sortCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onSortChanged);
    //onSortColumnChanged
    connect(ui->searchbut, &QPushButton::clicked, this, &MainWindow::onSearchClicked);

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tabcl->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tabcl->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "commandeID" << "IDclient" << "IDemployee" << "Date \n Commande" << "prix total");
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
   /* QFont tableFont = ui->tableWidget->font();
    tableFont.setPointSize(3);
    ui->tableWidget->setFont(tableFont);*/
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
     ui->tabcl->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabcl->setColumnCount(3);
    ui->tabcl->setHorizontalHeaderLabels(QStringList() <<  "IDclient"<<"email"<< "codeprod");
    ui->tabcl->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //QGraphicsScene* scene = new QGraphicsScene(this);
    //ui->chart->setScene(scene);

}

void MainWindow::onSubmitClicked()
{

    //QString text1 = ui->cmid->text();
    int cmdId = ui->cmid->text().toInt();
    int prix   = ui->prt->text().toInt();
    QString text2 = ui->idcl->text();
    QString text3 = ui->idemp->text();
    QString text4 = ui->datcmd->text();
    //QString text5 = ui->prt->text();


    int newRow = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(newRow);


   // ui->tableWidget->setItem(newRow, 0, new QTableWidgetItem(text1));
    ui->tableWidget->setItem(newRow, 1, new QTableWidgetItem(text2));
    ui->tableWidget->setItem(newRow, 2, new QTableWidgetItem(text3));
    ui->tableWidget->setItem(newRow, 3, new QTableWidgetItem(text4));
    //ui->tableWidget->setItem(newRow, 4, new QTableWidgetItem(text5));
    ui->tableWidget->setItem(newRow, 0, new QTableWidgetItem(QString::number(cmdId)));
    ui->tableWidget->item(newRow, 0)->setData(Qt::EditRole, cmdId);

    ui->tableWidget->setItem(newRow, 4, new QTableWidgetItem(QString::number(prix)));
    ui->tableWidget->item(newRow, 4)->setData(Qt::EditRole, prix);

    ui->cmid->clear();
    ui->idcl->clear();
    ui->idemp->clear();
    ui->datcmd->clear();
    ui->prt->clear();
}
void MainWindow::onSubmitClickedd()
{

    int cmdId = ui->cdprod->text().toInt();
    QString text2 = ui->idcll->text();
    QString text3 = ui->email->text();

    int newRow = ui->tabcl->rowCount();
    ui->tabcl->insertRow(newRow);

    ui->tabcl->setItem(newRow, 0, new QTableWidgetItem(text2));
    ui->tabcl->setItem(newRow, 1, new QTableWidgetItem(text3));


    ui->tabcl->setItem(newRow, 2, new QTableWidgetItem(QString::number(cmdId)));
    ui->tabcl->item(newRow, 2)->setData(Qt::EditRole, cmdId);



    ui->idcll->clear();
    ui->email->clear();
    ui->cdprod->clear();
}
void MainWindow::onCancelClickedd(){
    ui->idcll->clear();
    ui->email->clear();
    ui->cdprod->clear();
}
void MainWindow::onCancelClicked(){
    ui->cmid->clear();
    ui->idcl->clear();
    ui->idemp->clear();
    ui->datcmd->clear();
    ui->prt->clear();
}
/*void MainWindow::onDeleteClicked(){
    QModelIndexList selected = ui->tableWidget->selectionModel()->selectedRows();
    if (!selected.isEmpty()) {
        // remove the first selected row
        int row = selected.first().row();
        ui->tableWidget->removeRow(row);
    }
}*/
/*void MainWindow::onDeleteClicked()
{
    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();

    if (!selectedItems.isEmpty()) {
        int row = selectedItems.first()->row();
        ui->tableWidget->removeRow(row);
    } else {
        QMessageBox::warning(this, "Delete", "Please select a row to delete.");
    }*/
/*void MainWindow::onDeleteClicked()
{
    if(QApplication::focusWidget() == ui->deletebut&& del ==true){
    // Only delete if the table actually has a selection
    QModelIndexList selected = ui->tableWidget->selectionModel()->selectedRows();


    if (!selected.isEmpty() && ui->tableWidget->hasFocus()) {
        int row = selected.first().row();
        ui->tableWidget->removeRow(row);
    } else {
        QMessageBox::warning(this, "Delete", "Please select a row to delete.");
    }
    }
}*/
/*void MainWindow::onDeleteClicked()
{
    QModelIndexList selected = ui->tableWidget->selectionModel()->selectedRows();

    if (!selected.isEmpty()) {
        int row = selected.first().row();
        ui->tableWidget->removeRow(row);
    } else {
        QMessageBox::warning(this, "Delete", "Please select a row to delete.");
    }
}*/
void MainWindow::onDeleteClicked()
{
    QModelIndexList selected = ui->tableWidget->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Delete", "Please select a row to delete.");
        return;
    }

    // Ask the user to confirm
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Delete",
                                  "Are you sure you want to delete the selected row?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        int row = selected.first().row();
        ui->tableWidget->removeRow(row);
    }
}
void MainWindow::onDeleteClickedd()
{
    QModelIndexList selected = ui->tabcl->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Delete", "Please select a row to delete.");
        return;
    }

    // Ask the user to confirm
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Delete",
                                  "Are you sure you want to delete the selected row?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        int row = selected.first().row();
        ui->tabcl->removeRow(row);
    }
}
void MainWindow::onSearchClicked()
{
    QString text = ui->searchLineEdit->text();

    if (text.isEmpty())
        return;

    // Loop through the table to find a match
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
            QTableWidgetItem *item = ui->tableWidget->item(row, col);
            if (item && item->text().contains(text, Qt::CaseInsensitive)) {
                ui->tableWidget->setCurrentCell(row, col);
                return; // stop at first match
            }
        }
    }

    QMessageBox::information(this, "Not found", "No matching result in the table.");
}


void MainWindow::onEditClicked()
{
    // Allow editing by double click and by pressing F2
    ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);

    // Optional: directly start editing the currently selected cell
    QModelIndex currentIndex = ui->tableWidget->currentIndex();
    if (currentIndex.isValid()) {
        ui->tableWidget->edit(currentIndex);
    }
}
void MainWindow::onEditClickedd()
{
    // Allow editing by double click and by pressing F2
    ui->tabcl->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);

    // Optional: directly start editing the currently selected cell
    QModelIndex currentIndex = ui->tabcl->currentIndex();
    if (currentIndex.isValid()) {
        ui->tabcl->edit(currentIndex);
    }
}
void MainWindow::onSortChanged(int index)
{
    // Sort ascending by default; use Qt::DescendingOrder if needed
    ui->tabcl->sortItems(index, Qt::AscendingOrder);
}

/*void MainWindow::onSortColumnChanged(int index)
{
    // Sort the table by the selected column
    ui->tableWidget->sortItems(index);
}*/

MainWindow::~MainWindow()
{
    delete ui;
}
