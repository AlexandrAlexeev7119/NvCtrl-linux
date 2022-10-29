#pragma once

#include <QDialog>

#include "settings_manager.hpp"

namespace Ui { class StepsFanProfileDialog; }

class StepsFanProfileDialog : public QDialog
{
    Q_OBJECT

    static const unsigned row_max;
    enum { LINEEDIT_COLUMN_POS, SLIDER_COLUMN_POS, LABEL_COLUMN_POS };

public:
    explicit StepsFanProfileDialog(QWidget* parent = nullptr);
    ~StepsFanProfileDialog();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_pushButton_add_new_step_clicked();

private:
    Ui::StepsFanProfileDialog* ui;
    unsigned current_row_;
};
