#pragma once

#include <QObject>

#include "nvmlpp/nvmlpp_device.hpp"

class GpuClockController : public QObject
{
    Q_OBJECT
public:
    struct clock_values
    {
        unsigned graphics;
        unsigned video;
        unsigned sm;
        unsigned mem;
    };

    GpuClockController(const NVMLpp::NVML_device* nvml_device = nullptr, QObject* parrent = nullptr);
    inline void set_device(const NVMLpp::NVML_device* nvml_device) noexcept { current_gpu_ = nvml_device; }

public slots:
    void update_info();

signals:
    void info_ready(const GpuClockController::clock_values&);
    void error();

private:
    const NVMLpp::NVML_device* current_gpu_;
};
