#pragma once

#include <QThread>
#include <QProcess>

class UpdateChecker : public QThread
{
    Q_OBJECT

public:
    UpdateChecker(QObject* parent = nullptr);

public slots:
    void check_for_updates();

signals:
    void new_version_released(const QString&);
    void error_occured(const QString&);

protected:
    using QThread::start;
    void run() override;

private:
    QProcess retrieve_last_ver_process_;
};
