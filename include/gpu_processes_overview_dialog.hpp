#pragma once

#include <QDialog>
#include <QTimer>
#include "nvmlpp/nvmlpp_device.hpp"

namespace Ui { class GpuProcessesOverviewDialog; }

class GpuProcessesOverviewDialog final : public QDialog
{
    Q_OBJECT

    enum { CELL_PROC_PID, CELL_PROC_MEM_USAGE, CELL_PROC_NAME };

public:
    explicit GpuProcessesOverviewDialog(QWidget* parent = nullptr);
    ~GpuProcessesOverviewDialog();

    void set_current_gpu(const NVMLpp::NVML_device* curr_gpu) noexcept;

private slots:
    void on_buttonBox_rejected();
    void show_processes_info();
    void on_tableWidget_proc_info_cellDoubleClicked(int row, int column);

private:
    Ui::GpuProcessesOverviewDialog* ui;
    const NVMLpp::NVML_device* current_gpu_;
    QTimer timer_;

    void hideEvent(QHideEvent* hide_event);
    void showEvent(QShowEvent* show_event);
    void closeEvent(QCloseEvent* close_event);

    std::string get_process_name_by_pid(pid_t proc_pid) const;
};
