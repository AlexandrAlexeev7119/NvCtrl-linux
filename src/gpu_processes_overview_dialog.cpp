#include <QShowEvent>
#include <QCloseEvent>
#include <QLabel>
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
    ui->tableWidget_proc_info->setHorizontalHeaderLabels({"PID", "Memory", "Command"});

    timer_.setInterval(1200);
    connect(&timer_, &QTimer::timeout, &gpu_process_controller_, &GpuProcessesController::update_info);
    connect(&gpu_process_controller_, &GpuProcessesController::info_ready, this, &GpuProcessesOverviewDialog::on_GpuProcessController_info_ready);
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



void GpuProcessesOverviewDialog::on_GpuProcessController_info_ready(const std::vector<NVML_native::nvmlProcessInfo_t>& proc_list)
{
    using vec_size_t = std::vector<NVML_native::nvmlProcessInfo_t>::size_type;
    if (static_cast<vec_size_t>(ui->tableWidget_proc_info->rowCount()) != proc_list.size())
    {
        ui->tableWidget_proc_info->clearContents();
        ui->tableWidget_proc_info->setRowCount(proc_list.size());

        int curr_row {0};
        std::for_each(proc_list.begin(), proc_list.end(), [this, &curr_row](const NVML_native::nvmlProcessInfo_t& process)
        {
            ui->tableWidget_proc_info->setCellWidget(curr_row, CELL_PROC_PID, new QLabel {
                                                         QString::number(process.pid), ui->tableWidget_proc_info
                                                     });
            ui->tableWidget_proc_info->setCellWidget(curr_row, CELL_PROC_MEM_USAGE, new QLabel {
                                                         QString{"%1 MiB"}.arg(process.usedGpuMemory / 1024 / 1024), ui->tableWidget_proc_info
                                                     });
            ui->tableWidget_proc_info->setCellWidget(curr_row, CELL_PROC_NAME, new QLabel {
                                                         QString::fromStdString(get_process_name_by_pid(process.pid)), ui->tableWidget_proc_info
                                                     });
            curr_row++;
        });
    }
}



void GpuProcessesOverviewDialog::on_tableWidget_proc_info_cellDoubleClicked([[maybe_unused]] int row, [[maybe_unused]] int column)
{ }



void GpuProcessesOverviewDialog::hideEvent(QHideEvent* hide_event)
{
    timer_.stop();
    hide_event->accept();
}



void GpuProcessesOverviewDialog::showEvent(QShowEvent* show_event)
{
    gpu_process_controller_.update_info();
    timer_.start();
    show_event->accept();
}



void GpuProcessesOverviewDialog::closeEvent(QCloseEvent* close_event)
{
    timer_.stop();
    close_event->accept();
}



std::string GpuProcessesOverviewDialog::get_process_name_by_pid(pid_t proc_pid) const
{
    char file_name[64] {0};
    std::sprintf(file_name, "/proc/%d/cmdline", proc_pid);
    std::FILE* file {std::fopen(file_name, "r")};

    char buff[256] {0};
    if (file)
    {
        std::fread(buff, sizeof(char), sizeof(buff), file);
        std::fclose(file);
    }

    return buff;
}
