#include <QMessageBox>

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include "settings_manager.hpp"
#include "nvmlpp/nvmlpp_session.hpp"
#include "nvmlpp/util/nvmlpp_errors.hpp"

MainWindow::MainWindow(nlohmann::json app_settings, QWidget* parent)
    : QMainWindow {parent}
    , ui {new Ui::MainWindow}
    , tray_icon_ {this}
    , gpu_utilizations_controller_ {}
    , gpu_power_controller_ {}
    , gpu_clock_controller_ {}
    , gpu_fan_controller_ {}
    , dynamic_info_update_timer_ {}
    , nvmlpp_session_ {NVMLpp::Session::instance()}
    , current_gpu_ {NVMLpp::NVML_device::from_index(0)}
    , settings_dialog_window_ {this}
    , about_dialog_window_ {this}
    , report_a_bug_dialog_window_ {this}
    , fan_profile_dialog_window_ {this}
    , tray_menu_ {this}
    , curr_gpu_power_control_unsupported_ {false}
    , curr_gpu_clock_control_unsupported_ {false}
    , curr_gpu_fan_control_unsupported_ {false}
{
    ui->setupUi(this);
    setMinimumSize(size());

    app_settings_ = std::move(app_settings);

    connect_slots_and_signals();
    setup_tray_menu();
    load_and_validate_app_settings();

    set_current_gpu_for_controllers();
    set_static_info();
    update_dynamic_info();

    dynamic_info_update_timer_.start();
}



MainWindow::~MainWindow()
{
    delete ui;
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
    gpu_utilizations_controller_.update_info();
    gpu_power_controller_.update_info();
    gpu_clock_controller_.update_info();
    gpu_fan_controller_.update_info();
}



void MainWindow::on_SettingsDialog_settings_applied(const nlohmann::json& app_settings)
{
    minimize_to_tray_on_close_ = app_settings["minimize_to_tray_on_close"].get<bool>();
    update_freq_ms_ = app_settings["update_freq_ms"].get<unsigned>();
    dynamic_info_update_timer_.setInterval(update_freq_ms_);

    qInfo().noquote().nospace() << "New settings applied: " << app_settings.dump().c_str();
}



void MainWindow::on_FanProfileDialog_new_profile_created(const nlohmann::json& app_settings)
{
    ui->comboBox_select_fan_profile->addItem(QString::fromStdString(app_settings.back()["name"].get<std::string>()));
}



void MainWindow::on_GpuUtilizationsController_info_ready(const GpuUtilizationsController::utilization_rates& utilization_rates)
{
    ui->progressBar_GPU_usage->setValue(utilization_rates.gpu);
    ui->progressBar_GPU_mem_usage->setValue(utilization_rates.mem);
    ui->progressBar_GPU_encoder_usage->setValue(utilization_rates.encoder);
    ui->progressBar_GPU_decoder_usage->setValue(utilization_rates.decoder);
    ui->lineEdit_GPU_mem_usage->setText(QString::number(utilization_rates.mem_used / 1024 / 1024) + " MiB");
    ui->lineEdit_current_pstate->setText("Pstate: " + QString::number(utilization_rates.pstate));
    ui->label_current_gpu_core_temp_value->setText(QString::number(utilization_rates.gpu_core_temp) + "°C");
}



void MainWindow::on_GpuPowerController_info_ready(const GpuPowerController::power_rates& power_rates)
{
    ui->lineEdit_current_power_usage->setText(QString::number(power_rates.usage / 1000) + " W");
    ui->lineEdit_current_power_limit->setText(QString::number(power_rates.limit / 1000) + " W");
}



void MainWindow::on_GpuClockController_info_ready(const GpuClockController::clock_values& clock_values)
{
    ui->lineEdit_graphics_clock_current->setText(QString::number(clock_values.graphics) + " MHz");
    ui->lineEdit_video_clock_current->setText(QString::number(clock_values.video) + " MHz");
    ui->lineEdit_sm_clock_current->setText(QString::number(clock_values.sm) + " MHz");
    ui->lineEdit_memory_clock_current->setText(QString::number(clock_values.mem) + " MHz");
}



void MainWindow::on_GpuFanController_info_ready(const GpuFanController::fan_rates& fan_rates)
{
    ui->label_current_fan_speed_level_value->setText(QString::number(fan_rates.fan_speed_level) + "%");
}



