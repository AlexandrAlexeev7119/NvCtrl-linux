#include "GPU_static_info_loader.hpp"

GPUStaticInfoLoader::GPUStaticInfoLoader()
    : nvml_device_{}
{
    nvmlInit();
    nvmlDeviceGetHandleByIndex(0, &nvml_device_);
}

std::unordered_map<GPUStaticInfoType, QString> GPUStaticInfoLoader::get_static_GPU_info()
{
    char gpu_name[NVML_DEVICE_NAME_BUFFER_SIZE]{0};
    char vbios_version[NVML_DEVICE_VBIOS_VERSION_BUFFER_SIZE]{0};
    char driver_version[NVML_SYSTEM_DRIVER_VERSION_BUFFER_SIZE]{0};
    char gpu_uuid[NVML_DEVICE_UUID_BUFFER_SIZE]{0};
    char gpu_serial[NVML_DEVICE_SERIAL_BUFFER_SIZE]{0};
    nvmlPciInfo_t pci_info{};
    nvmlMemory_t memory_info{};

    nvmlDeviceGetName(nvml_device_, gpu_name, NVML_DEVICE_NAME_BUFFER_SIZE);
    nvmlDeviceGetVbiosVersion(nvml_device_, vbios_version, NVML_DEVICE_VBIOS_VERSION_BUFFER_SIZE);
    nvmlSystemGetDriverVersion(driver_version, NVML_SYSTEM_DRIVER_VERSION_BUFFER_SIZE);
    nvmlDeviceGetPciInfo(nvml_device_, &pci_info);
    nvmlDeviceGetUUID(nvml_device_, gpu_uuid, NVML_DEVICE_UUID_BUFFER_SIZE);

    const QString bus_info{
        "PCIe 16x - " + QString{pci_info.busId}
    };

    return {
        {GPUStaticInfoType::GPU_NAME, gpu_name},
        {GPUStaticInfoType::VBIOS_VER, vbios_version},
        {GPUStaticInfoType::DRIVER_VER, driver_version},
        {GPUStaticInfoType::BUS_INFO, bus_info},
        {GPUStaticInfoType::GPU_UUID, gpu_uuid},
    };
}

GPUStaticInfoLoader::~GPUStaticInfoLoader()
{
    nvmlShutdown();
}
