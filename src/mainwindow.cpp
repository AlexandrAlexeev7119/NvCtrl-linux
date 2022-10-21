#include <future>

#include <QMessageBox>
#include <QDebug>

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include "settings_manager.hpp"
#include "nvmlpp/nvmlpp_session.hpp"
#include "nvmlpp/util/nvmlpp_errors.hpp"

MainWindow::MainWindow(nlohmann::json app_settings, QWidget* parent)
    : QMainWindow {parent}
    , ui {new Ui::MainWindow}
    , tray_menu_ {this}
    , tray_icon_ {this}
    , dynamic_info_update_timer_ {}

    , nvmlpp_session_ {NVMLpp::Session::instance()}
    , current_gpu_ {NVMLpp::NVML_device::from_index(0)}

    , gpu_utilizations_controller_ {this}
    , gpu_power_controller_ {this}
    , gpu_clock_controller_ {this}
    , gpu_fan_controller_ {this}

    , settings_dialog_window_ {this}
    , about_dialog_window_ {this}
    , report_a_bug_dialog_window_ {this}
    , fan_profile_dialog_window_ {this}
    , edit_fan_profile_dialog_window_ {this}
    , clock_profile_dialog_window_ {this}
    , edit_clock_offset_profile_dialog_window_ {this}
{
    ui->setupUi(this);
    setMinimumSize(size());
    app_settings_ = std::move(app_settings);

    connect_slots_and_signals();
    set_static_info();

    set_current_gpu_for_controllers();
    load_and_validate_app_settings();

    update_dynamic_info();
    dynamic_info_update_timer_.start();

    setup_tray_menu();
}



MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::toggle_tray()
{
    if (isHidden())
    {
        tray_icon_.hide();
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
    last_fan_and_clock_offset_profiles_saved_ = app_settings["last_fan_and_clock_offset_profiles_saved"].get<bool>();
    update_freq_ms_ = app_settings["update_freq_ms"].get<unsigned>();
    dynamic_info_update_timer_.setInterval(update_freq_ms_);

    qInfo().noquote().nospace() << "New settings applied";
    ui->statusBar->showMessage("New settings applied", 2000);
}



void MainWindow::on_FanProfileDialog_new_profile_created(const nlohmann::json& curr_fan_profile)
{
    const QString new_fan_profile {QString::fromStdString(curr_fan_profile.back()["name"].get<std::string>())};
    ui->comboBox_select_fan_profile->addItem(new_fan_profile);
    ui->statusBar->showMessage("New fan profile created: " + new_fan_profile, 2000);
}



void MainWindow::on_EditFanProfileDialog_current_fan_profile_changed(const nlohmann::json& curr_fan_profile)
{
    const unsigned index {static_cast<unsigned>(ui->comboBox_select_fan_profile->currentIndex())};
    ui->comboBox_select_fan_profile->removeItem(index);
    ui->comboBox_select_fan_profile->insertItem(index, QString::fromStdString(curr_fan_profile["name"].get<std::string>()));
    ui->comboBox_select_fan_profile->setCurrentIndex(index);
}



void MainWindow::on_EditFanProfileDialog_current_fan_profile_removed()
{
    const unsigned index {static_cast<unsigned>(ui->comboBox_select_fan_profile->currentIndex())};
    qInfo().noquote().nospace() << "Fan profile removed: " << ui->comboBox_select_fan_profile->currentText();
    ui->statusBar->showMessage("Fan profile removed: " + ui->comboBox_select_fan_profile->currentText(), 2000);

    ui->comboBox_select_fan_profile->removeItem(index);
}



void MainWindow::on_ClockProfileDialog_new_profile_created(const nlohmann::json& curr_clock_profile)
{
    const auto& new_clock_profile_ref = curr_clock_profile.back();
    const QString new_clock_profile_name {QString::fromStdString(new_clock_profile_ref["name"].get<std::string>())};

    ui->comboBox_select_clock_offset_profile->addItem(new_clock_profile_name);
    ui->statusBar->showMessage("New clock profile created: " + new_clock_profile_name, 2000);
}



void MainWindow::on_EditClockOffsetProfileDialog_current_clock_offset_profile_changed(const nlohmann::json& curr_clock_profile)
{
    const unsigned index {static_cast<unsigned>(ui->comboBox_select_clock_offset_profile->currentIndex())};
    ui->comboBox_select_clock_offset_profile->removeItem(index);

    ui->comboBox_select_clock_offset_profile->insertItem(index, QString::fromStdString(curr_clock_profile["name"].get<std::string>()));
    ui->comboBox_select_clock_offset_profile->setCurrentIndex(index);
    ui->lineEdit_current_gpu_clock_offset->setText(QString::number(curr_clock_profile["gpu_clock_offset"].get<int>()) + " MHz");
    ui->lineEdit_current_mem_clock_offset->setText(QString::number(curr_clock_profile["mem_clock_offset"].get<int>()) + " MHz");
}



void MainWindow::on_EditClockOffsetProfileDialog_current_clock_offset_profile_removed()
{
    const unsigned index {static_cast<unsigned>(ui->comboBox_select_clock_offset_profile->currentIndex())};
    qInfo().noquote().nospace() << "Clock profile removed: " << ui->comboBox_select_clock_offset_profile->currentText();
    ui->statusBar->showMessage("Clock profile removed: " + ui->comboBox_select_clock_offset_profile->currentText(), 2000);

    ui->comboBox_select_clock_offset_profile->removeItem(index);
}



void MainWindow::on_GpuUtilizationsController_info_ready(const GpuUtilizationsController::utilization_rates& utilization_rates)
{
    ui->progressBar_GPU_usage->setValue(utilization_rates.gpu);
    ui->progressBar_GPU_mem_usage->setValue(utilization_rates.mem);
    ui->progressBar_GPU_encoder_usage->setValue(utilization_rates.encoder);
    ui->progressBar_GPU_decoder_usage->setValue(utilization_rates.decoder);
    ui->lineEdit_GPU_mem_usage->setText(QString::number(utilization_rates.mem_used / 1024 / 1024) + " MiB");
    ui->lineEdit_current_pstate->setText("Pstate: " + QString::number(utilization_rates.pstate));
    ui->lineEdit_current_gpu_core_temp->setText(QString::number(utilization_rates.gpu_core_temp) + " °C");
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
    ui->lineEdit_current_fan_speed_level->setText(QString::number(fan_rates.fan_speed_level) + "%");
}



void MainWindow::on_GpuUtilizationsController_encoder_decoder_unsupported()
{
    if (ui->progressBar_GPU_encoder_usage->isEnabled() &&
            ui->progressBar_GPU_decoder_usage->isEnabled())
    {
        ui->progressBar_GPU_encoder_usage->setEnabled(false);
        ui->progressBar_GPU_decoder_usage->setEnabled(false);
        qWarning().noquote().nospace() << "Encoder and decoder utilization rates unsupported, disabling widgets";
    }
}



void MainWindow::on_GpuPowerController_error()
{
    disconnect(&gpu_power_controller_, &GpuPowerController::error, this, &MainWindow::on_GpuPowerController_error);
    disconnect(&gpu_power_controller_, &GpuPowerController::info_ready, this, &MainWindow::on_GpuPowerController_info_ready);
    qWarning().noquote().nospace() << "Power control unsupported, signals disconnected";
}



void MainWindow::on_GpuClockController_error()
{
    disconnect(&gpu_clock_controller_, &GpuClockController::error, this, &MainWindow::on_GpuClockController_error);
    disconnect(&gpu_clock_controller_, &GpuClockController::info_ready, this, &MainWindow::on_GpuClockController_info_ready);
    qWarning().noquote().nospace() << "Clock control unsupported, signals disconnected";
}



void MainWindow::on_GpuFanController_error()
{
    ui->groupBox_fan_control->setEnabled(false);
    ui->groupBox_fan_control->setToolTip("Unsupported for current GPU");
    qWarning().noquote().nospace() << "Fan control unsupported, groupBox widget disabled";

    disconnect(&gpu_fan_controller_, &GpuFanController::error, this, &MainWindow::on_GpuFanController_error);
    disconnect(&gpu_fan_controller_, &GpuFanController::info_ready, this, &MainWindow::on_GpuFanController_info_ready);

    qWarning().noquote().nospace() << "Fan control unsupported, signals disconnected";
}



void MainWindow::connect_slots_and_signals()
{
    connect(&tray_icon_, &QSystemTrayIcon::activated, this, &MainWindow::toggle_tray);
    connect(ui->horizontalSlider_change_power_limit, &QSlider::valueChanged, [this](int value)
    {
        ui->label_power_limit_slider_indicator->setText(QString::number(value));
    });

    connect(&gpu_utilizations_controller_, &GpuUtilizationsController::info_ready, this, &MainWindow::on_GpuUtilizationsController_info_ready);
    connect(&gpu_power_controller_, &GpuPowerController::info_ready, this, &::MainWindow::on_GpuPowerController_info_ready);
    connect(&gpu_clock_controller_, &GpuClockController::info_ready, this, &MainWindow::on_GpuClockController_info_ready);
    connect(&gpu_fan_controller_, &GpuFanController::info_ready, this, &MainWindow::on_GpuFanController_info_ready);

    connect(&gpu_utilizations_controller_, &GpuUtilizationsController::encoder_decoder_unsupported, this, &MainWindow::on_GpuUtilizationsController_encoder_decoder_unsupported);
    connect(&gpu_power_controller_, &GpuPowerController::error, this, &MainWindow::on_GpuPowerController_error);
    connect(&gpu_clock_controller_, &GpuClockController::error, this, &MainWindow::on_GpuClockController_error);
    connect(&gpu_fan_controller_, &GpuFanController::error, this, &MainWindow::on_GpuFanController_error);

    connect(&dynamic_info_update_timer_, &QTimer::timeout, this, &MainWindow::update_dynamic_info);
}



void MainWindow::setup_tray_menu()
{
    tray_menu_.addAction("Show/Hide app window", this, &MainWindow::toggle_tray);
    tray_menu_.addSeparator();
    tray_menu_.addAction("Quit", this, &MainWindow::on_actionQuit_triggered);
    tray_icon_.setContextMenu(&tray_menu_);

    tray_icon_.setIcon(QIcon{"/usr/share/icons/gwepp/gwepp128.png"});
}



void MainWindow::load_and_validate_app_settings()
{
    minimize_to_tray_on_close_ = app_settings_["minimize_to_tray_on_close"].get<bool>();
    last_fan_and_clock_offset_profiles_saved_ = app_settings_["last_fan_and_clock_offset_profiles_saved"].get<bool>();
    update_freq_ms_ = app_settings_["update_freq_ms"].get<unsigned>();

    if (update_freq_ms_ < 500)
    {
        update_freq_ms_ = 500;
        app_settings_["update_freq_ms"] = update_freq_ms_;

        qWarning().noquote().nospace() << "Wrong update_freq_ms_ detected, fallback to default (" << update_freq_ms_ << ")";

        SettingsManager::instance().open_file(std::ios::out);
        SettingsManager::instance().write_settings(app_settings_);
        SettingsManager::instance().close_file();
    }

    load_fan_and_clock_offset_profiles();

    if (last_fan_and_clock_offset_profiles_saved_)
    {
        restore_last_fan_and_clock_offset_profiles();
    }

    dynamic_info_update_timer_.setInterval(update_freq_ms_);
    qInfo().noquote().nospace() << "Settings for MainWindow has been loaded";

    qDebug().noquote().nospace() << "Default settings: " << SettingsManager::default_settings.dump(4).c_str();
}



void MainWindow::load_fan_and_clock_offset_profiles()
{
    auto load_fan_profiles_future {std::async(std::launch::async, [this]()
        {
            const auto& fan_speed_profiles = app_settings_["fan_speed_profiles"];
            for (const auto& fan_speed_profile : fan_speed_profiles)
            {
                ui->comboBox_select_fan_profile->addItem(QString::fromStdString(
                                                             fan_speed_profile["name"].get<std::string>()
                                                         ));
            }
            qDebug().noquote().nospace() << "Total fan profiles loaded: " << fan_speed_profiles.size();
        })};

    auto load_clock_offset_profiles_future {std::async(std::launch::async, [this]()
        {
            const auto& clock_offset_profiles = app_settings_["clock_offset_profiles"];
            for (const auto& clock_offset_profile : clock_offset_profiles)
            {
                ui->comboBox_select_clock_offset_profile->addItem(QString::fromStdString(
                                                                      clock_offset_profile["name"].get<std::string>()
                                                                  ));
            }
            qDebug().noquote().nospace() << "Total clock offset profiles loaded: " << clock_offset_profiles.size();
        })};
}



void MainWindow::restore_last_fan_and_clock_offset_profiles()
{
    const unsigned last_fan_profile_index {app_settings_["last_fan_profile_index"].get<unsigned>()};
    const unsigned last_clock_offset_profile_index {app_settings_["last_clock_offset_profile_index"].get<unsigned>()};
    const auto& last_fan_profile = app_settings_["fan_speed_profiles"][last_fan_profile_index];
    const auto& last_clock_offset_profile = app_settings_["clock_offset_profiles"][last_clock_offset_profile_index];

    ui->comboBox_select_fan_profile->setCurrentIndex(last_fan_profile_index);
    ui->comboBox_select_clock_offset_profile->setCurrentIndex(last_clock_offset_profile_index);
    on_comboBox_select_fan_profile_activated(last_fan_profile_index);
    on_comboBox_select_clock_offset_profile_activated(last_clock_offset_profile_index);
    on_pushButton_apply_fan_speed_clicked();
    on_pushButton_apply_clock_offset_clicked();
}



void MainWindow::set_static_info()
{
    ui->lineEdit_GPU_name->setText(QString::fromStdString(current_gpu_.get_name()));
    ui->lineEdit_GPU_arch->setText(QString::fromStdString(current_gpu_.get_arch().to_string()));
    ui->lineEdit_GPU_vendor->setText(QString::fromStdString(current_gpu_.get_vendor().to_string()));
    ui->lineEdit_GPU_subvendor->setText(QString::fromStdString(current_gpu_.get_subvendor().to_string()));
    ui->lineEdit_GPU_VBIOS_ver->setText(QString::fromStdString(current_gpu_.get_vbios_version()));
    ui->lineEdit_GPU_driver_ver->setText(QString::fromStdString(nvmlpp_session_.get_system_driver_version()));
    ui->lineEdit_GPU_bus_type->setText(QString::fromStdString(current_gpu_.get_bus_type().to_string()));
    ui->lineEdit_GPU_bus_id->setText(QString::fromStdString(current_gpu_.get_pci_bus_id()));
    ui->lineEdit_GPU_total_mem->setText(QString::number(current_gpu_.get_total_memory() / 1024 / 1024) + " MiB");
    ui->lineEdit_GPU_slowdown_temp->setText(QString::number(current_gpu_.get_slowdown_temperature()) + " °C");
    ui->lineEdit_GPU_shutdown_temp->setText(QString::number(current_gpu_.get_shutdown_temperature()) + " °C");

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
        set_max_clock_values();
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



void MainWindow::set_max_clock_values(int gpu_clock_offset, int mem_clock_offset) const
{
    ui->lineEdit_graphics_clock_max->setText(QString::number(current_gpu_.get_max_clock_graphics() + gpu_clock_offset) + " MHz");
    ui->lineEdit_video_clock_max->setText(QString::number(current_gpu_.get_max_clock_video() + gpu_clock_offset) + " MHz");
    ui->lineEdit_sm_clock_max->setText(QString::number(current_gpu_.get_max_clock_sm() + gpu_clock_offset) + " MHz");
    ui->lineEdit_memory_clock_max->setText(QString::number(current_gpu_.get_max_clock_memory() + mem_clock_offset) + " MHz");
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
        if (last_fan_and_clock_offset_profiles_saved_)
        {
            app_settings_["last_fan_profile_index"] = ui->comboBox_select_fan_profile->currentIndex();
            app_settings_["last_clock_offset_profile_index"] = ui->comboBox_select_clock_offset_profile->currentIndex();
            SettingsManager::instance().open_file(std::ios::out);
            SettingsManager::instance().write_settings(app_settings_);
            SettingsManager::instance().close_file();
        }
        qInfo().noquote().nospace() << "Close event accepted, MainWindow closed";
        event->accept();
    }
}



void MainWindow::on_comboBox_select_fan_profile_activated(int index)
{
    if (index > FAN_PROFILE_AUTO)
    {
        gpu_fan_controller_.set_fan_control_state(true);
        ui->pushButton_edit_current_fan_profile->setEnabled(true);
    }
    else
    {
        gpu_fan_controller_.set_fan_control_state(false);
        ui->pushButton_edit_current_fan_profile->setEnabled(false);
    }

    qInfo().noquote().nospace() << "Selected fan control profile: " << ui->comboBox_select_fan_profile->currentText();
}



void MainWindow::on_comboBox_select_clock_offset_profile_activated(int index)
{
    if (index > CLOCK_PROFILE_NONE)
    {
        ui->pushButton_edit_current_clock_offset_profile->setEnabled(true);
        const auto& current_clock_profile = app_settings_["clock_offset_profiles"][index];
        ui->lineEdit_current_gpu_clock_offset->setText(QString::number(current_clock_profile["gpu_clock_offset"].get<int>()) + " MHz");
        ui->lineEdit_current_mem_clock_offset->setText(QString::number(current_clock_profile["mem_clock_offset"].get<int>()) + " MHz");
    }
    else
    {
        ui->pushButton_edit_current_clock_offset_profile->setEnabled(false);
        ui->lineEdit_current_gpu_clock_offset->setText("0 MHz");
        ui->lineEdit_current_mem_clock_offset->setText("0 MHz");
    }

    qInfo().noquote().nospace() << "Selected clock offset profile: " << ui->comboBox_select_clock_offset_profile->currentText();
}



void MainWindow::on_pushButton_apply_power_limit_clicked()
{
    gpu_power_controller_.set_power_limit(ui->horizontalSlider_change_power_limit->value());
    ui->statusBar->showMessage("Power limit applied: " + QString::number(ui->horizontalSlider_change_power_limit->value()) + "W", 2000);
}



void MainWindow::on_pushButton_apply_fan_speed_clicked()
{
    const unsigned index {static_cast<unsigned>(ui->comboBox_select_fan_profile->currentIndex())};
    if (index > FAN_PROFILE_AUTO)
    {
        const auto& current_fan_profile = app_settings_["fan_speed_profiles"][index];
        const unsigned fan_speed_level {current_fan_profile["fan_speed"].get<unsigned>()};
        gpu_fan_controller_.set_fan_speed(fan_speed_level);
    }

    ui->statusBar->showMessage("Fan profile applied: " + ui->comboBox_select_fan_profile->currentText(), 2000);
}



void MainWindow::on_pushButton_add_new_fan_profile_clicked()
{
    fan_profile_dialog_window_.load_app_settings(&app_settings_);
    fan_profile_dialog_window_.show();
}



void MainWindow::on_pushButton_edit_current_fan_profile_clicked()
{
    const unsigned current_fan_profile_index {static_cast<unsigned>(ui->comboBox_select_fan_profile->currentIndex())};
    edit_fan_profile_dialog_window_.load_app_settings(&app_settings_);
    edit_fan_profile_dialog_window_.set_current_fan_profile_index(current_fan_profile_index);
    edit_fan_profile_dialog_window_.show();
}



void MainWindow::on_pushButton_add_new_clock_offset_profile_clicked()
{
    clock_profile_dialog_window_.load_app_settings(&app_settings_);
    clock_profile_dialog_window_.show();
}



void MainWindow::on_pushButton_apply_clock_offset_clicked()
{
    const unsigned index {static_cast<unsigned>(ui->comboBox_select_clock_offset_profile->currentIndex())};
    if (index > CLOCK_PROFILE_NONE)
    {
        const auto& current_clock_profile = app_settings_["clock_offset_profiles"][index];
        const int gpu_clock_offset {current_clock_profile["gpu_clock_offset"].get<int>()};
        const int mem_clock_offset {current_clock_profile["mem_clock_offset"].get<int>()};

        gpu_clock_controller_.set_clock_offsets(gpu_clock_offset, mem_clock_offset);
        set_max_clock_values(gpu_clock_offset, mem_clock_offset);
    }
    else
    {
        gpu_clock_controller_.set_clock_offsets(0, 0);
        set_max_clock_values();
    }

    ui->statusBar->showMessage("Clock profile applied: " + ui->comboBox_select_clock_offset_profile->currentText(), 2000);
}



void MainWindow::on_pushButton_edit_current_clock_offset_profile_clicked()
{
    const unsigned current_clock_offset_profile_index {static_cast<unsigned>(ui->comboBox_select_clock_offset_profile->currentIndex())};
    edit_clock_offset_profile_dialog_window_.load_app_settings(&app_settings_);
    edit_clock_offset_profile_dialog_window_.set_current_clock_offset_profile_index(current_clock_offset_profile_index);
    edit_clock_offset_profile_dialog_window_.show();
}



void MainWindow::on_actionSettings_triggered()
{
    settings_dialog_window_.load_app_settins(&app_settings_);
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
