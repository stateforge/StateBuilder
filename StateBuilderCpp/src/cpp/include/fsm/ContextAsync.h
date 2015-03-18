//------------------------------------------------------------------------------
// <copyright file="ContextAsync.h" company="StateForge">
//      Copyright (c) 2010-2013 StateForge.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------
#ifndef FSM_CONTEXTASYNC_H
#define FSM_CONTEXTASYNC_H

#include <list>
#include <fsm/Definitions.h>
#include <fsm/Event.h>
#include <fsm/Timer.h>

#include <fsm/IContextAsync.h>
#include <fsm/ISchedule.h>

namespace fsm {

/** @class ContextAsync
  * @brief ContextAsync extends Context by adding a list of timers and a list of pending events
  */
template <typename TState, typename TContext, typename TContextParent>
class ContextAsync : public Context<TState, TContextParent> , public IContextAsync {
    #ifdef FSM_TIMERS_INTERNAL
    typedef FsmTimerList<TContext> TimerList;
    #endif //#ifdef FSM_TIMERS_INTERNAL
    
    typedef std::list<IEvent<TContext> * > EventList;

public:

     
    /** Main constructor
      * @param schedule the schedule functor
      * @param pContextParent the parent context
      */
    ContextAsync(ISchedule &schedule, TContextParent *pContextParent = NULL);

    /** Virtual destructor
      * delete all pending events
      */
    virtual ~ContextAsync();

    #ifdef FSM_TIMERS_INTERNAL
    /** start a timer
      * @param name timer's name
      * @param lDuration timer's duration in milliseconds
      * @return true if the timer has been started
      */
    virtual bool TimerStart(const std::string &name, long lDuration);

    /** stop a timer
      * @param name timer's name
      * @return true if the timer has been stopped
      */
    virtual bool TimerStop(const std::string &name);

    /** Get the timer's list
      */
    TimerList& GetTimerList() {
      return m_timerList;
    }

    #endif //#ifdef FSM_TIMERS_INTERNAL

    /** Process all events that are stored in the queues:
      * - Timers
      * - local events
      * - substates events
      * @param nextTick wait nextTick milliseconds to invoke ProcessEvent again
      * @return ProcessReturn
      */
    virtual ProcessReturn ProcessEvent(long &nextTick);
    

#ifndef STATEFORGE_NO_PERSISTENCE
    void Serialize(std::ostream &os) const {
        Context<TState, TContextParent>::SerializeState(os);
        SerializeEvent(os);
        Context<TState, TContextParent>::SerializeParallel(os);
    }
   
    void UnSerialize(std::istream &is) {
        Context<TState, TContextParent>::UnSerializeState(is);
        UnSerializeEvent(is);
        Context<TState, TContextParent>::UnSerializeParallel(is);
    }

#endif /*STATEFORGE_NO_PERSISTENCE*/

  protected:

    /** Get the event list
      */
    EventList& GetEventList() {
      return m_eventList;
    }

    #ifdef FSM_TIMERS_INTERNAL
    TimerList m_timerList; ///< List of timers
    #endif //#ifdef FSM_TIMERS_INTERNAL

    EventList m_eventList; ///< List of events

    ISchedule &m_schedule;

    virtual void PushBackEvent(int /*eventId*/){}

#ifndef STATEFORGE_NO_PERSISTENCE

    void SerializeEvent(std::ostream &os) const {
        size_t eventListSize = m_eventList.size();
        os << eventListSize << std::endl;
        typename std::list<IEvent<TContext> * >::const_iterator it = m_eventList.begin();
        for(; it != m_eventList.end(); ++it){
            IEvent<TContext> *pEvent = *it;
            int eventId = pEvent->GetEventId();
            // event with parameter and timer cannot be serialized
            assert(eventId > 0);
            os << eventId << std::endl;
        }
    }

    void UnSerializeEvent(std::istream &is) {
        size_t eventListSize;
        is >> eventListSize;
        for(size_t i = 0; i < eventListSize; i++){
            int eventId;
            is >> eventId;
            // Internal error here, event with parameter and timer cannot be serialized
            assert(eventId > 0);
            PushBackEvent(eventId);
        }
    }

#endif /*STATEFORGE_NO_PERSISTENCE*/

  private:
    /** empty copy contructor
     */
    ContextAsync(const ContextAsync&);

    /** empty copy assignment contructor
     */
    ContextAsync& operator=(const ContextAsync&);
};

}

#endif // namespace fsm {
