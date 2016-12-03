#ifndef ADDREFERENCEDIALOG_H
#define ADDREFERENCEDIALOG_H

#include <QDialog>

namespace Ui {
class AddReferenceDialog;
}

class AddReferenceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddReferenceDialog(QWidget *parent = 0);
    ~AddReferenceDialog();
    const QString& getText();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddReferenceDialog *ui;
    QString text;
};

#endif // ADDREFERENCEDIALOG_H
