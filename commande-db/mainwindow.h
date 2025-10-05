#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QBrush>



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void onSubmitClicked();
    void onCancelClicked();
    void onDeleteClicked();
    void onEditClicked();
    void onSubmitClickedd();
    void onCancelClickedd();
    void onDeleteClickedd();
    void onEditClickedd();
    //void onSortColumnChanged(int index);
    void onSearchClicked();
    void onSortChanged(int index);


};
#endif // MAINWINDOW_H
