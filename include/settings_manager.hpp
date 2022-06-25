#pragma once

#include <QObject>
#include <QFile>
#include <QJsonObject>

namespace settings_manager_details
{
    static const QString filename{"./gwepp.json"};
}

class SettingsManager : public QObject
{
    Q_OBJECT
public:
    SettingsManager();
    ~SettingsManager() = default;

    void set_file_name(const QString& filename);
    void open_file(QIODevice::OpenMode open_mode);
    void close_file();

    void save_settings(const QJsonObject& settings);
    QJsonObject load_settings();

    static SettingsManager& get_instance();

signals:
    void error(const QString&);

private:
    QFile config_file_;
};

SettingsManager& settings_manager_get_instance();
