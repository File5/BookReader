#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "settingsmanager.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

    Settings getSettings() const;
    void setSettings(const Settings &value);

protected:
    void showEvent(QShowEvent *event);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SettingsDialog *ui;
    Settings settings;
};

#endif // SETTINGSDIALOG_H
