#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentFont("Times New Roman", 16),
    currentChapterIndex(0),
    previousPages(0),
    pagesCount(0)
{
    ui->setupUi(this);
    bookView = new PagedTextEdit(this);
    connect(bookView, &PagedTextEdit::pageChanged, this, &MainWindow::displayPageNumber);
    ui->mainLayout->replaceWidget(ui->bookView, bookView);
    ui->bookView->hide();
    bookView->setFont(currentFont);

    QFile text(":/text/testbook.txt");
    text.open(QIODevice::ReadOnly);
    QString data(text.readAll());
    text.close();

    bookBuilder.setData(data);

    currentBook = bookBuilder.readBook();
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
    currentBook->setPageSize(bookView->document()->pageSize());
    currentBook->setFont(currentFont);

    pagesCount = currentBook->getPageCount();

    selectChapter(0);
    displayChaptersList();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    currentBook->setPageSize(bookView->document()->pageSize());
}

void MainWindow::displayPageNumber(int current, int lastPage)
{
    if (currentBook) {
        current += previousPages;
        lastPage = pagesCount;

        QString currentPageText = QString("Страница: %1/%2").arg(QString::number(current), QString::number(lastPage));
        ui->currentPageLabel->setText(currentPageText);

        ui->pageSpinBox->setMaximum(lastPage);

        ui->nextButton->setEnabled(current != lastPage);
        ui->prevButton->setEnabled(current != 1);
    }
}

void MainWindow::displayChaptersList()
{
    if (currentBook) {
        for (int i = 0; i < currentBook->getChapterCount(); i++) {
            ui->chapterList->addItem(currentBook->getChapterTitle(i));
        }
    }
}

void MainWindow::selectChapter(int index)
{
    if (currentBook && index < currentBook->getChapterCount()) {
        currentChapterIndex = index;
        previousPages = currentBook->getCurrentPage(0, index);

        bookView->setText(currentBook->getChapter(index));
    }
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
