#ifndef AUTOSCROLLDIALOG_H
#define AUTOSCROLLDIALOG_H

#include <QDialog>

namespace Ui {
class AutoScrollDialog;
}

class AutoScrollDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AutoScrollDialog(QWidget *parent = 0);
    ~AutoScrollDialog();

    int getInterval() const;
    void setInterval(int value);

protected:
    void showEvent(QShowEvent *event);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AutoScrollDialog *ui;
    int interval;
};

#endif // AUTOSCROLLDIALOG_H
