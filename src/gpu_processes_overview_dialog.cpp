#include <signal.h>
#include <sys/types.h>

#include <cstring>
#include <vector>

#include <QShowEvent>
#include <QCloseEvent>
#include <QMessageBox>
#include <QTableWidgetItem>
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

    initialize_table();

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

    if (proc_list.size() > MAX_ROWS)
    {
        resize_table(proc_list.size());
    }

    ui->tableWidget_proc_info->setRowCount(proc_list.size());
    int curr_row {0};
    std::for_each(proc_list.begin(), proc_list.end(), [this, &curr_row](const NVML_native::nvmlProcessInfo_t& process)
    {
        ui->tableWidget_proc_info->item(curr_row, CELL_PROC_PID)->setText(QString::number(process.pid));
        ui->tableWidget_proc_info->item(curr_row, CELL_PROC_MEM_USAGE)->setText(QString{"%1 MiB"}.arg(process.usedGpuMemory / 1024 / 1024));
        ui->tableWidget_proc_info->item(curr_row, CELL_PROC_NAME)->setText(QString::fromStdString(get_process_name_by_pid(process.pid)));
        curr_row++;
    });
}



void GpuProcessesOverviewDialog::on_tableWidget_proc_info_cellDoubleClicked(int row, [[maybe_unused]] int column)
{
    const int pid {ui->tableWidget_proc_info->item(row, CELL_PROC_PID)->text().toInt()};
    const auto ans {
        QMessageBox::question(this, "Confirm action",
                              QString{"Are you sure what you WANT TO KILL process with PID=%1?"}.arg(pid),
                              QMessageBox::Button::Yes, QMessageBox::Button::No)
    };
    if (ans == QMessageBox::Button::Yes)
    {
        // Is it good? Use Linux API directrly from C++ code
        // I doesnt`t know other method to kill (terminate) the process without using polkit
        if (::kill(pid, SIGTERM) == 0)
        {
            qInfo().noquote().nospace() << "Process " << pid << " succesfully killed!";
        }
        else
        {
            const QString err_str {QString{"Trying to kill process %1, got error: %2"}.arg(pid).arg(std::strerror(errno))};
            qCritical().noquote().nospace() << err_str;
            QMessageBox::critical(this, "Error occured", err_str);
        }
    }
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



void GpuProcessesOverviewDialog::initialize_table()
{
    ui->tableWidget_proc_info->setHorizontalHeaderLabels({"PID", "Memory", "Command"});
    ui->tableWidget_proc_info->setRowCount(MAX_ROWS);
    for (int row {0}; row < MAX_ROWS; row++)
    {
        ui->tableWidget_proc_info->setItem(row, CELL_PROC_PID, new QTableWidgetItem {});
        ui->tableWidget_proc_info->setItem(row, CELL_PROC_MEM_USAGE, new QTableWidgetItem {});
        ui->tableWidget_proc_info->setItem(row, CELL_PROC_NAME, new QTableWidgetItem {});
    }
}



void GpuProcessesOverviewDialog::resize_table(int new_size)
{
    for (int row {MAX_ROWS}; row < new_size; row++)
    {
        ui->tableWidget_proc_info->insertRow(row);
        ui->tableWidget_proc_info->setItem(row, CELL_PROC_PID, new QTableWidgetItem {});
        ui->tableWidget_proc_info->setItem(row, CELL_PROC_MEM_USAGE, new QTableWidgetItem {});
        ui->tableWidget_proc_info->setItem(row, CELL_PROC_NAME, new QTableWidgetItem {});
    }
}
