#pragma once

#include <QMainWindow>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QMenu>
#include <QJsonObject>

#include "NVML.hpp"
#include "settings_manager.hpp"
#include "settingswindow.hpp"
#include "newfanprofiledialog.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    QSystemTrayIcon& get_tray_icon();

private slots:
    void on_actionShow_hide_app_window_triggered();
    void on_actionSettings_triggered();
    void on_actionQuit_triggered();
    void on_pushButton_apply_power_settings_clicked();
    void on_comboBox_fan_profile_activated(int index);

    void toggle_tray();
    void update_dynamic_info();
    void apply_settings(const QJsonObject& settings);

    void on_fan_profile_created(const QJsonObject& profile);
    void on_pushButton_edit_curr_fan_profile_clicked();

private:
    Ui::MainWindow* ui;
    QTimer dynamic_info_update_timer_;
    QSystemTrayIcon tray_icon_;
    QMenu tray_menu_;

    NVMLDevice nvml_device_;

    SettingsManager& settings_manager_;
    SettingsWindow settings_window_;
    NewFanProfileDialog new_file_profile_dialog_;

    bool minimize_to_tray_on_close_;
    int update_freq_ms_;

    void set_static_info();
    void closeEvent(QCloseEvent* event_);
};
