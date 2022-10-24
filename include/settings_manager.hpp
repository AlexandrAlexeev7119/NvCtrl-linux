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

    void set_file_name(std::string_view file_name);
    std::string_view get_file_name() const; // prevent full std::string copy
    void open_file(std::ios::openmode open_mode);
    void close_file();

    void write_settings(const nlohmann::json& settings);
    std::string read_settings();

    static SettingsManager& instance();

signals:
    void error(const QString&);

private:
    std::unique_ptr<std::fstream> ptr_settings_file_;
    std::string file_name_;

    std::string get_filename_in_home_dir() const;
};
