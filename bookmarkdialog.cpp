#include "bookmarkdialog.h"
#include "ui_bookmarkdialog.h"

BookmarkDialog::BookmarkDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookmarkDialog)
{
    ui->setupUi(this);
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &BookmarkDialog::itemSelected);
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &BookmarkDialog::filter);
}

BookmarkDialog::~BookmarkDialog()
{
    delete ui;
}

const QList<Comment> *BookmarkDialog::getComments() const
{
    return comments;
}

void BookmarkDialog::setComments(const QList<Comment> *value)
{
    comments = value;
}

Comment BookmarkDialog::getSelectedComment() const
{
    return selectedComment;
}

void BookmarkDialog::showEvent(QShowEvent *showEvent)
{
    QDialog::showEvent(showEvent);

    filter("");
}

void BookmarkDialog::itemSelected(int index)
{
    selectedComment = filtered.at(index);
}

void BookmarkDialog::filter(const QString &text)
{
    filtered.clear();

    if (text.isEmpty()) {
        foreach (Comment comment, *comments) {
            filtered.append(comment);
        }
    } else {
        foreach (Comment comment, *comments) {
            if (comment.text.find(text.toStdString()) != string::npos) {
                filtered.append(comment);
            }
        }
    }

    showFiltered();
}

void BookmarkDialog::showFiltered()
{
    ui->listWidget->clear();
    foreach (Comment comment, filtered) {
        ui->listWidget->addItem(QString(comment.text.c_str()));
    }
}
