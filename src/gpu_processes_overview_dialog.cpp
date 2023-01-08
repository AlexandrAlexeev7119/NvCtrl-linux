#include <signal.h>
#include <sys/types.h>

#include <cstring>
#include <vector>

#include <QShowEvent>
#include <QCloseEvent>
#include <QMessageBox>
#include <QLabel>
#include <QDebug>

#include "gpu_processes_overview_dialog.hpp"
#include "ui_gpu_processes_overview_dialog.h"

GpuProcessesOverviewDialog::GpuProcessesOverviewDialog(QWidget* parent)
    : QDialog {parent}
    , ui {new Ui::GpuProcessesOverviewDialog}
    , current_gpu_ {nullptr}
    , timer_ {this}
{
    ui->setupUi(this);
    setMinimumSize(size());

    ui->tableWidget_proc_info->setHorizontalHeaderLabels({"PID", "Memory", "Command"});

    timer_.setInterval(1200);
    connect(&timer_, &QTimer::timeout, this, &GpuProcessesOverviewDialog::show_processes_info);
}



GpuProcessesOverviewDialog::~GpuProcessesOverviewDialog()
{
    delete ui;
}



void GpuProcessesOverviewDialog::set_current_gpu(const NVMLpp::NVML_device* curr_gpu) noexcept
{
    current_gpu_ = curr_gpu;
}



void GpuProcessesOverviewDialog::on_buttonBox_rejected()
{
    close();
}



void GpuProcessesOverviewDialog::show_processes_info()
{
    const auto proc_list {current_gpu_->get_running_processes()};
    ui->tableWidget_proc_info->setRowCount(proc_list.size());

    int curr_row {0};
    for (const auto& process : proc_list)
    {
        ui->tableWidget_proc_info->setCellWidget(curr_row, CELL_PROC_PID, new QLabel {QString::number(process.pid), this});
        ui->tableWidget_proc_info->setCellWidget(curr_row, CELL_PROC_MEM_USAGE, new QLabel {QString{QStringLiteral("%1 MiB")}.arg(process.usedGpuMemory / 1024 / 1024), this});
        ui->tableWidget_proc_info->setCellWidget(curr_row, CELL_PROC_NAME, new QLabel {get_process_name_by_pid(process.pid), this});
        curr_row++;
    }
}



void GpuProcessesOverviewDialog::on_tableWidget_proc_info_cellDoubleClicked([[maybe_unused]] int row, [[maybe_unused]] int column)
{
#ifndef _WIN32
    const int pid {static_cast<QLabel*>(
                    ui->tableWidget_proc_info->cellWidget(row, CELL_PROC_PID))
                ->text().toInt()};
    const auto ans {
        QMessageBox::question(this, QStringLiteral("Confirm action"),
                              QStringLiteral("Do you really WANT TO KILL process with PID=%1?").arg(pid),
                              QMessageBox::Button::Yes, QMessageBox::Button::No)
    };
    if (ans == QMessageBox::Button::Yes)
    {
        if (::kill(pid, SIGTERM) == 0)
        {
            qInfo().noquote().nospace() << "Process " << pid << " succesfully killed!";
        }
        else
        {
            const QString err_str {QString{"Trying to kill process %1, got error: %2"}.arg(pid).arg(std::strerror(errno))};
            qCritical().noquote().nospace() << err_str;
            QMessageBox::critical(this, QStringLiteral("Error occured"), err_str);
        }
    }
#endif
}



void GpuProcessesOverviewDialog::hideEvent(QHideEvent* hide_event)
{
    qDebug().noquote().nospace() << "Hide event emited, stopping timer";

    timer_.stop();
    hide_event->accept();
}



void GpuProcessesOverviewDialog::showEvent(QShowEvent* show_event)
{
    qDebug().noquote().nospace() << "Show event emited, restarting timer";

    show_processes_info();
    timer_.start();
    show_event->accept();
}



void GpuProcessesOverviewDialog::closeEvent(QCloseEvent* close_event)
{
    timer_.stop();
    close_event->accept();
}



QString GpuProcessesOverviewDialog::get_process_name_by_pid([[maybe_unused]] pid_t proc_pid) const
{
    char file_name[32] {0};
    std::sprintf(file_name, "/proc/%d/cmdline", proc_pid);
    std::FILE* file {std::fopen(file_name, "r")};

    if (file)
    {
        char buff[128] {0};
        std::fread(buff, sizeof(char), (sizeof(buff) - 1), file);
        std::fclose(file);
        return buff;
    }

    return {};
}
