#include <QShowEvent>
#include <QDebug>

#include "settings_dialog.hpp"
#include "ui_settings_dialog.h"

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog {parent}
    , ui {new Ui::SettingsDialog}
    , ptr_app_settings_ {nullptr}
{
    ui->setupUi(this);
    setMinimumSize(size());
    setMaximumSize(size());
}



SettingsDialog::~SettingsDialog()
{
    delete ui;
}



void SettingsDialog::load_app_settins(nlohmann::json* app_settings) noexcept
{
    ptr_app_settings_ = app_settings;
}



void SettingsDialog::on_buttonBox_accepted()
{
    auto& app_settings {*ptr_app_settings_};

    app_settings["minimize_to_tray_on_startup"] = ui->checkBox_minimize_to_tray_on_startup->isChecked();
    app_settings["minimize_to_tray_on_close"] = ui->checkBox_minimize_to_tray_on_close->isChecked();
    app_settings["last_fan_profile_saved"] = ui->checkBox_save_last_used_fan_profile->isChecked();
    app_settings["last_clock_offset_profile_saved"] = ui->checkBox_save_last_used_clock_offset_profile->isChecked();
    app_settings["last_power_profile_saved"] = ui->checkBox_save_last_used_power_profile->isChecked();
    app_settings["update_freq_ms"] = ui->spinBox_update_freq->value();

    save_settings_to_file(app_settings);
    emit settings_applied(app_settings);

    on_buttonBox_rejected();
}



void SettingsDialog::on_buttonBox_rejected()
{
    close();
}



void SettingsDialog::showEvent(QShowEvent* event_)
{
    const auto& app_settings {*ptr_app_settings_};

    ui->checkBox_minimize_to_tray_on_startup->setChecked(app_settings["minimize_to_tray_on_startup"].get<bool>());
    ui->checkBox_minimize_to_tray_on_close->setChecked(app_settings["minimize_to_tray_on_close"].get<bool>());
    ui->checkBox_save_last_used_fan_profile->setChecked(app_settings["last_fan_profile_saved"].get<bool>());
    ui->checkBox_save_last_used_clock_offset_profile->setChecked(app_settings["last_clock_offset_profile_saved"].get<bool>());
    ui->checkBox_save_last_used_power_profile->setChecked(app_settings["last_power_profile_saved"].get<bool>());
    ui->spinBox_update_freq->setValue(app_settings["update_freq_ms"].get<unsigned>());

    qInfo().noquote().nospace() << "Settings on form loaded";
    event_->accept();
}



void SettingsDialog::save_settings_to_file(const nlohmann::json& app_settings)
{
    SettingsManager::instance().open_file(std::ios::out);
    SettingsManager::instance().write_settings(app_settings);
    SettingsManager::instance().close_file();
    qInfo().noquote().nospace() << "Settings saved to file";
}
