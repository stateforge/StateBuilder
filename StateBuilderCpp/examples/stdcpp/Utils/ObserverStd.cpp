
#include <iostream>
#include "ObserverStd.h"

using namespace std;

namespace fsm {
  
ObserverStd *ObserverStd::m_pObserverStd = NULL;

ObserverStd::ObserverStd()
  : IObserver()
{
}

ObserverStd::~ObserverStd()
{
}

ObserverStd& ObserverStd::GetInstance()
{
  if(m_pObserverStd == NULL){
      m_pObserverStd = new ObserverStd();
  }
  return *m_pObserverStd;
}

void ObserverStd::OnTransitionBegin(const char *pContextName,
                               const char *pStatePrevious,
                               const char *pStateNext,
                               const char *pTransitionName) 
{
    cout << pContextName << ": transition begins from state " << pStatePrevious << " to " << pStateNext << ", event " << pTransitionName << endl;
}

void ObserverStd::OnTransitionEnd(const char *pContextName,
                                const char *pStatePrevious,
                                const char *pStateNext,
                                const char *pTransitionName) 
{
    cout << pContextName << ": transition has ended from state " << pStatePrevious << " to " << pStateNext << ", event " << pTransitionName << endl;
}

void ObserverStd::OnTimerStart(const char *pContextName, const char *pTimerName, long duration)
{
    cout << pContextName << ": start timer " << pTimerName << " for " << duration << " msec" << endl;
}

void ObserverStd::OnTimerStop(const char *pContextName, const char *pTimerName)
{
    cout << pContextName << ": stop timer " << pTimerName << endl;
}

void ObserverStd::OnEntry(const char *pContextName, const char *pStateName)
{
  cout << pContextName << ": entering in state " << pStateName << endl;
}

void ObserverStd::OnExit(const char *pContextName, const char *pStateName)
{
  cout << pContextName << ": leaving from state " << pStateName << endl;
}

void  ObserverStd::OnProcessEventStart(const char *pContextName, size_t remainingEvent)
{
    cout << pContextName << ": start processing, remaining event(s) " << remainingEvent << endl;
}

void ObserverStd::OnProcessEventStop(const char *pContextName, size_t remainingEvent)
{
    cout << pContextName << ": stop processing, remaining event(s) " << remainingEvent << endl;
}

} // namespace fsm {
