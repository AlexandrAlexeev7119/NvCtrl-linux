#pragma once

#include <QString>

#include "NVML.hpp"

class GPUStaticInfoLoader
{
public:
    GPUStaticInfoLoader();
    ~GPUStaticInfoLoader() = default;

private:
    NVMLDevice nvml_device_;
};
