#ifndef LOCKBOOST_H
#define LOCKBOOST_H

#ifdef _MSC_VER
#pragma warning(push, 0)
#endif /*#ifdef _MSC_VER*/
#include <boost/thread/mutex.hpp>
#ifdef _MSC_VER
#pragma warning(pop)
#endif /*#ifdef _MSC_VER*/

#include <fsm/ILock.h>

class LockBoost : public fsm::ILock
{
public:
    virtual ~LockBoost(){};
    void Lock(void);
    void UnLock(void);
private:
    boost::mutex m_mutex;
};

#endif
