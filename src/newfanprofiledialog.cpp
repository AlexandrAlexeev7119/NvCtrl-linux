#include "newfanprofiledialog.hpp"
#include "ui_newfanprofiledialog.h"

NewFanProfileDialog::NewFanProfileDialog(QWidget *parent)
    : QDialog{parent}
    , ui{new Ui::NewFanProfileDialog}
{
    ui->setupUi(this);
}

NewFanProfileDialog::~NewFanProfileDialog()
{
    delete ui;
}

void NewFanProfileDialog::on_buttonBox_accepted()
{
    emit fan_profile_created({
        {"profile_name",    ui->lineEdit_fan_profile_name->text()},
        {"fan_speed",       ui->spinBox_fan_speed_percentage->value()}
    });
}
