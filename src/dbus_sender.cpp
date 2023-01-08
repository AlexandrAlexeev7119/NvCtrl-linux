#include <QDBusConnection>
#include <QDBusReply>

#include "dbus_sender.hpp"

DBusSender::DBusSender(std::string_view service_name, QObject* parent)
    : QObject {parent}
    , dbus_iface_ {service_name.data(), QStringLiteral("/"), {}, QDBusConnection::sessionBus()}
{ }



int DBusSender::send_message()
{
    const QDBusReply<int> reply {dbus_iface_.call(QStringLiteral("handle_message"))};
    if (reply.isValid())
    {
        return reply.value();
    }
    return -1;
}



DBusSender::operator bool() const
{
    return dbus_iface_.isValid();
}
