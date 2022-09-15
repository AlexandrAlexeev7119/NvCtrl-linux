#include "nvmlpp/util/nvmlpp_errors.hpp"

#include "gpu_utilizations_controller.hpp"

GpuUtilizationsController::GpuUtilizationsController(const NVMLpp::NVML_device* nvml_device, QObject* parrent)
    : QObject {parrent}
    , current_gpu_ {nvml_device}
{ }

void GpuUtilizationsController::update_info()
{
    utilization_rates utilisation_rates_ {};

    try
    {
        utilisation_rates_.gpu = current_gpu_->get_gpu_utilization();
        utilisation_rates_.mem = current_gpu_->get_memory_utilization();
        utilisation_rates_.mem_used = current_gpu_->get_used_memory();
        utilisation_rates_.encoder = current_gpu_->get_decoder_utilization();
        utilisation_rates_.decoder = current_gpu_->get_encoder_utilization();
        utilisation_rates_.pstate = static_cast<unsigned>(current_gpu_->get_performance_state());

        emit info_ready(utilisation_rates_);
    }
    catch (const NVMLpp::errors::error_not_supported&)
    { }
}
