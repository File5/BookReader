#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

Settings SettingsDialog::getSettings() const
{
    return settings;
}

void SettingsDialog::on_buttonBox_accepted()
{
    settings.fontSize = ui->fontSize->value();
    settings.startWithLastPos = ui->startWithLastPos->isChecked();
    settings.startWithEditingMode = ui->startEditingMode->isChecked();
}

void SettingsDialog::setSettings(const Settings &value)
{
    settings = value;
}

void SettingsDialog::showEvent(QShowEvent *event)
{
    ui->fontSize->setValue(settings.fontSize);
    ui->startWithLastPos->setChecked(settings.startWithLastPos);
    ui->startEditingMode->setChecked(settings.startWithEditingMode);
}
