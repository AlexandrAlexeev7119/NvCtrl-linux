#include <QMessageBox>
#include <QDebug>

#include "fan_profile_dialog.hpp"
#include "ui_fan_profile_dialog.h"

FanProfileDialog::FanProfileDialog(QWidget* parent)
    : QDialog {parent}
    , ui {new Ui::FanProfileDialog}
    , ptr_app_settings_ {nullptr}
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
    const QString new_fan_profile_name {ui->lineEdit_profile_name->text()};

    if (new_fan_profile_name.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Empty name is not allowed");
    }
    else
    {
        auto& app_settings_ref {*ptr_app_settings_};
        auto& fan_speed_profiles = app_settings_ref["fan_speed_profiles"];

        fan_speed_profiles.emplace_back(nlohmann::json::object_t {
                                            {"name", new_fan_profile_name.toStdString()},
                                            {"fan_speed", ui->horizontalSlider_fan_speed->value()}
                                        });
        emit new_profile_created(fan_speed_profiles.back());

        SettingsManager::instance().write_settings(*ptr_app_settings_);

        qInfo().noquote().nospace() << "New fan profile created: " << new_fan_profile_name;
        on_pushButton_close_clicked();
    }
}



void FanProfileDialog::on_pushButton_close_clicked()
{
    ui->lineEdit_profile_name->clear();
    ui->horizontalSlider_fan_speed->setValue(ui->horizontalSlider_fan_speed->minimum());
    close();
}
