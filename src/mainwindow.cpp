#include <unistd.h>

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent}
    , ui{new Ui::MainWindow}
    , gpu_static_info_loader_{}
    , gpu_dynamic_info_loader_{}
{
    ui->setupUi(this);
    setMinimumSize(size());
    connect(&gpu_dynamic_info_loader_, &GPUDynamicInfoLoader::update_info, this, &MainWindow::update_dynamic_info);
    set_static_info();
}

void MainWindow::update_dynamic_info(const std::unordered_map<GPUDynamicInfoLoaderType, QString>& dynamic_info)
{
    const QString gpu_utilization_percengate{dynamic_info.at(GPUDynamicInfoLoaderType::GPU_UTILIZATION_PERCENTAGE)};
    const QString memory_utilization_percentage{dynamic_info.at(GPUDynamicInfoLoaderType::MEMORY_UTILIZATION_PERCENTAGE)};
    const QString encoder_utilization_percentage{dynamic_info.at(GPUDynamicInfoLoaderType::ENCODER_USAGE_PERCENTAGE)};
    const QString decoder_utilization_percentage{dynamic_info.at(GPUDynamicInfoLoaderType::DECODER_USAGE_PERCENTAGE)};
    const QString current_gpu_temperature{dynamic_info.at(GPUDynamicInfoLoaderType::GPU_CURRENT_TEMPERATURE)};

    ui->progressBar_GPU_usage_percentage->setValue(gpu_utilization_percengate.toUInt());
    ui->progressBar_memory_usage_percentage->setValue(memory_utilization_percentage.toUInt());
    ui->progressBar_encoder_usage_percentage->setValue(encoder_utilization_percentage.toUInt());
    ui->progressBar_decoder_usage_percentage->setValue(decoder_utilization_percentage.toUInt());

    ui->lineEdit_draw_power_consumption->setText(dynamic_info.at(GPUDynamicInfoLoaderType::POWER_USAGE_WATTS) + " W");
    ui->lineEdit_current_temperature->setText(current_gpu_temperature + " °C");
}

void MainWindow::set_static_info()
{
    const auto static_info{gpu_static_info_loader_.get_static_GPU_info()};

    const QString gpu_max_power_usage{static_info.at(GPUStaticInfoType::GPU_MAX_POWER_USAGE)};
    const QString gpu_min_power_usage{static_info.at(GPUStaticInfoType::GPU_MIN_POWER_USAGE)};
    const QString gpu_default_power_limit{static_info.at(GPUStaticInfoType::GPU_DEFAULT_POWER_LIMIT)};
    const QString gpu_current_power_limit{static_info.at(GPUStaticInfoType::GPU_CURRENT_POWER_LIMIT)};

    ui->lineEdit_GPU_name->setText(static_info.at(GPUStaticInfoType::GPU_NAME));
    ui->lineEdit_GPU_VBIOS_version->setText(static_info.at(GPUStaticInfoType::VBIOS_VER));
    ui->lineEdit_GPU_driver_version->setText(static_info.at(GPUStaticInfoType::DRIVER_VER));
    ui->lineEdit_PCIe_info->setText(static_info.at(GPUStaticInfoType::BUS_INFO));
    ui->lineEdit_GPU_UUID->setText(static_info.at(GPUStaticInfoType::GPU_UUID));
    ui->lineEdit_memory_info->setText(static_info.at(GPUStaticInfoType::GPU_TOTAL_MEMORY) + " MiB");

    ui->lineEdit_slowdown_temperature->setText(static_info.at(GPUStaticInfoType::GPU_SLOWDOWN_TEMPERATURE) + " °C");
    ui->lineEdit_shutdown_temperature->setText(static_info.at(GPUStaticInfoType::GPU_SHUTDOWN_TEMPERATURE) + " °C");

    ui->lineEdit_max_power_consumption->setText(gpu_max_power_usage + " W");
    ui->lineEdit_min_power_consumption->setText(gpu_min_power_usage + " W");
    ui->lineEdit_default_power_consumption->setText(gpu_default_power_limit + " W");
    ui->lineEdit_current_power_limit->setText(gpu_current_power_limit + " W");

    ui->horizontalSlider_power_limit->setMaximum(gpu_max_power_usage.toInt());
    ui->horizontalSlider_power_limit->setMinimum(gpu_min_power_usage.toUInt());
    ui->horizontalSlider_power_limit->setValue(gpu_current_power_limit.toInt());

    ui->label_min_power_limit->setText(gpu_min_power_usage);
    ui->label_max_power_limit->setText(gpu_max_power_usage);
}

void MainWindow::on_pushButton_apply_power_settings_clicked()
{
}

void MainWindow::on_action_Exit_triggered()
{
    close();
}

MainWindow::~MainWindow()
{
    delete ui;
}

