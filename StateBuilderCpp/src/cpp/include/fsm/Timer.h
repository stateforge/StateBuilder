//------------------------------------------------------------------------------
// <copyright file="Timer.h" company="StateForge">
//      Copyright (c) 2010-2013 StateForge.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#ifndef _FSM_TIMER_H_
#define _FSM_TIMER_H_

#include "fsm/Definitions.h"

#ifdef FSM_TIMERS_INTERNAL

// Includes
#include <string>
#include <map>
#include <set>

#ifdef _MSC_VER
#pragma warning(push, 0)
#endif /*#ifdef _MSC_VER*/
#include "boost/date_time/posix_time/posix_time.hpp"
#ifdef _MSC_VER
#pragma warning(pop)
#endif /*#ifdef _MSC_VER*/

// Namespace
using namespace std;

using namespace boost::posix_time;

//Forward declaration
template<class Fsm>
class FsmTimer;

template<class Fsm>
class TimerSort : public std::binary_function<FsmTimer<Fsm>*, FsmTimer<Fsm>*, bool> 
{
  public:
  bool operator()(FsmTimer<Fsm>* pTimerLeft, FsmTimer<Fsm>* pTimerRight) const {
    //cout << "FsmTimer<Fsm>::operator< " << pTimerLeft->GetName() << " " << pTimerRight->GetName() << endl; 
    if(pTimerLeft->GetExpiredTime() < pTimerRight->GetExpiredTime()){
      //cout << "FsmTimer<Fsm>::operator< TRUE" << endl; 
      return true;
    } else {
      //cout << "FsmTimer<Fsm>::operator< FALSE" << endl;
      return false;
    };
  };
};

// Class declaration
/** FsmTimerList manages a list of FsmTimer.
    FsmTimerList is responsible of calling the FsmTimer's expired method via the Process method
    FsmTimerList deletes all FsmTimers in its destrutor.
    4.Create a TimerException class in case of FsmTimerList::Start or FsmTimerList::Stop cannot find the given timer

 */

template<class Fsm>
class FsmTimerList {
  public:
    /**@name Construction */
    //@{
    /**Create a fsm timer list
     */
    FsmTimerList();

    /**Destroy a fsm timer list
     */
    virtual ~FsmTimerList();
    //@}
    /**@name operation */
    //@{
    /** Start a timer, return false if the timer cannot be found.
    */
    bool Start(
      const string &name,  ///< name of the timer to start
      long lDuration ///< duration in msec
    );

    /**
      * Stop a timer
      */
    bool Stop(
      const string &name     ///< name of the timer to stop
    );

    /** Find expired timers and invokes their callback
      * This function has to be called on a regular basis
      * @param nextTick wait nextTick milliseconds to invoke ProcessEvent again
      */
    void Process(long &nextTick);
    
    /** TimerAdd adds a FsmTimer to the list of timer, normally invoked inside the FsmTimer Constructor
      * There is no check for duplicated timers
      */
    void TimerAdd(
      FsmTimer<Fsm> *pFsmTimer ///< the timer to add
    );

    //@}
  protected:

    /** Delete all timers
      */
    void DeleteAll(void);  
    
    typedef std::map<std::string, FsmTimer<Fsm>* > TimerMap;
    typedef typename TimerMap::iterator TimerMapIterator;
    typedef std::set<FsmTimer<Fsm>*, TimerSort<Fsm> > TimerSet;
    typedef typename TimerSet::iterator TimerSetIterator;
    TimerMap m_timerMap;
    TimerSet m_timerRunningSet;  
    
};
    
/** @class FsmTimer
  * @brief FsmTimer represents a single timer.
 */

template<class Fsm>
class FsmTimer {
    typedef void(Fsm::*Method)();
  public:

    typedef enum {
      STOPPED,
      STARTED
  } FSM_TIMER_STATE;

    /**@name Construction */
    //@{

    /** default constructor
     */
    FsmTimer();

    /**Create a fsm timer.
      */
    FsmTimer(const string &name,            ///< name of this timer
             FsmTimerList<Fsm> *pTimerList, ///< this timer belongs the given FsmTimerList
             Fsm *pFsm,                     ///< the class's callback
             Method method                  ///< the callback to invoked when the timer expires
            );

    /**Copy constructor.
     */
    FsmTimer(const FsmTimer & fsmTimer);


    /**Destroy a fsm timer
     */
    virtual ~FsmTimer();
    //@}

    /**@name operation */
    //@{

    /** Start the timer for the specfic duration in msec
    *   The timer is restarted if it was already running
    */
    void Start(
      long lDuration ///< duration in msec
    );

    /**
      * Stop the timer
     */

    void Stop();

    //@}

    /**@name time operation */
    //@{

    /** return the duration until expiration.
      */
    long expiredIn();
    //@}
    /**@name menber access */
    //@{

    /** get the timer's state
      */
    FSM_TIMER_STATE GetState() const {
      return m_eState;
    };

    /** get the timer's name
      */
    const string& GetName() const {
      return m_name;
    };

    /** get the timer's list
     */
    FsmTimerList<Fsm>* GetTimerList() const {
      return m_pTimerList;
    };

    /** get the timer's fsm
     */
    Fsm* GetFsm() const {
      return m_pFsm;
    };

    /** get the timer's method
     */
    Method GetMethod() const {
      return m_method;
    };

    /** get the start time
      */
    const ptime& GetStartTime() const {
      return m_startTime;
    };

    const ptime& GetExpiredTime() const {
      return m_expiredTime;
    };

    //@}

  protected:

    /** set the timer's state
     */
    void SetState(FSM_TIMER_STATE eState) {
      m_eState = eState;
    };

    /** Invokes the class's method if the timer has expired,
        normally called by the FsmTimerList
      */
    void Invoke();
    
    FSM_TIMER_STATE     m_eState;
    string              m_name;
    FsmTimerList<Fsm>  *m_pTimerList;
    Fsm                *m_pFsm;
    Method              m_method;
    time_duration       m_duration;
    ptime               m_startTime;
    ptime               m_expiredTime;
    friend class FsmTimerList<Fsm>;
};

#endif //#ifdef FSM_TIMERS_INTERNAL

#endif // __FSM_TIMER_H


// End of File ///////////////////////////////////////////////////////////////
