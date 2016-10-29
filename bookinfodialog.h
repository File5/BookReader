#ifndef BOOKINFODIALOG_H
#define BOOKINFODIALOG_H

#include <QDialog>

namespace Ui {
class BookInfoDialog;
}

class BookInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookInfoDialog(QWidget *parent = 0);
    ~BookInfoDialog();

    QString getTitle() const;
    QString getAuthor() const;
    QString getAnnotation() const;
    void setTitle(const QString &value);
    void setAuthor(const QString &value);
    void setAnnotation(const QString &value);

protected:
    void showEvent(QShowEvent *showEvent);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::BookInfoDialog *ui;
    QString title;
    QString author;
    QString annotation;
};

#endif // BOOKINFODIALOG_H
