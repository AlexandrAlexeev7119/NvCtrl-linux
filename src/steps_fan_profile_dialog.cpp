#include <QDebug>
#include <QSpinBox>
#include <QSlider>
#include <QLabel>
#include <QMessageBox>

#include "steps_fan_profile_dialog.hpp"
#include "ui_steps_fan_profile_dialog.h"

const int StepsFanProfileDialog::row_max {10};


StepsFanProfileDialog::StepsFanProfileDialog(QWidget* parent)
    : QDialog {parent}
    , ui {new Ui::StepsFanProfileDialog}
    , current_row_ {0}
{
    ui->setupUi(this);
    setMinimumSize(size());
    setMaximumSize(size() * 1.5);
}



StepsFanProfileDialog::~StepsFanProfileDialog()
{
    delete ui;
}



void StepsFanProfileDialog::load_app_settings(nlohmann::json* app_settings) noexcept
{
    ptr_app_settings_ = app_settings;
}



void StepsFanProfileDialog::on_buttonBox_accepted()
{
    if (ui->lineEdit_new_profile_name->text().isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Empty name is not allowed");
    }
    else
    {
        auto& app_settings {*ptr_app_settings_};
        auto& fan_profiles_list {app_settings["fan_speed_profiles"]};

        nlohmann::json::object_t new_profile {
            {"name", ui->lineEdit_new_profile_name->text().toStdString()},
            {"steps", nlohmann::json::array_t {}}
        };

        auto const grid_layout_steps {findChild<QGridLayout*>("gridLayout_steps")};

        for (int row {0}; row < grid_layout_steps->rowCount(); row++)
        {
            const auto& spin_box {qobject_cast<QSpinBox*>(get_widget_of(grid_layout_steps, row, SPINBOX_COLUMN_POS))};
            const auto& h_slider {qobject_cast<QSlider*>(get_widget_of(grid_layout_steps, row, SLIDER_COLUMN_POS))};
            new_profile["steps"].emplace_back(std::make_pair(spin_box->value(), h_slider->value()));
        }

        fan_profiles_list.emplace_back(std::move(new_profile));
        emit new_profile_created(fan_profiles_list.back());

        on_buttonBox_rejected();
    }
}



void StepsFanProfileDialog::on_buttonBox_rejected()
{
    close();
}



void StepsFanProfileDialog::on_pushButton_add_new_step_clicked()
{
    if (current_row_ != row_max)
    {
        auto const grid_layout_steps {findChild<QGridLayout*>("gridLayout_steps")};
        const auto& parent_widget {qobject_cast<QWidget*>(grid_layout_steps)};

        QSpinBox* spin_box {new QSpinBox {parent_widget}};
        QSlider* h_slider {new QSlider {Qt::Orientation::Horizontal, parent_widget}};
        QLabel* label {new QLabel {parent_widget}};

        connect(h_slider, &QSlider::valueChanged, this, [label](int value) {
            label->setText(QString::number(value) + "%");
        });

        spin_box->setMaximum(100);
        h_slider->setMaximum(100);

        if (current_row_ == 0)
        {
            h_slider->setMinimum(0);
            label->setText("0%");
        }
        else
        {
            const auto& prev_spin_box {qobject_cast<QSpinBox*>(get_widget_of(grid_layout_steps, current_row_ - 1, SPINBOX_COLUMN_POS))};
            const auto& prev_h_slider {qobject_cast<QSlider*>(get_widget_of(grid_layout_steps, current_row_ - 1, SLIDER_COLUMN_POS))};
            spin_box->setMinimum(prev_spin_box->value() + 1);
            h_slider->setMinimum(prev_h_slider->value() + 1);
        }

        grid_layout_steps->addWidget(spin_box, current_row_, SPINBOX_COLUMN_POS);
        grid_layout_steps->addWidget(h_slider, current_row_, SLIDER_COLUMN_POS);
        grid_layout_steps->addWidget(label, current_row_, LABEL_COLUMN_POS);

        current_row_++;
        grid_layout_steps->update();
    }
    else
    {
        ui->pushButton_add_new_step->setEnabled(false);
        QMessageBox::warning(this, "Warning", QString{"Steps limit is %1"}.arg(row_max));
    }
}



QWidget* StepsFanProfileDialog::get_widget_of(QGridLayout* const grid_layout,
                                              unsigned row, unsigned col) const
{
    return grid_layout->itemAtPosition(row, col)->widget();
}
