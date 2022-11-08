#include <QDebug>
#include <QShowEvent>
#include <QDesktopServices>

#include "recent_updates_dialog.hpp"
#include "ui_recent_updates_dialog.h"

constexpr const char* changelog_file_url {"https://codeberg.org/AlexCr4ckPentest/GWEpp/raw/branch/single-gpu/CHANGELOG.md"};



RecentUpdatesDialog::RecentUpdatesDialog(QWidget* parent)
    : QDialog {parent}
    , ui {new Ui::RecentUpdatesDialog}
    , retrieve_changelog_process_ {this}
{
    ui->setupUi(this);
    setMinimumSize(size());
    setMaximumSize(size() * 1.2);
}



RecentUpdatesDialog::~RecentUpdatesDialog()
{
    delete ui;
}



void RecentUpdatesDialog::on_buttonBox_accepted()
{
    on_buttonBox_rejected();
}



void RecentUpdatesDialog::on_buttonBox_rejected()
{
    close();
}



void RecentUpdatesDialog::on_label_changelog_text_linkActivated(const QString& link)
{
    QDesktopServices::openUrl(QUrl{link});
}



void RecentUpdatesDialog::showEvent(QShowEvent* show_event)
{
    qInfo().noquote().nospace() << "Getting changelog...";

    retrieve_changelog_process_.start("/usr/bin/curl", {changelog_file_url});
    retrieve_changelog_process_.waitForFinished();

    const int err_code {retrieve_changelog_process_.exitCode()};

    if (err_code != 0)
    {
        const QString error_msg {QString{"Failed to get changelog: %1, code: %2"}.arg(retrieve_changelog_process_.errorString()).arg(err_code)};
        qCritical().noquote().nospace() << error_msg;
        ui->label_changelog_text->setText("### " + error_msg);
    }
    else
    {
        QString changelog {};
        changelog.reserve(retrieve_changelog_process_.bytesAvailable());
        changelog = retrieve_changelog_process_.readAll();

        ui->label_changelog_text->setText(changelog);
        qInfo().noquote().nospace() << "Changelog loaded!";
    }

    show_event->accept();
}
