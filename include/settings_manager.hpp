#pragma once

#include <QObject>
#include <QFile>
#include <QJsonObject>

class SettingsManager
{
public:
    SettingsManager();
    ~SettingsManager() = default;

    void set_file_name(const QString& filename);
    void open_file(QIODevice::OpenMode open_mode);
    void close_file();

    void save_settings(const QJsonObject& settings);
    QJsonObject load_settings();

private:
    QFile config_file_;
};
