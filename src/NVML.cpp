#include <stdexcept>
#include <QDebug>

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

void NVML::get_handle_by_index(unsigned index, nvmlDevice_t& device) noexcept
{
    nvmlDeviceGetHandleByIndex(index, &device);
}

std::string NVML::get_system_driver_version() const
{
    char driver_ver[NVML_SYSTEM_DRIVER_VERSION_BUFFER_SIZE]{0};
    nvmlSystemGetDriverVersion(driver_ver, sizeof(driver_ver));
    return driver_ver;
}

std::string NVML::get_system_nvml_version() const
{
    char nvml_version[NVML_SYSTEM_NVML_VERSION_BUFFER_SIZE]{0};
    nvmlSystemGetNVMLVersion(nvml_version, sizeof(nvml_version));
    return nvml_version;
}

std::string NVML::get_device_name(const nvmlDevice_t& device) const
{
    char name[NVML_DEVICE_NAME_BUFFER_SIZE]{0};
    nvmlDeviceGetName(device, name, sizeof(name));
    return name;
}

std::string NVML::get_device_uuid(const nvmlDevice_t& device) const
{
    char uuid[NVML_DEVICE_UUID_BUFFER_SIZE]{0};
    nvmlDeviceGetUUID(device, uuid, sizeof(uuid));
    return uuid;
}

std::string NVML::get_device_vbios_version(const nvmlDevice_t& device) const
{
    char vbios_ver[NVML_DEVICE_VBIOS_VERSION_BUFFER_SIZE]{0};
    nvmlDeviceGetVbiosVersion(device, vbios_ver, sizeof(vbios_ver));
    return vbios_ver;
}

std::string NVML::get_device_pci_info(const nvmlDevice_t& device) const
{
    nvmlPciInfo_t pci_info{};
    nvmlDeviceGetPciInfo(device, &pci_info);
    return pci_info.busId;
}

unsigned long long NVML::get_device_memory_total_bytes(const nvmlDevice_t& device) const noexcept
{
    nvmlMemory_t memory_info{};
    nvmlDeviceGetMemoryInfo(device, &memory_info);
    return memory_info.total;
}

unsigned NVML::get_device_max_power_usage(const nvmlDevice_t& device) const noexcept
{
    unsigned max_power_usage{0};
    unsigned dummy{0};
    nvmlDeviceGetPowerManagementLimitConstraints(device, &dummy, &max_power_usage);
    return max_power_usage;
}

unsigned NVML::get_device_min_power_usage(const nvmlDevice_t& device) const noexcept
{
    unsigned min_power_usage{0};
    unsigned dummy{0};
    nvmlDeviceGetPowerManagementLimitConstraints(device, &min_power_usage, &dummy);
    return min_power_usage;
}

unsigned NVML::get_device_default_power_usage(const nvmlDevice_t& device) const noexcept
{
    unsigned default_power_usage{0};
    nvmlDeviceGetPowerManagementDefaultLimit(device, &default_power_usage);
    return default_power_usage;
}

unsigned NVML::get_device_shutdown_temperature(const nvmlDevice_t& device) const noexcept
{
    unsigned gpu_shutdown_temperature{0};
    nvmlDeviceGetTemperatureThreshold(device, NVML_TEMPERATURE_THRESHOLD_SHUTDOWN, &gpu_shutdown_temperature);
    return gpu_shutdown_temperature;
}

unsigned NVML::get_device_slowdown_temperature(const nvmlDevice_t& device) const noexcept
{
    unsigned gpu_slowdown_temperature{0};
    nvmlDeviceGetTemperatureThreshold(device, NVML_TEMPERATURE_THRESHOLD_SLOWDOWN, &gpu_slowdown_temperature);
    return gpu_slowdown_temperature;
}

unsigned NVML::get_device_gpu_usage_percantage(const nvmlDevice_t& device) const noexcept
{
    nvmlUtilization_t utilization_rates{};
    nvmlDeviceGetUtilizationRates(device, &utilization_rates);
    return utilization_rates.gpu;
}

unsigned NVML::get_device_memory_usage_percantage(const nvmlDevice_t& device) const noexcept
{
    nvmlUtilization_t utilization_rates{};
    nvmlDeviceGetUtilizationRates(device, &utilization_rates);
    return utilization_rates.memory;
}

