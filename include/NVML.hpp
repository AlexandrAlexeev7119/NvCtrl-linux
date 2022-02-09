#pragma once

#include <memory>
#include <vector>
#include <string>

#include "lib/nvml/nvml.h"

class NVML
{
public:
    NVML();
    ~NVML();

    std::vector<nvmlDevice_t> get_devices_list();
    void get_handle_by_index(unsigned index, nvmlDevice_t& device);

    ///
    /// Static info
    ///
    std::string get_system_driver_version();
    std::string get_system_nvml_version();

    std::string get_device_name(nvmlDevice_t& device);
    std::string get_device_uuid(nvmlDevice_t& device);
    std::string get_device_vbios_version(nvmlDevice_t& device);
    std::string get_device_pci_info(nvmlDevice_t& device);

    ///
    /// Dynamic info
    ///
    unsigned get_device_gpu_usage_percantage(nvmlDevice_t& device) const;
    unsigned get_device_memory_usage_percantage(nvmlDevice_t& device) const;
    unsigned get_device_encoder_usage_percentage(nvmlDevice_t& device) const;
    unsigned get_device_decoder_usage_percentage(nvmlDevice_t& device) const;
    unsigned get_device_memory_total_bytes(nvmlDevice_t& device) const;
    unsigned get_device_memory_usage_bytes(nvmlDevice_t& device) const;
    unsigned get_device_current_power_usage(nvmlDevice_t& device) const;
    unsigned get_device_current_power_limit(nvmlDevice_t& device) const;
};

class NVMLDevice
{
public:
    NVMLDevice();
    ~NVMLDevice() = default;

    void update_dynamic_info();

    std::string get_system_driver_version();
    std::string get_system_nvml_version();

    std::string get_name();
    std::string get_uuid();
    std::string get_vbios_version();

    unsigned get_gpu_usage_percentage() const;
    unsigned get_memory_usage_percentage() const;
    unsigned get_encoder_usage_percentage() const;
    unsigned get_decoder_usage_percentage() const;
    unsigned get_memory_usage_bytes() const;
    unsigned get_current_power_usage() const;
    unsigned get_current_power_limit() const;

private:
    nvmlDevice_t device_handle_;
    const std::unique_ptr<NVML> nvml_api_;
    struct DynamicInfo
    {
        unsigned gpu_usage_percentage;
        unsigned memory_usage_percentage;
        unsigned encoder_usage_percentage;
        unsigned decoder_usage_percentage;
        unsigned memory_usage_bytes;
        unsigned current_power_usage;
        unsigned current_power_limit;
    } dynamic_info_;
};
