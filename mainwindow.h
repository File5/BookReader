#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pagedtextedit.h"
#include "book.h"
#include "bookbuilder.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void showEvent(QShowEvent * event);
    virtual void resizeEvent(QResizeEvent * event);
    virtual void closeEvent(QCloseEvent * event);

private:
    void loadBook(QString filename);
    void initBook();
    void saveBook(QString filename);

    Ui::MainWindow *ui;
    PagedTextEdit *bookView;
    QFont currentFont;
    Book *currentBook;
    int currentChapterIndex;
    int previousPages;
    int pagesCount;
    BookBuilder bookBuilder;

    bool editingMode;
    bool editingModeUsed;

private slots:
    void displayPageNumber(int current, int lastPage);
    void displayChaptersList();
    void displayBookmarkList();
    void selectChapter(int index, bool save = true);
    void saveChapterText();
    void goToBookmark(Bookmark bookmark, bool save = true);

    void on_actionBookInfo_triggered();
    void on_actionExit_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_chapterList_clicked(const QModelIndex &index);
    void on_bookmarkList_clicked(const QModelIndex &index);
    void on_addBookmarkButton_clicked();
    void on_removeBookmarkButton_clicked();
    void on_prevButton_clicked();
    void on_nextButton_clicked();
    void on_goToButton_clicked();
    void on_findButton_clicked();

    void on_actionNew_triggered();
    void on_actionCreationMode_triggered();
    void on_actionLoadFromFile_triggered();
    void on_actionLoadChapters_triggered();
    void on_actionSplit_triggered();
    void on_actionMerge_triggered();
};

#endif // MAINWINDOW_H
