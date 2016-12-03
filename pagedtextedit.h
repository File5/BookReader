#ifndef PAGEDTEXTEDIT_H
#define PAGEDTEXTEDIT_H

#include <QTextEdit>
#include <QResizeEvent>

class PagedTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    PagedTextEdit(QWidget *parent = Q_NULLPTR);
    int getPage();
    void setPage(int page);
    int getLastPage();
    void goToPos(int pos);
    int getSavedPos();
    int getCurrentPos();
    void savePos();

    void createReference(int pos1, int len, const QString& href);

public slots:
    void setEditingMode(bool editingEnabled);
    void selectText(int pos1, int len);
    void deselectText();

signals:
    void pageChanged(int currentPage, int lastPage);
    void referenceClicked(QString href);

protected:
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);

private slots:
    void updateScrollPos();
    void updateScrollBar(int min, int max);
    void onScrolled(int pos);

private:
    int docPos;
    bool editingMode;

    QTextCharFormat defaultCharFormat;
    QTextCharFormat referenceCharFormat;
};

#endif // PAGEDTEXTEDIT_H
