#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include "book.h"

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

    QFile text(":/text/lorem_ipsum.txt");
    text.open(QIODevice::ReadOnly);
    QByteArray data = text.readAll();
    text.close();
    QString string(data);
    bookView->setText(string);
}

MainWindow::~MainWindow()
{
    delete bookView;
    delete ui;
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    testBook();
}

void MainWindow::displayPageNumber(int current, int lastPage)
{
    QString currentPageText = QString("Страница: %1/%2").arg(QString::number(current), QString::number(lastPage));
    ui->currentPageLabel->setText(currentPageText);

    ui->pageSpinBox->setMaximum(lastPage);

    ui->nextButton->setEnabled(current != lastPage);
    ui->prevButton->setEnabled(current != 1);
}

void MainWindow::testBook()
{
    QFile text(":/text/lorem_ipsum.txt");
    Book book(text, this);
    QSizeF size = bookView->document()->pageSize();
    book.setPageSize(size);
    book.setFont(QFont("Times New Roman", 16));
    qDebug() << book.getPageCount(0);
}
