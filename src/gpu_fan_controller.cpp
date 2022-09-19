#include <QProcess>
#include <QDebug>

#include "nvmlpp/util/nvmlpp_errors.hpp"
#include "gpu_fan_controller.hpp"

static constexpr const char* NVIDIA_SETTINGS_BIN {"/usr/bin/nvidia-settings"};
static const QString NVIDIA_SETTINGS_GPU_FAN_CONTROL_STATE {"[gpu:%1]/GPUFanControlState=1"};
static const QString NVIDIA_SETTINGS_GPU_FAN_SPEED {"[fan:0]/GPUTargetFanSpeed=%1"};



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
    catch (const NVMLpp::errors::error_not_supported& err)
    {
        emit error();
    }
}

void GpuFanController::set_fan_speed(unsigned device_index, unsigned fan_speed_level)
{
    const auto enable_fan_control {NVIDIA_SETTINGS_GPU_FAN_CONTROL_STATE.arg(device_index)};
    const auto set_fan_speed {NVIDIA_SETTINGS_GPU_FAN_SPEED.arg(fan_speed_level)};

    auto err_code {QProcess::execute(NVIDIA_SETTINGS_BIN, {"-a", enable_fan_control, "-a", set_fan_speed})};
    if (err_code != 0)
    {
        qCritical().nospace().noquote() << "Error to apply fan speed for gpu" << device_index;
    }
}
