#pragma once

#include <unordered_map>

#include <QObject>

#include "nvmlpp/nvmlpp_device.hpp"

class GpuUtilizationsController : public QObject
{
    Q_OBJECT
public:
    struct utilization_rates
    {
        unsigned gpu;
        unsigned mem;
        unsigned mem_used;
        unsigned encoder;
        unsigned decoder;
        unsigned pstate;
    };

    GpuUtilizationsController(const NVMLpp::NVML_device* nvml_device = nullptr, QObject* parrent = nullptr);
    inline void set_device(const NVMLpp::NVML_device* nvml_device) noexcept { current_gpu_ = nvml_device; }

public slots:
    void update_info();

signals:
    void info_ready(const GpuUtilizationsController::utilization_rates&);
    void error(const QString&);

private:
    const NVMLpp::NVML_device* current_gpu_;
};
