#include <QMessageBox>
#include <QProcess>

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow {parent}
    , ui {new Ui::MainWindow}
    , tray_icon_ {this}
    , settings_manager_ {SettingsManager::get_instance()}
    , dynamic_info_update_timer_ {}
    , minimize_to_tray_on_close_ {false}
    , update_freq_ms_ {}
    , nvml_devices_list_ {}
{
    ui->setupUi(this);
    setMinimumSize(size());

    connect_slots();
    setup_tray_menu();
    load_app_settings();
    load_GPUs();

    dynamic_info_update_timer_.start();
}

MainWindow::~MainWindow()
{ delete ui; }



void MainWindow::toggle_tray()
{
    if (isHidden())
    {
        showNormal();
    }
    else
    {
        hide();
        tray_icon_.show();
    }
}

void MainWindow::apply_settings(const QJsonObject& settings)
{
    minimize_to_tray_on_close_ = settings["minimize_to_tray_on_close"].toBool();
    update_freq_ms_ = settings["update_freq_ms"].toInt();
}

void MainWindow::connect_slots()
{
    connect(&dynamic_info_update_timer_, &QTimer::timeout, this, &MainWindow::update_dynamic_info);
    connect(&settings_manager_, &SettingsManager::error, this,
            [this](const QString& err_msg)
    {
        qCritical().nospace().noquote() << err_msg;
        QMessageBox::critical(this, "Error", err_msg);
        close();
    });
}

void MainWindow::setup_tray_menu()
{
    QMenu* tray_menu {new QMenu {this}};
    tray_menu->addAction("Show/Hide app window", this, &MainWindow::toggle_tray);
    tray_menu->addAction("Quit", this, &MainWindow::close);
    tray_icon_.setContextMenu(tray_menu);
}

void MainWindow::load_app_settings()
{
    auto& settings_manager {SettingsManager::get_instance()};
    settings_manager.open_file(QIODevice::ReadOnly);

    const auto app_settings{settings_manager.load_settings()};
    settings_manager.close_file();

    minimize_to_tray_on_close_ = app_settings["minimize_to_tray_on_close"].toBool();
    update_freq_ms_ = app_settings["update_freq_ms"].toInt();
    dynamic_info_update_timer_.setInterval(update_freq_ms_);
}

void MainWindow::update_dynamic_info()
{
    const int current_device_index {ui->comboBox_select_GPU->currentIndex()};
    NVMLpp::NVML_device& current_device {nvml_devices_list_[current_device_index]};

    qInfo().noquote() << "GPU: " << QString::fromStdString(current_device.get_name());
    qInfo() << "Power limit: " << current_device.get_current_power_limit() << " W";
    qInfo() << "Temperature: " << current_device.get_current_temperature() << " C";
    qInfo() << "Power usage: " << current_device.get_current_power_usage() << " W";
}

void MainWindow::load_GPUs()
{
    nvml_devices_list_ = NVMLpp::Session::instance().get_devices();
    for (size_t i {0}; i < nvml_devices_list_.size(); i++)
    {
        ui->comboBox_select_GPU->addItem(QString::fromStdString(nvml_devices_list_[i].get_name()));
    }
    for (const auto& gpu : )
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (minimize_to_tray_on_close_)
    {
        event->ignore();
        hide();
        tray_icon_.show();
    }
    else
    {
        tray_icon_.hide();
        event->accept();
    }
}

void MainWindow::on_comboBox_select_GPU_activated(int index)
{

}
