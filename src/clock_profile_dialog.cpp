#include <QMessageBox>
#include <QDebug>
#include <QShowEvent>

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
    pstates_gpu_ = {
        {ui->checkBox_pstate0_gpu, ui->verticalSlider_pstate0_gpu_clock_offset, ui->spinBox_pstate0_gpu_clock_offset},
        {ui->checkBox_pstate1_gpu, ui->verticalSlider_pstate1_gpu_clock_offset, ui->spinBox_pstate1_gpu_clock_offset},
        {ui->checkBox_pstate2_gpu, ui->verticalSlider_pstate2_gpu_clock_offset, ui->spinBox_pstate2_gpu_clock_offset},
        {ui->checkBox_pstate3_gpu, ui->verticalSlider_pstate3_gpu_clock_offset, ui->spinBox_pstate3_gpu_clock_offset},
        {ui->checkBox_pstate4_gpu, ui->verticalSlider_pstate4_gpu_clock_offset, ui->spinBox_pstate4_gpu_clock_offset},
        {ui->checkBox_pstate5_gpu, ui->verticalSlider_pstate5_gpu_clock_offset, ui->spinBox_pstate5_gpu_clock_offset},
        {ui->checkBox_pstate6_gpu, ui->verticalSlider_pstate6_gpu_clock_offset, ui->spinBox_pstate6_gpu_clock_offset},
        {ui->checkBox_pstate7_gpu, ui->verticalSlider_pstate7_gpu_clock_offset, ui->spinBox_pstate7_gpu_clock_offset}
    };
    pstates_mem_ = {
        {ui->checkBox_pstate0_mem, ui->verticalSlider_pstate0_mem_clock_offset, ui->spinBox_pstate0_mem_clock_offset},
        {ui->checkBox_pstate1_mem, ui->verticalSlider_pstate1_mem_clock_offset, ui->spinBox_pstate1_mem_clock_offset},
        {ui->checkBox_pstate2_mem, ui->verticalSlider_pstate2_mem_clock_offset, ui->spinBox_pstate2_mem_clock_offset},
        {ui->checkBox_pstate3_mem, ui->verticalSlider_pstate3_mem_clock_offset, ui->spinBox_pstate3_mem_clock_offset},
        {ui->checkBox_pstate4_mem, ui->verticalSlider_pstate4_mem_clock_offset, ui->spinBox_pstate4_mem_clock_offset},
        {ui->checkBox_pstate5_mem, ui->verticalSlider_pstate5_mem_clock_offset, ui->spinBox_pstate5_mem_clock_offset},
        {ui->checkBox_pstate6_mem, ui->verticalSlider_pstate6_mem_clock_offset, ui->spinBox_pstate6_mem_clock_offset},
        {ui->checkBox_pstate7_mem, ui->verticalSlider_pstate7_mem_clock_offset, ui->spinBox_pstate7_mem_clock_offset},
    };
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
        nlohmann::json::object_t new_profile {
            {"name", new_clock_profile_name.toStdString()},
            {"offset_values",
                nlohmann::json::object_t {
                    {"gpu_offsets", nlohmann::json::array_t {}},
                    {"memory_offsets", nlohmann::json::array_t {}},
                }
            }
        };

        auto& gpu_offsets = new_profile["offset_values"]["gpu_offsets"];
        fill_array_from_widgets(gpu_offsets, pstates_gpu_);
        auto& memory_offests = new_profile["offset_values"]["memory_offsets"];
        fill_array_from_widgets(memory_offests, pstates_mem_);

        if (!gpu_offsets.empty() || !memory_offests.empty())
        {
            auto& app_settings_ref {*ptr_app_settings_};
            auto& clock_offset_profiles = app_settings_ref["clock_offset_profiles"];

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
    const auto connect_signals {
        [](const auto& widgets) {
            const auto& [check_box, slider, spin_box] {widgets};
            connect(check_box, &QCheckBox::clicked, slider, &QSlider::setEnabled);
            connect(check_box, &QCheckBox::clicked, spin_box, &QSpinBox::setEnabled);
            connect(slider, &QSlider::valueChanged, spin_box, &QSpinBox::setValue);
            connect(spin_box, QOverload<int>::of(&QSpinBox::valueChanged), slider, &QSlider::setValue);
        }
    };
    std::for_each(pstates_gpu_.begin(), pstates_gpu_.end(), connect_signals);
    std::for_each(pstates_mem_.begin(), pstates_mem_.end(), connect_signals);
}



void ClockProfileDialog::reset_on_form_widgets()
{
    const auto clear_widgets {
        [](const auto& widgets) {
            const auto& [check_box, slider, spin_box] {widgets};
            slider->setValue(0);
            spin_box->setValue(0);
            check_box->setChecked(false);
            check_box->click();
        }
    };
    ui->lineEdit_new_profile_name->clear();
    std::for_each(pstates_gpu_.begin(), pstates_gpu_.end(), clear_widgets);
    std::for_each(pstates_mem_.begin(), pstates_mem_.end(), clear_widgets);
}



void ClockProfileDialog::fill_array_from_widgets(nlohmann::json& array,
                                                 std::vector<std::tuple<QCheckBox*, QSlider*, QSpinBox*>>& widgets) const
{
    unsigned pstate_num {0};
    for (const auto& [check_box, slider, _] : widgets)
    {
        if (check_box->isChecked())
        {
            array.emplace_back(std::make_pair(pstate_num, slider->value()));
        }
        pstate_num++;
    }
}
