#include <QMessageBox>

#include "settingswindow.hpp"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget* parent)
    : QMainWindow{parent}
    , ui{new Ui::SettingsWindow}
    , settings_manager_{SettingsManager::get_instance()}
{
    ui->setupUi(this);
    setMinimumSize(size());
    setMaximumSize(size() * 1.2);

    connect(&settings_manager_, &SettingsManager::error, this,
            [this](const QString& err_msg)
    {
        qCritical().nospace().noquote() << err_msg;
        QMessageBox::critical(this, "Error", err_msg);
        close();
    });

    settings_manager_.open_file(QIODevice::ReadOnly);
    const QJsonObject app_settings{settings_manager_.load_settings()};
    settings_manager_.close_file();

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
    settings_manager_.open_file(QIODevice::WriteOnly);

    const QJsonObject new_app_settings{
        {"minimize_to_tray_on_close",   ui->checkBox_minimize_to_tray_on_close->isChecked()},
        {"minimize_to_tray_on_startup", ui->checkBox_minimize_to_tray_on_startup->isChecked()},
    };
    settings_manager_.save_settings(new_app_settings);
    settings_manager_.close_file();

    emit settings_applied(new_app_settings);
}
