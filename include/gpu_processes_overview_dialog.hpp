#pragma once

#include <vector>

#include <QDialog>
#include <QTimer>

#include "gpu_processes_controller.hpp"

namespace Ui { class GpuProcessesOverviewDialog; }

class GpuProcessesOverviewDialog final : public QDialog
{
    Q_OBJECT

    enum { CELL_PROC_PID, CELL_PROC_MEM_USAGE, CELL_PROC_NAME };

public:
    explicit GpuProcessesOverviewDialog(QWidget* parent = nullptr);
    ~GpuProcessesOverviewDialog();

    void set_current_gpu(NVMLpp::NVML_device* curr_gpu);

private slots:
    void on_buttonBox_rejected();
    void on_GpuProcessController_info_ready(const std::vector<NVML_native::nvmlProcessInfo_t>& proc_list);
    void on_tableWidget_proc_info_cellDoubleClicked(int row, int column);

private:
    Ui::GpuProcessesOverviewDialog* ui;
    GpuProcessesController gpu_process_controller_;
    QTimer timer_;

    void hideEvent(QHideEvent* hide_event);
    void showEvent(QShowEvent* show_event);
    void closeEvent(QCloseEvent* close_event);

    std::string get_process_name_by_pid(pid_t proc_pid) const;
};
