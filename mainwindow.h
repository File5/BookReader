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

private:
    Ui::MainWindow *ui;
    PagedTextEdit *bookView;
    Book *currentBook;
    BookBuilder bookBuilder;

private slots:
    void displayPageNumber(int current, int lastPage);
    void on_actionBookInfo_triggered();
    void on_actionExit_triggered();
};

#endif // MAINWINDOW_H
