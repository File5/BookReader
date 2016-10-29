#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include "bookinfodialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentFont("Times New Roman", 16),
    currentChapterIndex(-1),
    previousPages(0),
    pagesCount(0),
    editingMode(false),
    editingModeUsed(false)
{
    ui->setupUi(this);
    ui->actionCreationMode->setCheckable(true);
    ui->menuEdit->setEnabled(editingMode);
    bookView = new PagedTextEdit(this);
    connect(bookView, &PagedTextEdit::pageChanged, this, &MainWindow::displayPageNumber);
    ui->mainLayout->replaceWidget(ui->bookView, bookView);
    ui->bookView->hide();
    bookView->setFont(currentFont);

    currentBook = 0;
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

    loadBook(QString(":/text/testbook.txt"));
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    if (currentBook) {
        currentBook->setPageSize(bookView->document()->pageSize());

        pagesCount = currentBook->getPageCount();

        displayPageNumber(bookView->getPage(), bookView->getLastPage());
    }
}

void MainWindow::loadBook(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QString data(file.readAll());
    file.close();

    bookBuilder.setData(data);

    if (currentBook) {
        delete currentBook;
    }
    currentBook = bookBuilder.readBook();

    initBook();
}

void MainWindow::initBook()
{
    currentBook->setPageSize(bookView->document()->pageSize());
    currentBook->setFont(currentFont);

    pagesCount = currentBook->getPageCount();

    displayChaptersList();

    if (currentBook->getBookmarkCount() > 0) {
        goToBookmark(currentBook->getBookmark(0));
    } else {
        selectChapter(0);
    }
}

void MainWindow::saveBook(QString filename)
{
    saveChapterText();
    bookView->savePos();
    int pos = bookView->getSavedPos();
    Bookmark bookmark(currentChapterIndex, pos);
    currentBook->setBookmark(0, bookmark);
    BookBuilder::writeBook(currentBook, filename);
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
    ui->chapterList->clear();
    if (currentBook) {
        for (int i = 0; i < currentBook->getChapterCount(); i++) {
            ui->chapterList->addItem(currentBook->getChapterTitle(i));
        }
    }
}

void MainWindow::selectChapter(int index, bool save)
{
    if (currentBook && index < currentBook->getChapterCount()) {
        if (currentChapterIndex != -1 && editingModeUsed) {
            if (save) {
                saveChapterText();
            }
            editingModeUsed = editingMode;
        }
        currentChapterIndex = index;
        previousPages = currentBook->getCurrentPage(0, index);

        bookView->setText(currentBook->getChapter(index));
        if (index < ui->chapterList->count()) {
            ui->chapterList->item(index)->setSelected(true);
        }
    }
}

void MainWindow::saveChapterText()
{
    if (currentBook) {
        currentBook->setChapterText(currentChapterIndex, bookView->toPlainText());
    }
}

void MainWindow::goToBookmark(Bookmark bookmark)
{
    selectChapter(bookmark.chapterIndex);
    bookView->goToPos(bookmark.pos);
}

void MainWindow::on_actionBookInfo_triggered()
{
    if (currentBook) {
        if (editingMode) {
            BookInfoDialog *dialog = new BookInfoDialog(this);
            dialog->setTitle(currentBook->getTitle());
            dialog->setAuthor(currentBook->getAuthor());
            dialog->setAnnotation(currentBook->getAnnotation());
            int result = dialog->exec();
            if (result == QDialog::Accepted) {
                currentBook->setTitle(dialog->getTitle());
                currentBook->setAuthor(dialog->getAuthor());
                currentBook->setAnnotation(dialog->getAnnotation());
            }
            delete dialog;
        } else {
            QString text;
            text.append("Title: ");
            text.append(currentBook->getTitle());
            text.append("Author: ");
            text.append(currentBook->getAuthor());
            text.append(currentBook->getAnnotation());
            QMessageBox::information(this, "Информация о книге", text);
        }
    }
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this);
    loadBook(filename);
}

void MainWindow::on_actionSave_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this);
    saveBook(filename);
}

void MainWindow::on_chapterList_clicked(const QModelIndex &index)
{
    selectChapter(index.row());
}

void MainWindow::on_prevButton_clicked()
{
    if (bookView->getPage() == 1) {
        if (currentChapterIndex > 0) {
            int newChapter = currentChapterIndex - 1;
            selectChapter(newChapter);
            bookView->setPage(currentBook->getPageCount(newChapter));
        }
    } else {
        bookView->setPage(bookView->getPage() - 1);
    }
}

void MainWindow::on_nextButton_clicked()
{
    if (bookView->getPage() == bookView->getLastPage()) {
        if (currentChapterIndex - 1 < currentBook->getChapterCount()) {
            int newChapter = currentChapterIndex + 1;
            selectChapter(newChapter);
            bookView->setPage(1);
        }
    } else {
        bookView->setPage(bookView->getPage() + 1);
    }
}

void MainWindow::on_goToButton_clicked()
{
    int page = ui->pageSpinBox->value();
    int newChapter = currentBook->getChapterIndex(page);
    int newPage = page - currentBook->getCurrentPage(0, newChapter);
    selectChapter(newChapter);
    bookView->setPage(newPage);
}

void MainWindow::on_actionNew_triggered()
{
    if (currentBook) {
        delete currentBook;
    }
    currentBook = bookBuilder.getEmptyBook();
    initBook();
}

void MainWindow::on_actionCreationMode_triggered()
{
    editingMode = !editingMode;
    if (editingMode) {
        editingModeUsed = true;
    }
    bookView->setEditingMode(editingMode);
    ui->actionCreationMode->setChecked(editingMode);
    ui->menuEdit->setEnabled(editingMode);
}

void MainWindow::on_actionSplit_triggered()
{
    saveChapterText();
    int pos = bookView->getCurrentPos();
    currentBook->splitChapter(currentChapterIndex, pos);

    pagesCount = currentBook->getPageCount();
    displayChaptersList();
    selectChapter(currentChapterIndex + 1, false);
}

void MainWindow::on_actionMerge_triggered()
{
    saveChapterText();
    currentBook->mergeWithPreviousChapter(currentChapterIndex);

    displayChaptersList();
    selectChapter(currentChapterIndex - 1, false);
}
