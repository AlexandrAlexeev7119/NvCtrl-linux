#pragma once

#include <memory>
#include <fstream>

#include <QObject>

#include "nlohmann/json.hpp"

class SettingsManager : public QObject
{
    Q_OBJECT
private:
    SettingsManager();
    ~SettingsManager() = default;

public:
    static const nlohmann::json default_settings;

    std::string_view get_file_name() const; // prevent full std::string copy

    void write_settings(const nlohmann::json& app_settings);
    nlohmann::json read_settings();

    static SettingsManager& instance();

signals:
    void error_occured(const QString&);

private:
    std::unique_ptr<std::fstream> ptr_settings_file_;
    std::string file_name_;

    void open_file(std::ios::openmode open_mode);
    void close_file();

    const std::string& get_home_dir() const;
};
