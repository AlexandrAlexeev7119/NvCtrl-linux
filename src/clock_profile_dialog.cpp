#include <cmath>

#include <QMessageBox>
#include <QDebug>

#include "clock_profile_dialog.hpp"
#include "ui_clock_profile_dialog.h"

ClockProfileDialog::ClockProfileDialog(QWidget* parent)
    : QDialog {parent}
    , ui {new Ui::ClockProfileDialog}
    , ptr_app_settings_ {nullptr}
{
    ui->setupUi(this);
    setMinimumSize(size());
    setMaximumSize(size() * 1.2);
    connect_signals_and_slots();
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
    const QString new_clock_profile_name {ui->lineEdit_new_profile_name->text()};

    if (new_clock_profile_name.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Empty name is not allowed");
    }
    else
    {
        auto& app_settings_ref {*ptr_app_settings_};
        auto& clock_offset_profiles = app_settings_ref["clock_offset_profiles"];

        nlohmann::json::object_t new_profile {
            {"name", new_clock_profile_name.toStdString()},
            {"offset_values", nlohmann::json::array_t {}},
        };
        auto& offset_values = new_profile["offset_values"];

        if (ui->checkBox_pstate0->isChecked())
        {
            offset_values.emplace_back(std::make_pair(0, ui->verticalSlider_pstate0_gpu_clock_offset->value()));
        }
        if (ui->checkBox_pstate1->isChecked())
        {
            offset_values.emplace_back(std::make_pair(1, ui->verticalSlider_pstate1_gpu_clock_offset->value()));
        }
        if (ui->checkBox_pstate2->isChecked())
        {
            offset_values.emplace_back(std::make_pair(2, ui->verticalSlider_pstate2_gpu_clock_offset->value()));
        }
        if (ui->checkBox_pstate3->isChecked())
        {
            offset_values.emplace_back(std::make_pair(3, ui->verticalSlider_pstate3_gpu_clock_offset->value()));
        }
        if (ui->checkBox_pstate4->isChecked())
        {
            offset_values.emplace_back(std::make_pair(4, ui->verticalSlider_pstate4_gpu_clock_offset->value()));
        }
        if (ui->checkBox_pstate5->isChecked())
        {
            offset_values.emplace_back(std::make_pair(5, ui->verticalSlider_pstate5_gpu_clock_offset->value()));
        }
        if (ui->checkBox_pstate6->isChecked())
        {
            offset_values.emplace_back(std::make_pair(6, ui->verticalSlider_pstate6_gpu_clock_offset->value()));
        }
        if (ui->checkBox_pstate7->isChecked())
        {
            offset_values.emplace_back(std::make_pair(7, ui->verticalSlider_pstate7_gpu_clock_offset->value()));
        }

        if (!offset_values.empty())
        {
            clock_offset_profiles.emplace_back(std::move(new_profile));

            emit new_profile_created(clock_offset_profiles.back());
            SettingsManager::instance().write_settings(app_settings_ref);
            qInfo().noquote().nospace() << "New clock profile created: " << new_clock_profile_name;

            reset_on_form_widgets();
            on_buttonBox_rejected();
        }
        else
        {
            QMessageBox::warning(this, "Warning", "You should choose at least one pstate");
        }
    }
}



void ClockProfileDialog::on_buttonBox_rejected()
{
    close();
}



