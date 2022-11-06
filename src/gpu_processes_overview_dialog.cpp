#include <QDebug>

#include "gpu_processes_overview_dialog.hpp"
#include "ui_gpu_processes_overview_dialog.h"

GPUProcessesOverviewDialog::GPUProcessesOverviewDialog(QWidget* parent)
    : QDialog {parent}
    , ui {new Ui::GPUProcessesOverviewDialog}
{
    ui->setupUi(this);
    setMinimumSize(size());
    setMaximumSize(size() * 1.2);
}



GPUProcessesOverviewDialog::~GPUProcessesOverviewDialog()
{
    delete ui;
}
