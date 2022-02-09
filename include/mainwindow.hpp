#pragma once

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QProcess>
#include <QMenu>
#include <QCloseEvent>

#include "NVML.hpp"

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
    void on_action_Exit_triggered();
    void update_dynamic_info();
    void toggle_tray();

private:
    void closeEvent(QCloseEvent* event_);
    void set_static_info();

private:
    Ui::MainWindow* ui;
    QTimer dynamic_info_update_timer_;
    QSystemTrayIcon tray_icon_;
    QMenu tray_menu_;
    NVMLDevice nvml_device_;
};
