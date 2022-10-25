#include <QDebug>

#include "app_config.hpp"
#include "update_checker.hpp"

constexpr const char* target_file_url {"https://notabug.org/AlexCr4ckPentest/GWEpp/raw/single-gpu/.last_version"};



UpdateChecker::UpdateChecker(QObject* parent)
    : QThread {parent}
    , retrieve_last_ver_process_ {}
{ }



void UpdateChecker::check_for_updates()
{
    start();
}



void UpdateChecker::run()
{
    qInfo().noquote().nospace() << "Checking for updates...";

    retrieve_last_ver_process_.start("/usr/bin/curl", {target_file_url});
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
        if (last_app_version > GWEpp::config::APP_VERSION_STRING)
        {
            qInfo().noquote().nospace() << "Update available! New version: " << last_app_version;
            emit new_version_released(last_app_version);
        }
        else
        {
            qInfo().noquote().nospace() << "No updates available yet";
        }
    }
}
