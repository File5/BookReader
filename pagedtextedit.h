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

signals:
    void pageChanged(int currentPage, int lastPage);

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void updateScrollPos();
    void updateScrollBar(int min, int max);
    void onScrolled(int pos);
};

#endif // PAGEDTEXTEDIT_H
