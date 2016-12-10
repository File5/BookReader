#include "bookbuilder.h"

#include <cctype>
#include <fstream>

void BookBuilder::writeBook(Book *book, const QString filename)
{
    ofstream fout(filename.toStdString());
    fout << "[BookInfo]" << endl;

    fout << "[Title]" << endl;
    fout << book->title->toStdString();
    fout << "[Author]" << endl;
    fout << book->author->toStdString();
    fout << "[Annotation]" << endl;
    fout << book->annotation->toStdString();

    foreach (QString chapter, *book->chapters) {
        fout << "[Chapter]" << endl;
        fout << chapter.toStdString();
    }

    fout << "[Bookmarks]" << endl;
    foreach (Bookmark bookmark, *book->bookmarks) {
        fout << bookmark.chapterIndex << ":" << bookmark.pos << ";";
    }
    fout << endl;

    if (!book->referenses->empty()) {
        fout << "[References]" << endl;
        foreach (Reference reference, *book->referenses) {
            fout << reference.bookmark.chapterIndex << ":" <<
                    reference.bookmark.pos << ":" <<
                    reference.len << ":" <<
                    reference.text << ";" << endl;
        }
    }

    if (!book->images->empty()) {
        fout << "[Images]" << endl;
        foreach (Image image, *book->images) {
            fout << image.bookmark.chapterIndex << ":" <<
                    image.bookmark.pos << ":" <<
                    image.url << ";" << endl;
        }
    }

    if (!book->comments->empty()) {
        fout << "[Comments]" << endl;
        foreach (Comment comment, *book->comments) {
            fout << comment.bookmark.chapterIndex << ":" <<
                    comment.bookmark.pos << ":" <<
                    comment.len << ":" <<
                    comment.text << ";" << endl;
        }
    }

    fout.close();
}

BookBuilder::BookBuilder() :
    data(0),
    pos(0),
    len(0)
{

}

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
    if (data == 0) {
        return 0;
    }

    Book *book = new Book();
    while (pos < len) {
        procCmd();
    }
    book->title->append(title.c_str());
    removeCr(book->title);
    book->author->append(author.c_str());
    removeCr(book->author);
    book->annotation->append(annotation.c_str());
    removeCr(book->annotation);
    int i = 1;
    for (string *chapter : chapters) {
        QString *str = new QString(chapter->c_str());
        removeCr(str);
        book->chapters->append(*str);
        book->chapterTitles->append(QString("Chapter %1").arg(QString::number(i++)));
        delete str;
    }
    for (Bookmark bookmark : bookmarks) {
        book->bookmarks->append(bookmark);
    }
    for (Reference reference : references) {
        book->referenses->append(reference);
    }
    for (Image image : images) {
        book->images->append(image);
    }
    for (Comment comment : comments) {
        book->comments->append(comment);
    }
    return book;
}

Book *BookBuilder::getEmptyBook(int chapterCount)
{
    Book *book = new Book();
    QString chapter("");
    for (int i = 0; i< chapterCount; i++) {
        book->chapters->append(chapter);
        book->chapterTitles->append(QString("Chapter %1").arg(QString::number(i + 1)));
    }
    book->bookmarks->append(Bookmark(0, 0));
    return book;
}

void BookBuilder::setData(const QString &data)
{
    if (this->data) {
        delete this->data;
    }
    this->data = new string(data.toStdString());
    len = this->data->length();
    prevPos = 0;
    pos = 0;
    chapters.clear();
    title.clear();
    author.clear();
    annotation.clear();
    bookmarks.clear();
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
    string str = readValue();
    int chapterIndex = 0, pos = 0;
    bool chapter = true;
    for (int i = 0; i < str.length(); i++) {
        char c = str[i];
        switch (c) {
        case ':':
            chapter = false;
            break;
        case ';':
            bookmarks.push_back(Bookmark(chapterIndex, pos));
            chapterIndex = 0;
            pos = 0;
            chapter = true;
            break;
        default:
            if ('0' <= c && c <= '9') {
                if (chapter) {
                    chapterIndex *= 10;
                    chapterIndex += (c - '0');
                } else {
                    pos *= 10;
                    pos += (c - '0');
                }
            }
            break;
        }
    }
}

