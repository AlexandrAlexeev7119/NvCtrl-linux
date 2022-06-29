#pragma once

#include <memory>
#include <vector>
#include <string>

#include "lib/nvml/nvml.h"

class NVML
{
private:
    NVML();
    ~NVML();

public:
    static NVML& get_NVML_instance();

    std::vector<nvmlDevice_t> get_devices_list();
    void get_handle_by_index(unsigned index, nvmlDevice_t& device) noexcept;

    std::string get_system_driver_version() const;
    std::string get_system_nvml_version() const;
    std::string get_device_name(const nvmlDevice_t& device) const;
    std::string get_device_uuid(const nvmlDevice_t& device) const;
    std::string get_device_vbios_version(const nvmlDevice_t& device) const;
    std::string get_device_pci_info(const nvmlDevice_t& device) const;

    unsigned long long get_device_memory_total_bytes(const nvmlDevice_t& device) const noexcept;
    unsigned get_device_max_power_usage(const nvmlDevice_t& device) const noexcept;
    unsigned get_device_min_power_usage(const nvmlDevice_t& device) const noexcept;
    unsigned get_device_default_power_usage(const nvmlDevice_t& device) const noexcept;
    unsigned get_device_shutdown_temperature(const nvmlDevice_t& device) const noexcept;
    unsigned get_device_slowdown_temperature(const nvmlDevice_t& device) const noexcept;

    unsigned get_device_gpu_usage_percantage(const nvmlDevice_t& device) const noexcept;
    unsigned get_device_memory_usage_percantage(const nvmlDevice_t& device) const noexcept;
    unsigned get_device_encoder_usage_percentage(const nvmlDevice_t& device) const noexcept;
    unsigned get_device_decoder_usage_percentage(const nvmlDevice_t& device) const noexcept;
    unsigned long long get_device_memory_usage_bytes(const nvmlDevice_t& device) const noexcept;
    unsigned get_device_current_power_usage(const nvmlDevice_t& device) const noexcept;
    unsigned get_device_current_power_limit(const nvmlDevice_t& device) const noexcept;
    unsigned get_device_current_gpu_temperature(const nvmlDevice_t& device) const noexcept;
    unsigned get_device_current_fan_speed_percentage(const nvmlDevice_t& device) const noexcept;

    nvmlUnitInfo_t get_unit_info(const nvmlUnit_t& unit) const noexcept;
};

class NVMLDevice
{
    struct DynamicInfo;

public:
    NVMLDevice();
    ~NVMLDevice() = default;

    std::string get_system_driver_version() const;
    std::string get_system_nvml_version() const;
    std::string get_name() const;
    std::string get_uuid() const;
    std::string get_vbios_version() const;

    unsigned long long get_memory_total_bytes() const noexcept;
    unsigned get_max_power_usage() const noexcept;
    unsigned get_min_power_usage() const noexcept;
    unsigned get_default_power_usage() const noexcept;
    unsigned get_shutdown_temperature() const noexcept;
    unsigned get_slowdown_temperature() const noexcept;

    void update_dynamic_info();
    const DynamicInfo& get_dynamic_info() const noexcept;

private:
    nvmlDevice_t device_handle_;
    NVML& nvml_api_;
    struct DynamicInfo
    {
        unsigned gpu_usage_percentage;
        unsigned memory_usage_percentage;
        unsigned encoder_usage_percentage;
        unsigned decoder_usage_percentage;
        unsigned long long memory_usage_bytes;
        unsigned current_power_usage;
        unsigned current_power_limit;
        unsigned current_gpu_temperature;
        unsigned current_fan_speed_percentage;
    } dynamic_info_;
};

