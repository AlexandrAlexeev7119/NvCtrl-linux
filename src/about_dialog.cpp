#include "about_dialog.hpp"
#include "ui_about_dialog.h"

#include "app_config.hpp"

AboutDialog::AboutDialog(QWidget* parent)
    : QDialog {parent}
    , ui {new Ui::AboutDialog}
{
    ui->setupUi(this);
    setMinimumSize(size());
    setMaximumSize(size());
    ui->label_version->setText(QStringLiteral("v%1").arg(NvCtrl::config::APP_VERSION_WITH_BUILD_TYPE));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_pushButton_ok_clicked()
{
    close();
}
