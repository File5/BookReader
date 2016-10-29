#include "pagedtextedit.h"

#include <QScrollBar>

PagedTextEdit::PagedTextEdit(QWidget *parent) :
    QTextEdit(parent),
    docPos(0),
    editingMode(false)
{
    setEditingMode(editingMode);
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
    int documentHeight = pageHeight * (doc->pageCount() - 1);

    verticalScrollBar()->setValue(pageHeight * (page - 1));

    emit pageChanged(page, documentHeight / pageHeight + 1);
}

int PagedTextEdit::getLastPage()
{
    QTextDocument *doc = document();
    int pageHeight = doc->pageSize().height();
    int documentHeight = pageHeight * (doc->pageCount() - 1);

    return documentHeight / pageHeight + 1;
}

void PagedTextEdit::goToPos(int pos)
{
    docPos = pos;
    updateScrollPos();
}

int PagedTextEdit::getSavedPos()
{
    return docPos;
}

int PagedTextEdit::getCurrentPos()
{
    return textCursor().position();
}

void PagedTextEdit::savePos()
{
    docPos = cursorForPosition(QPoint(0, 0)).position();
}

void PagedTextEdit::setEditingMode(bool editingEnabled)
{
    editingMode = editingEnabled;
    if (editingMode) {
        setReadOnly(false);
        setTextInteractionFlags(Qt::TextEditorInteraction);
    } else {
        setReadOnly(true);
        setTextInteractionFlags(0);
    }
}

void PagedTextEdit::resizeEvent(QResizeEvent *event)
{
    QTextEdit::resizeEvent(event);
    document()->setPageSize(event->size());
    updateScrollPos();
}

void PagedTextEdit::updateScrollPos()
{
    QTextDocument *doc = document();
    int pageHeight = doc->pageSize().height();

    QScrollBar *scrollBar = verticalScrollBar();

    QTextCursor cursor = textCursor();
    if (docPos != -1 && cursor.position() != docPos) {
        cursor.setPosition(docPos);
    }
    QRect rect = cursorRect(cursor);
    int pos = rect.bottom();
    int newPos = pos / pageHeight;
    scrollBar->setValue(newPos * pageHeight);
}

void PagedTextEdit::updateScrollBar(int min, int max)
{
    Q_UNUSED(min) Q_UNUSED(max)

    QScrollBar *scrollBar = verticalScrollBar();
    QTextDocument *doc = document();
    int pageHeight = doc->pageSize().height();
    int documentHeight = pageHeight * (doc->pageCount() - 1);

    if (max == documentHeight) return;

    int pos = scrollBar->value();
    scrollBar->setMaximum(documentHeight);
    scrollBar->setSingleStep(pageHeight);
    scrollBar->setPageStep(pageHeight);
    scrollBar->setValue(pos);

    emit pageChanged(pos / pageHeight + 1, documentHeight / pageHeight + 1);
}

void PagedTextEdit::onScrolled(int pos)
{
    QTextDocument *doc = document();
    int pageHeight = doc->pageSize().height();
    int documentHeight = pageHeight * (doc->pageCount() - 1);

    emit pageChanged(pos / pageHeight + 1, documentHeight / pageHeight + 1);
}
