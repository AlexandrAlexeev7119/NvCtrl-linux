#include <iostream>

#include <QApplication>
#include <QMessageBox>

#include "mainwindow.hpp"
#include "settings_manager.hpp"

static void qt_msg_handler(QtMsgType msg_type, const QMessageLogContext& context,
                           const QString& message)
{
    bool is_stderr{false};
    switch (msg_type)
    {
    case QtMsgType::QtCriticalMsg:
        std::cout << "[CRITICAL]: ";
        is_stderr = true;
        break;
    case QtMsgType::QtDebugMsg:
        std::cout << "[DEBUG]: " << context.file << ":" << context.line << " " << context.function << ": ";
        break;
    case QtMsgType::QtFatalMsg:
        std::cerr << "[FATAL]: ";
        is_stderr = true;
        break;
    case QtMsgType::QtInfoMsg:
        std::cout << "[INFO]: ";
        break;
    case QtMsgType::QtWarningMsg:
        std::cout << "[WARNING]: ";
        break;
    }

    (is_stderr ? std::cerr : std::cout) << message.toStdString() << "\n";
}

int main(int argc, char** argv)
{
    qInstallMessageHandler(qt_msg_handler);

    QApplication app {argc, argv};
    MainWindow main_window {};
    SettingsManager& settings_manager {SettingsManager::instance()};

    QObject::connect(&settings_manager, &SettingsManager::error,
                     [](const QString& err_msg)
    {
        qCritical().nospace().noquote() << err_msg;
        QMessageBox::critical(nullptr, "Error", err_msg);
        std::exit(1);
    });

    settings_manager.open_file(QIODevice::ReadOnly);
    const auto app_settings{settings_manager.load_settings()};
    settings_manager.close_file();
    const bool minimize_to_tray_on_startup{app_settings["minimize_to_tray_on_startup"].toBool()};


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
