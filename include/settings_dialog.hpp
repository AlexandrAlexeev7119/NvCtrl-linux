#pragma once

#include <QDialog>

#include "nlohmann/json.hpp"
#include "settings_manager.hpp"

namespace Ui { class SettingsDialog; }

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget* parent = nullptr);
    ~SettingsDialog();

    void load_app_settins(nlohmann::json* app_settings) noexcept;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

signals:
    void settings_applied(const nlohmann::json&);

private:
    Ui::SettingsDialog* ui;
    nlohmann::json* ptr_app_settings_;

    void showEvent(QShowEvent* event_);
    void save_settings_to_file(const nlohmann::json& app_settings);
};
