#include "settings_manager.hpp"

SettingsManager::SettingsManager()
    : file_{}
{}

SettingsManager::~SettingsManager()
{
    file_.close();
}
