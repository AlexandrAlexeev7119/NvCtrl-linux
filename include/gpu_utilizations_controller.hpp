#pragma once

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
        unsigned gpu_core_temp;
    };

    GpuUtilizationsController(QObject* parrent = nullptr);
    inline void set_device(const NVMLpp::NVML_device* nvml_device) noexcept { current_gpu_ = nvml_device; }

public slots:
    void update_info();

signals:
    void info_ready(const GpuUtilizationsController::utilization_rates&);
    void encoder_decoder_unsupported();

private:
    const NVMLpp::NVML_device* current_gpu_;
};
