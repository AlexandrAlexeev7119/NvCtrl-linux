#pragma once

#include <QMainWindow>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QMenu>

#include "nvmlpp/nvmlpp_device.hpp"

#include "gpu_utilizations_controller.hpp"
#include "gpu_power_controller.hpp"
#include "gpu_clock_controller.hpp"
#include "gpu_fan_controller.hpp"

#include "settings_dialog.hpp"
#include "about_dialog.hpp"
#include "report_a_bug_dialog.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum { FAN_PROFILE_AUTO, FAN_PROFILE_MANUAL };
    enum { CLOCK_PROFILE_NONE, CLOCK_PROFILE_MANUAL };

public:
    MainWindow(QJsonObject app_settings, QWidget* parent = nullptr);
    ~MainWindow();

    inline QSystemTrayIcon& get_tray_icon() noexcept { return tray_icon_; }

private slots:
    void toggle_tray();
    void update_dynamic_info();
    void on_SettingsDialog_settings_applied(const QJsonObject& app_settings);

    void on_GpuUtilizationsController_info_ready(const GpuUtilizationsController::utilization_rates& utilization_rates);
    void on_GpuPowerController_info_ready(const GpuPowerController::power_rates& power_rates);
    void on_GpuClockController_info_ready(const GpuClockController::clock_values& clock_values);
    void on_GpuFanController_info_ready(const GpuFanController::fan_rates& fan_rates);

    void on_GpuPowerController_error();
    void on_GpuClockController_error();
    void on_GpuFanController_error();

    void on_comboBox_select_GPU_activated(int index);
    void on_comboBox_select_fan_profile_activated(int index);
    void on_comboBox_select_clock_offset_profile_activated(int index);

    void on_pushButton_apply_power_limit_clicked();
    void on_pushButton_apply_fan_speed_clicked();
    void on_pushButton_add_new_fan_profile_clicked();
    void on_pushButton_edit_current_fan_profile_clicked();

    void on_actionUpdate_GPUs_list_triggered();
    void on_actionSettings_triggered();
    void on_actionQuit_triggered();
    void on_actionAbout_triggered();
    void on_actionReport_a_bug_triggered();
    void on_actionShow_GPU_UUID_toggled(bool checked);

private:
    Ui::MainWindow* ui;
    QSystemTrayIcon tray_icon_;
    QMenu tray_menu_;

    GpuUtilizationsController gpu_utilizations_controller_;
    GpuPowerController gpu_power_controller_;
    GpuClockController gpu_clock_controller_;
    GpuFanController gpu_fan_controller_;

    QTimer dynamic_info_update_timer_;
    bool minimize_to_tray_on_close_;
    int update_freq_ms_;

    std::vector<NVMLpp::NVML_device> nvml_devices_list_;

    SettingsDialog settings_dialog_window_;
    AboutDialog about_dialog_window_;
    ReportABugDialog report_a_bug_dialog_window_;

    bool curr_gpu_power_control_unsupported_;
    bool curr_gpu_clock_control_unsupported_;
    bool curr_gpu_fan_control_unsupported_;

protected:
    void closeEvent(QCloseEvent* event);

    void connect_slots_and_signals();
    void setup_tray_menu();
    void load_and_validate_app_settings(QJsonObject app_settings);
    void set_static_info();
    void load_GPUs();
    NVMLpp::NVML_device* get_current_gpu();
    void set_current_gpu_for_controllers() noexcept;
    void check_and_reconnect_signals_from_controllers();
    void check_and_enable_groupbox_widgets();
    void manual_fan_speed_control_widgets_enabled(bool value);
};
