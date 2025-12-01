#ifndef RAVITAILLEMENTDIALOG_H
#define RAVITAILLEMENTDIALOG_H

#include <QDialog>

class QComboBox;
class QLineEdit;
class QSpinBox;
class QTextEdit;
class QPushButton;
class QSqlQueryModel;

class RavitaillementDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RavitaillementDialog(QWidget *parent = nullptr);
    ~RavitaillementDialog();

signals:
    void ravitaillementAdded();

private slots:
    void onSendClicked();

private:
    QLineEdit *productEdit;
    QSpinBox *quantitySpin;
    QComboBox *supplierCombo; // shows supplier id + name
    QTextEdit *messageEdit;
    QSpinBox *noteSpin;
    QPushButton *sendBtn;
    QPushButton *cancelBtn;

    void loadSuppliers();
    int currentSelectedSupplierId() const;
};

#endif // RAVITAILLEMENTDIALOG_H
