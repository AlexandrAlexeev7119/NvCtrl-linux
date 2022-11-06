#pragma once

#include <QDialog>
#include <QTimer>

#include "gpu_processes_controller.hpp"

namespace Ui { class GpuProcessesOverviewDialog; }

class GpuProcessesOverviewDialog final : public QDialog
{
    Q_OBJECT

public:
    explicit GpuProcessesOverviewDialog(QWidget* parent = nullptr);
    ~GpuProcessesOverviewDialog();

    void set_current_gpu(NVMLpp::NVML_device* curr_gpu);

private slots:
    void on_buttonBox_rejected();

private:
    Ui::GpuProcessesOverviewDialog* ui;
    GpuProcessesController gpu_process_controller_;
    QTimer timer_;

    void showEvent(QShowEvent* show_event);
    void closeEvent(QCloseEvent* close_event);
};
