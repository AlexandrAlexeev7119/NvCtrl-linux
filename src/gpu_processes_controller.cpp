#include <QDebug>

#include "gpu_processes_controller.hpp"

GpuProcessesController::GpuProcessesController(QObject* parent)
    : QObject {parent}
    , current_gpu_ {nullptr}
{ }



void GpuProcessesController::set_current_gpu(const NVMLpp::NVML_device* current_gpu) noexcept
{
    current_gpu_ = current_gpu;
}



void GpuProcessesController::update_info()
{
    emit info_ready(current_gpu_->get_running_processes());
}
