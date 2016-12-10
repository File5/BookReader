#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    ui->alignment->addItem("Left");
    ui->alignment->addItem("Center");
    ui->alignment->addItem("Right");
    ui->alignment->addItem("Justify");
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
    settings.fontRed = ui->fontRed->value();
    settings.fontGreen = ui->fontGreen->value();
    settings.fontBlue = ui->fontBlue->value();
    settings.bold = ui->bold->isChecked();
    settings.italic = ui->italic->isChecked();
    settings.lineInterval = ui->lineInterval->value();
    settings.alignment = ui->alignment->currentIndex();
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
    ui->fontRed->setValue(settings.fontRed);
    ui->fontGreen->setValue(settings.fontGreen);
    ui->fontBlue->setValue(settings.fontBlue);
    ui->bold->setChecked(settings.bold);
    ui->italic->setChecked(settings.italic);
    ui->lineInterval->setValue(settings.lineInterval);
    ui->alignment->setCurrentIndex(settings.alignment);
}
