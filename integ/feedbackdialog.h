#ifndef FEEDBACKDIALOG_H
#define FEEDBACKDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QTableView>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

class FeedbackDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FeedbackDialog(QWidget *parent = nullptr);
    ~FeedbackDialog() override;

private slots:
    void on_sendButton_clicked();

private:
    QTableView* tableView;
    QTextEdit* messageEdit;
    // star buttons for rating (1..5)
    QList<QPushButton*> starButtons;
    int currentRating = 0;
    QPushButton* sendButton;
    QVBoxLayout* mainLayout;
};

#endif // FEEDBACKDIALOG_H
