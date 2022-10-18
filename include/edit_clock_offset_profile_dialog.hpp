#pragma once

#include <QDialog>

#include "nlohmann/json.hpp"

namespace Ui { class EditClockOffsetProfileDialog; }

class EditClockOffsetProfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditClockOffsetProfileDialog(QWidget* parent = nullptr);
    ~EditClockOffsetProfileDialog();

    void load_app_settings(nlohmann::json* app_settings) noexcept;
    void set_current_clock_offset_profile_index(unsigned index) noexcept;

signals:
    void current_clock_offset_profile_changed(const nlohmann::json&);
    void current_clock_offset_profile_removed();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_pushButton_remove_current_profile_clicked();

private:
    Ui::EditClockOffsetProfileDialog* ui;
    unsigned current_clock_offset_profile_index_;
    nlohmann::json* ptr_app_settings_;

    void showEvent(QShowEvent* event_);
    void write_new_settings(const nlohmann::json& app_settings);
};
