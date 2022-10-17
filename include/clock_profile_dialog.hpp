#pragma once

#include <QDialog>

#include "settings_manager.hpp"

namespace Ui { class ClockProfileDialog; }

class ClockProfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClockProfileDialog(QWidget* parent = nullptr);
    ~ClockProfileDialog();

    void load_app_settings(nlohmann::json* app_settings) noexcept;

signals:
    void new_profile_created(const nlohmann::json&);

private slots:
    void on_pushButton_create_new_profile_clicked();
    void on_pushButton_close_clicked();

private:
    Ui::ClockProfileDialog* ui;
    nlohmann::json* ptr_app_settings_;
};

