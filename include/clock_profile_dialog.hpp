#pragma once

#include <QDialog>

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

    void connect_signals_and_slots();
    void reset_on_form_widgets();
};
