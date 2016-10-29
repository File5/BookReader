#ifndef BOOK_H
#define BOOK_H

#include <QFile>
#include <QList>
#include <QString>
#include <QSize>
#include <QFont>

class BookBuilder;

struct Bookmark;

class Book : public QObject
{
    Q_OBJECT

public:
    explicit Book(QFile &file, QObject *parent = 0);
    explicit Book(const QString &data, QObject *parent = 0);
    virtual ~Book();

    const QString& getTitle() const;
    void setTitle(const QString& value);
    const QString& getAuthor() const;
    void setAuthor(const QString& value);
    const QString& getAnnotation() const;
    void setAnnotation(const QString& value);

    const QString& getChapter(int index) const;
    const QString& getChapterTitle(int index) const;
    const Bookmark getBookmark(int index) const;
    int getChapterIndex(int pageNo) const;
    int getChapterCount() const;
    int getBookmarkCount() const;
    int getPageCount(int chapterIndex) const;
    int getPageCount() const;
    int getCurrentPage(int page, int chapterIndex);

    void setBookmark(int index, Bookmark bookmark);
    void deleteBookmark(int index);

    void splitChapter(int chapterIndex, int pos);
    void mergeWithPreviousChapter(int chapterIndex);
    void setChapterText(int chapterIndex, const QString &text);

public slots:
    void setPageSize(QSizeF newSize);
    void setFont(const QFont &newFont);

private:
    explicit Book(QObject *parent = 0);

    void allocMemory();
    void parseData(const QString &data);
    void calcPageCount();

    QString *title;
    QString *author;
    QString *annotation;

    QList<QString> *chapters;
    QList<QString> *chapterTitles;
    QList<int> *pageCount;
    QList<Bookmark> *bookmarks;

    QSizeF currentSize;
    QFont currentFont;

    friend class BookBuilder;
};

struct Bookmark {
    int chapterIndex;
    int pos;

    Bookmark() :
        chapterIndex(0),
        pos(0) {}

    Bookmark(int chapterIndex, int pos) :
        chapterIndex(chapterIndex),
        pos(pos) {}
};

#endif // BOOK_H
