#pragma once

#include <QObject>
#include "nvmlpp/nvmlpp_device.hpp"

class GpuFanController : public QObject
{
    Q_OBJECT
public:
    struct fan_rates
    {
        unsigned speed;
    };

    GpuFanController(NVMLpp::NVML_device* nvml_device = nullptr, QObject* parent = nullptr);
    inline void set_device(NVMLpp::NVML_device* nvml_device) noexcept { current_gpu_ = nvml_device; }

public slots:
    void update_info();
    void set_fan_speed(unsigned fan_speed_level);
    void set_fan_control_state(bool value);

signals:
    void info_ready(const GpuFanController::fan_rates&);
    void error();

private:
    NVMLpp::NVML_device* current_gpu_;

    void run_nvidia_settings(const QString& arg);
};
