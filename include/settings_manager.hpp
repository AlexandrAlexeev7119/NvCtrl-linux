#pragma once

#include <fstream>

#include "nlohmann/json.hpp"

class SettingsManager final
{
public:
    static const nlohmann::json default_settings;

    void write_settings(const nlohmann::json& app_settings);
    nlohmann::json read_settings();
    bool file_is_open() const;

    static SettingsManager& instance();

private:
    std::fstream settings_file_;
    std::string file_name_;

    SettingsManager();
    ~SettingsManager();

    void open_file(std::ios::openmode open_mode);
    void close_file();
    const std::string& get_home_dir() const;
};
