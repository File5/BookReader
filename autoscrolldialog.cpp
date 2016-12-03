#include "autoscrolldialog.h"
#include "ui_autoscrolldialog.h"

AutoScrollDialog::AutoScrollDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AutoScrollDialog),
    interval(60)
{
    ui->setupUi(this);
}

AutoScrollDialog::~AutoScrollDialog()
{
    delete ui;
}

int AutoScrollDialog::getInterval() const
{
    return interval;
}

void AutoScrollDialog::setInterval(int value)
{
    interval = value;
}

void AutoScrollDialog::showEvent(QShowEvent *event)
{
    ui->interval->setValue(interval);
}

void AutoScrollDialog::on_buttonBox_accepted()
{
    interval = ui->interval->value();
}
