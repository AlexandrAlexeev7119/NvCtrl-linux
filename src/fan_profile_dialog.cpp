#include "fan_profile_dialog.hpp"
#include "ui_fan_profile_dialog.h"

FanProfileDialog::FanProfileDialog(QWidget* parent)
    : QDialog {parent}
    , ui {new Ui::FanProfileDialog}
{
    ui->setupUi(this);
    connect(ui->horizontalSlider_fan_speed, &QSlider::valueChanged, this, [this](int value) {
        ui->label_fan_speed_slider_indicator->setText(QString::number(value) + "%");
    });
}

FanProfileDialog::~FanProfileDialog()
{
    delete ui;
}

void FanProfileDialog::on_pushButton_create_new_profile_clicked()
{

}

void FanProfileDialog::on_pushButton_close_clicked()
{
    close();
}
