#include "pagedtextedit.h"

#include <QScrollBar>

PagedTextEdit::PagedTextEdit(QWidget *parent) :
    QTextEdit(parent)
{
    setReadOnly(true);
    setTextInteractionFlags(0);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(verticalScrollBar(), &QScrollBar::rangeChanged, this, &PagedTextEdit::updateScrollBar);
    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &PagedTextEdit::onScrolled);
}

int PagedTextEdit::getPage()
{
    QTextDocument *doc = document();
    int pageHeight = doc->pageSize().height();
    int pos = verticalScrollBar()->value();

    return pos / pageHeight + 1;
}

void PagedTextEdit::setPage(int page)
{
    QTextDocument *doc = document();
    int pageHeight = doc->pageSize().height();

    verticalScrollBar()->setValue(pageHeight * (page - 1));
}

int PagedTextEdit::getLastPage()
{
    QTextDocument *doc = document();
    int pageHeight = doc->pageSize().height();
    int documentHeight = pageHeight * (doc->pageCount() - 1);

    return documentHeight / pageHeight + 1;
}

void PagedTextEdit::resizeEvent(QResizeEvent *event)
{
    document()->setPageSize(event->size());
}

void PagedTextEdit::updateScrollPos()
{
    QTextDocument *doc = document();
    int pageHeight = doc->pageSize().height();
    int documentHeight = pageHeight * (doc->pageCount() - 1);

    QScrollBar *scrollBar = verticalScrollBar();
    double pos = scrollBar->value() / (double) documentHeight;

    int newPos = scrollBar->value() / pageHeight;
    scrollBar->setValue(newPos * pageHeight);
}

void PagedTextEdit::updateScrollBar(int min, int max)
{
    Q_UNUSED(min) Q_UNUSED(max)

    QScrollBar *scrollBar = verticalScrollBar();
    QTextDocument *doc = document();
    int pageHeight = doc->pageSize().height();
    int documentHeight = pageHeight * (doc->pageCount() - 1);
    scrollBar->setMaximum(documentHeight);
    scrollBar->setSingleStep(pageHeight);
    scrollBar->setPageStep(pageHeight);

    updateScrollPos();

    int pos = scrollBar->value();
    emit pageChanged(pos / pageHeight + 1, documentHeight / pageHeight + 1);
}

void PagedTextEdit::onScrolled(int pos)
{
    QTextDocument *doc = document();
    int pageHeight = doc->pageSize().height();
    int documentHeight = pageHeight * (doc->pageCount() - 1);

    emit pageChanged(pos / pageHeight + 1, documentHeight / pageHeight + 1);
}
