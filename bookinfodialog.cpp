#include "bookinfodialog.h"
#include "ui_bookinfodialog.h"

BookInfoDialog::BookInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookInfoDialog)
{
    ui->setupUi(this);
}

BookInfoDialog::~BookInfoDialog()
{
    delete ui;
}

QString BookInfoDialog::getTitle() const
{
    return title;
}

QString BookInfoDialog::getAuthor() const
{
    return author;
}

QString BookInfoDialog::getAnnotation() const
{
    return annotation;
}

void BookInfoDialog::on_buttonBox_accepted()
{
    title = ui->titleEdit->text().trimmed();
    title.append('\n');
    author = ui->authorEdit->text().trimmed();
    author.append('\n');
    annotation = ui->annotationEdit->toPlainText().trimmed();
    annotation.append('\n');
}

void BookInfoDialog::setAnnotation(const QString &value)
{
    annotation = value;
}

void BookInfoDialog::showEvent(QShowEvent *showEvent)
{
    ui->titleEdit->setText(title);
    ui->authorEdit->setText(author);
    ui->annotationEdit->setText(annotation);
}

void BookInfoDialog::setAuthor(const QString &value)
{
    author = value;
}

void BookInfoDialog::setTitle(const QString &value)
{
    title = value;
}
