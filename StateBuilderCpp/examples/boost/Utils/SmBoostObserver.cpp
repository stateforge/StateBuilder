

#ifdef _MSC_VER
#pragma warning(push, 0)
#endif /*#ifdef _MSC_VER*/
#include <logging.hpp>
#ifdef _MSC_VER
#pragma warning(pop)
#endif /*#ifdef _MSC_VER*/

#include "SmBoostObserver.h"

using namespace std;

SmBoostObserver *SmBoostObserver::m_pObserver = NULL;

SmBoostObserver::SmBoostObserver()
: IObserver()
{
}

SmBoostObserver::~SmBoostObserver()
{
}

SmBoostObserver& SmBoostObserver::GetInstance()
{
    if(m_pObserver == NULL){
        m_pObserver = new SmBoostObserver();
    }
    return *m_pObserver;
}

void SmBoostObserver::OnTransitionBegin(const char *pContextName,
                                        const char *pStatePrevious,
                                        const char *pStateNext,
                                        const char *pTransitionName) 
{
    BOOST_LOG_L4(pContextName << ", from " << pStatePrevious << " to " << pStateNext << ", event " << pTransitionName);;
};

void SmBoostObserver::OnTransitionEnd(const char *pContextName,
                                      const char *pStatePrevious,
                                      const char *pStateNext,
                                      const char *pTransitionName) 
{
    BOOST_LOG_L4(pContextName << ", from " << pStatePrevious << " to " << pStateNext << ", event " << pTransitionName);
};

void SmBoostObserver::OnTimerStart(const char *pContextName, const char *pTimerName, long duration)
{
    BOOST_LOG_L4(pContextName << ", timer " << pTimerName << ", duration " << duration << " msec");;
}

void SmBoostObserver::OnTimerStop(const char *pContextName, const char *pTimerName)
{
    BOOST_LOG_L4(pContextName << ", timer " << pTimerName);;
}

void SmBoostObserver::OnEntry(const char *pContextName, const char *pStateName)
{
    BOOST_LOG_L4(pContextName << ", state " << pStateName);;
}

void SmBoostObserver::OnExit(const char *pContextName, const char *pStateName)
{
    BOOST_LOG_L4(pContextName << ", state " << pStateName);;
}

void  SmBoostObserver::OnProcessEventStart(const char *pContextName, size_t remainingEvent)
{
    BOOST_LOG_L4(pContextName << ", remaining event(s) " << remainingEvent);;
};

void SmBoostObserver::OnProcessEventStop(const char *pContextName, size_t remainingEvent)
{
    BOOST_LOG_L4(pContextName << ", remaining event(s) " << remainingEvent);;
};
