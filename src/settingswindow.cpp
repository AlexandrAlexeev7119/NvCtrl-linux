#include <stdexcept>
#include <QMessageBox>

#include "settingswindow.hpp"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget* parent)
    : QMainWindow{parent}
    , ui{new Ui::SettingsWindow}
    , settings_manager_instance_{SettingsManager::get_instance()}
{
    ui->setupUi(this);
    setMinimumSize(size());
    setMaximumSize(size() * 1.2);

    settings_manager_instance_.open_file(QIODevice::ReadOnly);
    const QJsonObject app_settings{settings_manager_instance_.load_settings()};
    settings_manager_instance_.close_file();

    ui->checkBox_minimize_to_tray_on_close->setChecked(app_settings["minimize_to_tray_on_close"].toBool());
    ui->checkBox_minimize_to_tray_on_startup->setChecked(app_settings["minimize_to_tray_on_startup"].toBool());
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::on_pushButton_close_window_clicked()
{
    if (isHidden())
    {
        showNormal();
    }
    else
    {
        hide();
    }
}

void SettingsWindow::on_pushButton_apply_settings_clicked()
{
    settings_manager_instance_.open_file(QIODevice::WriteOnly);

    const QJsonObject settings{
        {"minimize_to_tray_on_close",   ui->checkBox_minimize_to_tray_on_close->isChecked()},
        {"minimize_to_tray_on_startup", ui->checkBox_minimize_to_tray_on_startup->isChecked()},
    };

    settings_manager_instance_.save_settings(settings);
    settings_manager_instance_.close_file();

    emit settings_applied(settings);
}
