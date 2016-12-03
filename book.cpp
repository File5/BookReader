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
    delete bookmarks;
    delete referenses;
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

const Bookmark Book::getBookmark(int index) const
{
    return bookmarks->at(index);
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

int Book::getBookmarkCount() const
{
    return bookmarks->size();
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

int Book::getReferencesCount()
{
    return referenses->size();
}

Reference Book::getReference(int index)
{
    return referenses->at(index);
}

void Book::addReference(const Reference &reference)
{
    referenses->append(reference);
}

void Book::deleteReference(int chapterIndex, int pos)
{
    for (int i = 0; i < referenses->size(); i++) {
        if (referenses->at(i).contains(chapterIndex, pos)) {
            referenses->removeAt(i);
            return;
        }
    }
}

void Book::addBookmark(Bookmark bookmark)
{
    bookmarks->append(bookmark);
}

void Book::setBookmark(int index, Bookmark bookmark)
{
    bookmarks->removeAt(index);
    bookmarks->insert(index, bookmark);
}

void Book::deleteBookmark(int index)
{
    bookmarks->removeAt(index);
}

void Book::splitChapter(int chapterIndex, int pos)
{
    QString text = chapters->at(chapterIndex);
    QString newChapter = text.right(text.length() - pos);
    text.resize(pos);
    chapters->replace(chapterIndex, text);
    chapters->insert(chapterIndex + 1, newChapter);

    chapterTitles->insert(chapterIndex + 1, QString("New chapter"));
    calcPageCount();
}

void Book::mergeWithPreviousChapter(int chapterIndex)
{
    QString text = chapters->at(chapterIndex - 1);
    text.append(chapters->at(chapterIndex));
    chapters->replace(chapterIndex - 1, text);
    chapters->removeAt(chapterIndex);

    chapterTitles->removeAt(chapterIndex);
    calcPageCount();
}

void Book::setChapterText(int chapterIndex, const QString &text)
{
    chapters->replace(chapterIndex, text);

    calcPageCount();
}

QList<Bookmark> Book::findText(QString text)
{
    QList<Bookmark> results;
    int textLength = text.length();

    for (int chapterIndex = 0; chapterIndex < getChapterCount(); chapterIndex++) {
        QString chapter = getChapter(chapterIndex);
        int chapterLength = chapter.length();

        int textIndex = 0;
        while (textIndex != -1 && textIndex < chapterLength) {
            textIndex = chapter.indexOf(text, textIndex);
            if (textIndex != -1) {
                results.append(Bookmark(chapterIndex, textIndex));
                textIndex += textLength;
            }
        }
    }

    return results;
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
    bookmarks = new QList<Bookmark>();
    referenses = new QList<Reference>();
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
