#pragma once

#include <QMainWindow>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QMenu>

#include "settings_manager.hpp"
#include "nvmlpp/nvmlpp_device.hpp"
#include "gpu_utilizations_controller.hpp"
#include "gpu_power_controller.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    inline QSystemTrayIcon& get_tray_icon() noexcept { return tray_icon_; }

private slots:
    void toggle_tray();
    void apply_settings(const QJsonObject& settings);

    void on_GpuUtilizationsController_gpu_utilization(unsigned gpu_utilization);
    void on_GpuUtilizationsController_memory_utilization(unsigned memory_utilization, unsigned used_memory);
    void on_GpuUtilizationsController_encoder_decoder_utilization(unsigned encoder_utilization, unsigned decoder_utilization);

    void on_GpuPowerController_power_usage(unsigned power_usage);
    void on_GpuPowerController_power_limit(unsigned power_limit);

    void on_comboBox_select_GPU_activated(int index);
    void on_pushButton_apply_power_limit_clicked();

    void on_actionUpdate_GPUs_list_triggered();

    void on_actionExit_triggered();

private:
    Ui::MainWindow* ui;
    QSystemTrayIcon tray_icon_;
    SettingsManager& settings_manager_;

    GpuUtilizationsContoller gpu_utilizations_controller_;
    GpuPowerController gpu_power_controller_;

    QTimer dynamic_info_update_timer_;
    bool minimize_to_tray_on_close_;
    int update_freq_ms_;

    std::vector<NVMLpp::NVML_device> nvml_devices_list_;

protected:
    void connect_slots_and_signals();
    void setup_tray_menu();
    void load_app_settings();
    void set_static_info();
    void load_GPUs();    
    NVMLpp::NVML_device* get_current_gpu();

    void closeEvent(QCloseEvent* event);
};
