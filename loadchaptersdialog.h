#ifndef LOADCHAPTERSDIALOG_H
#define LOADCHAPTERSDIALOG_H

#include <QDialog>

namespace Ui {
class LoadChaptersDialog;
}

class LoadChaptersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoadChaptersDialog(QWidget *parent = 0);
    ~LoadChaptersDialog();

    QStringList getFilenames() const;

private slots:
    void on_chooseFile_clicked();

private:
    Ui::LoadChaptersDialog *ui;
};

#endif // LOADCHAPTERSDIALOG_H
