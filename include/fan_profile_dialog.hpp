#pragma once

#include <QDialog>

#include "settings_manager.hpp"

namespace Ui { class FanProfileDialog; }

class FanProfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FanProfileDialog(QWidget* parent = nullptr);
    ~FanProfileDialog();

    void load_app_settings(nlohmann::json* app_settings) noexcept;

signals:
    void new_profile_created(const nlohmann::json&);

private slots:
    void on_pushButton_create_new_profile_clicked();
    void on_pushButton_close_clicked();

private:
    Ui::FanProfileDialog* ui;
    nlohmann::json* ptr_app_settings_;
};
