#include <QShowEvent>
#include <QMessageBox>

#include "settings_manager.hpp"

#include "edit_clock_offset_profile_dialog.hpp"
#include "ui_edit_clock_offset_profile_dialog.h"

EditClockOffsetProfileDialog::EditClockOffsetProfileDialog(QWidget* parent)
    : QDialog {parent}
    , ui {new Ui::EditClockOffsetProfileDialog}
    , current_clock_offset_profile_index_ {0}
    , ptr_app_settings_ {}
{
    ui->setupUi(this);
    setMinimumSize(size());
    setMaximumSize(size() * 1.2);

    connect(ui->horizontalSlider_gpu_clock_offset, &QSlider::valueChanged, this, [this](int value)
    {
        ui->label_gpu_clock_offset_indicator->setText(QString::number(value) + " HMz");
    });
    connect(ui->horizontalSlider_mem_clock_offset, &QSlider::valueChanged, this, [this](int value)
    {
        ui->label_mem_clock_offset_indicator->setText(QString::number(value) + " HMz");
    });
}



EditClockOffsetProfileDialog::~EditClockOffsetProfileDialog()
{
    delete ui;
}



void EditClockOffsetProfileDialog::load_app_settings(nlohmann::json* app_settings) noexcept
{
    ptr_app_settings_ = app_settings;
}



void EditClockOffsetProfileDialog::set_current_clock_offset_profile_index(unsigned index) noexcept
{
    current_clock_offset_profile_index_= index;
}



void EditClockOffsetProfileDialog::on_buttonBox_accepted()
{
    if (ui->lineEdit_current_profile_name->text().isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Empty name is not allowed");
    }
    else
    {
        auto& current_clock_offset_profile = ptr_app_settings_->at("clock_offset_profiles")[current_clock_offset_profile_index_];

        current_clock_offset_profile["name"] = ui->lineEdit_current_profile_name->text().toStdString();
        current_clock_offset_profile["gpu_clock_offset"] = ui->horizontalSlider_gpu_clock_offset->value();
        current_clock_offset_profile["mem_clock_offset"] = ui->horizontalSlider_mem_clock_offset->value();

        emit current_clock_offset_profile_changed(current_clock_offset_profile);
        write_settings_to_file(*ptr_app_settings_);

        on_buttonBox_rejected();
    }
}



void EditClockOffsetProfileDialog::on_buttonBox_rejected()
{
    ui->lineEdit_current_profile_name->clear();
    ui->horizontalSlider_gpu_clock_offset->setValue(0);
    ui->horizontalSlider_mem_clock_offset->setValue(0);
    ui->label_gpu_clock_offset_indicator->clear();
    ui->label_mem_clock_offset_indicator->clear();
    close();
}



void EditClockOffsetProfileDialog::showEvent(QShowEvent* event_)
{
    const auto& current_clock_offset_profile = ptr_app_settings_->at("clock_offset_profiles")[current_clock_offset_profile_index_];
    const int gpu_clock_offset {current_clock_offset_profile["gpu_clock_offset"].get<int>()};
    const int mem_clock_offset {current_clock_offset_profile["mem_clock_offset"].get<int>()};

    ui->lineEdit_current_profile_name->setText(QString::fromStdString(current_clock_offset_profile["name"].get<std::string>()));
    ui->horizontalSlider_gpu_clock_offset->setValue(gpu_clock_offset);
    ui->horizontalSlider_mem_clock_offset->setValue(mem_clock_offset);
    ui->label_gpu_clock_offset_indicator->setText(QString::number(gpu_clock_offset) + " MHz");
    ui->label_mem_clock_offset_indicator->setText(QString::number(mem_clock_offset) + " MHz");

    event_->accept();
}



void EditClockOffsetProfileDialog::write_settings_to_file(const nlohmann::json& app_settings)
{
    SettingsManager::instance().write_settings(app_settings);
}



void EditClockOffsetProfileDialog::on_pushButton_remove_current_profile_clicked()
{
    auto& clock_offset_profiles = ptr_app_settings_->at("clock_offset_profiles");

    clock_offset_profiles.erase(current_clock_offset_profile_index_);
    emit current_clock_offset_profile_removed();
    write_settings_to_file(*ptr_app_settings_);

    on_buttonBox_rejected();
}
