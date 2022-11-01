#include <QProcess>
#include <QDebug>

#include "nvmlpp/util/nvmlpp_errors.hpp"
#include "gpu_clock_controller.hpp"

constexpr const char* NVIDIA_SETTINGS_BIN {"/usr/bin/nvidia-settings"};
constexpr const char* NVIDIA_SETTINGS_GPU_CLOCK_OFFSET {"[gpu:%1]/GPUGraphicsClockOffset[3]=%2"};
constexpr const char* NVIDIA_SETTINGS_MEM_CLOCK_OFFSET {"[gpu:%1]/GPUMemoryTransferRateOffset[3]=%2"};



GpuClockController::GpuClockController(QObject* parrent)
    : QObject {parrent}
    , current_gpu_ {nullptr}
{ }



void GpuClockController::apply_current_clock_profile()
{
    const auto& current_clock_profile {*ptr_current_clock_profile_};
    const auto& clock_offset_values = current_clock_profile["offset_values"];
    for (const auto& value : clock_offset_values)
    {
        const auto& [pstate, gpu_clock_offset] {value.get<std::pair<int, int>>()};
        qDebug().noquote().nospace() << "pstate: " << pstate << " gpu_clock_offset: " << gpu_clock_offset;
    }
}



void GpuClockController::reset_values()
{
    set_clock_offsets(0, 0);
}



void GpuClockController::update_info()
{
    try
    {
        clock_values clock_values_ {
            .graphics = current_gpu_->get_clock_graphics(),
            .video = current_gpu_->get_clock_video(),
            .sm = current_gpu_->get_clock_sm(),
            .memory = current_gpu_->get_clock_memory()
        };
        emit info_ready(clock_values_);
    }
    catch (const NVMLpp::errors::error_not_supported&)
    {
        emit error_occured();
    }
}



void GpuClockController::set_clock_offsets(unsigned gpu_clock_offset, unsigned memory_clock_offset)
{
    run_nvidia_settings(QString{NVIDIA_SETTINGS_GPU_CLOCK_OFFSET}.arg(current_gpu_->get_index()).arg(gpu_clock_offset));
    run_nvidia_settings(QString{NVIDIA_SETTINGS_MEM_CLOCK_OFFSET}.arg(current_gpu_->get_index()).arg(memory_clock_offset));
}



void GpuClockController::run_nvidia_settings(const QString& arg)
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
