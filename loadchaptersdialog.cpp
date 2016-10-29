#include "loadchaptersdialog.h"
#include "ui_loadchaptersdialog.h"
#include <QFileDialog>

LoadChaptersDialog::LoadChaptersDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadChaptersDialog)
{
    ui->setupUi(this);
}

LoadChaptersDialog::~LoadChaptersDialog()
{
    delete ui;
}

QStringList LoadChaptersDialog::getFilenames() const
{
    QStringList filenames;
    int count = ui->fileList->count();
    for (int i = 0; i < count; i++) {
        filenames.append(ui->fileList->item(i)->text());
    }
    return filenames;
}

void LoadChaptersDialog::on_chooseFile_clicked()
{
    QString filename = QFileDialog::getOpenFileName();
    if (filename.isNull()) return;

    ui->fileList->addItem(filename);
}
