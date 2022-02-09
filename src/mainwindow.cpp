#include "NVML.hpp"

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent}
    , ui{new Ui::MainWindow}
    , nvml_device_{}
{
    ui->setupUi(this);
    setMinimumSize(size());

    ui->lineEdit_GPU_name->setText(QString::fromStdString(nvml_device_.get_name()));
    ui->lineEdit_GPU_driver_version->setText(QString::fromStdString(nvml_device_.get_system_driver_version()));
    ui->lineEdit_GPU_VBIOS_version->setText(QString::fromStdString(nvml_device_.get_vbios_version()));
    ui->lineEdit_GPU_UUID->setText(QString::fromStdString(nvml_device_.get_uuid()));
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
