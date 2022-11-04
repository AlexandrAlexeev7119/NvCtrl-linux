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
        const auto last_app_version {retrieve_last_ver_process_.readAll().trimmed().split('.')};
        const auto current_app_version {QString{GWEpp::config::APP_VERSION_STRING}.split('.')};
        const int last_major {last_app_version[MAJOR_VER].toInt()};
        const int last_minor {last_app_version[MINOR_VER].toInt()};
        const int last_patch {last_app_version[PATCH_VER].toInt()};
        const int current_major {current_app_version[MAJOR_VER].toInt()};
        const int current_minor {current_app_version[MINOR_VER].toInt()};
        const int current_patch {current_app_version[PATCH_VER].toInt()};

        qDebug().noquote().nospace() << "Retrieved version: " << last_app_version;

        if (last_patch > current_patch ||
                last_minor > current_minor ||
                last_major > current_major)
        {
            qInfo().noquote().nospace() << "Update available! New version: " << last_app_version;
            emit new_version_released(last_app_version.join("."));
        }
        else
        {
            qInfo().noquote().nospace() << "No updates available yet";
            emit update_not_found();
        }
    }
}
