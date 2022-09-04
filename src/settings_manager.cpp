#include <QJsonDocument>

#include "settings_manager.hpp"

// Default settings filename
static constexpr const char* DEFAULT_FILENAME{"/usr/share/gwepp/gwepp.json"};

SettingsManager::SettingsManager()
    : settings_file_{}
{}

SettingsManager::SettingsManager(const QString& filename)
    : settings_file_{filename}
{}

void SettingsManager::set_file_name(const QString& filename)
{
    settings_file_.setFileName(filename);
}

QString SettingsManager::get_file_name() const
{
    return settings_file_.fileName();
}

void SettingsManager::open_file(QIODevice::OpenMode open_mode)
{
    if (!settings_file_.open(open_mode))
    {
        emit error(settings_file_.errorString() + ": " + get_file_name());
    }
}

void SettingsManager::close_file()
{
    if (settings_file_.isOpen())
    {
        settings_file_.close();
    }
}

void SettingsManager::save_settings(const QJsonObject& settings)
{
    const QJsonDocument json_doc{settings};
    const QByteArray json_data{json_doc.toJson(QJsonDocument::JsonFormat::Compact)};
    settings_file_.write(json_data);
}

QJsonObject SettingsManager::load_settings()
{
    const QByteArray raw_data{settings_file_.readAll()};
    const QJsonObject json_obj{QJsonDocument::fromJson(raw_data).object()};
    return json_obj;
}

SettingsManager& SettingsManager::instance()
{
    static SettingsManager settings_manager{DEFAULT_FILENAME};
    return settings_manager;
}
