#pragma once

#include <QMainWindow>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QProcess>
#include <QMenu>

#include "NVML.hpp"
#include "settings_manager.hpp"
#include "settingswindow.hpp"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_apply_power_settings_clicked();
    void update_dynamic_info();
    void toggle_tray();
    void on_action_Exit_triggered();
    void on_actionShow_hide_app_window_triggered();
    void on_actionSettings_triggered();

private:
    Ui::MainWindow* ui;
    QTimer dynamic_info_update_timer_;
    QSystemTrayIcon tray_icon_;
    QMenu tray_menu_;
    NVMLDevice nvml_device_;
    SettingsWindow settings_window_;

private:
    void closeEvent(QCloseEvent* event_);
    void set_static_info();
};
