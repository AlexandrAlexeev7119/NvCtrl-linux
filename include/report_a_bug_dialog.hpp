#pragma once

#include <QDialog>

namespace Ui { class ReportABugDialog; }

class ReportABugDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ReportABugDialog(QWidget* parent = nullptr);
    ~ReportABugDialog();

private slots:
    void on_pushButton_ok_clicked();
    void on_label_text_linkActivated(const QString& link);

private:
    Ui::ReportABugDialog* ui;
};
