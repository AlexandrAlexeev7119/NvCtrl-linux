#include <cstdlib>
#include <iterator>

#include <QDebug>

#include "settings_manager.hpp"

static constexpr const char* DEFAULT_FILENAME{"/usr/share/gwepp/gwepp.json"};

static std::string get_filename_in_home_dir()
{
    const std::string username {std::getenv("USER")};
    return "/home/" + username + "/.config/gwepp/gwepp.json";
}



const nlohmann::json SettingsManager::default_settings
{
    {
        {"update_freq_ms", 500},
        {"minimize_to_tray_on_close", false},
        {"minimize_to_tray_on_startup", false},
        {"last_fan_and_clock_offset_profiles_saved", false},
        {"fan_speed_profiles", nullptr},
        {"clock_offset_profiles", nullptr},
    }
};



SettingsManager::SettingsManager()
    : ptr_settings_file_ {std::make_unique<std::fstream>()}
    , file_name_ {get_filename_in_home_dir()}
{ }

void SettingsManager::set_file_name(std::string_view file_name)
{
    file_name_ = file_name;
}

std::string SettingsManager::get_file_name() const
{
    return file_name_;
}

void SettingsManager::open_file(std::ios::openmode open_mode)
{
    // First of all, try to open settings file from /home/<user>/.config/gwepp/
    ptr_settings_file_->open(file_name_, open_mode);
    if (!ptr_settings_file_->is_open())
    {
        // Otherwise try to open from default location /usr/share/gwepp/
        file_name_ = DEFAULT_FILENAME;
        ptr_settings_file_->open(file_name_, open_mode);
        if (!ptr_settings_file_->is_open())
        {
            emit error("Failed to open file: " + QString::fromStdString(get_file_name()));
        }
    }
}

void SettingsManager::close_file()
{
    ptr_settings_file_->close();
}

void SettingsManager::write_settings(const nlohmann::json& settings)
{
    (*ptr_settings_file_) << settings;
    qInfo().noquote().nospace() << "Save settings to: " << get_file_name().c_str();
}

std::string SettingsManager::read_settings()
{
    std::string raw_json {std::istream_iterator<char>{*ptr_settings_file_},
                          std::istream_iterator<char>{}};
    qInfo().noquote().nospace() << "Read settings from: " << get_file_name().c_str();
    return raw_json;
}

SettingsManager& SettingsManager::instance()
{
    static SettingsManager settings_manager{};
    return settings_manager;
}
