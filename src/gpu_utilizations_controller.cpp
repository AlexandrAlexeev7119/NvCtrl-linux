#include "nvmlpp/util/nvmlpp_errors.hpp"

#include "gpu_utilizations_controller.hpp"

GpuUtilizationsController::GpuUtilizationsController(const NVMLpp::NVML_device* nvml_device, QObject* parrent)
    : QObject {parrent}
    , current_gpu_ {nvml_device}
{ }

void GpuUtilizationsController::update_info()
{
    try
    {
        utilization_rates utilisation_rates_ {
            .gpu = current_gpu_->get_gpu_utilization(),
            .mem = current_gpu_->get_memory_utilization(),
            .mem_used = static_cast<unsigned>(current_gpu_->get_used_memory()),
            .encoder = current_gpu_->get_encoder_utilization(),
            .decoder = current_gpu_->get_decoder_utilization(),
            .pstate = static_cast<unsigned>(current_gpu_->get_performance_state())
        };
        emit info_ready(utilisation_rates_);
    }
    catch (const NVMLpp::errors::error_not_supported&)
    { }
}
