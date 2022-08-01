#pragma once

#include <QObject>

#include "nvmlpp/nvmlpp_device.hpp"

class GpuClockController : public QObject
{
    Q_OBJECT

public:
    GpuClockController(const NVMLpp::NVML_device* nvml_device = nullptr, QObject* parrent = nullptr);
    inline void set_device(const NVMLpp::NVML_device* nvml_device) noexcept { current_gpu_ = nvml_device; }

public slots:
    void update_info();

signals:
    void graphics_clock(unsigned);
    void video_clock(unsigned);
    void sm_clock(unsigned);
    void memory_clock(unsigned);

private:
    const NVMLpp::NVML_device* current_gpu_;
};
