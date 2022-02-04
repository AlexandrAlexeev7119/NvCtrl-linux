#include "GPU_dynamic_info_loader.hpp"

GPUDynamicInfoLoader::GPUDynamicInfoLoader()
    : timer_{this}
    , nvml_device_{}
{
    nvmlInit();
    nvmlDeviceGetHandleByIndex(0, &nvml_device_);

    connect(&timer_, &QTimer::timeout, this, &GPUDynamicInfoLoader::timer_tick);
    timer_.setInterval(1500);
    timer_.start();
}

GPUDynamicInfoLoader::~GPUDynamicInfoLoader()
{
    nvmlShutdown();
}

void GPUDynamicInfoLoader::timer_tick()
{
    const auto gpu_dynamic_info{get_dynamic_GPU_info()};
    emit update_info(gpu_dynamic_info);
}

std::unordered_map<GPUDynamicInfoLoaderType, QString> GPUDynamicInfoLoader::get_dynamic_GPU_info()
{
    nvmlUtilization_t utilization_info{};
    unsigned encoder_utilization_percentage{};
    unsigned decoder_utilization_percentage{};
    unsigned sampling_period{};
    unsigned power_usage_milliwatts{};
    unsigned gpu_temperature{};

    nvmlDeviceGetUtilizationRates(nvml_device_, &utilization_info);
    nvmlDeviceGetEncoderUtilization(nvml_device_, &encoder_utilization_percentage, &sampling_period);
    nvmlDeviceGetDecoderUtilization(nvml_device_, &decoder_utilization_percentage, &sampling_period);
    nvmlDeviceGetPowerUsage(nvml_device_, &power_usage_milliwatts);
    nvmlDeviceGetTemperature(nvml_device_, NVML_TEMPERATURE_GPU, &gpu_temperature);

    const double power_usage_watts{power_usage_milliwatts / 1000.0};

    return {
        {GPUDynamicInfoLoaderType::GPU_UTILIZATION_PERCENTAGE,      QString::number(utilization_info.gpu)},
        {GPUDynamicInfoLoaderType::MEMORY_UTILIZATION_PERCENTAGE,   QString::number(utilization_info.memory)},
        {GPUDynamicInfoLoaderType::ENCODER_USAGE_PERCENTAGE,        QString::number(encoder_utilization_percentage)},
        {GPUDynamicInfoLoaderType::DECODER_USAGE_PERCENTAGE,        QString::number(decoder_utilization_percentage)},
        {GPUDynamicInfoLoaderType::POWER_USAGE_WATTS,               QString::number(power_usage_watts)},
        {GPUDynamicInfoLoaderType::GPU_CURRENT_TEMPERATURE,         QString::number(gpu_temperature)},
    };
}
