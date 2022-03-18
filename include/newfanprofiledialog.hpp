#pragma once

#include <QDialog>

namespace Ui { class NewFanProfileDialog; }

class NewFanProfileDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NewFanProfileDialog(QWidget* parent = nullptr);
    ~NewFanProfileDialog();

signals:
    void fan_profile_created();

private:
    Ui::NewFanProfileDialog* ui;
};
