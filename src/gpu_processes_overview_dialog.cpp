#include <QShowEvent>
#include <QCloseEvent>
#include <QDebug>

#include "gpu_processes_overview_dialog.hpp"
#include "ui_gpu_processes_overview_dialog.h"

GpuProcessesOverviewDialog::GpuProcessesOverviewDialog(QWidget* parent)
    : QDialog {parent}
    , ui {new Ui::GpuProcessesOverviewDialog}
    , gpu_process_controller_ {this}
    , timer_ {this}
{
    ui->setupUi(this);
    setMinimumSize(size());
    setMaximumSize(size() * 1.2);

    timer_.setInterval(1300);
    connect(&timer_, &QTimer::timeout, &gpu_process_controller_, &GpuProcessesController::update_info);
}



GpuProcessesOverviewDialog::~GpuProcessesOverviewDialog()
{
    delete ui;
}



void GpuProcessesOverviewDialog::set_current_gpu(NVMLpp::NVML_device* curr_gpu)
{
    gpu_process_controller_.set_current_gpu(curr_gpu);
}



void GpuProcessesOverviewDialog::on_buttonBox_rejected()
{
    close();
}



void GpuProcessesOverviewDialog::showEvent(QShowEvent* show_event)
{
    timer_.start();
    show_event->accept();
}



void GpuProcessesOverviewDialog::closeEvent(QCloseEvent* close_event)
{
    timer_.stop();
    close_event->accept();
}
