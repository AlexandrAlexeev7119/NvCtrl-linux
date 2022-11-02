#include <QShowEvent>
#include <QMessageBox>
#include <QDebug>

#include "settings_manager.hpp"

#include "edit_clock_offset_profile_dialog.hpp"
#include "ui_edit_clock_offset_profile_dialog.h"

EditClockOffsetProfileDialog::EditClockOffsetProfileDialog(QWidget* parent)
    : QDialog {parent}
    , ui {new Ui::EditClockOffsetProfileDialog}
    , ptr_app_settings_ {nullptr}
    , current_clock_offset_profile_index_ {0}
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



EditClockOffsetProfileDialog::~EditClockOffsetProfileDialog()
{
    delete ui;
}



void EditClockOffsetProfileDialog::on_buttonBox_accepted()
{
    if (ui->lineEdit_current_profile_name->text().isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Empty name is not allowed");
    }
    else
    {
        auto& current_clock_offset_profile = (*ptr_app_settings_)["clock_offset_profiles"][current_clock_offset_profile_index_];

        auto new_gpu_offsets_array = nlohmann::json::array();
        auto new_memory_offsets_array = nlohmann::json::array();

        qDebug().noquote().nospace() << new_gpu_offsets_array.dump(4).c_str();
        qDebug().noquote().nospace() << new_memory_offsets_array.dump(4).c_str();

        fill_array_from_widgets(new_gpu_offsets_array, pstates_gpu_);
        fill_array_from_widgets(new_memory_offsets_array, pstates_mem_);

        if (!new_gpu_offsets_array.empty() || !new_memory_offsets_array.empty())
        {
            auto& gpu_offsets = current_clock_offset_profile["offset_values"]["gpu_offsets"];
            auto& memory_offests = current_clock_offset_profile["offset_values"]["memory_offsets"];

            gpu_offsets.swap(new_gpu_offsets_array);
            memory_offests.swap(new_memory_offsets_array);
            current_clock_offset_profile["name"] = ui->lineEdit_current_profile_name->text().toStdString();

            emit current_clock_offset_profile_changed(current_clock_offset_profile);
            SettingsManager::instance().write_settings(*ptr_app_settings_);

            reset_on_form_widgets();
            on_buttonBox_rejected();
        }
    }
}



void EditClockOffsetProfileDialog::on_buttonBox_rejected()
{
    close();
}



void EditClockOffsetProfileDialog::showEvent(QShowEvent* show_event)
{
    const auto set_values_on_widgets {
        [](auto& widgets, const nlohmann::json& values) {
            const auto& [pstate_num, offset] {values.get<std::pair<int, int>>()};
            auto& [check_box, slider, spin_box] {widgets[pstate_num]};
            check_box->setChecked(true);
            check_box->clicked(true);
            slider->setValue(offset);
            spin_box->setValue(offset);
        }
    };
    const auto set_values_for_gpu_offsets {std::bind(set_values_on_widgets, pstates_gpu_, std::placeholders::_1)};
    const auto set_values_for_memory_offsets {std::bind(set_values_on_widgets, pstates_mem_, std::placeholders::_1)};

    const auto& current_clock_offset_profile = (*ptr_app_settings_)["clock_offset_profiles"][current_clock_offset_profile_index_];
    const auto& gpu_offsets = current_clock_offset_profile["offset_values"]["gpu_offsets"];
    const auto& memory_offsets = current_clock_offset_profile["offset_values"]["memory_offsets"];

    std::for_each(gpu_offsets.begin(), gpu_offsets.end(), set_values_for_gpu_offsets);
    std::for_each(memory_offsets.begin(), memory_offsets.end(), set_values_for_memory_offsets);
    ui->lineEdit_current_profile_name->setText(QString::fromStdString(current_clock_offset_profile["name"].get<std::string>()));

    show_event->accept();
}



void EditClockOffsetProfileDialog::on_pushButton_remove_current_profile_clicked()
{
    const auto used_choise {
        QMessageBox::question(this, "Remove profile", "Are you sure that you want TO DELETE THIS profile?",
                              QMessageBox::Button::Yes, QMessageBox::Button::No)
    };
    if (used_choise == QMessageBox::Button::Yes)
    {
        auto& clock_offset_profiles = (*ptr_app_settings_)["clock_offset_profiles"];

        clock_offset_profiles.erase(current_clock_offset_profile_index_);
        emit current_clock_offset_profile_removed();
        SettingsManager::instance().write_settings(*ptr_app_settings_);
    }
    on_buttonBox_rejected();
}



void EditClockOffsetProfileDialog::connect_signals_and_slots()
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



void EditClockOffsetProfileDialog::reset_on_form_widgets()
{
    const auto clear_widgets {
        [](const auto& widgets) {
            const auto& [check_box, slider, spin_box] {widgets};
            slider->setValue(0);
            spin_box->setValue(0);
            check_box->setChecked(false);
            check_box->clicked();
        }
    };
    ui->lineEdit_current_profile_name->clear();
    std::for_each(pstates_gpu_.begin(), pstates_gpu_.end(), clear_widgets);
    std::for_each(pstates_mem_.begin(), pstates_mem_.end(), clear_widgets);
}



void EditClockOffsetProfileDialog::fill_array_from_widgets(nlohmann::json& array,
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
