#pragma once

#include <QDialog>

#include "nlohmann/json.hpp"

namespace Ui { class EditFanProfileDialog; }

class EditFanProfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditFanProfileDialog(QWidget* parent = nullptr);
    ~EditFanProfileDialog();

    void load_app_settings(nlohmann::json* app_settings) noexcept;
    void set_current_fan_profile_index(unsigned index) noexcept;

signals:
    void current_fan_profile_changed(const nlohmann::json&);
    void current_fan_profile_removed();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_pushButton_remove_current_profile_clicked();

private:
    Ui::EditFanProfileDialog* ui;
    unsigned current_fan_profile_index_;
    nlohmann::json* ptr_app_settings_;

    void showEvent(QShowEvent* event_);
    void write_new_settings(const nlohmann::json& app_settings);
};
