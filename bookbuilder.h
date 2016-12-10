#ifndef BOOKBUILDER_H
#define BOOKBUILDER_H

#include <QString>
#include <string>
#include <list>

#include "book.h"

using namespace std;

class BookBuilder
{
public:
    static QList<BookCover> getBookCovers();
    static void writeBook(Book* book, const QString filename);
    BookBuilder();
    BookBuilder(const QString& data);
    ~BookBuilder();
    Book* readBook();
    Book* getEmptyBook(int chapterCount);
    void setData(const QString& data);

private:
    static BookCover readBookCover(QString filename);

    void readBookInfo();
    void readChapter();
    void readBookmarks();
    void readReferences();
    void readImages();
    void readComments();

    void procCmd();
    string readCmd();
    void unreadCmd();
    string readValue();
    string readToChar(char c, bool inc = false);

    static void removeCr(QString *string);

    string *data;
    int pos;
    int prevPos;
    int len;

    list<string*> chapters;
    string title;
    string author;
    string annotation;
    string date;
    string genre;
    list<Bookmark> bookmarks;
    list<Reference> references;
    list<Image> images;
    list<Comment> comments;
};

#endif // BOOKBUILDER_H