void MainWindow::on_GpuPowerController_error()
{
    disconnect(&gpu_power_controller_, &GpuPowerController::error, this, &MainWindow::on_GpuPowerController_error);
    disconnect(&gpu_power_controller_, &GpuPowerController::info_ready, this, &MainWindow::on_GpuPowerController_info_ready);
    curr_gpu_power_control_unsupported_ = true;
    qWarning().noquote().nospace() << "Power control unsupported, signals disconnected";
}



void MainWindow::on_GpuClockController_error()
{
    disconnect(&gpu_clock_controller_, &GpuClockController::error, this, &MainWindow::on_GpuClockController_error);
    disconnect(&gpu_clock_controller_, &GpuClockController::info_ready, this, &MainWindow::on_GpuClockController_info_ready);
    curr_gpu_fan_control_unsupported_ = true;
    qWarning().noquote().nospace() << "Clock control unsupported, signals disconnected";
}



void MainWindow::on_GpuFanController_error()
{
    ui->groupBox_fan_control->setEnabled(false);
    ui->groupBox_fan_control->setToolTip("Unsupported for current GPU");
    qWarning().noquote().nospace() << "Fan control unsupported, groupBox widget disabled";

    disconnect(&gpu_fan_controller_, &GpuFanController::error, this, &MainWindow::on_GpuFanController_error);
    disconnect(&gpu_fan_controller_, &GpuFanController::info_ready, this, &MainWindow::on_GpuFanController_info_ready);
    curr_gpu_fan_control_unsupported_ = true;

    qWarning().noquote().nospace() << "Fan control unsupported, signals disconnected";
}



void MainWindow::connect_slots_and_signals()
{
    connect(&tray_icon_, &QSystemTrayIcon::activated, this, &MainWindow::toggle_tray);

    connect(&gpu_utilizations_controller_, &GpuUtilizationsController::info_ready, this, &MainWindow::on_GpuUtilizationsController_info_ready);
    connect(&gpu_power_controller_, &GpuPowerController::info_ready, this, &::MainWindow::on_GpuPowerController_info_ready);
    connect(&gpu_clock_controller_, &GpuClockController::info_ready, this, &MainWindow::on_GpuClockController_info_ready);
    connect(&gpu_fan_controller_, &GpuFanController::info_ready, this, &MainWindow::on_GpuFanController_info_ready);

    connect(&gpu_power_controller_, &GpuPowerController::error, this, &MainWindow::on_GpuPowerController_error);
    connect(&gpu_clock_controller_, &GpuClockController::error, this, &MainWindow::on_GpuClockController_error);
    connect(&gpu_fan_controller_, &GpuFanController::error, this, &MainWindow::on_GpuFanController_error);

    connect(&dynamic_info_update_timer_, &QTimer::timeout, this, &MainWindow::update_dynamic_info);

    connect(ui->horizontalSlider_change_power_limit, &QSlider::valueChanged, this, [this](int value)
    {
        ui->label_power_limit_slider_indicator->setText(QString::number(value));
    });
    connect(ui->horizontalSlider_set_fan_speed, &QSlider::valueChanged, this, [this](int value) {
        ui->label_set_fan_speed_slider_indicator->setText(QString::number(value) + "%");
    });
}



void MainWindow::setup_tray_menu()
{
    tray_menu_.addAction("Show/Hide app window", this, &MainWindow::toggle_tray);
    tray_menu_.addAction("Exit", this, &MainWindow::on_actionQuit_triggered);
    tray_icon_.setContextMenu(&tray_menu_);
}



void MainWindow::load_and_validate_app_settings()
{
    minimize_to_tray_on_close_ = app_settings_["minimize_to_tray_on_close"].get<bool>();
    update_freq_ms_ = app_settings_["update_freq_ms"].get<unsigned>();
    auto& fan_speed_profiles = app_settings_["fan_speed_profiles"];

    if (update_freq_ms_ < 500)
    {
        update_freq_ms_ = 500;
        app_settings_["update_freq_ms"] = update_freq_ms_;

        qWarning().noquote().nospace() << "Wrong update_freq_ms_ detected, fallback to default (" << update_freq_ms_ << ")";

        SettingsManager::instance().open_file(std::ios::out);
        SettingsManager::instance().write_settings(app_settings_);
        SettingsManager::instance().close_file();
    }

    if (!fan_speed_profiles.is_null())
    {
        for (auto& fan_speed_profile : fan_speed_profiles)
        {
            ui->comboBox_select_fan_profile->addItem(QString::fromStdString(fan_speed_profile["name"].get<std::string>()));
        }
    }

    dynamic_info_update_timer_.setInterval(update_freq_ms_);
    qInfo().noquote().nospace() << "Settings for MainWindow has been loaded";
}



