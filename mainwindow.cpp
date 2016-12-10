#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include "bookinfodialog.h"
#include "loadchaptersdialog.h"
#include "addreferencedialog.h"
#include "settingsdialog.h"
#include "autoscrolldialog.h"
#include "librarydialog.h"

#include <QDebug>

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
    timer = new QTimer(this);
    statusBarLabel = new QLabel(ui->statusBar);
    ui->statusBar->addWidget(statusBarLabel);
    initSettings();

    connect(bookView, &PagedTextEdit::pageChanged, this, &MainWindow::displayPageNumber);
    connect(bookView, &PagedTextEdit::referenceClicked, this, &MainWindow::showReference);
    connect(timer, &QTimer::timeout, this, &MainWindow::on_nextButton_clicked);
    connect(bookView, &PagedTextEdit::cursorPositionChanged, this, &MainWindow::showPosInStatusBar);
    ui->mainLayout->replaceWidget(ui->bookView, bookView);
    ui->bookView->hide();
    bookView->setFont(currentFont);

    currentBook = 0;
}

MainWindow::~MainWindow()
{
    delete statusBarLabel;
    delete timer;
    delete currentBook;
    delete bookView;
    delete ui;
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);

    loadBook(QString("lastbook.txt"));
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

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveBook("lastbook.txt");

    QMainWindow::closeEvent(event);
}

void MainWindow::loadBook(QString filename)
{
    QFile file(filename);
    if (!file.exists()) return;

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
    displayBookmarkList();

    bool goToLastPos = smanager.getCurrentSettings().startWithLastPos;
    if (currentBook->getBookmarkCount() > 0 && goToLastPos) {
        goToBookmark(currentBook->getBookmark(0), false);
    } else {
        goToBookmark(Bookmark(0, 0), false);
    }
    displayPageNumber(bookView->getPage(), pagesCount);
}

void MainWindow::initSettings()
{
    smanager.readSettings();
    Settings settings = smanager.getCurrentSettings();

    if (settings.startWithEditingMode) {
        on_actionCreationMode_triggered();
    }
    currentFont.setPointSize(settings.fontSize);
}

