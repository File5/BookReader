#include "bookbuilder.h"

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

void BookBuilder::readBookInfo()
{
    for (int i = 0; i < 3; i++) {
        string cmd = readCmd();
        if (cmd == "title") {
            title = readValue();
        } else if (cmd == "author") {
            author = readValue();
        } else if (cmd == "annotation") {
            annotation = readValue();
        } else {
            pos -= cmd.length() + 2;
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
    return readToChar(']');
}

string BookBuilder::readValue()
{
    return readToChar('[');
}

string BookBuilder::readToChar(char c)
{
    char cur = '\0';
    string res;
    while (cur != c || pos < len) {
        cur = data->at(pos++);
        if (cur == c) {
            pos++;
            break;
        }
        res.push_back(cur);
    }
    return res;
}
