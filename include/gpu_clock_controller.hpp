#pragma once

#include <QObject>

#include "nvmlpp/nvmlpp_device.hpp"
#include "nlohmann/json.hpp"

class GpuClockController : public QObject
{
    Q_OBJECT
public:
    struct clock_values
    {
        unsigned graphics;
        unsigned video;
        unsigned sm;
        unsigned memory;
    };

    GpuClockController(QObject* parrent = nullptr);
    inline void set_device(const NVMLpp::NVML_device* nvml_device) noexcept { current_gpu_ = nvml_device; }
    inline void load_profile(const nlohmann::json* clock_offset_profile) noexcept { ptr_current_clock_profile_ = clock_offset_profile; }

    void apply_current_clock_profile();
    void reset_clocks();

public slots:
    void update_info();

signals:
    void info_ready(const GpuClockController::clock_values&);
    void error_occured();

private:
    const NVMLpp::NVML_device* current_gpu_;
    const nlohmann::json* ptr_current_clock_profile_;

    void run_nvidia_settings(QStringList&& args);
};
