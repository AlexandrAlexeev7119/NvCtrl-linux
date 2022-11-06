#pragma once

#include <QObject>

#include "nvmlpp/nvmlpp_device.hpp"

class GpuProcessesController : public QObject
{
    Q_OBJECT

public:
    GpuProcessesController(QObject* parent = nullptr);
    void set_current_gpu(const NVMLpp::NVML_device* current_gpu) noexcept;

public slots:
    void update_info();

signals:
    void info_ready();

private:
    const NVMLpp::NVML_device* current_gpu_;
};
