#include <QDebug>

#include "app_config.hpp"
#include "update_checker.hpp"

constexpr const char* last_version_file_url {"https://notabug.org/AlexCr4ckPentest/GWEpp/raw/main/.last_version"};


UpdateChecker::UpdateChecker(QObject* parent)
    : QObject {parent}
    , retrieve_last_ver_process_ {}
{ }



void UpdateChecker::check_for_updates()
{
    retrieve_last_ver_process_.start("/usr/bin/curl", {last_version_file_url});
    retrieve_last_ver_process_.waitForFinished();

    const int err_code {retrieve_last_ver_process_.exitCode()};

    if (err_code != 0)
    {
        qCritical().noquote().nospace() << retrieve_last_ver_process_.errorString() << ", code: " << err_code;
    }
    else
    {
        const QString last_app_version {retrieve_last_ver_process_.readAll().trimmed()};
        if (last_app_version > GWEpp::config::APP_VERSION_STRING)
        {
            qDebug().noquote().nospace() << "Updates available! new version: " << last_app_version;
        }
    }
}
