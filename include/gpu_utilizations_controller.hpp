#pragma once

#include <QObject>
#include "nvmlpp/nvmlpp_device.hpp"

class GpuUtilizationsContoller : public QObject
{
    Q_OBJECT
public:
    GpuUtilizationsContoller(const NVMLpp::NVML_device* nvml_device = nullptr, QObject* parrent = nullptr);
    inline void set_device(const NVMLpp::NVML_device* nvml_device) noexcept { current_gpu_ = nvml_device; }

public slots:
    void update_info();

signals:
    void gpu_utilization(unsigned);
    void memory_utilization(unsigned, unsigned);
    void encoder_decoder_utilization(unsigned, unsigned);
    void error(const QString&);

private:
    const NVMLpp::NVML_device* current_gpu_;
};
