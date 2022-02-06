#pragma once

#include <unordered_map>
#include <QString>

#include "lib/nvml/nvml.h"

enum class GPUStaticInfoType
{
    GPU_NAME,
    VBIOS_VER,
    DRIVER_VER,
    BUS_INFO,
    GPU_UUID,
    GPU_TOTAL_MEMORY,
    GPU_SLOWDOWN_TEMPERATURE,
    GPU_SHUTDOWN_TEMPERATURE,
    GPU_MAX_POWER_USAGE,
    GPU_MIN_POWER_USAGE,
    GPU_DEFAULT_POWER_LIMIT,
    GPU_CURRENT_POWER_LIMIT,
};

class GPUStaticInfoLoader
{
public:
    GPUStaticInfoLoader();
    std::unordered_map<GPUStaticInfoType, QString> get_static_GPU_info();
    ~GPUStaticInfoLoader();

private:
    nvmlDevice_t nvml_device_;
};
