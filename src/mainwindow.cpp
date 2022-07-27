#include <QMessageBox>
#include <QProcess>

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

constexpr int STATUSBAR_MESSAGE_TIMEOUT_MS{1500};

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent}
    , ui{new Ui::MainWindow}
    , dynamic_info_update_timer_{}
    , tray_icon_{this}
    , tray_menu_{}
    , nvmlpp_session_instance_{NVMLpp::Session::instance()}
    , nvml_device_{NVMLpp::NVML_device::from_index(0)}
    , settings_manager_{SettingsManager::get_instance()}
    , settings_window_{this}
    , minimize_to_tray_on_close_{false}
    , update_freq_ms_{}
    , new_file_profile_dialog_{this}
{
    ui->setupUi(this);
    setMinimumSize(size());

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
    update_freq_ms_ = app_settings["update_freq_ms"].toInt();

    set_static_info();
    update_dynamic_info();

    dynamic_info_update_timer_.setInterval(update_freq_ms_);
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
        ui->statusbar->showMessage(QString{"Set new power limit: %1"}.arg(ui->horizontalSlider_power_limit->value()), STATUSBAR_MESSAGE_TIMEOUT_MS);
        qInfo().nospace().noquote() << "Set new power limit: " << ui->lineEdit_current_power_limit->text();
    }
    else
    {
        ui->statusbar->showMessage("Failed to set power limit", STATUSBAR_MESSAGE_TIMEOUT_MS);
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
    ui->progressBar_GPU_usage_percentage->setValue(nvml_device_.get_gpu_utilization());
    ui->progressBar_memory_usage_percentage->setValue(nvml_device_.get_memory_utilization());
    ui->lineEdit_memory_usage_mib->setText(QString::number(nvml_device_.get_used_memory()) + " MiB");
    ui->progressBar_encoder_usage_percentage->setValue(nvml_device_.get_encoder_utilization());
    ui->progressBar_decoder_usage_percentage->setValue(nvml_device_.get_decoder_utilization());

    ui->lineEdit_current_power_limit->setText(QString::number(nvml_device_.get_current_power_limit()) + " W");
    ui->lineEdit_enforced_power_usage->setText(ui->lineEdit_current_power_limit->text());
    ui->lineEdit_current_power_usage->setText(QString::number(nvml_device_.get_current_power_usage()) + " W");
    ui->lineEdit_current_temperature->setText(QString::number(nvml_device_.get_current_temperature()) + " °C");

    ui->label_fan_speed_percentage->setText(QString::number(nvml_device_.get_current_fan_speed_level()) + "%");
}

void MainWindow::apply_settings(const QJsonObject& settings)
{
    minimize_to_tray_on_close_ = settings["minimize_to_tray_on_close"].toBool();
    update_freq_ms_ = settings["update_freq_ms"].toInt();

    if (dynamic_info_update_timer_.interval() != update_freq_ms_)
    {
        dynamic_info_update_timer_.setInterval(update_freq_ms_);
    }

    ui->statusbar->showMessage("New settings applied", STATUSBAR_MESSAGE_TIMEOUT_MS);
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
    const unsigned max_power_limit {nvml_device_.get_max_power_limit()};
    const unsigned min_power_limit {nvml_device_.get_min_power_limit()};
    const unsigned default_power_limit {nvml_device_.get_default_power_limit()};
    const unsigned current_power_limit {nvml_device_.get_current_power_limit()};

    ui->lineEdit_GPU_name->setText(QString::fromStdString(nvml_device_.get_name()));
    ui->lineEdit_GPU_driver_version->setText(QString::fromStdString(nvmlpp_session_instance_.get_system_driver_version()));
    ui->lineEdit_GPU_VBIOS_version->setText(QString::fromStdString(nvml_device_.get_vbios_version()));
    ui->lineEdit_GPU_UUID->setText(QString::fromStdString(nvml_device_.get_uuid()));
    ui->lineEdit_total_memory_mib->setText(QString::number(nvml_device_.get_total_memory()) + " MiB");

    ui->lineEdit_max_power_usage->setText(QString::number(max_power_limit) + " W");
    ui->lineEdit_min_power_usage->setText(QString::number(min_power_limit) + " W");
    ui->lineEdit_default_power_usage->setText(QString::number(default_power_limit) + " W");

    ui->horizontalSlider_power_limit->setMaximum(max_power_limit);
    ui->horizontalSlider_power_limit->setMinimum(min_power_limit);
    ui->horizontalSlider_power_limit->setValue(current_power_limit);

    ui->label_current_power_limit_slider->setText(QString::number(current_power_limit));
    ui->label_max_power_limit_slider->setText(QString::number(max_power_limit));

    ui->lineEdit_shutdown_temperature->setText(QString::number(nvml_device_.get_temperature_threshold(NVMLpp::NVML_device::temperature_threshold::shutdown)) + " °C");
    ui->lineEdit_slowdown_temperature->setText(QString::number(nvml_device_.get_temperature_threshold(NVMLpp::NVML_device::temperature_threshold::slowdown)) + " °C");
}

void MainWindow::on_comboBox_fan_profile_activated(int index)
{
    if (index == 0)
    {
        ui->pushButton_edit_curr_fan_profile->setEnabled(false);
    }
    else
    {
        if (!ui->pushButton_edit_curr_fan_profile->isEnabled())
        {
            ui->pushButton_edit_curr_fan_profile->setEnabled(true);
        }
    }
}

void MainWindow::on_pushButton_edit_curr_fan_profile_clicked()
{

}
