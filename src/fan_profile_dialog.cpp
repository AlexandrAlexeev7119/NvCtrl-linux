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
    auto app_settings = nlohmann::json::parse(SettingsManager::instance().read_settings());
    SettingsManager::instance().close_file();

    create_or_apped_fan_speed_profile(app_settings);
    emit new_profile_created(app_settings);
    qInfo().noquote().nospace() << "New fan profile created: " << ui->lineEdit_profile_name->text();

    SettingsManager::instance().open_file(std::ios::out);
    SettingsManager::instance().write_settings(app_settings);
    SettingsManager::instance().close_file();

    on_pushButton_close_clicked();
}

void FanProfileDialog::on_pushButton_close_clicked()
{
    ui->lineEdit_profile_name->clear();
    ui->horizontalSlider_fan_speed->setValue(ui->horizontalSlider_fan_speed->minimum());
    close();
}

void FanProfileDialog::create_or_apped_fan_speed_profile(nlohmann::json& app_settings)
{
    if (app_settings["fan_speed_profiles"].is_null())
    {
        const nlohmann::json::array_t new_array {
            {
                {"name", ui->lineEdit_profile_name->text().toStdString()},
                {"fan_speed", ui->horizontalSlider_fan_speed->value()}
            }
        };
        app_settings["fan_speed_profiles"] = new_array;
        qDebug().noquote().nospace() << "fan_speed_profiles is null, create a new JSON array";
    }
    else
    {
        auto fan_speed_profiles = app_settings["fan_speed_profiles"];
        fan_speed_profiles.emplace_back(nlohmann::json::object_t {
                                            {"name", ui->lineEdit_profile_name->text().toStdString()},
                                            {"fan_speed", ui->horizontalSlider_fan_speed->value()}
                                        });
        app_settings["fan_speed_profiles"] = fan_speed_profiles;
        qDebug().noquote().nospace() << "fan_speed_profiles exists, add new element";
    }
}
