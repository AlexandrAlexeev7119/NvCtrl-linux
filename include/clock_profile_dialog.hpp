#pragma once

#include <QDialog>
#include <QCheckBox>
#include <QSlider>
#include <QSpinBox>

#include "settings_manager.hpp"

namespace Ui { class ClockProfileDialog; }

class ClockProfileDialog : public QDialog
{
    Q_OBJECT
    enum { GPU_CLOCK_OFFSET_MIN = -200, GPU_CLOCK_OFFSET_MAX = 1000 };

public:
    explicit ClockProfileDialog(QWidget* parent = nullptr);
    ~ClockProfileDialog();

    void load_app_settings(nlohmann::json* app_settings) noexcept;

signals:
    void new_profile_created(const nlohmann::json&);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::ClockProfileDialog* ui;
    nlohmann::json* ptr_app_settings_;
    std::vector<std::tuple<QCheckBox*, QSlider*, QSpinBox*>> pstates_gpu_;
    std::vector<std::tuple<QCheckBox*, QSlider*, QSpinBox*>> pstates_mem_;

    void connect_signals_and_slots();
    void reset_on_form_widgets();
    void fill_gpu_clock_offsets(nlohmann::json& gpu_offsets);
    void fill_mem_clock_offsets(nlohmann::json& gpu_offsets);

    void showEvent(QShowEvent* show_event);
};
