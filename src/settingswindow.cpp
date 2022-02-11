#include <stdexcept>
#include <QMessageBox>

#include "settingswindow.hpp"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget* parent)
    : QMainWindow{parent}
    , ui{new Ui::SettingsWindow}
    , settings_manager_{}
{
    ui->setupUi(this);
    setMinimumSize(size());
    setMaximumSize(size() * 1.2);

    const QString config_filename{"./gwepp.json"};
    settings_manager_.set_file_name(config_filename);

    try
    {
        settings_manager_.open_file(QIODevice::ReadOnly);
    }
    catch (const std::exception& ex)
    {
        QMessageBox::critical(this, "Error", ex.what());
        close();
    }

    const QJsonObject settings{settings_manager_.load_settings()};
    settings_manager_.close_file();

    ui->checkBox_minimize_to_tray_on_close->setChecked(settings["minimize_to_tray_on_close"].toBool());
    ui->checkBox_minimize_to_tray_on_startup->setChecked(settings["minimize_to_tray_on_startup"].toBool());
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
    try
    {
        settings_manager_.open_file(QIODevice::WriteOnly);
    }
    catch (const std::exception& ex)
    {
        QMessageBox::critical(this, "Error", ex.what());
        return;
    }

    const QJsonObject settings{
        {"minimize_to_tray_on_close",   ui->checkBox_minimize_to_tray_on_close->isChecked()},
        {"minimize_to_tray_on_startup", ui->checkBox_minimize_to_tray_on_startup->isChecked()},
    };
    settings_manager_.save_settings(settings);
    settings_manager_.close_file();

    emit settings_applied(settings);
}
