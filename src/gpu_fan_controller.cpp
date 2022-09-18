#include "nvmlpp/util/nvmlpp_errors.hpp"

#include "gpu_fan_controller.hpp"

GpuFanController::GpuFanController(NVMLpp::NVML_device* nvml_device, QObject* parent)
    : QObject {parent}
    , current_gpu_ {nvml_device}
{ }

void GpuFanController::update_info()
{
    try
    {
        fan_rates fan_rates_ {
            .speed = current_gpu_->get_current_fan_speed_level()
        };
        emit info_ready(fan_rates_);
    }
    catch (const NVMLpp::errors::error&) { }
}

void GpuFanController::set_fan_speed(unsigned fan_speed_level)
{
}
