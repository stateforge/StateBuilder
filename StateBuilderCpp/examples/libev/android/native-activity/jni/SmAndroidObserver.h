#ifndef SMANDROIDOBSERVER_H
#define SMANDROIDOBSERVER_H

#include <fsm/Definitions.h>
#include <fsm/IObserver.h>

using namespace fsm;

/**
SmAndroidObserver implements fsm:IObserver, it logs all messages from the state machine through the android log system
@author frederic heem
 */
class SmAndroidObserver : public IObserver
{
  public:
    
    static SmAndroidObserver& GetInstance();
    
    ~SmAndroidObserver();

   void OnTransitionBegin(const string &context,
                          const string &statePrevious,
                          const string &stateNext,
                          const string &transition);
    
    void OnTransitionEnd(const string &context,
                         const string &statePrevious,
                         const string &stateNext,
                         const string &transition);
                        
    void OnTimerStart(const string &context, const string &timer, long duration);
    void OnTimerStop(const string &context, const string &timer);
    void OnEntry(const string &context, const string &state);
    void OnExit(const string &context, const string &state);

    void OnProcessEventStart(const string &context, size_t remainingEvent);
    void OnProcessEventStop(const string &context, size_t remainingEvent);
    
  protected:
    SmAndroidObserver();
    // static class 
    static SmAndroidObserver *m_pObserver;
};

#endif
