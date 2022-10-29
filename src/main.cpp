#include <QApplication>
#include <QMessageBox>
#include <QUrl>
#include <QDebug>

#include "spdlog/spdlog.h"

#include "mainwindow.hpp"
#include "settings_manager.hpp"

static void qt_message_handler(QtMsgType msg_type, const QMessageLogContext& context,
                           const QString& message)
{
    switch (msg_type)
    {
    case QtMsgType::QtCriticalMsg:
        spdlog::critical(message.toStdString());
        break;
    case QtMsgType::QtDebugMsg:
        spdlog::debug("[{}:{}]: {}", QUrl{context.file}.fileName().toStdString(), context.line, message.toStdString());
        break;
    case QtMsgType::QtFatalMsg:
        spdlog::critical(message.toStdString());
        break;
    case QtMsgType::QtInfoMsg:
        spdlog::info(message.toStdString());
        break;
    case QtMsgType::QtWarningMsg:
        spdlog::warn(message.toStdString());
        break;
    }
}



int main(int argc, char** argv)
{
#ifndef NDEBUG
    spdlog::set_level(spdlog::level::debug);
#else
    spdlog::set_level(spdlog::level::info);
#endif

    Q_INIT_RESOURCE(icons);
    qInstallMessageHandler(qt_message_handler);

    QApplication app {argc, argv};
    SettingsManager& settings_manager {SettingsManager::instance()};
    QObject::connect(&settings_manager, &SettingsManager::error_occured,
                     [](const QString& err_msg)
    {
        qCritical().nospace().noquote() << err_msg;
        QMessageBox::critical(nullptr, "Error", err_msg);
        std::exit(1);
    });

    auto app_settings = settings_manager.read_settings();

    const bool minimize_to_tray_on_startup {app_settings["minimize_to_tray_on_startup"].get<bool>()};
    MainWindow main_window {std::move(app_settings)};

    if (minimize_to_tray_on_startup)
    {
        main_window.get_tray_icon().show();
        qInfo().nospace().noquote() << "Start minimized to tray";
    }
    else
    {
        main_window.show();
        qInfo().nospace().noquote() << "Start normal window";
    }

    return app.exec();
}
