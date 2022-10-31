#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QSlider>
#include <QGridLayout>
#include <QMessageBox>

#include "steps_fan_profile_dialog.hpp"
#include "ui_steps_fan_profile_dialog.h"

const unsigned StepsFanProfileDialog::row_max {10};


StepsFanProfileDialog::StepsFanProfileDialog(QWidget* parent)
    : QDialog {parent}
    , ui {new Ui::StepsFanProfileDialog}
    , current_row_ {0}
{
    ui->setupUi(this);
    setMinimumSize(size());
}



StepsFanProfileDialog::~StepsFanProfileDialog()
{
    delete ui;
}



void StepsFanProfileDialog::on_buttonBox_accepted()
{
    qWarning().noquote().nospace() << "Not implemented yet";
    on_buttonBox_rejected();
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

        QLineEdit* line_edit {new QLineEdit {parent_widget}};
        QSlider* h_slider {new QSlider {Qt::Orientation::Horizontal, parent_widget}};
        QLabel* label {new QLabel {parent_widget}};

        connect(h_slider, &QSlider::valueChanged, [label](unsigned value) {
            label->setText(QString::number(value) + "%");
        });

        h_slider->setMaximum(100);

        if (current_row_ == 0)
        {
            line_edit->setText("0");
            h_slider->setMinimum(0);
            label->setText("0%");
        }
        else
        {
            const auto find_widget {
                [&grid_layout_steps](unsigned row, unsigned col) -> QWidget* {
                    return grid_layout_steps->itemAtPosition(row, col)->widget();
                }
            };
            const auto& prev_line_edit {qobject_cast<QLineEdit*>(find_widget(current_row_ - 1, LINEEDIT_COLUMN_POS))};
            const auto& prev_h_slider {qobject_cast<QSlider*>(find_widget(current_row_ - 1, SLIDER_COLUMN_POS))};

            line_edit->setText(QString::number(prev_line_edit->text().toInt() + 1));
            h_slider->setMinimum(prev_h_slider->minimum() + 1);
            h_slider->setValue(prev_h_slider->minimum() + 1);
        }

        grid_layout_steps->addWidget(line_edit, current_row_, LINEEDIT_COLUMN_POS);
        grid_layout_steps->addWidget(h_slider, current_row_, SLIDER_COLUMN_POS);
        grid_layout_steps->addWidget(label, current_row_, LABEL_COLUMN_POS);

        current_row_++;
        grid_layout_steps->update();
    }
    else
    {
        ui->pushButton_add_new_step->setEnabled(false);
    }
}
