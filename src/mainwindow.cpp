#include <QMessageBox>
#include <QProcess>

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent}
    , ui{new Ui::MainWindow}
    , dynamic_info_update_timer_{}
    , tray_icon_{this}
    , tray_menu_{}
    , nvml_device_{}
    , settings_manager_{SettingsManager::get_instance()}
    , settings_window_{this}
    , minimize_to_tray_on_close_{false}
    , new_file_profile_dialog_{this}
{
    ui->setupUi(this);
    setMinimumSize(size());
    setMaximumSize(size() * 1.5);

    connect(&settings_manager_, &SettingsManager::error, this,
            [this](const QString& err_msg)
    {
        qCritical().nospace().noquote() << err_msg;
        QMessageBox::critical(nullptr, "Error", err_msg);
        close();
    });
    connect(&dynamic_info_update_timer_, &QTimer::timeout, this, &MainWindow::update_dynamic_info);
    connect(&settings_window_, &SettingsWindow::settings_applied, this, &MainWindow::apply_settings);
    connect(ui->horizontalSlider_power_limit, &QAbstractSlider::valueChanged, this,
            [this](int value)
    {
        ui->label_current_power_limit_slider->setText(QString::number(value));
    });
    connect(&tray_icon_, &QSystemTrayIcon::activated, this,
            [this](QSystemTrayIcon::ActivationReason)
    {
        toggle_tray();
    });

    tray_menu_.addAction("Show/hide app window", this, &MainWindow::toggle_tray);
    tray_menu_.addAction("App settings", &settings_window_, &QMainWindow::showNormal);
    tray_menu_.addAction("Quit", this, &MainWindow::on_actionQuit_triggered);
    tray_icon_.setContextMenu(&tray_menu_);

    settings_manager_.open_file(QIODevice::ReadOnly);
    const auto app_settings{settings_manager_.load_settings()};
    settings_manager_.close_file();
    minimize_to_tray_on_close_ = app_settings["minimize_to_tray_on_close"].toBool();

    set_static_info();
    update_dynamic_info();

    dynamic_info_update_timer_.setInterval(1250);
    dynamic_info_update_timer_.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QSystemTrayIcon& MainWindow::get_tray_icon()
{
    return tray_icon_;
}

void MainWindow::on_actionShow_hide_app_window_triggered()
{
    toggle_tray();
}

void MainWindow::on_actionSettings_triggered()
{
    settings_window_.show();
}

void MainWindow::on_actionQuit_triggered()
{
    minimize_to_tray_on_close_= false;
    close();
}

void MainWindow::on_pushButton_apply_power_settings_clicked()
{
    const int ret_code{QProcess::execute("/usr/bin/pkexec", {"/usr/bin/nvidia-smi", "-pl", QString::number(ui->horizontalSlider_power_limit->value())})};
    if (ret_code == 0)
    {
        ui->statusbar->showMessage(QString{"Set new power limit: %1"}.arg(ui->horizontalSlider_power_limit->value()));
        qInfo().nospace().noquote() << "Set new power limit: " << ui->lineEdit_current_power_limit->text();
    }
    else
    {
        ui->statusbar->showMessage("Failed to set power limit");
        qCritical().nospace().noquote() << "Failed to set power limit, code: " << ret_code;
    }
}

void MainWindow::toggle_tray()
{
    if (isHidden())
    {
        showNormal();
    }
    else
    {
        hide();
        tray_icon_.show();
    }
}


void MainWindow::update_dynamic_info()
{
    nvml_device_.update_dynamic_info();
    const auto& dynamic_info{nvml_device_.get_dynamic_info()};

    ui->progressBar_GPU_usage_percentage->setValue(dynamic_info.gpu_usage_percentage);
    ui->progressBar_memory_usage_percentage->setValue(dynamic_info.memory_usage_percentage);
    ui->lineEdit_memory_usage_mib->setText(QString::number(dynamic_info.memory_usage_bytes / 1024 / 1024) + " MiB");
    ui->progressBar_encoder_usage_percentage->setValue(dynamic_info.encoder_usage_percentage);
    ui->progressBar_decoder_usage_percentage->setValue(dynamic_info.decoder_usage_percentage);

    ui->lineEdit_current_power_limit->setText(QString::number(dynamic_info.current_power_limit / 1000) + " W");
    ui->lineEdit_enforced_power_usage->setText(ui->lineEdit_current_power_limit->text());
    ui->lineEdit_current_power_usage->setText(QString::number(dynamic_info.current_power_usage / 1000.f) + " W");
    ui->lineEdit_current_temperature->setText(QString::number(dynamic_info.current_gpu_temperature) + " °C");

    ui->label_fan_speed_percentage->setText(QString::number(dynamic_info.current_fan_speed_percentage) + "%");
}

void MainWindow::apply_settings(const QJsonObject& settings)
{
    minimize_to_tray_on_close_ = settings["minimize_to_tray_on_close"].toBool();
    qInfo().nospace().noquote() << "new settings applied: " << settings;
}

void MainWindow::on_fan_profile_created(const QJsonObject& profile)
{
    /// TODO:
    /// implement fan profiles
}

void MainWindow::closeEvent(QCloseEvent* event_)
{
    if (minimize_to_tray_on_close_)
    {
        event_->ignore();
        hide();
        tray_icon_.show();
    }
    else
    {
        tray_icon_.hide();
        event_->accept();
    }
}

void MainWindow::set_static_info()
{
    ui->lineEdit_GPU_name->setText(QString::fromStdString(nvml_device_.get_name()));
    ui->lineEdit_GPU_driver_version->setText(QString::fromStdString(nvml_device_.get_system_driver_version()));
    ui->lineEdit_GPU_VBIOS_version->setText(QString::fromStdString(nvml_device_.get_vbios_version()));
    ui->lineEdit_GPU_UUID->setText(QString::fromStdString(nvml_device_.get_uuid()));
    ui->lineEdit_total_memory_mib->setText(QString::number(nvml_device_.get_memory_total_bytes() / 1024 / 1024) + " MiB");

    ui->lineEdit_max_power_usage->setText(QString::number(nvml_device_.get_max_power_usage() / 1000) + " W");
    ui->lineEdit_min_power_usage->setText(QString::number(nvml_device_.get_min_power_usage() / 1000) + " W");
    ui->lineEdit_default_power_usage->setText(QString::number(nvml_device_.get_default_power_usage() / 1000.f) + " W");

    ui->horizontalSlider_power_limit->setMaximum(nvml_device_.get_max_power_usage() / 1000);
    ui->horizontalSlider_power_limit->setMinimum(nvml_device_.get_min_power_usage() / 1000);
    ui->horizontalSlider_power_limit->setValue(nvml_device_.get_dynamic_info().current_power_limit / 1000);

    ui->label_current_power_limit_slider->setText(QString::number(nvml_device_.get_dynamic_info().current_power_limit / 1000));
    ui->label_max_power_limit_slider->setText(QString::number(nvml_device_.get_max_power_usage() / 1000));

    ui->lineEdit_shutdown_temperature->setText(QString::number(nvml_device_.get_shutdown_temperature()) + " °C");
    ui->lineEdit_slowdown_temperature->setText(QString::number(nvml_device_.get_slowdown_temperature()) + " °C");
}

void MainWindow::on_comboBox_fan_profile_activated(int index)
{
    if (index == (ui->comboBox_fan_profile->count() - 1))
    {
        const auto user_choise{new_file_profile_dialog_.exec()};
        if (user_choise == QDialog::DialogCode::Accepted)
        {
            reinterpret_cast<QComboBox*>(sender())->addItem("Add new profile...");
        }
    }
}
