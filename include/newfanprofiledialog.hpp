#pragma once

#include <QDialog>
#include <QJsonObject>

namespace Ui { class NewFanProfileDialog; }

class NewFanProfileDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NewFanProfileDialog(QWidget* parent = nullptr);
    ~NewFanProfileDialog();

signals:
    void fan_profile_created(const QJsonObject&);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::NewFanProfileDialog* ui;
};
