#include <QDebug>

#include "app_config.hpp"
#include "update_checker.hpp"

constexpr const char* version_file_url_main {"https://codeberg.org/AlexCr4ckPentest/GWEpp/raw/branch/main/.last_version"};
constexpr const char* version_file_url_develop {"https://codeberg.org/AlexCr4ckPentest/GWEpp/raw/branch/single-gpu/.last_version"};



UpdateChecker::UpdateChecker(QObject* parent)
    : QThread {parent}
    , retrieve_last_ver_process_ {}
    , branch_type_ {0}
{ }



void UpdateChecker::set_update_branch(unsigned branch_type) noexcept
{
    branch_type_= branch_type;
}



void UpdateChecker::check_for_updates()
{
    start();
}



void UpdateChecker::run()
{
    qInfo().noquote().nospace() << "Checking for updates...";

    if (branch_type_ == MAIN_BRANCH)
    {
        retrieve_last_ver_process_.start("/usr/bin/curl", {version_file_url_main});
    }
    else
    {
        retrieve_last_ver_process_.start("/usr/bin/curl", {version_file_url_develop});
    }

    retrieve_last_ver_process_.waitForFinished();

    const int err_code {retrieve_last_ver_process_.exitCode()};

    if (err_code != 0)
    {
        const QString error_msg {QString{"Failed to check update: %1, code: %2"}.arg(retrieve_last_ver_process_.errorString()).arg(err_code)};
        qCritical().noquote().nospace() << error_msg;
        emit error_occured(error_msg);
    }
    else
    {
        const QString last_app_version {retrieve_last_ver_process_.readAll().trimmed()};
        qDebug().noquote().nospace() << "Retrieved version: " << last_app_version;
        if (last_app_version > GWEpp::config::APP_VERSION_STRING)
        {
            qInfo().noquote().nospace() << "Update available! New version: " << last_app_version;
            emit new_version_released(last_app_version);
        }
        else
        {
            qInfo().noquote().nospace() << "No updates available yet";
            emit update_not_found();
        }
    }
}
