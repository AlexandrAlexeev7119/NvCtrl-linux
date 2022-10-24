#include <cstdlib>
#include <iterator>

#include <QDebug>

#include "settings_manager.hpp"

static constexpr const char* DEFAULT_FILENAME{"/usr/share/gwepp/gwepp.json"};



const nlohmann::json SettingsManager::default_settings
{
    {
        {"update_freq_ms", 1000},
        {"minimize_to_tray_on_close", false},
        {"minimize_to_tray_on_startup", false},
        {"last_fan_and_clock_offset_profiles_saved", false},
        {"last_power_profile_saved", false},
        {"fan_speed_profiles", nlohmann::json::array_t { {{"name", "Auto (VBIOS controlled) (default)"}} } },
        {"clock_offset_profiles", nlohmann::json::array_t { {{"name", "None (default)"}} } }
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



std::string_view SettingsManager::get_file_name() const
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
            emit error("Failed to open file: " + QString::fromStdString(file_name_));
        }
    }
}



void SettingsManager::close_file()
{
    ptr_settings_file_->close();
}



void SettingsManager::write_settings(const nlohmann::json& settings)
{
    (*ptr_settings_file_) << settings.dump(4);
    qDebug().noquote().nospace() << "Save settings to: " << file_name_.c_str();
}



std::string SettingsManager::read_settings()
{
    std::string raw_json_string {std::istreambuf_iterator<char>{*ptr_settings_file_},
                                 std::istreambuf_iterator<char>{}};
    qDebug().noquote().nospace() << "Read settings from: " << file_name_.c_str();
    return raw_json_string;
}



SettingsManager& SettingsManager::instance()
{
    static SettingsManager settings_manager {};
    return settings_manager;
}



std::string SettingsManager::get_filename_in_home_dir() const
{
    const std::string username {std::getenv("USER")};
    return "/home/" + username + "/.config/gwepp/gwepp.json";
}