void BookBuilder::readReferences()
{
    enum { CHAPTER, POS, LEN, TEXT, MODES_COUNT };

    string str = readValue();
    int chapterIndex = 0, pos = 0, len = 0;
    string text;
    int mode = CHAPTER;
    for (int i = 0; i < str.length(); i++) {
        char c = str[i];
        switch (c) {
        case ':':
            mode = (mode + 1) % MODES_COUNT;
            break;
        case ';':
            references.push_back(Reference(Bookmark(chapterIndex, pos), len, text));
            chapterIndex = 0;
            pos = 0;
            len = 0;
            text.clear();
            mode = 0;
            break;
        default:
            if ('0' <= c && c <= '9') {
                if (mode == CHAPTER) {
                    chapterIndex *= 10;
                    chapterIndex += (c - '0');
                } else if (mode == POS) {
                    pos *= 10;
                    pos += (c - '0');
                } else if (mode == LEN) {
                    len *= 10;
                    len += (c - '0');
                } else {
                    text.push_back(c);
                }
            } else if (mode == TEXT) {
                text.push_back(c);
            }
            break;
        }
    }
}

void BookBuilder::readImages()
{
    enum { CHAPTER, POS, URL, MODES_COUNT };

    string str = readValue();
    int chapterIndex = 0, pos = 0;
    string url;
    int mode = CHAPTER;
    for (int i = 0; i < str.length(); i++) {
        char c = str[i];
        switch (c) {
        case ':':
            mode = (mode + 1) % MODES_COUNT;
            break;
        case ';':
            images.push_back(Image(Bookmark(chapterIndex, pos), url));
            chapterIndex = 0;
            pos = 0;
            len = 0;
            url.clear();
            mode = 0;
            break;
        default:
            if ('0' <= c && c <= '9') {
                if (mode == CHAPTER) {
                    chapterIndex *= 10;
                    chapterIndex += (c - '0');
                } else if (mode == POS) {
                    pos *= 10;
                    pos += (c - '0');
                } else {
                    url.push_back(c);
                }
            } else if (mode == URL) {
                url.push_back(c);
            }
            break;
        }
    }
}

void BookBuilder::readComments()
{
    enum { CHAPTER, POS, LEN, TEXT, MODES_COUNT };

    string str = readValue();
    int chapterIndex = 0, pos = 0, len = 0;
    string text;
    int mode = CHAPTER;
    for (int i = 0; i < str.length(); i++) {
        char c = str[i];
        switch (c) {
        case ':':
            mode = (mode + 1) % MODES_COUNT;
            break;
        case ';':
            comments.push_back(Comment(Bookmark(chapterIndex, pos), len, text));
            chapterIndex = 0;
            pos = 0;
            len = 0;
            text.clear();
            mode = 0;
            break;
        default:
            if ('0' <= c && c <= '9') {
                if (mode == CHAPTER) {
                    chapterIndex *= 10;
                    chapterIndex += (c - '0');
                } else if (mode == POS) {
                    pos *= 10;
                    pos += (c - '0');
                } else if (mode == LEN) {
                    len *= 10;
                    len += (c - '0');
                } else {
                    text.push_back(c);
                }
            } else if (mode == TEXT) {
                text.push_back(c);
            }
            break;
        }
    }
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
    } else if (cmd == "References") {
        readReferences();
    } else if (cmd == "Images") {
        readImages();
    } else if (cmd == "Comments") {
        readComments();
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

void BookBuilder::removeCr(QString *string)
{
    *string = string->remove(QRegExp(QString("\r")));
}
