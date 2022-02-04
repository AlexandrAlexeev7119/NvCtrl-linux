#pragma once

#include <unordered_map>
#include <QObject>
#include <QTimer>

#include "lib/nvml/nvml.h"

enum class GPUDynamicInfoLoaderType
{
    GPU_UTILIZATION_PERCENTAGE,
    MEMORY_UTILIZATION_PERCENTAGE,
    ENCODER_USAGE_PERCENTAGE,
    DECODER_USAGE_PERCENTAGE,
    POWER_USAGE_WATTS,
};

class GPUDynamicInfoLoader : public QObject
{
    Q_OBJECT

public:
    GPUDynamicInfoLoader();
    ~GPUDynamicInfoLoader();

signals:
    void update_info(const std::unordered_map<GPUDynamicInfoLoaderType, QString>&);

private slots:
    void timer_tick();

private:
    nvmlDevice_t nvml_device_;
    QTimer timer_;

    std::unordered_map<GPUDynamicInfoLoaderType, QString> get_dynamic_GPU_info();
};
