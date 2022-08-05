#include "nvmlpp/util/nvmlpp_errors.hpp"

#include "gpu_clock_controller.hpp"

GpuClockController::GpuClockController(const NVMLpp::NVML_device* nvml_device, QObject* parrent)
    : QObject {parrent}
    , current_gpu_ {nvml_device}
{ }

void GpuClockController::update_info()
{
    try
    {
        emit graphics_clock(current_gpu_->get_clock_graphics());
        emit video_clock(current_gpu_->get_clock_video());
        emit sm_clock(current_gpu_->get_clock_sm());
        emit memory_clock(current_gpu_->get_clock_memory());
    }
    catch (const NVMLpp::errors::error&)
    {
        emit error();
    }
}
