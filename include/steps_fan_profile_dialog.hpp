#pragma once

#include <QDialog>
#include <QGridLayout>

#include "settings_manager.hpp"

namespace Ui { class StepsFanProfileDialog; }

class StepsFanProfileDialog : public QDialog
{
    Q_OBJECT

    static const int row_max;
    enum { SPINBOX_COLUMN_POS, SLIDER_COLUMN_POS, LABEL_COLUMN_POS };

public:
    explicit StepsFanProfileDialog(QWidget* parent = nullptr);
    ~StepsFanProfileDialog();

    void load_app_settings(nlohmann::json* app_settings) noexcept;

signals:
    void new_profile_created(const nlohmann::json&);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_pushButton_add_new_step_clicked();

private:
    Ui::StepsFanProfileDialog* ui;
    nlohmann::json* ptr_app_settings_;
    int current_row_;

    QWidget* get_widget_of(QGridLayout* const grid_layout, unsigned row, unsigned col) const;
};
