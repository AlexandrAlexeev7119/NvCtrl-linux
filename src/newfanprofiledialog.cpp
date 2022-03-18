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
