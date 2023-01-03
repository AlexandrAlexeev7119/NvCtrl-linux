#pragma once

#include <QThread>
#include <QProcess>

class UpdateChecker final : public QThread
{
    Q_OBJECT

    enum { MAIN_BRANCH, DEV_BRANCH };
    enum { MAJOR_VER, MINOR_VER, PATCH_VER };

public:
    UpdateChecker(QObject* parent = nullptr);

    void set_update_branch(unsigned branch_type) noexcept;

public slots:
    void check_for_updates();

signals:
    void new_version_released(const QString&);
    void update_not_found();
    void error_occured(const QString&);

protected:
    using QThread::start;
    void run() override;

private:
    QProcess retrieve_last_ver_process_;
    unsigned branch_type_;
};
