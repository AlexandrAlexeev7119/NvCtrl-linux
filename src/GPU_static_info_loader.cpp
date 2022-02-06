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

    unsigned pcie_link_width{};
    unsigned slowdown_temperature{};
    unsigned shutdown_temperature{};
    unsigned gpu_max_power_usage{};
    unsigned gpu_min_power_usage{};
    unsigned gpu_default_power_limit{};
    unsigned gpu_current_power_limit{};

    nvmlPciInfo_t pci_info{};
    nvmlMemory_t memory_info{};

    nvmlDeviceGetName(nvml_device_, gpu_name, NVML_DEVICE_NAME_BUFFER_SIZE);
    nvmlDeviceGetVbiosVersion(nvml_device_, vbios_version, NVML_DEVICE_VBIOS_VERSION_BUFFER_SIZE);
    nvmlSystemGetDriverVersion(driver_version, NVML_SYSTEM_DRIVER_VERSION_BUFFER_SIZE);
    nvmlDeviceGetUUID(nvml_device_, gpu_uuid, NVML_DEVICE_UUID_BUFFER_SIZE);
    nvmlDeviceGetPciInfo(nvml_device_, &pci_info);
    nvmlDeviceGetCurrPcieLinkWidth(nvml_device_, &pcie_link_width);
    nvmlDeviceGetMemoryInfo(nvml_device_, &memory_info);
    nvmlDeviceGetTemperatureThreshold(nvml_device_, NVML_TEMPERATURE_THRESHOLD_SLOWDOWN, &slowdown_temperature);
    nvmlDeviceGetTemperatureThreshold(nvml_device_, NVML_TEMPERATURE_THRESHOLD_SHUTDOWN, &shutdown_temperature);
    nvmlDeviceGetPowerManagementLimitConstraints(nvml_device_, &gpu_min_power_usage, &gpu_max_power_usage);
    nvmlDeviceGetPowerManagementDefaultLimit(nvml_device_, &gpu_default_power_limit);
    nvmlDeviceGetPowerManagementLimit(nvml_device_, &gpu_current_power_limit);

    // Convert to watts
    gpu_max_power_usage /= 1000;
    gpu_min_power_usage /= 1000;
    gpu_default_power_limit /= 1000;
    gpu_current_power_limit /= 1000;

    // Convert total memory to MiB
    const double gpu_total_memory_mib{static_cast<double>(memory_info.total / 1024 / 1024.0)};

    return {
        {GPUStaticInfoType::GPU_NAME,                   QString{gpu_name}},
        {GPUStaticInfoType::VBIOS_VER,                  QString{vbios_version}},
        {GPUStaticInfoType::DRIVER_VER,                 QString{driver_version}},
        {GPUStaticInfoType::BUS_INFO,                   QString{"PCIe%1x %2"}.arg(QString::number(pcie_link_width)).arg(pci_info.busId)},
        {GPUStaticInfoType::GPU_UUID,                   QString{gpu_uuid}},
        {GPUStaticInfoType::GPU_SLOWDOWN_TEMPERATURE,   QString::number(slowdown_temperature)},
        {GPUStaticInfoType::GPU_SHUTDOWN_TEMPERATURE,   QString::number(shutdown_temperature)},
        {GPUStaticInfoType::GPU_MAX_POWER_USAGE,        QString::number(gpu_max_power_usage)},
        {GPUStaticInfoType::GPU_MIN_POWER_USAGE,        QString::number(gpu_min_power_usage)},
        {GPUStaticInfoType::GPU_DEFAULT_POWER_LIMIT,    QString::number(gpu_default_power_limit)},
        {GPUStaticInfoType::GPU_CURRENT_POWER_LIMIT,    QString::number(gpu_current_power_limit)},
        {GPUStaticInfoType::GPU_TOTAL_MEMORY,           QString::number(gpu_total_memory_mib)},
    };
}

GPUStaticInfoLoader::~GPUStaticInfoLoader()
{
    nvmlShutdown();
}
