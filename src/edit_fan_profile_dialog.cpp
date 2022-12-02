#include <QShowEvent>
#include <QMessageBox>

#include "settings_manager.hpp"

#include "edit_fan_profile_dialog.hpp"
#include "ui_edit_fan_profile_dialog.h"

EditFanProfileDialog::EditFanProfileDialog(QWidget* parent)
    : QDialog {parent}
    , ui {new Ui::EditFanProfileDialog}
    , current_fan_profile_index_ {0}
    , ptr_app_settings_ {}
{
    ui->setupUi(this);
    setMinimumSize(size());
    setMaximumSize(size() * 1.2);

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



void EditFanProfileDialog::load_app_settings(nlohmann::json* app_settings) noexcept
{
    ptr_app_settings_ = app_settings;
}



void EditFanProfileDialog::on_buttonBox_accepted()
{
    if (ui->lineEdit_current_profile_name->text().isEmpty())
    {
        QMessageBox::warning(this, QStringLiteral("Warning"), QStringLiteral("Empty name is not allowed"));
    }
    else
    {
        auto& current_fan_profile = ptr_app_settings_->at("fan_speed_profiles")[current_fan_profile_index_];
        current_fan_profile["fan_speed"] = ui->horizontalSlider_fan_speed_level->value();
        current_fan_profile["name"] = ui->lineEdit_current_profile_name->text().toStdString();

        emit current_fan_profile_changed(current_fan_profile);
        write_new_settings(*ptr_app_settings_);

        on_buttonBox_rejected();
    }
}



void EditFanProfileDialog::on_buttonBox_rejected()
{
    ui->lineEdit_current_profile_name->clear();
    ui->horizontalSlider_fan_speed_level->setValue(0);
    close();
}



void EditFanProfileDialog::on_pushButton_remove_current_profile_clicked()
{
    auto& fan_profiles = ptr_app_settings_->at("fan_speed_profiles");

    fan_profiles.erase(current_fan_profile_index_);
    emit current_fan_profile_removed();
    write_new_settings(*ptr_app_settings_);

    on_buttonBox_rejected();
}



void EditFanProfileDialog::showEvent(QShowEvent* event_)
{
    const auto& current_fan_profile = ptr_app_settings_->at("fan_speed_profiles")[current_fan_profile_index_];
    ui->lineEdit_current_profile_name->setText(QString::fromStdString(current_fan_profile["name"].get<std::string>()));
    ui->horizontalSlider_fan_speed_level->setValue(current_fan_profile["fan_speed"].get<unsigned>());

    event_->accept();
}



void EditFanProfileDialog::write_new_settings(const nlohmann::json& app_settings)
{
    SettingsManager::instance().write_settings(app_settings);
}
