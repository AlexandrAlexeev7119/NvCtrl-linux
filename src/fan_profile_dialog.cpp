#include "fan_profile_dialog.hpp"
#include "ui_fan_profile_dialog.h"

FanProfileDialog::FanProfileDialog(QWidget* parent)
    : QDialog {parent}
    , ui {new Ui::FanProfileDialog}
{
    ui->setupUi(this);
    connect(ui->horizontalSlider_fan_speed, &QSlider::valueChanged, this, [this](int value) {
        ui->label_fan_speed_slider_indicator->setText(QString::number(value) + "%");
    });
}

FanProfileDialog::~FanProfileDialog()
{
    delete ui;
}

void FanProfileDialog::on_pushButton_create_new_profile_clicked()
{
    SettingsManager::instance().open_file(std::ios::in);
    auto app_settings {nlohmann::json::parse(SettingsManager::instance().read_settings())};
    SettingsManager::instance().close_file();

    if (app_settings["fan_speed_profiles"].is_null())
    {
        const nlohmann::json::array_t new_array {
            nlohmann::json::object_t {
                {"name", ui->label_profile_name->text().toStdString()},
                {"fan_speed", ui->horizontalSlider_fan_speed->value()}
            }
        };
        app_settings["fan_speed_profiles"] = new_array;
    }
    else
    {
        auto fan_speed_profiles {app_settings["fan_speed_profiles"]};
        fan_speed_profiles.emplace_back(nlohmann::json::object_t {
                                            {"name", ui->label_profile_name->text().toStdString()},
                                            {"fan_speed", ui->horizontalSlider_fan_speed->value()}
                                        });
        app_settings["fan_speed_profiles"] = fan_speed_profiles;
    }

    SettingsManager::instance().open_file(std::ios::out);
    SettingsManager::instance().write_settings(app_settings);
    SettingsManager::instance().close_file();
}

void FanProfileDialog::on_pushButton_close_clicked()
{
    close();
}
