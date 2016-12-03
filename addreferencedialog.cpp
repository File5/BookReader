#include "addreferencedialog.h"
#include "ui_addreferencedialog.h"

AddReferenceDialog::AddReferenceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddReferenceDialog)
{
    ui->setupUi(this);
}

AddReferenceDialog::~AddReferenceDialog()
{
    delete ui;
}

const QString &AddReferenceDialog::getText()
{
    return text;
}

void AddReferenceDialog::on_buttonBox_accepted()
{
    text = ui->textEdit->toPlainText();
}
