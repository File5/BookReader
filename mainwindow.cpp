#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    bookView = new PagedTextEdit(this);
    connect(bookView, &PagedTextEdit::pageChanged, this, &MainWindow::displayPageNumber);
    ui->mainLayout->replaceWidget(ui->bookView, bookView);
    ui->bookView->hide();

    bookView->setFont(QFont("Times New Roman", 16));

    QFile text(":/text/testbook.txt");
    text.open(QIODevice::ReadOnly);
    QString data(text.readAll());
    text.close();

    bookBuilder.setData(data);
    currentBook = bookBuilder.readBook();

    bookView->setText(currentBook->getChapter(0));
}

MainWindow::~MainWindow()
{
    delete currentBook;
    delete bookView;
    delete ui;
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
}

void MainWindow::displayPageNumber(int current, int lastPage)
{
    QString currentPageText = QString("Страница: %1/%2").arg(QString::number(current), QString::number(lastPage));
    ui->currentPageLabel->setText(currentPageText);

    ui->pageSpinBox->setMaximum(lastPage);

    ui->nextButton->setEnabled(current != lastPage);
    ui->prevButton->setEnabled(current != 1);
}

void MainWindow::on_actionBookInfo_triggered()
{
    if (currentBook) {
        QString text;
        text.append("Title: ");
        text.append(currentBook->getTitle());
        text.append("Author: ");
        text.append(currentBook->getAuthor());
        text.append(currentBook->getAnnotation());
        QMessageBox::information(this, "Информация о книге", text);
    }
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}
