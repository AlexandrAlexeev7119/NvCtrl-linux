#pragma once

#include <QDialog>
#include <QJsonObject>

namespace Ui { class FanProfileDialog; }

class FanProfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FanProfileDialog(QWidget* parent = nullptr);
    ~FanProfileDialog();

signals:
    void new_profile_created(const QJsonObject&);

private slots:
    void on_pushButton_create_new_profile_clicked();
    void on_pushButton_close_clicked();

private:
    Ui::FanProfileDialog* ui;
};

