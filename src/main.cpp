#include <iostream>

#include <QApplication>
#include <QMessageBox>

#include "mainwindow.hpp"
#include "settings_manager.hpp"

static void qt_msg_handler(QtMsgType msg_type, const QMessageLogContext& context,
                           const QString& message)
{
    switch (msg_type)
    {
    case QtMsgType::QtCriticalMsg:
        std::cout << "[CRITICAL]: ";
        break;
    case QtMsgType::QtDebugMsg:
        std::cout << "[DEBUG]: ";
        break;
    case QtMsgType::QtFatalMsg:
        std::cout << "[FATAL]: ";
        break;
    case QtMsgType::QtInfoMsg:
        std::cout << "[INFO]: ";
        break;
    case QtMsgType::QtWarningMsg:
        std::cout << "[WARNING]: ";
        break;
    }
    std::cout << message.toStdString() << "\n";
}

int main(int argc, char** argv)
{
    qInstallMessageHandler(qt_msg_handler);

    QApplication app{argc, argv};
    MainWindow main_window{};
    SettingsManager& settings_manager_instance{SettingsManager::get_instance()};

    QObject::connect(&settings_manager_instance, &SettingsManager::error, [](const QString& err_msg) {
        QMessageBox::critical(nullptr, "Error", err_msg);
        exit(1);
    });

    settings_manager_instance.open_file(QIODevice::ReadOnly);
    const auto app_settings{settings_manager_instance.load_settings()};
    settings_manager_instance.close_file();
    const bool minimize_to_tray_on_startup{app_settings["minimize_to_tray_on_startup"].toBool()};

    if (minimize_to_tray_on_startup)
    {
        main_window.get_tray_icon().show();
    }
    else
    {
        main_window.show();
    }

    return app.exec();
}
