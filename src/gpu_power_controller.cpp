#include <QProcess>
#include <QDebug>

#include "gpu_power_controller.hpp"
#include "nvmlpp/util/nvmlpp_errors.hpp"

static constexpr const char* PKEXEC_BIN {"/usr/bin/pkexec"};
static constexpr const char* NVIDIA_SMI_BIN {"/usr/bin/nvidia-smi"};

GpuPowerController::GpuPowerController(QObject* parrent)
    : QObject {parrent}
    , current_gpu_ {nullptr}
{ }

void GpuPowerController::set_power_limit(unsigned limit)
{
    const auto ret_code {QProcess::execute(PKEXEC_BIN, {NVIDIA_SMI_BIN, "-pl", QString::number(limit)})};
    if (ret_code != 0)
    {
        qCritical().nospace().noquote() << "Failed to apply power limit";
        emit error();
    }
    qInfo().nospace().noquote() << "Power limit was set to: " << limit;
}

void GpuPowerController::update_info()
{
    try
    {
        power_rates power_rates_ {
            .usage = current_gpu_->get_current_power_usage(),
            .limit = current_gpu_->get_current_power_limit()
        };
        emit info_ready(power_rates_);
    }
    catch (const NVMLpp::errors::error_not_supported&)
    {
        emit error();
    }
}
