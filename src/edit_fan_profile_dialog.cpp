#include <QShowEvent>

#include "edit_fan_profile_dialog.hpp"
#include "ui_edit_fan_profile_dialog.h"

EditFanProfileDialog::EditFanProfileDialog(QWidget* parent)
    : QDialog {parent}
    , ui {new Ui::EditFanProfileDialog}
    , app_settings_ {}
{
    ui->setupUi(this);
    connect(ui->horizontalSlider_fan_speed_level, &QSlider::valueChanged, this, [this](int value)
    {
        ui->label_fan_speed_level_slider_indicator->setText(QString::number(value) + "%");
    });
}

void EditFanProfileDialog::set_current_fan_profile_index(unsigned index) noexcept
{
    current_fan_profile_index_ = index;
}

EditFanProfileDialog::~EditFanProfileDialog()
{
    delete ui;
}

void EditFanProfileDialog::on_buttonBox_accepted()
{
    auto& current_fan_profile = app_settings_["fan_speed_profiles"][current_fan_profile_index_];
    current_fan_profile["fan_speed"] = ui->horizontalSlider_fan_speed_level->value();

    emit current_fan_profile_changed(current_fan_profile);

    SettingsManager::instance().open_file(std::ios::out);
    SettingsManager::instance().write_settings(app_settings_);
    SettingsManager::instance().close_file();
}

void EditFanProfileDialog::on_buttonBox_rejected()
{
    close();
}

void EditFanProfileDialog::showEvent(QShowEvent* event_)
{
    SettingsManager::instance().open_file(std::ios::in);
    app_settings_ = nlohmann::json::parse(SettingsManager::instance().read_settings());
    SettingsManager::instance().close_file();

    auto& current_fan_profile = app_settings_["fan_speed_profiles"][current_fan_profile_index_];

    ui->label_current_profile_name->setText(QString::fromStdString(current_fan_profile["name"].get<std::string>()));
    ui->horizontalSlider_fan_speed_level->setValue(current_fan_profile["fan_speed"].get<unsigned>());

    event_->accept();
}
