#pragma once

#include <vector>

#include <QDialog>

namespace Ui { class GPUProcessesOverviewDialog; }

class GPUProcessesOverviewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GPUProcessesOverviewDialog(QWidget* parent = nullptr);
    ~GPUProcessesOverviewDialog();

private:
    Ui::GPUProcessesOverviewDialog* ui;
};
