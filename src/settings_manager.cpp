#include <cstdlib>
#include <filesystem>
#include <iterator>

#include <QDebug>

#include "settings_manager.hpp"




const nlohmann::json SettingsManager::default_settings
{
    {"check_for_updates_on_startup", false},
    {"branch_where_get_updates", 0},
    {"minimize_to_tray_on_close", false},
    {"minimize_to_tray_on_startup", false},

    {"last_fan_profile_index", 0},
    {"last_fan_profile_saved", false},

    {"last_clock_offset_profile_index", 0},
    {"last_clock_offset_profile_saved", false},

    {"last_power_profile_value", 0},
    {"last_power_profile_saved", false},

    {"fan_speed_profiles", nlohmann::json::array_t { {{"name", "Auto (VBIOS controlled) (default)"}} } },
    {"clock_offset_profiles", nlohmann::json::array_t { {{"name", "None (default)"}} } },

    {"update_freq_ms", 1000}
};



SettingsManager::SettingsManager()
    : ptr_settings_file_ {std::make_unique<std::fstream>()}
    , file_name_ {}
{
    if (!std::filesystem::exists(get_home_dir()))
    {
        std::filesystem::create_directory(get_home_dir());
        qDebug().noquote().nospace() << "Directory " << get_home_dir().c_str() << " doesn`t exists and will be created";
    }
    file_name_ = get_home_dir() + "gwepp.json";
}



std::string_view SettingsManager::get_file_name() const
{
    return file_name_;
}



void SettingsManager::write_settings(const nlohmann::json& app_settings)
{
    if (!ptr_settings_file_->is_open())
    {
        open_file(std::ios::out);
    }
    (*ptr_settings_file_) << app_settings.dump(4);
    close_file();
    qDebug().noquote().nospace() << "Save settings to: " << file_name_.c_str();
}



nlohmann::json SettingsManager::read_settings()
{
    open_file(std::ios::in | std::ios::out);

    std::string raw_json_string {std::istreambuf_iterator<char>{*ptr_settings_file_},
                                 std::istreambuf_iterator<char>{}};

    qDebug().noquote().nospace() << "Read settings from: " << file_name_.c_str();

    // Assumes that the user is never editting file manually
    // Any atempt to modify config file manually may results to runtime errors and other issues
    auto app_settings = nlohmann::json::parse(std::move(raw_json_string));

    // But user can manually edit value of timer update frequency
    // To prevent this, next code validate app settings before it returned to caller
    const unsigned update_freq_ms {app_settings["update_freq_ms"].get<unsigned>()};
    if (update_freq_ms < 500 || update_freq_ms > 3000)
    {
        app_settings["update_freq_ms"] = default_settings["update_freq_ms"].get<unsigned>();
        write_settings(app_settings);
        qWarning().noquote().nospace() << "Wrong update_freq_ms_ detected, fallback to default ("
                                       << default_settings["update_freq_ms"].get<unsigned>() << ")";
    }

    close_file();

    return app_settings;
}



SettingsManager& SettingsManager::instance()
{
    static SettingsManager settings_manager {};
    return settings_manager;
}



void SettingsManager::open_file(std::ios::openmode open_mode)
{
    ptr_settings_file_->open(file_name_, open_mode);
    if (!ptr_settings_file_->is_open())
    {
        qDebug().noquote().nospace() << "File " << file_name_.c_str() << " doesn`t exists and will ge created";
        write_settings(default_settings);
        ptr_settings_file_->open(file_name_, open_mode);
    }
}



void SettingsManager::close_file()
{
    ptr_settings_file_->close();
}



std::string& SettingsManager::get_home_dir() const
{
    static std::string home_dir {"/home/" + std::string{std::getenv("USER")} + "/.config/gwepp/"};
    return home_dir;
}
