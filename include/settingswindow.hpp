#pragma once

#include <QMainWindow>
#include <QJsonObject>

#include "settings_manager.hpp"

namespace Ui { class SettingsWindow; }

class SettingsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget* parent = nullptr);
    ~SettingsWindow();

signals:
    void settings_applied(const QJsonObject&);

private slots:
    void on_pushButton_close_window_clicked();
    void on_pushButton_apply_settings_clicked();

private:
    Ui::SettingsWindow* ui;
    SettingsManager& settings_manager_instance_;
};

