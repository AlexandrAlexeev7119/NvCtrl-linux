#include <QCryptographicHash>
#include <QDebug>

#include "single_instance_app_guard.hpp"



SingleInstanceAppGuard::SingleInstanceAppGuard(const QString& key)
    : shared_mem_key_ {generate_hash(key, "gwepp_shared_mem_key_")}
    , semaphore_key_ {generate_hash(key, "gwepp_semaphore_key_")}
    , shared_mem_ {shared_mem_key_}
    , system_semaphore_ {semaphore_key_, 1}
{
    const std::lock_guard<QSystemSemaphore> lock {system_semaphore_};
    {
        QSharedMemory tmp {shared_mem_key_};
        tmp.attach();
    }
}



SingleInstanceAppGuard::~SingleInstanceAppGuard()
{
    release();
}



bool SingleInstanceAppGuard::is_running()
{
    if (shared_mem_.isAttached())
    {
        return false;
    }

    const std::lock_guard<QSystemSemaphore> lock {system_semaphore_};
    const bool is_running {shared_mem_.attach()};
    if (is_running)
    {
        shared_mem_.detach();
    }

    return is_running;
}



bool SingleInstanceAppGuard::run()
{
    if (is_running())
    {
        return false;
    }

    {
        const std::lock_guard<QSystemSemaphore> lock {system_semaphore_};
        const bool success {shared_mem_.create(sizeof(char))};
        if (!success)
        {
            release();
            return false;
        }
    }

    return true;
}



void SingleInstanceAppGuard::release()
{
    const std::lock_guard<QSystemSemaphore> lock {system_semaphore_};
    if (shared_mem_.isAttached())
    {
        shared_mem_.detach();
    }
}



QString SingleInstanceAppGuard::generate_hash(const QString& key, const QString& salt) const
{
    QByteArray hash;

    hash.append(key.toUtf8());
    hash.append(salt.toUtf8());
    hash = QCryptographicHash::hash(hash, QCryptographicHash::Algorithm::Sha3_256).toHex();

    return hash;
}
