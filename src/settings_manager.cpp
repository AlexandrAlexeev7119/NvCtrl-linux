#include <cstdlib>

#include <QJsonDocument>

#include "settings_manager.hpp"


static constexpr const char* DEFAULT_FILENAME{"/usr/share/gwepp/gwepp.json"};

static const auto get_filename_in_home_dir {
    []() -> QString {
        QString current_user {std::getenv("USER")};
        return "/home/" + current_user + "/.config/gwepp/gwepp.json";
    }
};



SettingsManager::SettingsManager()
    : settings_file_{}
{
    settings_file_.setFileName(get_filename_in_home_dir());
}

QString SettingsManager::get_file_name() const
{ return settings_file_.fileName(); }

void SettingsManager::open_file(QIODevice::OpenMode open_mode)
{
    // First of all, try to open settings file from /home/<user>/.config/gwepp/
    if (!settings_file_.open(open_mode))
    {
        // Otherwise try to open from default location /usr/share/gwepp/
        settings_file_.setFileName(DEFAULT_FILENAME);
        if (!settings_file_.open(open_mode))
        {
            emit error(settings_file_.errorString() + ": " + get_file_name());
        }
    }

    qInfo().noquote().nospace() << "Settings file opened: " << get_file_name();
}

void SettingsManager::close_file()
{
    if (settings_file_.isOpen())
    {
        settings_file_.close();
    }
}

void SettingsManager::write_settings(const QJsonObject& settings)
{
    const QJsonDocument json_doc{settings};
    const QByteArray json_data{json_doc.toJson(QJsonDocument::JsonFormat::Compact)};
    settings_file_.write(json_data);
}

QJsonObject SettingsManager::read_settings()
{
    const QByteArray raw_data{settings_file_.readAll()};
    const QJsonObject json_obj{QJsonDocument::fromJson(raw_data).object()};
    return json_obj;
}

SettingsManager& SettingsManager::instance()
{
    static SettingsManager settings_manager{};
    return settings_manager;
}
