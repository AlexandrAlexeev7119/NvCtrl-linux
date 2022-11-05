#pragma once

#include <mutex>

#include <QSharedMemory>
#include <QSystemSemaphore>


// Specialization for QSystemSemaphore
namespace std
{
template<>
struct lock_guard<QSystemSemaphore>
{
    using mutex_type = QSystemSemaphore;

    lock_guard(const lock_guard&) = delete;
    lock_guard& operator=(const lock_guard&) = delete;

    explicit lock_guard(mutex_type& semaphore)
        : semaphore_ {semaphore}
    { semaphore_.acquire(); }

    ~lock_guard()
    { semaphore_.release(); }

private:
    mutex_type& semaphore_;
};
} // namespace std



class SingleInstanceAppGuard
{
public:
    SingleInstanceAppGuard(const QString& key);
    ~SingleInstanceAppGuard();

    bool is_running();
    bool run();

private:
    const QString shared_mem_key_;
    const QString semaphore_key_;
    QSharedMemory shared_mem_;
    QSystemSemaphore system_semaphore_;

    void release();
    QString generate_hash(const QString& key, const QString& salt) const;
};
