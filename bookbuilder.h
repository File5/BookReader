#ifndef BOOKBUILDER_H
#define BOOKBUILDER_H

#include <QString>
#include <string>
#include <list>

using namespace std;

class BookBuilder
{
public:
    BookBuilder(const QString& data);
    ~BookBuilder();
    void readBook();

private:
    void readBookInfo();
    void readChapter();
    void readBookmarks();

    void procCmd();
    string readCmd();
    void unreadCmd();
    string readValue();
    string readToChar(char c, bool inc = false);

    string *data;
    int pos;
    int prevPos;
    int len;

    list<string*> chapters;
    string title;
    string author;
    string annotation;
};

#endif // BOOKBUILDER_H
