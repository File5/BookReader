#include "bookbuilder.h"

#include <cctype>

BookBuilder::BookBuilder(const QString &data) :
    pos(0)
{
    this->data = new string(data.toStdString());
    len = this->data->length();
}

BookBuilder::~BookBuilder()
{
    delete data;
}

Book *BookBuilder::readBook()
{
    Book *book = new Book();
    while (pos < len) {
        procCmd();
    }
    book->title->append(title.c_str());
    book->author->append(author.c_str());
    book->annotation->append(annotation.c_str());
    for (string *chapter : chapters) {
        book->chapters->append(QString(chapter->c_str()));
    }
    return book;
}

void BookBuilder::readBookInfo()
{
    for (int i = 0; i < 3; i++) {
        string cmd = readCmd();
        if (cmd == "Title") {
            title = readValue();
        } else if (cmd == "Author") {
            author = readValue();
        } else if (cmd == "Annotation") {
            annotation = readValue();
        } else {
            unreadCmd();
            break;
        }
    }
}

void BookBuilder::readChapter()
{
    chapters.push_back(new string(readValue()));
}

void BookBuilder::readBookmarks()
{
    // TODO bookmarks
}

void BookBuilder::procCmd()
{
    string cmd = readCmd();
    if (cmd == "BookInfo") {
        readBookInfo();
    } else if (cmd == "Chapter") {
        readChapter();
    } else if (cmd == "Bookmarks") {
        readBookmarks();
    }
}

string BookBuilder::readCmd()
{
    prevPos = pos;
    while (pos < len && isspace(data->at(pos))) {
        pos++;
    }
    if (pos < len && data->at(pos) == '[') {
        pos++;
    }
    string res = readToChar(']', true);
    while (pos < len && isspace(data->at(pos))) {
        pos++;
    }
    return res;
}

void BookBuilder::unreadCmd()
{
    pos = prevPos;
}

string BookBuilder::readValue()
{
    return readToChar('[');
}

string BookBuilder::readToChar(char c, bool inc)
{
    char cur = '\0';
    string res;
    while (cur != c && pos < len) {
        cur = data->at(pos++);
        if (cur == c) {
            if (!inc) pos--;
            break;
        }
        res.push_back(cur);
    }
    return res;
}
