#pragma once

#include <QDialog>
#include <QDebug>

#include "settings_manager.hpp"

namespace Ui { class FanProfileDialog; }

class FanProfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FanProfileDialog(QWidget* parent = nullptr);
    ~FanProfileDialog();

signals:
    void new_profile_created(const nlohmann::json&);

private slots:
    void on_pushButton_create_new_profile_clicked();
    void on_pushButton_close_clicked();

private:
    Ui::FanProfileDialog* ui;

    void create_or_apped_fan_speed_profile(nlohmann::json& app_settings);
};
