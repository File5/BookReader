#include "book.h"

#include <QTextDocument>

Book::Book(QFile &file, QObject *parent) :
    QObject(parent)
{
    if (!file.isOpen()) {
        file.open(QIODevice::ReadOnly);
    }
    QString *data = new QString(file.readAll());
    file.close();

    allocMemory();
    parseData(*data);

    delete data;
}

Book::Book(const QString &data, QObject *parent) :
    QObject(parent)
{
    allocMemory();
    parseData(data);
}

Book::~Book()
{
    delete title;
    delete author;
    delete annotation;

    delete chapters;
    delete chapterTitles;
    delete pageCount;
}

const QString &Book::getTitle() const
{
    return *title;
}

void Book::setTitle(const QString &value)
{
    *title = value;
}

const QString &Book::getAuthor() const
{
    return *author;
}

void Book::setAuthor(const QString &value)
{
    *author = value;
}
const QString &Book::getAnnotation() const
{
    return *annotation;
}

void Book::setAnnotation(const QString &value)
{
    *annotation = value;
}

const QString &Book::getChapter(int index) const
{
    return chapters->at(index);
}

const QString &Book::getChapterTitle(int index) const
{
    return chapterTitles->at(index);
}

int Book::getChapterIndex(int pageNo) const
{
    int len = 0;
    for (int i = 0; i < getChapterCount(); i++) {
        int currentPageCount = getPageCount(i);
        if (len + currentPageCount >= pageNo) {
            return i;
        }
        len += currentPageCount;
    }
    return getChapterCount() - 1;
}

int Book::getChapterCount() const
{
    return chapters->size();
}

int Book::getPageCount(int chapterIndex) const
{
    return pageCount->at(chapterIndex);
}

int Book::getPageCount() const
{
    int len = 0;
    for (int i = 0; i < getChapterCount(); i++) {
        len += getPageCount(i);
    }
    return len;
}

int Book::getCurrentPage(int page, int chapterIndex)
{
    int len = 0;
    for (int i = 0; i < getChapterCount() && i < chapterIndex; i++) {
        len += getPageCount(i);
    }
    return len + page;
}

void Book::setPageSize(QSizeF newSize)
{
    if (newSize != currentSize) {
        currentSize = newSize;
        calcPageCount();
    }
}

void Book::setFont(const QFont& newFont)
{
    if (newFont != currentFont) {
        currentFont = newFont;
        calcPageCount();
    }
}

Book::Book(QObject *parent) :
    QObject(parent)
{
    allocMemory();
}

void Book::allocMemory()
{
    title = new QString();
    author = new QString();
    annotation = new QString();

    chapters = new QList<QString>();
    chapterTitles = new QList<QString>();
    pageCount = new QList<int>();
}

void Book::parseData(const QString &data)
{
    *chapters << data;
}

void Book::calcPageCount()
{
    QTextDocument *doc = new QTextDocument(this);
    doc->setPageSize(currentSize);
    doc->setDefaultFont(currentFont);
    pageCount->clear();

    foreach (const QString &chapter, *chapters) {
        doc->setPlainText(chapter);
        int p = doc->pageCount();
        pageCount->append(p);
    }

    delete doc;
}
