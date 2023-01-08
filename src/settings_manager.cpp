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



void SettingsManager::write_settings(const nlohmann::json& app_settings)
{
    close_file();
    open_file(std::ios::in | std::ios::out | std::ios::trunc);
    settings_file_ << app_settings.dump(2);
    settings_file_.flush();
    qDebug().noquote().nospace() << "Saving settings to: " << file_name_.c_str();
}



nlohmann::json SettingsManager::read_settings()
{
    std::string raw_json_string {std::istreambuf_iterator<char>{settings_file_},
                                 std::istreambuf_iterator<char>{}};

    qDebug().noquote().nospace() << "Reading settings from: " << file_name_.c_str();

    // Assumes that the user is never editting file manually
    // Any atempt to modify config file manually may results to runtime errors and other issues
    auto app_settings = nlohmann::json::parse(std::move(raw_json_string));

    return app_settings;
}



bool SettingsManager::file_is_open() const
{
    return settings_file_.is_open();
}



SettingsManager& SettingsManager::instance()
{
    static SettingsManager settings_manager {};
    return settings_manager;
}



SettingsManager::SettingsManager()
    : settings_file_ {}
    , file_name_ {get_home_dir() + "NvCtrl-Linux.json"}
{
    if (!std::filesystem::exists(get_home_dir()))
    {
        qDebug().noquote().nospace() << "Directory " << get_home_dir().c_str() << " doesn`t exists and will be created";
        std::filesystem::create_directory(get_home_dir());
    }

    if (!std::filesystem::exists(file_name_))
    {
        qDebug().noquote().nospace() << "File " << file_name_.c_str() << " doesn`t exists and will ge created";
        open_file(std::ios::out);
        write_settings(default_settings);
        close_file();
    }

    open_file(std::ios::in | std::ios::out);
}



SettingsManager::~SettingsManager()
{
    close_file();
}



void SettingsManager::open_file(std::ios::openmode open_mode)
{
    settings_file_.open(file_name_, open_mode);
}



void SettingsManager::close_file()
{
    settings_file_.close();
}



const std::string& SettingsManager::get_home_dir() const
{
#ifdef _WIN32
    static const std::string home_dir {"C:\\Users\\" + std::string{std::getenv("USERNAME")} + "\\NvCtrl-Linux\\"};
#else
    static const std::string home_dir {"/home/" + std::string{std::getenv("USER")} + "/.config/NvCtrl-Linux/"};
#endif
    return home_dir;
}
