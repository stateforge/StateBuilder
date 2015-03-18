#ifndef SMBOOSTOBSERVER_H
#define SMBOOSTOBSERVER_H

#include <fsm/Definitions.h>
#include <fsm/IObserver.h>

/**
SmBoostObserver implements fsm:IObserver, it logs all messages from the state machine to bootlite
@author frederic heem
 */
class SmBoostObserver : public fsm::IObserver
{
  public:
    
    static SmBoostObserver& GetInstance();
    
    ~SmBoostObserver();

   void OnTransitionBegin(const char *pContextName,
                          const char *pStatePrevious,
                          const char *pStateNext,
                          const char *pTransitionName);
    
    void OnTransitionEnd(const char *pContextName,
                         const char *pStatePrevious,
                         const char *pStateNext,
                         const char *pTransitionName);
                        
    void OnTimerStart(const char *pContextName, const char *pTimerName, long duration);
    void OnTimerStop(const char *pContextName, const char *pTimerName);
    void OnEntry(const char *pContextName, const char *pStateName);
    void OnExit(const char *pContextName, const char *pStateName);

    void OnProcessEventStart(const char *pContextName, size_t remainingEvent);
    void OnProcessEventStop(const char *pContextName, size_t remainingEvent);
    
  protected:
    SmBoostObserver();
    // static class 
    static SmBoostObserver *m_pObserver;
};

#endif
