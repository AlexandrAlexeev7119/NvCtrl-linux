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
    CUDA_INFO,
    GPU_UUID,
    MEMORY_BUS_INFO,
    GPU_SLOWDOWN_TEMPERATURE,
    GPU_SHUTDOWN_TEMPERATURE,
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
