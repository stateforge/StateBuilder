#ifndef SMQTOBSERVER_H
#define SMQTOBSERVER_H

#include <fsm/Definitions.h>
#include <fsm/IObserver.h>

using namespace fsm;

/**
SmQtObserver implements fsm:Observer, it logs all messages from the fsm to QsLog
 */
class SmQtObserver : public IObserver
{
  public:
    
    static SmQtObserver& GetInstance();
    
    ~SmQtObserver();

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

    void OnProcessEventStart(const char *pContextName, size_t uiRemainingEvent);
    void OnProcessEventStop(const char *pContextName, size_t uiRemainingEvent);
    
  protected:
    SmQtObserver();
    // static class 
    static SmQtObserver *m_pObserver;
};

#endif
