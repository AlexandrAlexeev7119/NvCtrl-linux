#pragma once

#include <QMainWindow>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QMenu>

#include "nlohmann/json.hpp"

#include "nvmlpp/nvmlpp_session.hpp"
#include "nvmlpp/nvmlpp_device.hpp"

#include "gpu_utilizations_controller.hpp"
#include "gpu_power_controller.hpp"
#include "gpu_clock_controller.hpp"
#include "gpu_fan_controller.hpp"

#include "settings_dialog.hpp"
#include "about_dialog.hpp"
#include "report_a_bug_dialog.hpp"
#include "fan_profile_dialog.hpp"
#include "edit_fan_profile_dialog.hpp"
#include "clock_profile_dialog.hpp"
#include "edit_clock_offset_profile_dialog.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    enum { FAN_PROFILE_AUTO };
    enum { CLOCK_PROFILE_NONE };

public:
    MainWindow(nlohmann::json app_settings, QWidget* parent = nullptr);
    ~MainWindow();

    QSystemTrayIcon& get_tray_icon() noexcept { return tray_icon_; }

private slots:
    void toggle_tray();
    void update_dynamic_info();

    void on_SettingsDialog_settings_applied(const nlohmann::json& app_settings);
    void on_FanProfileDialog_new_profile_created(const nlohmann::json& curr_fan_profile);
    void on_EditFanProfileDialog_current_fan_profile_changed(const nlohmann::json& curr_fan_profile);
    void on_EditFanProfileDialog_current_fan_profile_removed();
    void on_ClockProfileDialog_new_profile_created(const nlohmann::json& curr_clock_profile);
    void on_EditClockOffsetProfileDialog_current_clock_offset_profile_changed(const nlohmann::json& curr_clock_profile);
    void on_EditClockOffsetProfileDialog_current_clock_offset_profile_removed();

    void on_GpuUtilizationsController_info_ready(const GpuUtilizationsController::utilization_rates& utilization_rates);
    void on_GpuPowerController_info_ready(const GpuPowerController::power_rates& power_rates);
    void on_GpuClockController_info_ready(const GpuClockController::clock_values& clock_values);
    void on_GpuFanController_info_ready(const GpuFanController::fan_rates& fan_rates);

    void on_GpuUtilizationsController_encoder_decoder_unsupported();
    void on_GpuPowerController_error();
    void on_GpuClockController_error();
    void on_GpuFanController_error();

    void on_comboBox_select_fan_profile_activated(int index);
    void on_comboBox_select_clock_offset_profile_activated(int index);

    void on_pushButton_apply_power_limit_clicked();
    void on_pushButton_apply_fan_speed_clicked();
    void on_pushButton_add_new_fan_profile_clicked();
    void on_pushButton_edit_current_fan_profile_clicked();
    void on_pushButton_add_new_clock_offset_profile_clicked();
    void on_pushButton_apply_clock_offset_clicked();
    void on_pushButton_edit_current_clock_offset_profile_clicked();

    void on_actionSettings_triggered();
    void on_actionQuit_triggered();
    void on_actionAbout_triggered();
    void on_actionReport_a_bug_triggered();
    void on_actionShow_GPU_UUID_toggled(bool checked);

private:
    Ui::MainWindow* ui;
    QMenu tray_menu_;
    QSystemTrayIcon tray_icon_;
    QTimer dynamic_info_update_timer_;

    bool minimize_to_tray_on_close_;
    bool last_fan_and_clock_offset_profiles_saved_;
    int update_freq_ms_;

    nlohmann::json app_settings_;
    NVMLpp::Session& nvmlpp_session_;
    NVMLpp::NVML_device current_gpu_;

    GpuUtilizationsController gpu_utilizations_controller_;
    GpuPowerController gpu_power_controller_;
    GpuClockController gpu_clock_controller_;
    GpuFanController gpu_fan_controller_;

    SettingsDialog settings_dialog_window_;
    AboutDialog about_dialog_window_;
    ReportABugDialog report_a_bug_dialog_window_;
    FanProfileDialog fan_profile_dialog_window_;
    EditFanProfileDialog edit_fan_profile_dialog_window_;
    ClockProfileDialog clock_profile_dialog_window_;
    EditClockOffsetProfileDialog edit_clock_offset_profile_dialog_window_;

protected:
    void closeEvent(QCloseEvent* event);

    void connect_slots_and_signals();
    void setup_tray_menu();

    void load_and_validate_app_settings();
    void load_fan_and_clock_offset_profiles();
    void restore_last_fan_and_clock_offset_profiles();

    void set_static_info();
    void set_current_gpu_for_controllers() noexcept;
    void set_max_clock_values(int gpu_clock_offset = 0, int mem_clock_offset = 0) const;
};
