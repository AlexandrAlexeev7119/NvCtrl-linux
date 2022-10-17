#include <QMessageBox>
#include <QDebug>

#include "fan_profile_dialog.hpp"
#include "ui_fan_profile_dialog.h"

FanProfileDialog::FanProfileDialog(QWidget* parent)
    : QDialog {parent}
    , ui {new Ui::FanProfileDialog}
    , ptr_app_settings_ {}
{
    ui->setupUi(this);
    setMinimumSize(size());
    setMaximumSize(size() * 1.5);

    connect(ui->horizontalSlider_fan_speed, &QSlider::valueChanged, this, [this](int value)
    {
        ui->label_fan_speed_slider_indicator->setText(QString::number(value) + "%");
    });
}



FanProfileDialog::~FanProfileDialog()
{
    delete ui;
}



void FanProfileDialog::load_app_settings(nlohmann::json* app_settings) noexcept
{
    ptr_app_settings_ = app_settings;
}



void FanProfileDialog::on_pushButton_create_new_profile_clicked()
{
    if (ui->lineEdit_profile_name->text().isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Empty name is not allowed");
    }
    else
    {
        auto& app_settings_ref {*ptr_app_settings_};
        auto& fan_speed_profiles = app_settings_ref["fan_speed_profiles"];

        if (fan_speed_profiles.is_null())
        {
            const nlohmann::json::array_t new_array {
                {
                    {"name", ui->lineEdit_profile_name->text().toStdString()},
                    {"fan_speed", ui->horizontalSlider_fan_speed->value()}
                }
            };
            app_settings_ref["fan_speed_profiles"] = new_array;
            qDebug().noquote().nospace() << "fan_speed_profiles is null, create a new JSON array";
        }
        else
        {
            fan_speed_profiles.emplace_back(nlohmann::json::object_t {
                                                {"name", ui->lineEdit_profile_name->text().toStdString()},
                                                {"fan_speed", ui->horizontalSlider_fan_speed->value()}
                                            });
            qDebug().noquote().nospace() << "fan_speed_profiles exists, add new element";
        }

        emit new_profile_created(fan_speed_profiles);
        qInfo().noquote().nospace() << "New fan profile created: " << ui->lineEdit_profile_name->text();

        SettingsManager::instance().open_file(std::ios::out);
        SettingsManager::instance().write_settings(app_settings_ref);
        SettingsManager::instance().close_file();

        on_pushButton_close_clicked();
    }
}



void FanProfileDialog::on_pushButton_close_clicked()
{
    ui->lineEdit_profile_name->clear();
    ui->horizontalSlider_fan_speed->setValue(ui->horizontalSlider_fan_speed->minimum());
    close();
}
