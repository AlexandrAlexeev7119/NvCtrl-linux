#pragma once

#include <QObject>
#include <QDBusInterface>

class DBusSender final : public QObject
{
    Q_OBJECT

public:
    DBusSender(std::string_view service_name, QObject* parent = nullptr);
    int send_message();

    operator bool() const;

private:
    QDBusInterface dbus_iface_;
};
