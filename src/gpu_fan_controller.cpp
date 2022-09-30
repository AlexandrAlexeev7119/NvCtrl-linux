#include <QProcess>
#include <QDebug>

#include "nvmlpp/util/nvmlpp_errors.hpp"
#include "gpu_fan_controller.hpp"

static constexpr const char* NVIDIA_SETTINGS_BIN {"/usr/bin/nvidia-settings"};
static constexpr const char* NVIDIA_SETTINGS_GPU_FAN_CONTROL_STATE {"[gpu:%1]/GPUFanControlState=%2"};
static constexpr const char* NVIDIA_SETTINGS_GPU_FAN_SPEED {"[fan:0]/GPUTargetFanSpeed=%1"};



GpuFanController::GpuFanController(NVMLpp::NVML_device* nvml_device, QObject* parent)
    : QObject {parent}
    , current_gpu_ {nvml_device}
{ }



void GpuFanController::update_info()
{
    try
    {
        fan_rates fan_rates_ {
            .fan_speed_level = current_gpu_->get_current_fan_speed_level()
        };
        emit info_ready(fan_rates_);
    }
    catch (const NVMLpp::errors::error_not_supported& err)
    {
        emit error();
    }
}



void GpuFanController::set_fan_speed(unsigned fan_speed_level)
{
    const auto set_fan_speed {QString{NVIDIA_SETTINGS_GPU_FAN_SPEED}.arg(fan_speed_level)};
    run_nvidia_settings(set_fan_speed);
}



void GpuFanController::set_fan_control_state(bool value)
{
    const auto enable_fan_control {
        QString{NVIDIA_SETTINGS_GPU_FAN_CONTROL_STATE}
                .arg(current_gpu_->get_index())
                .arg(static_cast<int>(value))
    };
    run_nvidia_settings(enable_fan_control);
}



void GpuFanController::run_nvidia_settings(const QString& arg)
{
    auto err_code {QProcess::execute(NVIDIA_SETTINGS_BIN, {"-a", arg})};
    if (err_code == 0)
    {
        qInfo().noquote().nospace() << "Option applied: " << arg << " for current gpu";
    }
    else
    {
        qCritical().nospace().noquote() << "Failed to apply " << arg << " for current gpu";
    }
}
