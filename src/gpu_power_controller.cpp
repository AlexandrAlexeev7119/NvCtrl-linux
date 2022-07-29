#include <QProcess>

#include "gpu_power_controller.hpp"

static constexpr const char* PKEXEC_BIN {"/usr/bin/pkexec"};

GpuPowerController::GpuPowerController(const NVMLpp::NVML_device* nvml_device, QObject* parrent)
    : QObject {parrent}
    , current_gpu_ {nvml_device}
{ }

void GpuPowerController::set_power_limit(unsigned limit)
{
    const auto ret_code {QProcess::execute(PKEXEC_BIN, {"-pl", QString::number(limit)})};
    if (ret_code != 0)
    {
        emit error("Error: failed to apply power limit");
    }
}

void GpuPowerController::update_info()
{
    if (current_gpu_)
    {
        emit power_usage(current_gpu_->get_current_power_usage());
        emit power_limit(current_gpu_->get_current_power_limit());
    }
}
