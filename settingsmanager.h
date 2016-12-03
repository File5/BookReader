#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QString>

struct Settings {
    bool startWithLastPos;
    bool startWithEditingMode;
    int fontSize;

    Settings() {
        startWithLastPos = true;
        startWithEditingMode = false;
        fontSize = 16;
    }
};

class SettingsManager
{
public:
    SettingsManager();
    SettingsManager(const QString &filename);
    Settings getCurrentSettings() const;

    void readSettings();
    void saveSettings(Settings settings);
    void writeSettings();

private:
    QString filename;
    Settings currentSettings;
};

#endif // SETTINGSMANAGER_H
