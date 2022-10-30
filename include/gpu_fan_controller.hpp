#pragma once

#include <QObject>
#include <QTimer>

#include "nvmlpp/nvmlpp_device.hpp"
#include "nlohmann/json.hpp"

class GpuFanController final : public QObject
{
    Q_OBJECT
    enum { TEMPERATURE_VALUE, FAN_SPEED_VALUE };

public:
    struct fan_rates
    {
        unsigned fan_speed_level;
    };

    GpuFanController(QObject* parent = nullptr);
    inline void set_device(NVMLpp::NVML_device* nvml_device) noexcept { current_gpu_ = nvml_device; }

public slots:
    void update_info();

    void load_fan_speed_profile(const nlohmann::json* fan_profile) noexcept;
    void set_fan_control_state_enabled(bool enabled);

signals:
    void info_ready(const GpuFanController::fan_rates&);
    void error_occured();

private:
    NVMLpp::NVML_device* current_gpu_;
    const nlohmann::json* ptr_current_fan_profile_;
    QTimer timer_;

    void apply_fan_speed_from_profile();
    void set_fan_speed(unsigned fan_speed);
    void run_nvidia_settings(const QString& arg);
};