unsigned NVML::get_device_encoder_usage_percentage(const nvmlDevice_t& device) const noexcept
{
    unsigned encoder_usage_percentage{0};
    nvmlDeviceGetEncoderUtilization(device, &encoder_usage_percentage, nullptr);
    return encoder_usage_percentage;
}

unsigned NVML::get_device_decoder_usage_percentage(const nvmlDevice_t& device) const noexcept
{
    unsigned decoder_usage_percentage{0};
    nvmlDeviceGetDecoderUtilization(device, &decoder_usage_percentage, nullptr);
    return decoder_usage_percentage;
}

unsigned long long NVML::get_device_memory_usage_bytes(const nvmlDevice_t& device) const noexcept
{
    nvmlMemory_t memory_info{};
    nvmlDeviceGetMemoryInfo(device, &memory_info);
    return memory_info.used;
}

unsigned NVML::get_device_current_power_usage(const nvmlDevice_t& device) const noexcept
{
    unsigned current_power_usage{0};
    nvmlDeviceGetPowerUsage(device, &current_power_usage);
    return current_power_usage;
}

unsigned NVML::get_device_current_power_limit(const nvmlDevice_t& device) const noexcept
{
    unsigned current_power_limit{0};
    nvmlDeviceGetPowerManagementLimit(device, &current_power_limit);
    return current_power_limit;
}

unsigned NVML::get_device_current_gpu_temperature(const nvmlDevice_t& device) const noexcept
{
    unsigned current_gpu_temperature{0};
    nvmlDeviceGetTemperature(device, NVML_TEMPERATURE_GPU, &current_gpu_temperature);
    return current_gpu_temperature;
}

unsigned NVML::get_device_current_fan_speed_percentage(const nvmlDevice_t& device) const noexcept
{
    unsigned current_fan_speed{0};
    nvmlDeviceGetFanSpeed(device, &current_fan_speed);
    return current_fan_speed;
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
    dynamic_info_.gpu_usage_percentage          = nvml_api_->get_device_gpu_usage_percantage(device_handle_);
    dynamic_info_.memory_usage_percentage       = nvml_api_->get_device_memory_usage_percantage(device_handle_);
    dynamic_info_.encoder_usage_percentage      = nvml_api_->get_device_encoder_usage_percentage(device_handle_);
    dynamic_info_.decoder_usage_percentage      = nvml_api_->get_device_decoder_usage_percentage(   device_handle_);
    dynamic_info_.memory_usage_bytes            = nvml_api_->get_device_memory_usage_bytes(device_handle_);
    dynamic_info_.current_power_usage           = nvml_api_->get_device_current_power_usage(device_handle_);
    dynamic_info_.current_power_limit           = nvml_api_->get_device_current_power_limit(device_handle_);
    dynamic_info_.current_gpu_temperature       = nvml_api_->get_device_current_gpu_temperature(device_handle_);
    dynamic_info_.current_fan_speed_percentage  = nvml_api_->get_device_current_fan_speed_percentage(device_handle_);
}

std::string NVMLDevice::get_system_driver_version() const
{
    return nvml_api_->get_system_driver_version();
}

std::string NVMLDevice::get_name() const
{
    return nvml_api_->get_device_name(device_handle_);
}

std::string NVMLDevice::get_uuid() const
{
    return nvml_api_->get_device_uuid(device_handle_);
}

std::string NVMLDevice::get_vbios_version() const
{
    return nvml_api_->get_device_vbios_version(device_handle_);
}

unsigned long long NVMLDevice::get_memory_total_bytes() const noexcept
{
    return nvml_api_->get_device_memory_total_bytes(device_handle_);
}

unsigned NVMLDevice::get_max_power_usage() const noexcept
{
    return nvml_api_->get_device_max_power_usage(device_handle_);
}

unsigned NVMLDevice::get_min_power_usage() const noexcept
{
    return nvml_api_->get_device_min_power_usage(device_handle_);
}

unsigned NVMLDevice::get_default_power_usage() const noexcept
{
    return nvml_api_->get_device_default_power_usage(device_handle_);
}

unsigned NVMLDevice::get_shutdown_temperature() const noexcept
{
    return nvml_api_->get_device_shutdown_temperature(device_handle_);
}

unsigned NVMLDevice::get_slowdown_temperature() const noexcept
{
    return nvml_api_->get_device_slowdown_temperature(device_handle_);
}

const NVMLDevice::DynamicInfo& NVMLDevice::get_dynamic_info() const noexcept
{
    return dynamic_info_;
}
