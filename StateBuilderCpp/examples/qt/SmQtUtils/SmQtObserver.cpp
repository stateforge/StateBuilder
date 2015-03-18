#include <QsLog.h>
#include <QsLogDest.h>
#include <QtCore/QCoreApplication>
#include <QDir>

#include "SmQtObserver.h"

using namespace std;

SmQtObserver *SmQtObserver::m_pObserver = NULL;

SmQtObserver::SmQtObserver()
: IObserver()
{
}

SmQtObserver::~SmQtObserver()
{
}

SmQtObserver& SmQtObserver::GetInstance()
{
    if(m_pObserver == NULL){
        m_pObserver = new SmQtObserver();
    }
    return *m_pObserver;
}

void SmQtObserver::OnTransitionBegin(const char *pContextName,
                                    const char *pStatePrevious,
                                    const char *pStateNext,
                                    const char *pTransitionName) 
{
    QLOG_DEBUG() << pContextName << 
                    ", from " << pStatePrevious << 
                    " to " << pStateNext << 
                    ", event " << pTransitionName;
};

void SmQtObserver::OnTransitionEnd(const char *pContextName,
                                   const char *pStatePrevious,
                                   const char *pStateNext,
                                   const char *pTransitionName) 
{
    QLOG_DEBUG() << pContextName << 
                    ", from " << pStatePrevious << 
                    " to " << pStateNext << 
                    ", event " << pTransitionName;
};

void SmQtObserver::OnTimerStart(const char *pContextName, const char *pTimerName, long duration)
{
    QLOG_DEBUG() << pContextName << ", timer " << pTimerName << " duration " << duration << " msec";
}

void SmQtObserver::OnTimerStop(const char *pContextName, const char *pTimerName)
{
    QLOG_DEBUG() << pContextName << ", timer " << pTimerName;
}

void SmQtObserver::OnEntry(const char *pContextName, const char *pStateName)
{
    QLOG_DEBUG() << pContextName << pStateName;
}

void SmQtObserver::OnExit(const char *pContextName, const char *pStateName)
{
    QLOG_DEBUG() << pContextName << ", state " << pStateName;
}

void  SmQtObserver::OnProcessEventStart(const char *pContextName, size_t remainingEvent)
{
    QLOG_DEBUG() << pContextName << ", remaining event(s) " << remainingEvent;
};

void SmQtObserver::OnProcessEventStop(const char *pContextName, size_t remainingEvent)
{
    QLOG_DEBUG() << pContextName << ", remaining event(s) " << remainingEvent;
}