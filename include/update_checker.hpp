#pragma once

#include <QObject>
#include <QProcess>

class UpdateChecker : public QObject
{
    Q_OBJECT

public:
    UpdateChecker(QObject* parent = nullptr);

public slots:
    void check_for_updates();

signals:
    void new_version_released(QStringView);
    void error_occured(QStringView);

private:
    QProcess retrieve_last_ver_process_;
};
