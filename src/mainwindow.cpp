#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent}
    , ui{new Ui::MainWindow}
{
    ui->setupUi(this);

    auto info{gpu_info_loader_.get_static_GPU_info()};

    ui->lineEdit_GPU_name->setText(info[GPUStaticInfoType::GPU_NAME]);
    ui->lineEdit_GPU_VBIOS_version->setText(info[GPUStaticInfoType::VBIOS_VER]);
    ui->lineEdit_GPU_driver_version->setText(info[GPUStaticInfoType::DRIVER_VER]);
    ui->lineEdit_PCIe_info->setText(info[GPUStaticInfoType::BUS_INFO]);
    ui->lineEdit_GPU_UUID->setText(info[GPUStaticInfoType::GPU_UUID]);
}

MainWindow::~MainWindow()
{
    delete ui;
}

