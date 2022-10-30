#include <QProcess>
#include <QDebug>

#include "nvmlpp/util/nvmlpp_errors.hpp"
#include "gpu_fan_controller.hpp"

constexpr const char* NVIDIA_SETTINGS_BIN {"/usr/bin/nvidia-settings"};
constexpr const char* NVIDIA_SETTINGS_GPU_FAN_CONTROL_STATE {"[gpu:%1]/GPUFanControlState=%2"};
constexpr const char* NVIDIA_SETTINGS_GPU_FAN_SPEED {"[fan:0]/GPUTargetFanSpeed=%1"};



GpuFanController::GpuFanController(QObject* parent)
    : QObject {parent}
    , current_gpu_ {nullptr}
    , ptr_current_fan_profile_ {nullptr}
    , timer_ {this}
{
    timer_.setInterval(1000);
    connect(&timer_, &QTimer::timeout, this, &GpuFanController::apply_fan_speed_from_profile);
}



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
        emit error_occured();
    }
}



void GpuFanController::set_fan_control_state_enabled(bool enabled)
{
    const auto set_fan_control_command {
        QString{NVIDIA_SETTINGS_GPU_FAN_CONTROL_STATE}
                .arg(current_gpu_->get_index())
                .arg(static_cast<int>(enabled))
    };
    if (!enabled)
    {
        timer_.stop();
    }
    run_nvidia_settings(set_fan_control_command);
}



void GpuFanController::load_fan_speed_profile(const nlohmann::json* fan_profile) noexcept
{
    ptr_current_fan_profile_ = fan_profile;
    timer_.start();
}



void GpuFanController::apply_fan_speed_from_profile()
{
    const auto& fan_profile {*ptr_current_fan_profile_};
    const auto& temperature_steps = fan_profile["steps"];
    // Very very bad implementation
    // But it works
    // #FIXME
    const unsigned current_temp {current_gpu_->get_current_temperature()};
    if (current_temp >= temperature_steps.back()[TEMPERATURE_VALUE].get<unsigned>())
    {
        set_fan_speed(temperature_steps.back()[FAN_SPEED_VALUE].get<unsigned>());
    }
    else
    {
        for (auto& step : temperature_steps)
        {
            if (current_temp >= step[TEMPERATURE_VALUE].get<unsigned>())
            {
                set_fan_speed(step[FAN_SPEED_VALUE].get<unsigned>());
                break;
            }
        }
    }
}



void GpuFanController::set_fan_speed(unsigned fan_speed)
{
    const QString set_fan_speed_command {QString{NVIDIA_SETTINGS_GPU_FAN_SPEED}.arg(fan_speed)};
    run_nvidia_settings(set_fan_speed_command);
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
