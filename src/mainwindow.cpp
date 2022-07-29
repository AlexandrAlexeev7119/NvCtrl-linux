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

    set_static_info();
    update_dynamic_info();

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
    auto& settings_manager {SettingsManager::instance()};
    settings_manager.open_file(QIODevice::ReadOnly);

    const auto app_settings{settings_manager.load_settings()};
    settings_manager.close_file();

    minimize_to_tray_on_close_ = app_settings["minimize_to_tray_on_close"].toBool();
    update_freq_ms_ = app_settings["update_freq_ms"].toInt();
    dynamic_info_update_timer_.setInterval(update_freq_ms_);
}

void MainWindow::set_static_info()
{
    const auto& current_gpu {get_current_gpu()};
    ui->lineEdit_GPU_name->setText(QString::fromStdString(current_gpu.get_name()));
    ui->lineEdit_GPU_arch->setText(QString::fromStdString(current_gpu.get_arch()));
    ui->lineEdit_GPU_uuid->setText(QString::fromStdString(current_gpu.get_uuid()));
    ui->lineEdit_GPU_VBIOS_ver->setText(QString::fromStdString(current_gpu.get_vbios_version()));
    ui->lineEdit_GPU_driver_ver->setText(QString::fromStdString(NVMLpp::Session::instance().get_system_driver_version()));
    ui->lineEdit_GPU_total_mem->setText(QString::number(current_gpu.get_total_memory()) + " MiB");
}

void MainWindow::update_dynamic_info()
{
    const auto& current_gpu {get_current_gpu()};
    ui->progressBar_GPU_usage->setValue(current_gpu.get_gpu_utilization());
    ui->progressBar_GPU_mem_usage->setValue(current_gpu.get_memory_utilization());
    ui->lineEdit_GPU_mem_usage->setText(QString::number(current_gpu.get_used_memory()) + " MiB");
    ui->progressBar_GPU_decoder_usage->setValue(current_gpu.get_decoder_utilization());
    ui->progressBar_GPU_encoder_usage->setValue(current_gpu.get_encoder_utilization());

    qDebug().noquote() << "Update dynamic info for: " << QString::fromStdString(current_gpu.get_name());
}

void MainWindow::load_GPUs()
{
    nvml_devices_list_ = NVMLpp::Session::instance().get_devices();
    for (const auto& gpu : nvml_devices_list_)
    {
        ui->comboBox_select_GPU->addItem(QString::fromStdString(gpu.get_name()));
    }
}

NVMLpp::NVML_device& MainWindow::get_current_gpu()
{
    const int current_device_index {ui->comboBox_select_GPU->currentIndex()};
    return nvml_devices_list_[current_device_index];
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
