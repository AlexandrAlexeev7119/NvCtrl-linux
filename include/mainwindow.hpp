#pragma once

#include <QMainWindow>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QMenu>

#include "nvmlpp/nvmlpp_device.hpp"

#include "settings_manager.hpp"

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
    void update_dynamic_info();

    void on_comboBox_select_GPU_activated(int index);

private:
    Ui::MainWindow* ui;
    QSystemTrayIcon tray_icon_;
    SettingsManager& settings_manager_;

    QTimer dynamic_info_update_timer_;
    bool minimize_to_tray_on_close_;
    int update_freq_ms_;

    std::vector<NVMLpp::NVML_device> nvml_devices_list_;

protected:
    void connect_slots();
    void setup_tray_menu();
    void load_app_settings();
    void set_static_info();
    void load_GPUs();    
    NVMLpp::NVML_device& get_current_gpu();

    void closeEvent(QCloseEvent* event);
};
