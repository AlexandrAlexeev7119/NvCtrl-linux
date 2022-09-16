#include "about_dialog.hpp"
#include "ui_about_dialog.h"

AboutDialog::AboutDialog(QWidget* parent)
    : QDialog {parent}
    , ui {new Ui::AboutDialog}
{
    ui->setupUi(this);
    setMinimumSize(size());
    setMaximumSize(size());
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_pushButton_ok_clicked()
{
    close();
}
