#include <iostream>

#include <QApplication>
#include "mainwindow.hpp"

static void qt_msg_handler(QtMsgType msg_type, const QMessageLogContext& context,
                           const QString& message)
{
    switch (msg_type)
    {
    case QtMsgType::QtCriticalMsg:
        std::cout << "[CRITICAL]: ";
        break;
    case QtMsgType::QtDebugMsg:
        std::cout << "[DEBUG]: " << context.file << ":" << context.line << " in " << context.function << ": ";
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
    QApplication a{argc, argv};
    MainWindow main_window{};
    qInstallMessageHandler(qt_msg_handler);
    main_window.show();
    return a.exec();
}
