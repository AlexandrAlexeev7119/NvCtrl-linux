#include <QDesktopServices>
#include <QUrl>

#include "report_a_bug_dialog.hpp"
#include "ui_report_a_bug_dialog.h"

ReportABugDialog::ReportABugDialog(QWidget *parent)
    : QDialog {parent}
    , ui {new Ui::ReportABugDialog}
{
    ui->setupUi(this);
    setMinimumSize(size());
    setMaximumSize(size());
}

ReportABugDialog::~ReportABugDialog()
{
    delete ui;
}

void ReportABugDialog::on_pushButton_ok_clicked()
{
    close();
}

void ReportABugDialog::on_label_text_linkActivated(const QString& link)
{
    QDesktopServices::openUrl(QUrl{link});
    close();
}