void ClockProfileDialog::connect_signals_and_slots()
{
    connect(ui->checkBox_pstate0, &QCheckBox::clicked, [this](bool value) {
        ui->verticalSlider_pstate0_gpu_clock_offset->setEnabled(value);
        ui->spinBox_pstate0_gpu_clock_offset->setEnabled(value);
    });
    connect(ui->checkBox_pstate1, &QCheckBox::clicked, [this](bool value) {
        ui->verticalSlider_pstate1_gpu_clock_offset->setEnabled(value);
        ui->spinBox_pstate1_gpu_clock_offset->setEnabled(value);
    });
    connect(ui->checkBox_pstate2, &QCheckBox::clicked, [this](bool value) {
        ui->verticalSlider_pstate2_gpu_clock_offset->setEnabled(value);
        ui->spinBox_pstate2_gpu_clock_offset->setEnabled(value);
    });
    connect(ui->checkBox_pstate3, &QCheckBox::clicked, [this](bool value) {
        ui->verticalSlider_pstate3_gpu_clock_offset->setEnabled(value);
        ui->spinBox_pstate3_gpu_clock_offset->setEnabled(value);
    });
    connect(ui->checkBox_pstate4, &QCheckBox::clicked, [this](bool value) {
        ui->verticalSlider_pstate4_gpu_clock_offset->setEnabled(value);
        ui->spinBox_pstate4_gpu_clock_offset->setEnabled(value);
    });
    connect(ui->checkBox_pstate5, &QCheckBox::clicked, [this](bool value) {
        ui->verticalSlider_pstate5_gpu_clock_offset->setEnabled(value);
        ui->spinBox_pstate5_gpu_clock_offset->setEnabled(value);
    });
    connect(ui->checkBox_pstate6, &QCheckBox::clicked, [this](bool value) {
        ui->verticalSlider_pstate6_gpu_clock_offset->setEnabled(value);
        ui->spinBox_pstate6_gpu_clock_offset->setEnabled(value);
    });
    connect(ui->checkBox_pstate7, &QCheckBox::clicked, [this](bool value) {
        ui->verticalSlider_pstate7_gpu_clock_offset->setEnabled(value);
        ui->spinBox_pstate7_gpu_clock_offset->setEnabled(value);
    });

    connect(ui->verticalSlider_pstate0_gpu_clock_offset, &QSlider::valueChanged, [this](int value) {
        ui->spinBox_pstate0_gpu_clock_offset->setValue(value);
    });
    connect(ui->verticalSlider_pstate1_gpu_clock_offset, &QSlider::valueChanged, [this](int value) {
        ui->spinBox_pstate1_gpu_clock_offset->setValue(value);
    });
    connect(ui->verticalSlider_pstate2_gpu_clock_offset, &QSlider::valueChanged, [this](int value) {
        ui->spinBox_pstate2_gpu_clock_offset->setValue(value);
    });
    connect(ui->verticalSlider_pstate3_gpu_clock_offset, &QSlider::valueChanged, [this](int value) {
        ui->spinBox_pstate3_gpu_clock_offset->setValue(value);
    });
    connect(ui->verticalSlider_pstate4_gpu_clock_offset, &QSlider::valueChanged, [this](int value) {
        ui->spinBox_pstate4_gpu_clock_offset->setValue(value);
    });
    connect(ui->verticalSlider_pstate5_gpu_clock_offset, &QSlider::valueChanged, [this](int value) {
        ui->spinBox_pstate5_gpu_clock_offset->setValue(value);
    });
    connect(ui->verticalSlider_pstate6_gpu_clock_offset, &QSlider::valueChanged, [this](int value) {
        ui->spinBox_pstate6_gpu_clock_offset->setValue(value);
    });
    connect(ui->verticalSlider_pstate7_gpu_clock_offset, &QSlider::valueChanged, [this](int value) {
        ui->spinBox_pstate7_gpu_clock_offset->setValue(value);
    });

    connect(ui->spinBox_pstate0_gpu_clock_offset, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
        ui->verticalSlider_pstate0_gpu_clock_offset->setValue(value);
    });
    connect(ui->spinBox_pstate1_gpu_clock_offset, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
        ui->verticalSlider_pstate1_gpu_clock_offset->setValue(value);
    });
    connect(ui->spinBox_pstate2_gpu_clock_offset, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
        ui->verticalSlider_pstate2_gpu_clock_offset->setValue(value);
    });
    connect(ui->spinBox_pstate3_gpu_clock_offset, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
        ui->verticalSlider_pstate3_gpu_clock_offset->setValue(value);
    });
    connect(ui->spinBox_pstate4_gpu_clock_offset, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
        ui->verticalSlider_pstate4_gpu_clock_offset->setValue(value);
    });
    connect(ui->spinBox_pstate5_gpu_clock_offset, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
        ui->verticalSlider_pstate5_gpu_clock_offset->setValue(value);
    });
    connect(ui->spinBox_pstate6_gpu_clock_offset, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
        ui->verticalSlider_pstate6_gpu_clock_offset->setValue(value);
    });
    connect(ui->spinBox_pstate7_gpu_clock_offset, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
        ui->verticalSlider_pstate7_gpu_clock_offset->setValue(value);
    });
}



void ClockProfileDialog::reset_on_form_widgets()
{
    ui->lineEdit_new_profile_name->clear();
    ui->verticalSlider_pstate0_gpu_clock_offset->setValue(0);
    ui->verticalSlider_pstate1_gpu_clock_offset->setValue(0);
    ui->verticalSlider_pstate2_gpu_clock_offset->setValue(0);
    ui->verticalSlider_pstate3_gpu_clock_offset->setValue(0);
    ui->verticalSlider_pstate4_gpu_clock_offset->setValue(0);
    ui->verticalSlider_pstate5_gpu_clock_offset->setValue(0);
    ui->verticalSlider_pstate6_gpu_clock_offset->setValue(0);
    ui->verticalSlider_pstate7_gpu_clock_offset->setValue(0);
    ui->checkBox_pstate0->clicked(); ui->checkBox_pstate0->setChecked(false);
    ui->checkBox_pstate1->clicked(); ui->checkBox_pstate1->setChecked(false);
    ui->checkBox_pstate2->clicked(); ui->checkBox_pstate2->setChecked(false);
    ui->checkBox_pstate3->clicked(); ui->checkBox_pstate3->setChecked(false);
    ui->checkBox_pstate4->clicked(); ui->checkBox_pstate4->setChecked(false);
    ui->checkBox_pstate5->clicked(); ui->checkBox_pstate5->setChecked(false);
    ui->checkBox_pstate6->clicked(); ui->checkBox_pstate6->setChecked(false);
    ui->checkBox_pstate7->clicked(); ui->checkBox_pstate7->setChecked(false);
}