void MainWindow::set_static_info()
{
    ui->lineEdit_GPU_name->setText(QString::fromStdString(current_gpu_.get_name()));
    ui->lineEdit_GPU_arch->setText(QString::fromStdString(current_gpu_.get_arch()));
    ui->lineEdit_GPU_vendor->setText(QString::fromStdString(current_gpu_.get_vendor()));
    ui->lineEdit_GPU_subvendor->setText(QString::fromStdString(current_gpu_.get_subvendor()));
    ui->lineEdit_GPU_VBIOS_ver->setText(QString::fromStdString(current_gpu_.get_vbios_version()));
    ui->lineEdit_GPU_driver_ver->setText(QString::fromStdString(nvmlpp_session_.get_system_driver_version()));
    ui->lineEdit_GPU_bus_type->setText(QString::fromStdString(current_gpu_.get_bus_type()));
    ui->lineEdit_GPU_bus_id->setText(QString::fromStdString(current_gpu_.get_pci_bus_id()));
    ui->lineEdit_GPU_total_mem->setText(QString::number(current_gpu_.get_total_memory() / 1024 / 1024) + " MiB");
    ui->lineEdit_GPU_slowdown_temp->setText(QString::number(current_gpu_.get_slowdown_temperature()) + "°C");
    ui->lineEdit_GPU_shutdown_temp->setText(QString::number(current_gpu_.get_shutdown_temperature()) + "°C");

    if (ui->actionShow_GPU_UUID->isChecked())
    {
        ui->lineEdit_GPU_uuid->setText(QString::fromStdString(current_gpu_.get_uuid()));
    }
    else
    {
        ui->lineEdit_GPU_uuid->setText("************************");
        ui->lineEdit_GPU_uuid->setToolTip("Disabled for a privacy reasons (enable: View -> show GPU UUID)");
    }

    try
    {
        const unsigned min_power_limit {current_gpu_.get_min_power_limit() / 1000};
        const unsigned max_power_limit {current_gpu_.get_max_power_limit() / 1000};
        const unsigned current_power_limit {current_gpu_.get_current_power_limit() / 1000};

        ui->lineEdit_default_power_limit->setText(QString::number(current_gpu_.get_default_power_limit() / 1000) + " W");
        ui->lineEdit_enforced_power_limit->setText(QString::number(current_gpu_.get_enforced_power_limit() / 1000) + " W");
        ui->label_power_limit_slider_indicator->setText(QString::number(current_power_limit));
        ui->lineEdit_min_power_limit->setText(QString::number(min_power_limit) + " W");
        ui->lineEdit_max_power_limit->setText(QString::number(max_power_limit) + " W");
        ui->horizontalSlider_change_power_limit->setMinimum(min_power_limit);
        ui->horizontalSlider_change_power_limit->setMaximum(max_power_limit);
        ui->horizontalSlider_change_power_limit->setValue(current_power_limit);
    }
    catch (const NVMLpp::errors::error_not_supported&)
    {
        ui->groupBox_power_control->setEnabled(false);
        ui->groupBox_power_control->setToolTip("Unsupported for current GPU");
        qWarning().noquote().nospace() << "Power control not supported, groupBox widget disabled";
    }

    try
    {
        ui->lineEdit_graphics_clock_max->setText(QString::number(current_gpu_.get_max_clock_graphics()) + " MHz");
        ui->lineEdit_video_clock_max->setText(QString::number(current_gpu_.get_max_clock_video()) + " MHz");
        ui->lineEdit_sm_clock_max->setText(QString::number(current_gpu_.get_max_clock_sm()) + " MHz");
        ui->lineEdit_memory_clock_max->setText(QString::number(current_gpu_.get_max_clock_memory()) + " MHz");
    }
    catch (const NVMLpp::errors::error_not_supported&)
    {
        ui->groupBox_clock_info->setEnabled(false);
        ui->groupBox_clock_info->setToolTip("Unsupported for current GPU");
        qWarning().noquote().nospace() << "Clock control not supported, groupBox widget disabled";
    }

    qInfo().noquote().nospace() << "Static info has been set";
}



void MainWindow::set_current_gpu_for_controllers() noexcept
{
    gpu_utilizations_controller_.set_device(&current_gpu_);
    gpu_power_controller_.set_device(&current_gpu_);
    gpu_clock_controller_.set_device(&current_gpu_);
    gpu_fan_controller_.set_device(&current_gpu_);
}



