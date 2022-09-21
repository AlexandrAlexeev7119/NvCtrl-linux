#include "about_dialog.hpp"
#include "ui_about_dialog.h"

#include "app_version.hpp"

AboutDialog::AboutDialog(QWidget* parent)
    : QDialog {parent}
    , ui {new Ui::AboutDialog}
{
    ui->setupUi(this);
    setMinimumSize(size());
    setMaximumSize(size());
    ui->label_version->setText(QString{"v%1"}.arg(GWEpp::config::APP_VERSION_STRING));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_pushButton_ok_clicked()
{
    close();
}