void MainWindow::saveBook(QString filename)
{
    if (!currentBook) return;

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

void MainWindow::displayBookmarkList()
{
    ui->bookmarkList->clear();
    if (currentBook) {
        ui->bookmarkList->addItem(QString("Last close position"));
        for (int i = 1; i < currentBook->getBookmarkCount(); i++) {
            ui->bookmarkList->addItem(QString("Bookmark %1").arg(QString::number(i)));
        }
    }
}

void MainWindow::initChapterReferences()
{
    int count = currentBook->getReferencesCount();
    for (int i = 0; i < count; i++) {
        Reference reference = currentBook->getReference(i);
        if (reference.bookmark.chapterIndex == currentChapterIndex) {
            bookView->createReference(reference.bookmark.pos, reference.len, QString(reference.text.c_str()));
        }
    }
}

void MainWindow::initChapterImages()
{
    int count = currentBook->getImagesCount();
    for (int i = 0; i < count; i++) {
        Image image = currentBook->getImage(i);
        if (image.bookmark.chapterIndex == currentChapterIndex) {
            bookView->createImage(image.bookmark.pos, image.url.c_str());
        }
    }
}

void MainWindow::initChapterComments()
{
    int count = currentBook->getCommentsCount();
    for (int i = 0; i < count; i++) {
        Comment comment = currentBook->getComment(i);
        if (comment.bookmark.chapterIndex == currentChapterIndex) {
            bookView->createComment(comment.bookmark.pos, comment.len, QString(comment.text.c_str()));
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

        bookView->deselectText();
        bookView->setText(currentBook->getChapter(index));
        if (index < ui->chapterList->count()) {
            ui->chapterList->item(index)->setSelected(true);
        }
        initChapterReferences();
        initChapterImages();
        initChapterComments();
        bookView->setPage(1);
    }
}

void MainWindow::saveChapterText()
{
    if (currentBook) {
        QString text = bookView->toPlainText();
        currentBook->setChapterText(currentChapterIndex, text);
    }
}

void MainWindow::goToBookmark(Bookmark bookmark, bool save)
{
    selectChapter(bookmark.chapterIndex, save);
    bookView->goToPos(bookmark.pos);
}

void MainWindow::showReference(QString href)
{
    QMessageBox::information(this, "Information", href);
}

void MainWindow::loadSettings()
{
    smanager.readSettings();
}

void MainWindow::showPosInStatusBar()
{
    int pos = bookView->textCursor().position();
    statusBarLabel->setText(QString::number(pos));
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

void MainWindow::on_bookmarkList_clicked(const QModelIndex &index)
{
    goToBookmark(currentBook->getBookmark(index.row()));
}

void MainWindow::on_addBookmarkButton_clicked()
{
    bookView->savePos();
    int bookmarkPos = bookView->getSavedPos();
    currentBook->addBookmark(Bookmark(currentChapterIndex, bookmarkPos));
    displayBookmarkList();
}

void MainWindow::on_removeBookmarkButton_clicked()
{
    int selectedIndex = ui->bookmarkList->currentRow();
    currentBook->deleteBookmark(selectedIndex);
    displayBookmarkList();
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
        if (currentChapterIndex + 1 < currentBook->getChapterCount()) {
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

void MainWindow::on_findButton_clicked()
{
    if (currentBook) {
        QString textToFind = ui->findEdit->text();
        if (textToFind.isEmpty()) {
            bookView->deselectText();
            return;
        }

        searchResults = currentBook->findText(textToFind);

        if (!searchResults.empty()) {
            currentSearchResult = 0;
            currentSearchSelectionLength = textToFind.length();
        } else {
            currentSearchResult = -1;
            currentSearchSelectionLength = 0;
            QMessageBox::information(this, "Найти", "Совпадений нет");
            return;
        }

        bookView->savePos();
        int pos = bookView->getSavedPos();
        Bookmark currentScrollPos(currentChapterIndex, pos);
        for (int i = 1; i < searchResults.size(); i++) {
            if (currentScrollPos < searchResults.at(i)) {
                currentSearchResult = i;
                break;
            }
        }

        Bookmark currentPos = searchResults.at(currentSearchResult);
        goToBookmark(currentPos);
        bookView->selectText(currentPos.pos, currentSearchSelectionLength);
    }
}

void MainWindow::on_findNextButton_clicked()
{
    if (currentSearchResult == -1) return;

    currentSearchResult++;
    currentSearchResult %= searchResults.size();

    Bookmark currentPos = searchResults.at(currentSearchResult);
    goToBookmark(currentPos);
    bookView->selectText(currentPos.pos, currentSearchSelectionLength);
}

void MainWindow::on_findPrevButton_clicked()
{
    if (currentSearchResult == -1) return;

    currentSearchResult--;
    if (currentSearchResult < 0) {
        currentSearchResult += searchResults.size();
    }

    Bookmark currentPos = searchResults.at(currentSearchResult);
    goToBookmark(currentPos);
    bookView->selectText(currentPos.pos, currentSearchSelectionLength);
}

void MainWindow::on_actionNew_triggered()
{
    if (currentBook) {
        delete currentBook;
    }
    currentBook = bookBuilder.getEmptyBook(1);
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

void MainWindow::on_actionLoadFromFile_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this);
    if (filename.isNull()) return;

    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QString *text = new QString(file.readAll());
    bookView->setText(*text);
    delete text;
    file.close();
}

void MainWindow::on_actionLoadChapters_triggered()
{
    LoadChaptersDialog *dialog = new LoadChaptersDialog(this);
    int res = dialog->exec();
    if (res != QDialog::Accepted) return;

    QStringList files = dialog->getFilenames();
    delete dialog;

    if (currentBook) {
        delete currentBook;
    }
    currentBook = bookBuilder.getEmptyBook(files.size());
    for (int i = 0; i < files.size(); i++) {
        QString filename = files[i];
        QFile file(filename);
        file.open(QIODevice::ReadOnly);
        QString *text = new QString(file.readAll());
        file.close();
        currentBook->setChapterText(i, *text);
        delete text;
    }
    initBook();
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

void MainWindow::on_actionAddReference_triggered()
{
    AddReferenceDialog *dialog = new AddReferenceDialog(this);
    int result = dialog->exec();
    if (result == QDialog::Accepted) {
        QString refText = dialog->getText();
        Selection textSelection = bookView->getSelection();
        bookView->setSelectedAsReference(refText);
        currentBook->addReference(
            Reference(
                Bookmark(currentChapterIndex, textSelection.pos),
                textSelection.len,
                refText.toStdString()
            )
        );
    }
    delete dialog;
}

void MainWindow::on_actionDeleteReference_triggered()
{
    Selection textSelection = bookView->getSelection();
    currentBook->deleteReference(currentChapterIndex, textSelection.pos, textSelection.len);
    bookView->setSelectedAsNormalText();
}

void MainWindow::on_actionAddImage_triggered()
{
    QDir directory;
    QString filename = QFileDialog::getOpenFileName(this);
    filename = directory.relativeFilePath(filename);
    int pos = bookView->getCurrentPos();
    bookView->createImage(pos, filename);
    currentBook->addImage(Image(Bookmark(currentChapterIndex, pos), filename.toStdString()));
}

void MainWindow::on_actionDeleteImage_triggered()
{
    Selection textSelection = bookView->getSelection();
    currentBook->deleteImage(currentChapterIndex, textSelection.pos, textSelection.len);
    bookView->deleteSelectedText();
}

void MainWindow::on_actionAddComment_triggered()
{
    AddReferenceDialog *dialog = new AddReferenceDialog(this);
    int result = dialog->exec();
    if (result == QDialog::Accepted) {
        QString refText = dialog->getText();
        Selection textSelection = bookView->getSelection();
        bookView->setSelectedAsComment(refText);
        currentBook->addComment(
            Comment(
                Bookmark(currentChapterIndex, textSelection.pos),
                textSelection.len,
                refText.toStdString()
            )
        );
    }
    delete dialog;
}

void MainWindow::on_actionDeleteComment_triggered()
{
    Selection textSelection = bookView->getSelection();
    currentBook->deleteComment(currentChapterIndex, textSelection.pos, textSelection.len);
    bookView->setSelectedAsNormalText();
}

void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog *dialog = new SettingsDialog(this);
    dialog->setSettings(smanager.getCurrentSettings());
    int result = dialog->exec();
    if (result == QDialog::Accepted) {
        smanager.saveSettings(dialog->getSettings());
    }
    delete dialog;
}

void MainWindow::on_actionAutoScroll_triggered()
{
    timer->stop();
    AutoScrollDialog *dialog = new AutoScrollDialog(this);
    int result = dialog->exec();
    if (result == QDialog::Accepted) {
        timer->start(dialog->getInterval() * 1000);
    }
}

void MainWindow::on_actionLibrary_triggered()
{
    LibraryDialog *dialog = new LibraryDialog(this);

    QList<BookCover> bookCovers = BookBuilder::getBookCovers();
    dialog->setBookCovers(bookCovers);

    int result = dialog->exec();
    delete dialog;
}
