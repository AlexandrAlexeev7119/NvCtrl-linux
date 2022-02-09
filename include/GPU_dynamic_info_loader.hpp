#pragma once

#include <QObject>
#include <QTimer>

#include "NVML.hpp"

class GPUDynamicInfoLoader : public QObject
{
    Q_OBJECT

public:
    GPUDynamicInfoLoader();
    ~GPUDynamicInfoLoader() = default;

private slots:
    void timer_tick();

private:
    QTimer timer_;
    NVMLDevice nvml_device_;
};
