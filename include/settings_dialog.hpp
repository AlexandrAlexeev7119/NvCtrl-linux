#pragma once

#include <QDialog>
#include <QJsonObject>
#include "settings_manager.hpp"

namespace Ui { class SettingsDialog; }

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget* parent = nullptr);
    ~SettingsDialog();

private slots:
    void on_pushButton_apply_clicked();
    void on_pushButton_cancel_clicked();

    void on_SettingsManager_error(const QString& msg);

signals:
    void settings_applied(const QJsonObject&);

private:
    Ui::SettingsDialog* ui;
    SettingsManager& settings_manager_;

    void showEvent(QShowEvent* event_);

    void load_settings_from_file();
    void save_settings_to_file(const QJsonObject& app_settings);
};
