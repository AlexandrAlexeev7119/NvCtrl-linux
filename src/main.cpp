#include <iostream>
#include <QApplication>

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
    SettingsManager settings_manager{};

    settings_manager.set_file_name("./gwepp.json");

    try
    {
        settings_manager.open_file(QIODevice::ReadOnly);
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what();
        return 1;
    }

    const bool minimize_to_tray_on_startup{settings_manager.load_settings()["minimize_to_tray_on_startup"].toBool()};
    settings_manager.close_file();

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
