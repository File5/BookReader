#ifndef LIBRARYDIALOG_H
#define LIBRARYDIALOG_H

#include <QDialog>
#include "book.h"
#include <set>

using namespace std;

namespace Ui {
class LibraryDialog;
}

class LibraryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LibraryDialog(QWidget *parent = 0);
    ~LibraryDialog();

    QList<BookCover> getBookCovers() const;
    void setBookCovers(const QList<BookCover> &value);

protected:
    void showEvent(QShowEvent *showEvent);

private:
    Ui::LibraryDialog *ui;
    QList<BookCover> bookCovers;
    QList<BookCover> filtered;
    set<string> titles;
    set<string> authors;
    set<string> dates;
    set<string> genres;

private slots:
    void initFilter();
    void filter(int indexChanged);
    void showFiltered();
};

#endif // LIBRARYDIALOG_H
