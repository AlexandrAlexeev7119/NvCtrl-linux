#pragma once

#include <QObject>
#include <QFile>
#include <QJsonObject>

class SettingsManager : public QObject
{
    Q_OBJECT
private:
    SettingsManager();
    SettingsManager(const QString& filename);
    ~SettingsManager() = default;

public:
    void set_file_name(const QString& filename);
    QString get_file_name() const;
    void open_file(QIODevice::OpenMode open_mode);
    void close_file();

    void save_settings(const QJsonObject& settings);
    QJsonObject load_settings();

    static SettingsManager& get_instance();

signals:
    void error(const QString&);

private:
    QFile settings_file_;
};

SettingsManager& settings_manager_get_instance();
