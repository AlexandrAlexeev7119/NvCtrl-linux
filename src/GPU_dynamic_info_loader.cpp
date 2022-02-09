#include "GPU_dynamic_info_loader.hpp"

GPUDynamicInfoLoader::GPUDynamicInfoLoader()
    : timer_{this}
    , nvml_device_{}
{
    connect(&timer_, &QTimer::timeout, this, &GPUDynamicInfoLoader::timer_tick);
    timer_.setInterval(1500);
    timer_.start();
}

void GPUDynamicInfoLoader::timer_tick()
{
}
