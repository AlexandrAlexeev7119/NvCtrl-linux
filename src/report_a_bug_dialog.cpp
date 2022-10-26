#include <QShowEvent>
#include <QDesktopServices>
#include <QUrl>

#include <cstdlib>

#include "nvmlpp/nvmlpp_session.hpp"

#include "report_a_bug_dialog.hpp"
#include "ui_report_a_bug_dialog.h"

ReportABugDialog::ReportABugDialog(QWidget* parent)
    : QDialog {parent}
    , ui {new Ui::ReportABugDialog}
    , external_process_ {this}
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



void ReportABugDialog::showEvent(QShowEvent* show_event)
{
    if (ui->plainTextEdit_system_info->toPlainText().isEmpty())
    {
        const QString session_type {std::getenv("XDG_SESSION_TYPE")};
        const QString desktop_env {std::getenv("XDG_CURRENT_DESKTOP")};

        external_process_.start("/usr/bin/uname", {"-r"}, QIODevice::ReadOnly);
        external_process_.waitForFinished();
        const QString kernel_ver {external_process_.readAll()};

        external_process_.start("/usr/bin/lsb_release", {"-a"}, QIODevice::ReadOnly);
        external_process_.waitForFinished();
        const QString lsb_release_info {external_process_.readAll()};

        QString sys_info {};
        sys_info.append(lsb_release_info + "\n");
        sys_info.append("Kernel ver: " + kernel_ver);
        sys_info.append("Driver ver: " + QString::fromStdString(NVMLpp::Session::instance().get_system_driver_version()) + "\n");
        sys_info.append("Session type: " + session_type + "\n");
        sys_info.append("Desktop env: " + desktop_env);

        ui->plainTextEdit_system_info->insertPlainText(sys_info);
    }

    show_event->accept();
}
