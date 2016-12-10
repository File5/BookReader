#ifndef BOOKMARKDIALOG_H
#define BOOKMARKDIALOG_H

#include <QDialog>
#include "book.h"

namespace Ui {
class BookmarkDialog;
}

class BookmarkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookmarkDialog(QWidget *parent = 0);
    ~BookmarkDialog();

    const QList<Comment> *getComments() const;
    void setComments(const QList<Comment> *value);

    Comment getSelectedComment() const;

protected:
    void showEvent(QShowEvent *showEvent);

private:
    Ui::BookmarkDialog *ui;
    const QList<Comment> *comments;
    QList<Comment> filtered;
    Comment selectedComment;

private slots:
    void itemSelected(int index);
    void filter(const QString &text);
    void showFiltered();
};

#endif // BOOKMARKDIALOG_H