void MainWindow::manual_fan_speed_control_widgets_enabled(bool value)
{
    ui->horizontalSlider_set_fan_speed->setEnabled(value);
    ui->label_set_fan_speed_slider_indicator->setEnabled(value);
    ui->pushButton_apply_fan_speed->setEnabled(value);
}



void MainWindow::manual_clock_offset_control_widgets_enabled(bool value)
{
    ui->horizontalSlider_set_gpu_clock_offset->setEnabled(value);
    ui->horizontalSlider_set_mem_clock_profile->setEnabled(value);
    ui->pushButton_apply_clock_offset->setEnabled(value);
}



void MainWindow::closeEvent(QCloseEvent* event)
{
    if (minimize_to_tray_on_close_)
    {
        event->ignore();
        hide();
        tray_icon_.show();
        qInfo().noquote().nospace() << "Close event ignored, minimized to tray";
    }
    else
    {
        tray_icon_.hide();
        event->accept();
        qInfo().noquote().nospace() << "Close event accepted, MainWindow closed";
    }
}



void MainWindow::on_comboBox_select_fan_profile_activated(int index)
{
    switch (index)
    {
    case FAN_PROFILE_AUTO:
        gpu_fan_controller_.set_fan_control_state(false);
        manual_fan_speed_control_widgets_enabled(false);
        ui->pushButton_edit_current_fan_profile->setEnabled(false);
        break;
    case FAN_PROFILE_MANUAL:
        gpu_fan_controller_.set_fan_control_state(true);
        manual_fan_speed_control_widgets_enabled(true);
        ui->pushButton_edit_current_fan_profile->setEnabled(false);
        break;
    default:
        gpu_fan_controller_.set_fan_control_state(true);
        manual_fan_speed_control_widgets_enabled(false);
        ui->pushButton_edit_current_fan_profile->setEnabled(true);
        {
            auto& fan_profiles  = app_settings_["fan_speed_profiles"];
            const unsigned profile_index_in_array {static_cast<unsigned>(index - 2)};
            const unsigned fan_speed_level {fan_profiles[profile_index_in_array]["fan_speed"].get<unsigned>()};
            gpu_fan_controller_.set_fan_speed(fan_speed_level);
        }
        break;
    }

    qInfo().noquote().nospace() << "Selected fan control profile: " << ui->comboBox_select_fan_profile->currentText();
}



void MainWindow::on_comboBox_select_clock_offset_profile_activated(int index)
{
    switch (index)
    {
    case CLOCK_PROFILE_NONE:
        ui->pushButton_edit_curr_clock_offset_profile->setEnabled(false);
        manual_clock_offset_control_widgets_enabled(false);
        break;
    case CLOCK_PROFILE_MANUAL:
        ui->pushButton_edit_curr_clock_offset_profile->setEnabled(false);
        manual_clock_offset_control_widgets_enabled(true);
        break;
    default:
        ui->pushButton_edit_curr_clock_offset_profile->setEnabled(true);
        manual_clock_offset_control_widgets_enabled(false);
        break;
    }
}



void MainWindow::on_pushButton_apply_power_limit_clicked()
{
    gpu_power_controller_.set_power_limit(ui->horizontalSlider_change_power_limit->value());
}



void MainWindow::on_pushButton_apply_fan_speed_clicked()
{
    gpu_fan_controller_.set_fan_speed(ui->horizontalSlider_set_fan_speed->value());
}



void MainWindow::on_pushButton_add_new_fan_profile_clicked()
{
    fan_profile_dialog_window_.show();
}



void MainWindow::on_pushButton_edit_current_fan_profile_clicked()
{ }



void MainWindow::on_pushButton_apply_clock_offset_clicked()
{ }



void MainWindow::on_actionSettings_triggered()
{
    settings_dialog_window_.show();
}



void MainWindow::on_actionQuit_triggered()
{
    minimize_to_tray_on_close_ = false;
    close();
}



void MainWindow::on_actionAbout_triggered()
{
    about_dialog_window_.show();
}



void MainWindow::on_actionReport_a_bug_triggered()
{
    report_a_bug_dialog_window_.show();
}



void MainWindow::on_actionShow_GPU_UUID_toggled(bool checked)
{
    if (checked)
    {
        ui->lineEdit_GPU_uuid->setText(QString::fromStdString(current_gpu_.get_uuid()));
        ui->lineEdit_GPU_uuid->setToolTip("");
    }
    else
    {
        ui->lineEdit_GPU_uuid->setText("************************");
        ui->lineEdit_GPU_uuid->setToolTip("Disabled for a privacy reasons (enable: View -> show GPU UUID)");
    }
}
