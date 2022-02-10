#pragma once

#include <QObject>
#include <QFile>
#include <QJsonObject>
#include <QJsonValue>

class SettingsManager : QObject
{
    Q_OBJECT
public:
    SettingsManager();
    ~SettingsManager();

private:
    QFile file_;
};
