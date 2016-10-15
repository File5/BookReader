#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pagedtextedit.h"

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

private slots:
    void displayPageNumber(int current, int lastPage);
    void testBook();
};

#endif // MAINWINDOW_H
