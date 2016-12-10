#include "pagedtextedit.h"

#include <QScrollBar>
#include <QTextDocumentFragment>

PagedTextEdit::PagedTextEdit(QWidget *parent) :
    QTextEdit(parent),
    docPos(0),
    editingMode(false)
{
    setEditingMode(editingMode);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(verticalScrollBar(), &QScrollBar::rangeChanged, this, &PagedTextEdit::updateScrollBar);
    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &PagedTextEdit::onScrolled);

    QPalette p = palette();
    p.setColor(QPalette::Highlight, QColor(255, 255, 0));
    p.setColor(QPalette::HighlightedText, QColor(0, 0, 0));
    setPalette(p);

    defaultCharFormat = textCursor().charFormat();

    referenceCharFormat = QTextCharFormat(defaultCharFormat);
    referenceCharFormat.setAnchor(true);
    referenceCharFormat.setFontUnderline(true);

    commentCharFormat = QTextCharFormat(defaultCharFormat);
    commentCharFormat.setAnchor(true);
    commentCharFormat.setBackground(QBrush(QColor(200, 200, 200)));
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

void PagedTextEdit::createReference(int pos1, int len, const QString& href)
{
    QTextCursor cursor = textCursor();
    cursor.setPosition(pos1);
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, len);
    QTextCharFormat format = QTextCharFormat(referenceCharFormat);
    format.setAnchorHref(href);
    cursor.setCharFormat(format);
    cursor.clearSelection();
}

void PagedTextEdit::setSelectedAsReference(const QString &href)
{
    QTextCursor cursor = textCursor();
    QTextCharFormat format = QTextCharFormat(referenceCharFormat);
    format.setAnchorHref(href);
    cursor.setCharFormat(format);
}

void PagedTextEdit::setSelectedAsNormalText()
{
    QTextCursor cursor = textCursor();
    cursor.setCharFormat(defaultCharFormat);
}

void PagedTextEdit::createImage(int pos, QString filename)
{
    QTextDocumentFragment fragment = QTextDocumentFragment::fromHtml(
        QString("<img src='") + filename + QString("'>")
    );
    QTextCursor cursor = textCursor();
    cursor.setPosition(pos);
    cursor.insertHtml(QString("<img src='") + filename + QString("'>"));
    //cursor.insertFragment(fragment);
}

void PagedTextEdit::deleteSelectedText()
{
    QTextCursor cursor = textCursor();
    cursor.removeSelectedText();
}

void PagedTextEdit::createComment(int pos1, int len, const QString &href)
{
    QTextCursor cursor = textCursor();
    cursor.setPosition(pos1);
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, len);
    QTextCharFormat format = QTextCharFormat(commentCharFormat);
    format.setAnchorHref(href);
    cursor.setCharFormat(format);
    cursor.clearSelection();
}

void PagedTextEdit::setSelectedAsComment(const QString &href)
{
    QTextCursor cursor = textCursor();
    QTextCharFormat format = QTextCharFormat(commentCharFormat);
    format.setAnchorHref(href);
    cursor.setCharFormat(format);
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

void PagedTextEdit::selectText(int pos1, int len)
{
    deselectText();
    QTextCursor cursor = textCursor();
    cursor.setPosition(pos1);
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, len);
    setTextCursor(cursor);
}

Selection PagedTextEdit::getSelection()
{
    QTextCursor cursor = textCursor();
    Selection selection;
    int begin = cursor.selectionStart();
    int end = cursor.selectionEnd();
    if (begin > end) {
        int tmp = begin;
        begin = end;
        end = tmp;
    }
    selection.pos = begin;
    selection.len = end - begin;
    return selection;
}

void PagedTextEdit::deselectText()
{
    QTextCursor cursor = textCursor();
    cursor.clearSelection();
    cursor.setCharFormat(defaultCharFormat);
    setTextCursor(cursor);
}

void PagedTextEdit::resizeEvent(QResizeEvent *event)
{
    QTextEdit::resizeEvent(event);
    document()->setPageSize(event->size());
    updateScrollPos();
}

void PagedTextEdit::mousePressEvent(QMouseEvent *event)
{
    QTextEdit::mousePressEvent(event);

    QString href = anchorAt(event->pos());
    if (!href.isEmpty()) {
        emit referenceClicked(href);
    }
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
