#pragma once

#include <vector>

#include <QDialog>
#include <QCheckBox>
#include <QSlider>
#include <QSpinBox>

#include "nlohmann/json.hpp"

namespace Ui { class EditClockOffsetProfileDialog; }

class EditClockOffsetProfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditClockOffsetProfileDialog(QWidget* parent = nullptr);
    ~EditClockOffsetProfileDialog();

    inline void load_app_settings(nlohmann::json* app_settings) noexcept { ptr_app_settings_ = app_settings; }
    inline void set_current_clock_offset_profile_index(unsigned index) noexcept { current_clock_offset_profile_index_= index; }

signals:
    void current_clock_offset_profile_changed(const nlohmann::json&);
    void current_clock_offset_profile_removed();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_pushButton_remove_current_profile_clicked();

private:
    Ui::EditClockOffsetProfileDialog* ui;
    nlohmann::json* ptr_app_settings_;
    unsigned current_clock_offset_profile_index_;

    std::vector<std::tuple<QCheckBox*, QSlider*, QSpinBox*>> pstates_gpu_;
    std::vector<std::tuple<QCheckBox*, QSlider*, QSpinBox*>> pstates_mem_;

    void connect_signals_and_slots();
    void reset_on_form_widgets();
    void fill_array_from_widgets(nlohmann::json& array,
                                 std::vector<std::tuple<QCheckBox*, QSlider*, QSpinBox*>>& widgets) const;

    void showEvent(QShowEvent* show_event);
};
