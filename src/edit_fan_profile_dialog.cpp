#include <QShowEvent>

#include "edit_fan_profile_dialog.hpp"
#include "ui_edit_fan_profile_dialog.h"

EditFanProfileDialog::EditFanProfileDialog(QWidget* parent)
    : QDialog {parent}
    , ui {new Ui::EditFanProfileDialog}
    , ptr_app_settings_ {}
{
    ui->setupUi(this);
    setMinimumSize(size());
    setMaximumSize(size() * 1.5);

    connect(ui->horizontalSlider_fan_speed_level, &QSlider::valueChanged, this, [this](int value)
    {
        ui->label_fan_speed_level_slider_indicator->setText(QString::number(value) + "%");
    });
}



EditFanProfileDialog::~EditFanProfileDialog()
{
    delete ui;
}



void EditFanProfileDialog::set_current_fan_profile_index(unsigned index) noexcept
{
    current_fan_profile_index_ = index;
}



void EditFanProfileDialog::load_app_settins(nlohmann::json* app_settings) noexcept
{
    ptr_app_settings_ = app_settings;
}



void EditFanProfileDialog::on_buttonBox_accepted()
{
    auto& app_settings_ref {*ptr_app_settings_};
    auto& current_fan_profile = app_settings_ref["fan_speed_profiles"][current_fan_profile_index_];

    if (ui->checkBox_remove_current_profile->isChecked())
    {
        auto& fan_profiles = app_settings_ref["fan_speed_profiles"];
        fan_profiles.erase(current_fan_profile_index_);
        emit current_fan_profile_removed();
    }
    else
    {
        current_fan_profile["fan_speed"] = ui->horizontalSlider_fan_speed_level->value();
        current_fan_profile["name"] = ui->lineEdit_current_profile_name->text().toStdString();
        emit current_fan_profile_changed(current_fan_profile);
    }

    SettingsManager::instance().open_file(std::ios::out);
    SettingsManager::instance().write_settings(app_settings_ref);
    SettingsManager::instance().close_file();

    close();
}



void EditFanProfileDialog::on_buttonBox_rejected()
{
    close();
}



void EditFanProfileDialog::showEvent(QShowEvent* event_)
{
    auto& current_fan_profile = ptr_app_settings_->at("fan_speed_profiles")[current_fan_profile_index_];

    ui->lineEdit_current_profile_name->setText(QString::fromStdString(current_fan_profile["name"].get<std::string>()));
    ui->horizontalSlider_fan_speed_level->setValue(current_fan_profile["fan_speed"].get<unsigned>());

    event_->accept();
}



void EditFanProfileDialog::closeEvent(QCloseEvent* event_)
{
    ui->lineEdit_current_profile_name->clear();
    ui->horizontalSlider_fan_speed_level->setValue(0);
    ui->checkBox_remove_current_profile->setChecked(false);
    event_->accept();
}
