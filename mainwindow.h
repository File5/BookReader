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

private:
    void loadBook(QFile file);

    Ui::MainWindow *ui;
    PagedTextEdit *bookView;
    QFont currentFont;
    Book *currentBook;
    int currentChapterIndex;
    int previousPages;
    int pagesCount;
    BookBuilder bookBuilder;

private slots:
    void displayPageNumber(int current, int lastPage);
    void displayChaptersList();
    void selectChapter(int index);

    void on_actionBookInfo_triggered();
    void on_actionExit_triggered();
    void on_chapterList_clicked(const QModelIndex &index);
    void on_prevButton_clicked();
    void on_nextButton_clicked();
    void on_goToButton_clicked();
};

#endif // MAINWINDOW_H
