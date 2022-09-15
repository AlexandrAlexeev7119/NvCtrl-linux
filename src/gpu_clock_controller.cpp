#include "nvmlpp/util/nvmlpp_errors.hpp"

#include "gpu_clock_controller.hpp"

GpuClockController::GpuClockController(const NVMLpp::NVML_device* nvml_device, QObject* parrent)
    : QObject {parrent}
    , current_gpu_ {nvml_device}
{ }

void GpuClockController::update_info()
{
    clock_values clock_values_;
    try
    {
        clock_values_.graphics = current_gpu_->get_clock_graphics();
        clock_values_.video = current_gpu_->get_clock_video();
        clock_values_.sm = current_gpu_->get_clock_sm();
        clock_values_.mem = current_gpu_->get_clock_memory();
        emit info_ready(clock_values_);
    }
    catch (const NVMLpp::errors::error&)
    {
        emit error();
    }
}
