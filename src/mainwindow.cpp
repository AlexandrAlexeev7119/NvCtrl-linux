#include <QMessageBox>
#include <QProcess>

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include "nvmlpp/nvmlpp_session.hpp"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow {parent}
    , ui {new Ui::MainWindow}
    , tray_icon_ {this}
    , settings_manager_ {SettingsManager::instance()}
    , gpu_utilizations_controller_ {}
    , gpu_power_controller_ {}
    , dynamic_info_update_timer_ {}
    , minimize_to_tray_on_close_ {false}
    , update_freq_ms_ {}
    , nvml_devices_list_ {}
{
    ui->setupUi(this);
    setMinimumSize(size());

    connect_slots_and_signals();
    setup_tray_menu();
    load_app_settings();
    load_GPUs();

    set_static_info();
    gpu_utilizations_controller_.update_info();
    gpu_power_controller_.update_info();

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

    qDebug().noquote().nospace() << "new settings applied: " << settings;
}

void MainWindow::on_GpuUtilizationsController_gpu_utilization(unsigned gpu_utilization)
{
    ui->progressBar_GPU_usage->setValue(gpu_utilization);
}

void MainWindow::on_GpuUtilizationsController_memory_utilization(unsigned memory_utilization, unsigned used_memory)
{
    ui->progressBar_GPU_mem_usage->setValue(memory_utilization);
    ui->lineEdit_GPU_mem_usage->setText(QString::number(used_memory) + " Mib");
}

void MainWindow::on_GpuUtilizationsController_encoder_decoder_utilization(unsigned encoder_utilization, unsigned decoder_utilization)
{
    ui->progressBar_GPU_encoder_usage->setValue(encoder_utilization);
    ui->progressBar_GPU_decoder_usage->setValue(decoder_utilization);
}

void MainWindow::on_GpuPowerController_power_usage(unsigned power_usage)
{
}

void MainWindow::on_GpuPowerController_power_limit(unsigned power_limit)
{
}

void MainWindow::connect_slots_and_signals()
{
    connect(&tray_icon_, &QSystemTrayIcon::activated, this, &MainWindow::toggle_tray);

    connect(&gpu_utilizations_controller_, &GpuUtilizationsContoller::gpu_utilization, this, &MainWindow::on_GpuUtilizationsController_gpu_utilization);
    connect(&gpu_utilizations_controller_, &GpuUtilizationsContoller::memory_utilization, this, &MainWindow::on_GpuUtilizationsController_memory_utilization);
    connect(&gpu_utilizations_controller_, &GpuUtilizationsContoller::encoder_decoder_utilization, this, &MainWindow::on_GpuUtilizationsController_encoder_decoder_utilization);
    connect(&gpu_power_controller_, &GpuPowerController::power_usage, this, &MainWindow::on_GpuPowerController_power_usage);
    connect(&gpu_power_controller_, &GpuPowerController::power_limit, this, &MainWindow::on_GpuPowerController_power_limit);

    connect(&dynamic_info_update_timer_, &QTimer::timeout, this, [this]()
    {
        gpu_utilizations_controller_.update_info();
        gpu_power_controller_.update_info();
    });
    connect(&settings_manager_, &SettingsManager::error, this, [this](const QString& err_msg)
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
    tray_menu->addAction("Quit", this, [this]()
    {
        minimize_to_tray_on_close_ = false;
        close();
    });

    tray_icon_.setContextMenu(tray_menu);
}

void MainWindow::load_app_settings()
{
    auto& settings_manager {SettingsManager::instance()};
    settings_manager.open_file(QIODevice::ReadOnly);

    const auto app_settings{settings_manager.load_settings()};
    settings_manager.close_file();

    minimize_to_tray_on_close_ = app_settings["minimize_to_tray_on_close"].toBool();
    update_freq_ms_ = app_settings["update_freq_ms"].toInt();
    dynamic_info_update_timer_.setInterval(update_freq_ms_);

    qDebug().noquote().nospace() << "Settings has been loaded: " << app_settings;
}

void MainWindow::set_static_info()
{
    const auto& current_gpu {get_current_gpu()};
    ui->lineEdit_GPU_name->setText(QString::fromStdString(current_gpu->get_name()));
    ui->lineEdit_GPU_arch->setText(QString::fromStdString(current_gpu->get_arch()));
    ui->lineEdit_GPU_uuid->setText(QString::fromStdString(current_gpu->get_uuid()));
    ui->lineEdit_GPU_VBIOS_ver->setText(QString::fromStdString(current_gpu->get_vbios_version()));
    ui->lineEdit_GPU_driver_ver->setText(QString::fromStdString(NVMLpp::Session::instance().get_system_driver_version()));
    ui->lineEdit_GPU_total_mem->setText(QString::number(current_gpu->get_total_memory()) + " MiB");

    qDebug().noquote().nospace() << "Static info has been set";
}

void MainWindow::load_GPUs()
{
    nvml_devices_list_ = NVMLpp::Session::instance().get_devices();
    for (const auto& gpu : nvml_devices_list_)
    {
        ui->comboBox_select_GPU->addItem(QString::fromStdString(gpu.get_name()));
    }

    const auto& current_gpu {get_current_gpu()};
    gpu_utilizations_controller_.set_device(current_gpu);
    gpu_power_controller_.set_device(current_gpu);

    qDebug().noquote().nospace() << "Total GPUs found and loaded: " << nvml_devices_list_.size();
}

NVMLpp::NVML_device* MainWindow::get_current_gpu()
{
    const int current_device_index {ui->comboBox_select_GPU->currentIndex()};
    return &nvml_devices_list_[current_device_index];
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    qDebug().noquote().nospace() << "minimize_to_tray_on_close_=" << minimize_to_tray_on_close_;

    if (minimize_to_tray_on_close_)
    {
        event->ignore();
        hide();
        tray_icon_.show();
        qDebug().noquote().nospace() << "Event ignored, minimized to tray";
    }
    else
    {
        tray_icon_.hide();
        event->accept();
        qDebug().noquote().nospace() << "Event accepted, MainWindow closed";
    }
}

void MainWindow::on_comboBox_select_GPU_activated(int index)
{
    const auto& current_gpu {get_current_gpu()};
    gpu_utilizations_controller_.set_device(current_gpu);
    gpu_power_controller_.set_device(current_gpu);
    qDebug().noquote().nospace() << "GPU selected: " << ui->comboBox_select_GPU->currentText();
}
