#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent}
    , ui{new Ui::MainWindow}
    , gpu_static_info_loader_{}
    , gpu_dynamic_info_loader_{}
{
    ui->setupUi(this);
    connect(&gpu_dynamic_info_loader_, &GPUDynamicInfoLoader::update_info, this, &MainWindow::update_dynamic_info);

    const auto static_info{gpu_static_info_loader_.get_static_GPU_info()};

    ui->lineEdit_GPU_name->setText(static_info.at(GPUStaticInfoType::GPU_NAME));
    ui->lineEdit_GPU_VBIOS_version->setText(static_info.at(GPUStaticInfoType::VBIOS_VER));
    ui->lineEdit_GPU_driver_version->setText(static_info.at(GPUStaticInfoType::DRIVER_VER));
    ui->lineEdit_PCIe_info->setText(static_info.at(GPUStaticInfoType::BUS_INFO));
    ui->lineEdit_GPU_UUID->setText(static_info.at(GPUStaticInfoType::GPU_UUID));
}

void MainWindow::update_dynamic_info(const std::unordered_map<GPUDynamicInfoLoaderType, QString>& dynamic_info)
{
    const QString gpu_utilization_percengate{dynamic_info.at(GPUDynamicInfoLoaderType::GPU_UTILIZATION_PERCENTAGE)};
    const QString memory_utilization_percentage{dynamic_info.at(GPUDynamicInfoLoaderType::MEMORY_UTILIZATION_PERCENTAGE)};
    const QString encoder_utilization_percentage{dynamic_info.at(GPUDynamicInfoLoaderType::ENCODER_USAGE_PERCENTAGE)};
    const QString decoder_utilization_percentage{dynamic_info.at(GPUDynamicInfoLoaderType::DECODER_USAGE_PERCENTAGE)};

    ui->lineEdit_GPU_usage_percentage->setText(gpu_utilization_percengate + "%");
    ui->lineEdit_memory_usage_percentage->setText(memory_utilization_percentage + "%");
    ui->lineEdit_encoder_usage_percentage->setText(encoder_utilization_percentage + "%");
    ui->lineEdit_decoder_usage_percentage->setText(decoder_utilization_percentage + "%");

    ui->progressBar_GPU_usage_percentage->setValue(gpu_utilization_percengate.toUInt());
    ui->progressBar_memory_usage_percentage->setValue(memory_utilization_percentage.toUInt());
    ui->progressBar_encoder_usage_percentage->setValue(encoder_utilization_percentage.toUInt());
    ui->progressBar_decoder_usage_percentage->setValue(decoder_utilization_percentage.toUInt());

    ui->lineEdit_draw_power_consumption->setText(dynamic_info.at(GPUDynamicInfoLoaderType::POWER_USAGE_WATTS) + " W");
}


MainWindow::~MainWindow()
{
    delete ui;
}
