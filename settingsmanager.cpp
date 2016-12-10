#include "settingsmanager.h"

#include <fstream>

using std::ofstream;
using std::ifstream;

SettingsManager::SettingsManager()
{
    filename = "./settings.txt";
}

SettingsManager::SettingsManager(const QString &filename)
{
    this->filename = filename;
}

Settings SettingsManager::getCurrentSettings() const
{
    return currentSettings;
}

void SettingsManager::readSettings()
{
    ifstream fin(filename.toStdString().c_str());
    fin >> currentSettings.startWithLastPos >>
           currentSettings.startWithEditingMode >>
           currentSettings.fontSize >>
           currentSettings.fontRed >>
           currentSettings.fontGreen >>
           currentSettings.fontBlue >>
           currentSettings.bold >>
           currentSettings.italic >>
           currentSettings.lineInterval >>
           currentSettings.alignment;
    fin.close();
}

void SettingsManager::saveSettings(Settings settings)
{
    currentSettings = settings;
    writeSettings();
}

void SettingsManager::writeSettings()
{
    ofstream fout(filename.toStdString().c_str());
    fout << currentSettings.startWithLastPos << " " <<
            currentSettings.startWithEditingMode << " " <<
            currentSettings.fontSize << " " <<
            currentSettings.fontRed << " " <<
            currentSettings.fontGreen << " " <<
            currentSettings.fontBlue << " " <<
            currentSettings.bold << " " <<
            currentSettings.italic << " " <<
            currentSettings.lineInterval << " " <<
            currentSettings.alignment;
    fout.close();
}
