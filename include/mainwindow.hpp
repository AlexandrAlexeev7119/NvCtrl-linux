#pragma once

#include <QMainWindow>

#include "NVML.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_apply_power_settings_clicked();
    void on_action_Exit_triggered();

private:
    Ui::MainWindow* ui;
    NVMLDevice nvml_device_;
};
