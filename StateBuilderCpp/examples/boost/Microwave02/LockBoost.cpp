#include "LockBoost.h"

void LockBoost::Lock(void)
{
    m_mutex.lock();
}

void LockBoost::UnLock(void)
{
    m_mutex.unlock();
}
