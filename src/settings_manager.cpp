#include <stdexcept>
#include <QJsonDocument>

#include "settings_manager.hpp"

SettingsManager::SettingsManager()
    : config_file_{}
{}

void SettingsManager::set_file_name(const QString& filename)
{
    config_file_.setFileName(filename);
}

void SettingsManager::open_file(QIODevice::OpenMode open_mode)
{
    if (!config_file_.open(open_mode))
    {
        throw std::runtime_error{config_file_.errorString().toStdString()};
    }
}

void SettingsManager::close_file()
{
    if (config_file_.isOpen())
    {
        config_file_.close();
    }
}

void SettingsManager::save_settings(const QJsonObject& settings)
{
    const QJsonDocument json_doc{settings};
    const QByteArray json_data{json_doc.toJson(QJsonDocument::JsonFormat::Compact)};
    config_file_.write(json_data);
}

QJsonObject SettingsManager::load_settings()
{
    const QByteArray raw_data{config_file_.readAll()};
    const QJsonObject json_obj{QJsonDocument::fromJson(raw_data).object()};
    return json_obj;
}
