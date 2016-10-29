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
    static void writeBook(Book* book, const QString filename);
    BookBuilder();
    BookBuilder(const QString& data);
    ~BookBuilder();
    Book* readBook();
    Book* getEmptyBook();
    void setData(const QString& data);

private:
    void readBookInfo();
    void readChapter();
    void readBookmarks();

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
    list<Bookmark> bookmarks;
};

#endif // BOOKBUILDER_H
