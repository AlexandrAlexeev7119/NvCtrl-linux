#include <QMessageBox>
#include <QDebug>

#include "clock_profile_dialog.hpp"
#include "ui_clock_profile_dialog.h"

ClockProfileDialog::ClockProfileDialog(QWidget* parent)
    : QDialog {parent}
    , ui {new Ui::ClockProfileDialog}
{
    ui->setupUi(this);
    setMinimumSize(size());
    setMaximumSize(size() * 1.2);

    connect(ui->horizontalSlider_gpu_clock_offset, &QSlider::valueChanged, this, [this](int value)
    {
        ui->label_gpu_clock_offset_indicator->setText(QString::number(value) + " MHz");
    });
    connect(ui->horizontalSlider_mem_clock_offset, &QSlider::valueChanged, this, [this](int value)
    {
        ui->label_mem_clock_offset_indicator->setText(QString::number(value) + " MHz");
    });
}



ClockProfileDialog::~ClockProfileDialog()
{
    delete ui;
}



void ClockProfileDialog::load_app_settings(nlohmann::json* app_settings) noexcept
{
    ptr_app_settings_ = app_settings;
}



void ClockProfileDialog::on_buttonBox_accepted()
{
    if (ui->lineEdit_profile_name->text().isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Empty name is not allowed");
    }
    else
    {
        auto& app_settings_ref {*ptr_app_settings_};
        auto& clock_offset_profiles = app_settings_ref["clock_offset_profiles"];

        if (clock_offset_profiles.is_null())
        {
            const nlohmann::json new_array {
                {
                    {"name", ui->lineEdit_profile_name->text().toStdString()},
                    {"gpu_clock_offset", ui->horizontalSlider_gpu_clock_offset->value()},
                    {"mem_clock_offset", ui->horizontalSlider_mem_clock_offset->value()}
                }
            };
            app_settings_ref["clock_offset_profiles"] = new_array;
            qDebug().noquote().nospace() << "clock_offset_profiles is null, create a new JSON array";
        }
        else
        {
            clock_offset_profiles.emplace_back(nlohmann::json::object_t{
                                                   {"name", ui->lineEdit_profile_name->text().toStdString()},
                                                   {"gpu_clock_offset", ui->horizontalSlider_gpu_clock_offset->value()},
                                                   {"mem_clock_offset", ui->horizontalSlider_mem_clock_offset->value()}
                                               });
            qDebug().noquote().nospace() << "clock_offset_profiles exists, add new element";
        }

        emit new_profile_created(clock_offset_profiles.back());

        SettingsManager::instance().open_file(std::ios::out);
        SettingsManager::instance().write_settings(app_settings_ref);
        SettingsManager::instance().close_file();

        qInfo().noquote().nospace() << "New clock profile created: " << ui->lineEdit_profile_name->text();
        on_buttonBox_rejected();
    }
}



void ClockProfileDialog::on_buttonBox_rejected()
{
    ui->lineEdit_profile_name->clear();
    ui->horizontalSlider_gpu_clock_offset->setValue(0);
    ui->horizontalSlider_mem_clock_offset->setValue(0);
    close();
}
