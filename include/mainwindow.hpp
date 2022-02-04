#pragma once

#include <QMainWindow>

#include "GPU_static_info_loader.hpp"
#include "GPU_dynamic_info_loader.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void update_dynamic_info(const std::unordered_map<GPUDynamicInfoLoaderType, QString>& dynamic_info);

private:
    Ui::MainWindow* ui;
    GPUStaticInfoLoader gpu_static_info_loader_;
    GPUDynamicInfoLoader gpu_dynamic_info_loader_;
};
