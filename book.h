#ifndef BOOK_H
#define BOOK_H

#include <QFile>
#include <QList>
#include <QString>
#include <QSize>
#include <QFont>

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
    int getChapterCount() const;
    int getPageCount(int chapterIndex) const;

public slots:
    void setPageSize(QSizeF newSize);
    void setFont(const QFont &newFont);

private:
    void allocMemory();
    void parseData(const QString &data);
    void calcPageCount();

    QString *title;
    QString *author;
    QString *annotation;

    QList<QString> *chapters;
    QList<QString> *chapterTitles;
    QList<int> *pageCount;

    QSizeF currentSize;
    QFont currentFont;
};

#endif // BOOK_H
