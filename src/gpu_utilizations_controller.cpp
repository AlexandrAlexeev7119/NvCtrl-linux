#include "nvmlpp/util/nvmlpp_errors.hpp"

#include "gpu_utilizations_controller.hpp"

GpuUtilizationsController::GpuUtilizationsController(QObject* parrent)
    : QObject {parrent}
    , current_gpu_ {nullptr}
{ }

void GpuUtilizationsController::update_info()
{
    utilization_rates utilisation_rates_ {
        .gpu = current_gpu_->get_gpu_utilization(),
        .mem = current_gpu_->get_memory_utilization(),
        .mem_used = static_cast<unsigned>(current_gpu_->get_used_memory()),
        .encoder = 0,
        .decoder = 0,
        .pstate = static_cast<unsigned>(current_gpu_->get_performance_state()),
        .gpu_core_temp = current_gpu_->get_current_temperature()
    };

    try
    {
        utilisation_rates_.encoder = current_gpu_->get_encoder_utilization();
        utilisation_rates_.decoder = current_gpu_->get_decoder_utilization();
    }
    catch (const NVMLpp::errors::error_not_supported&)
    {
        emit encoder_decoder_unsupported();
    }

    emit info_ready(utilisation_rates_);
}
