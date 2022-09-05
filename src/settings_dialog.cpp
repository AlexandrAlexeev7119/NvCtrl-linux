#include "settings_dialog.hpp"
#include "ui_settings_dialog.h"

#include <QShowEvent>
#include <QMessageBox>

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog {parent}
    , ui {new Ui::SettingsDialog}
    , settings_manager_ {SettingsManager::instance()}
{
    ui->setupUi(this);
    setMinimumSize(size());
    setMaximumSize(size());

    connect(&settings_manager_, &SettingsManager::error, this, &SettingsDialog::on_SettingsManager_error);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_pushButton_apply_clicked()
{
    const QJsonObject app_settings {
        {"minimize_to_tray_on_startup", ui->checkBox_minimize_to_tray_on_startup->isChecked()},
        {"minimize_to_tray_on_close", ui->checkBox_minimize_to_tray_on_close->isChecked()},
        {"update_freq_ms", ui->spinBox_update_freq->value()},
    };
    save_settings_to_file(app_settings);
    emit settings_applied(app_settings);
}

void SettingsDialog::on_pushButton_cancel_clicked()
{
    close();
}

void SettingsDialog::on_SettingsManager_error(const QString& msg)
{
    QMessageBox::critical(this, "Error", msg);
    qCritical().noquote().nospace() << msg;
    close();
}

void SettingsDialog::showEvent(QShowEvent* event_)
{
    load_settings_from_file();
    event_->accept();
}

void SettingsDialog::load_settings_from_file()
{
    settings_manager_.open_file(QIODevice::ReadOnly);
    const QJsonObject app_settings {settings_manager_.load_settings()};
    settings_manager_.close_file();

    ui->checkBox_minimize_to_tray_on_startup->setChecked(app_settings["minimize_to_tray_on_startup"].toBool());
    ui->checkBox_minimize_to_tray_on_close->setChecked(app_settings["minimize_to_tray_on_close"].toBool());
    ui->spinBox_update_freq->setValue(app_settings["update_freq_ms"].toInt());

    qInfo().noquote().nospace() << "Settings on form loaded";
}

void SettingsDialog::save_settings_to_file(const QJsonObject& app_settings)
{
    settings_manager_.open_file(QIODevice::WriteOnly);
    settings_manager_.save_settings(app_settings);
    settings_manager_.close_file();

    qInfo().noquote().nospace() << "Settings saved to file";
}
