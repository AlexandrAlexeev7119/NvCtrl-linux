#include <stdexcept>

#include "NVML.hpp"

NVML::NVML()
{
    nvmlReturn_t err_code{nvmlInit()};
    if(err_code != NVML_SUCCESS)
    {
        throw std::runtime_error{std::string{nvmlErrorString(err_code)}};
    }
}

NVML::~NVML()
{
    nvmlShutdown();
}

std::vector<nvmlDevice_t> NVML::get_devices_list()
{
    unsigned devices_count{0};
    nvmlReturn_t err_code{nvmlDeviceGetCount(&devices_count)};

    if (err_code != NVML_SUCCESS)
    {
        throw std::runtime_error{std::string{nvmlErrorString(err_code)}};
    }

    std::vector<nvmlDevice_t> devices_list{};
    devices_list.reserve(devices_count);

    for (unsigned i{0}; i < devices_count; i++)
    {
        NVML::get_handle_by_index(i, devices_list[i]);
    }
    return devices_list;
}

void NVML::get_handle_by_index(unsigned index, nvmlDevice_t& device)
{
    nvmlDeviceGetHandleByIndex(index, &device);
}

std::string NVML::get_system_driver_version()
{
    char driver_ver[NVML_SYSTEM_DRIVER_VERSION_BUFFER_SIZE]{0};
    nvmlSystemGetDriverVersion(driver_ver, sizeof(driver_ver));
    return driver_ver;
}

std::string NVML::get_system_nvml_version()
{
    char nvml_version[NVML_SYSTEM_NVML_VERSION_BUFFER_SIZE]{0};
    nvmlSystemGetNVMLVersion(nvml_version, sizeof(nvml_version));
    return nvml_version;
}

std::string NVML::get_device_name(nvmlDevice_t& device)
{
    char name[NVML_DEVICE_NAME_BUFFER_SIZE]{0};
    nvmlDeviceGetName(device, name, sizeof(name));
    return name;
}

std::string NVML::get_device_uuid(nvmlDevice_t& device)
{
    char uuid[NVML_DEVICE_UUID_BUFFER_SIZE]{0};
    nvmlDeviceGetUUID(device, uuid, sizeof(uuid));
    return uuid;
}

std::string NVML::get_device_vbios_version(nvmlDevice_t& device)
{
    char vbios_ver[NVML_DEVICE_VBIOS_VERSION_BUFFER_SIZE]{0};
    nvmlDeviceGetVbiosVersion(device, vbios_ver, sizeof(vbios_ver));
    return vbios_ver;
}

std::string NVML::get_device_pci_info(nvmlDevice_t& device)
{
    nvmlPciInfo_t pci_info{};
    nvmlDeviceGetPciInfo(device, &pci_info);
    return pci_info.busId;
}

unsigned NVML::get_device_gpu_usage_percantage(nvmlDevice_t& device) const
{
    nvmlUtilization_t utilization_rates{};
    nvmlDeviceGetUtilizationRates(device, &utilization_rates);
    return utilization_rates.gpu;
}

unsigned NVML::get_device_memory_usage_percantage(nvmlDevice_t& device) const
{
    nvmlUtilization_t utilization_rates{};
    nvmlDeviceGetUtilizationRates(device, &utilization_rates);
    return utilization_rates.memory;
}

unsigned NVML::get_device_encoder_usage_percentage(nvmlDevice_t& device) const
{
    unsigned encoder_usage_percentage{0};
    nvmlDeviceGetEncoderUtilization(device, &encoder_usage_percentage, nullptr);
    return encoder_usage_percentage;
}

unsigned NVML::get_device_decoder_usage_percentage(nvmlDevice_t& device) const
{
    unsigned decoder_usage_percentage{0};
    nvmlDeviceGetDecoderUtilization(device, &decoder_usage_percentage, nullptr);
    return decoder_usage_percentage;
}

unsigned NVML::get_device_memory_total_bytes(nvmlDevice_t& device) const
{
    nvmlMemory_t memory_info{};
    nvmlDeviceGetMemoryInfo(device, &memory_info);
    return memory_info.total;
}

unsigned NVML::get_device_memory_usage_bytes(nvmlDevice_t& device) const
{
    nvmlMemory_t memory_info{};
    nvmlDeviceGetMemoryInfo(device, &memory_info);
    return memory_info.used;
}

unsigned NVML::get_device_current_power_usage(nvmlDevice_t& device) const
{
    unsigned current_power_usage{0};
    nvmlDeviceGetPowerUsage(device, &current_power_usage);
    return current_power_usage;
}

unsigned NVML::get_device_current_power_limit(nvmlDevice_t& device) const
{
    unsigned current_power_limit{0};
    nvmlDeviceGetPowerManagementLimit(device, &current_power_limit);
    return current_power_limit;
}




NVMLDevice::NVMLDevice()
    : nvml_api_{new NVML}
    , device_handle_{}
    , dynamic_info_{}
{
    nvml_api_->get_handle_by_index(0, device_handle_);
    update_dynamic_info();
}

void NVMLDevice::update_dynamic_info()
{
    dynamic_info_.gpu_usage_percentage      = nvml_api_->get_device_gpu_usage_percantage(device_handle_);
    dynamic_info_.memory_usage_percentage   = nvml_api_->get_device_memory_usage_percantage(device_handle_);
    dynamic_info_.encoder_usage_percentage  = nvml_api_->get_device_encoder_usage_percentage(device_handle_);
    dynamic_info_.decoder_usage_percentage  = nvml_api_->get_device_decoder_usage_percentage(device_handle_);
    dynamic_info_.memory_usage_bytes        = nvml_api_->get_device_memory_usage_bytes(device_handle_);
    dynamic_info_.current_power_usage       = nvml_api_->get_device_current_power_usage(device_handle_);
    dynamic_info_.current_power_limit       = nvml_api_->get_device_current_power_limit(device_handle_);
}

std::string NVMLDevice::get_system_driver_version()
{
    return nvml_api_->get_system_driver_version();
}

std::string NVMLDevice::get_name()
{
    return nvml_api_->get_device_name(device_handle_);
}

std::string NVMLDevice::get_uuid()
{
    return nvml_api_->get_device_uuid(device_handle_);
}

std::string NVMLDevice::get_vbios_version()
{
    return nvml_api_->get_device_vbios_version(device_handle_);
}

unsigned NVMLDevice::get_gpu_usage_percentage() const
{
    return dynamic_info_.gpu_usage_percentage;
}

unsigned NVMLDevice::get_memory_usage_percentage() const
{
    return dynamic_info_.memory_usage_percentage;
}

unsigned NVMLDevice::get_encoder_usage_percentage() const
{
    return dynamic_info_.encoder_usage_percentage;
}

unsigned NVMLDevice::get_decoder_usage_percentage() const
{
    return dynamic_info_.encoder_usage_percentage;
}

unsigned NVMLDevice::get_memory_usage_bytes() const
{
    return dynamic_info_.memory_usage_bytes;
}

unsigned NVMLDevice::get_current_power_usage() const
{
    return dynamic_info_.current_power_usage;
}

unsigned NVMLDevice::get_current_power_limit() const
{
    return dynamic_info_.current_power_limit;
}
