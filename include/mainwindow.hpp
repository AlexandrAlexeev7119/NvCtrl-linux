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

#include "settings_dialog.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:

    MainWindow(const QJsonObject& app_settings, QWidget* parent = nullptr);
    ~MainWindow();

    inline QSystemTrayIcon& get_tray_icon() noexcept { return tray_icon_; }

private slots:
    void toggle_tray();
    void on_SettingsDialog_settings_applied(const QJsonObject& app_settings);

    void on_GpuUtilizationsController_gpu_utilization(unsigned gpu_utilization);
    void on_GpuUtilizationsController_memory_utilization(unsigned memory_utilization, unsigned used_memory);
    void on_GpuUtilizationsController_encoder_decoder_utilization(unsigned encoder_utilization, unsigned decoder_utilization);
    void on_GpuUtilizationsController_pstate_level(unsigned pstate_level);
    void on_GpuPowerController_power_usage(unsigned power_usage);
    void on_GpuPowerController_power_limit(unsigned power_limit);
    void on_GpuClockController_graphics_clock(unsigned graphics_clock);
    void on_GpuClockController_video_clock(unsigned video_clock);
    void on_GpuClockController_sm_clock(unsigned sm_clock);
    void on_GpuClockController_memory_clock(unsigned memory_clock);
    void on_GpuClockController_error();

    void on_comboBox_select_GPU_activated(int index);
    void on_pushButton_apply_power_limit_clicked();

    void on_actionUpdate_GPUs_list_triggered();
    void on_actionSettings_triggered();
    void on_actionQuit_triggered();

private:
    Ui::MainWindow* ui;
    QSystemTrayIcon tray_icon_;

    GpuUtilizationsController gpu_utilizations_controller_;
    GpuPowerController gpu_power_controller_;
    GpuClockController gpu_clock_controller_;

    QTimer dynamic_info_update_timer_;
    bool minimize_to_tray_on_close_;
    int update_freq_ms_;

    std::vector<NVMLpp::NVML_device> nvml_devices_list_;

    SettingsDialog settings_dialog_window_;

protected:
    void closeEvent(QCloseEvent* event);

    void connect_slots_and_signals();
    void setup_tray_menu();
    void load_app_settings(const QJsonObject& app_settings);
    void set_static_info();
    void load_GPUs();    
    NVMLpp::NVML_device* get_current_gpu();
    void set_current_gpu_for_controllers() noexcept;
};
