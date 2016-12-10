#include "librarydialog.h"
#include "ui_librarydialog.h"

#include <string>

using namespace std;

LibraryDialog::LibraryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LibraryDialog)
{
    ui->setupUi(this);

    QStringList columns;
    columns << QString("Title") << QString("Author") << QString("Date") << QString("Genre");
    ui->tableWidget->setColumnCount(columns.size());
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setHorizontalHeaderLabels(columns);

    connect(ui->comboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &LibraryDialog::filter);
    connect(ui->comboBox_2, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &LibraryDialog::filter);
    connect(ui->comboBox_3, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &LibraryDialog::filter);
    connect(ui->comboBox_4, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &LibraryDialog::filter);
}

LibraryDialog::~LibraryDialog()
{
    delete ui;
}

QList<BookCover> LibraryDialog::getBookCovers() const
{
    return bookCovers;
}

void LibraryDialog::setBookCovers(const QList<BookCover> &value)
{
    bookCovers.clear();
    foreach (BookCover bookCover, value) {
        bookCover.title = QString(bookCover.title.c_str()).trimmed().toStdString();
        bookCover.author = QString(bookCover.author.c_str()).trimmed().toStdString();
        bookCover.date = QString(bookCover.date.c_str()).trimmed().toStdString();
        bookCover.genre = QString(bookCover.genre.c_str()).trimmed().toStdString();
        bookCovers.append(bookCover);
    }
    initFilter();
}

void LibraryDialog::showEvent(QShowEvent *showEvent)
{
    QDialog::showEvent(showEvent);

    filter(1);
}

void LibraryDialog::initFilter()
{
    ui->comboBox->clear();
    ui->comboBox_2->clear();
    ui->comboBox_3->clear();
    ui->comboBox_4->clear();

    ui->comboBox->addItem(QString("Title"));
    ui->comboBox_2->addItem(QString("Author"));
    ui->comboBox_3->addItem(QString("Date"));
    ui->comboBox_4->addItem(QString("Genre"));

    foreach (BookCover bookCover, bookCovers) {
        if (!titles.count(bookCover.title)) {
            ui->comboBox->addItem(bookCover.title.c_str());
            titles.insert(bookCover.title);
        }

        if (!authors.count(bookCover.author)) {
            ui->comboBox_2->addItem(bookCover.author.c_str());
            authors.insert(bookCover.author);
        }

        if (!dates.count(bookCover.date)) {
            ui->comboBox_3->addItem(bookCover.date.c_str());
            dates.insert(bookCover.date);
        }

        if (!genres.count(bookCover.genre)) {
            ui->comboBox_4->addItem(bookCover.genre.c_str());
            genres.insert(bookCover.genre);
        }
    }
}

void LibraryDialog::filter(int indexChanged)
{
    string title;
    if (ui->comboBox->currentIndex() != 0) {
        title = ui->comboBox->currentText().toStdString();
    }

    string author;
    if (ui->comboBox_2->currentIndex() != 0) {
        author = ui->comboBox_2->currentText().toStdString();
    }

    string date;
    if (ui->comboBox_3->currentIndex() != 0) {
        date = ui->comboBox_3->currentText().toStdString();
    }

    string genre;
    if (ui->comboBox_4->currentIndex() != 0) {
        genre = ui->comboBox_4->currentText().toStdString();
    }

    filtered.clear();
    foreach (BookCover bookCover, bookCovers) {
        if (
            (title.empty() || bookCover.title == title) &&
            (author.empty() || bookCover.author == author) &&
            (date.empty() || bookCover.date == date) &&
            (genre.empty() || bookCover.genre == genre)
        ) {
            filtered.append(bookCover);
        }
    }

    showFiltered();
}

void LibraryDialog::showFiltered()
{
    int count = filtered.size();
    ui->tableWidget->setRowCount(count);

    for (int row = 0; row < count; row++) {
        BookCover book = filtered.at(row);

        for (int column = 0; column < 4; column++) {
            string text;
            if (column == 0) {
                text = book.title;
            } else if (column == 1) {
                text = book.author;
            } else if (column == 2) {
                text = book.date;
            } else {
                text = book.genre;
            }

            QTableWidgetItem *item = new QTableWidgetItem(QString(text.c_str()));
            ui->tableWidget->setItem(row, column, item);
        }
    }
}
