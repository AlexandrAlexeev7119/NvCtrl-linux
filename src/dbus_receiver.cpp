#include <QDBusConnection>

#include "dbus_receiver.hpp"

DBusReceiver::DBusReceiver(std::string_view service_name, QObject* parent)
    : QObject {parent}
{
    QDBusConnection::sessionBus().registerService(service_name.data());
    QDBusConnection::sessionBus().registerObject(QStringLiteral("/"), this, QDBusConnection::ExportAllSlots);
}



int DBusReceiver::handle_message()
{
    emit message_received();
    return 0;
}
