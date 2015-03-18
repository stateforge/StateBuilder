//------------------------------------------------------------------------------
// <copyright file="Timer.hpp" company="StateForge">
//      Copyright (c) 2010-2013 StateForge.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------
#ifndef _FSM_TIMER_HPP_
#define _FSM_TIMER_HPP_

#ifdef FSM_TIMERS_INTERNAL

#include <cassert>
#include <fsm/Timer.h>

/** FsmTimerList
 */

template <class Fsm>
FsmTimerList<Fsm>::FsmTimerList()
{
  //std::cerr <<  "FsmTimerList ctor" << endl;
}

template <class Fsm>
FsmTimerList<Fsm>::~FsmTimerList()
{
  //std::cerr <<  "FsmTimerList dtor" << endl;
  DeleteAll();
}

template <class Fsm>
bool FsmTimerList<Fsm>::Start(const string &name, long lDuration)
{
  FsmTimer<Fsm>* pTimer = m_timerMap[name];
  if(pTimer){
    cerr << "FsmTimerList<Fsm>::Start " <<  name << " had " << m_timerRunningSet.size() << " timer(s)" << endl;
    if(pTimer->GetState() == FsmTimer<Fsm>::STOPPED){
      m_timerRunningSet.insert(pTimer);
    } else {
      cerr << "Timer " << name << " is already running" << endl;
    }
    cerr << "FsmTimerList<Fsm>::Start " <<  name << " has now " <<  m_timerRunningSet.size() << " timer(s)" << endl;
    pTimer->Start(lDuration);
    return true;
  } else {
    cerr << "FsmTimerList<Fsm>::Start timer  " << name << " do not exist" << endl;
    assert(0);
    return false;
  }
}

template <class Fsm>
bool FsmTimerList<Fsm>::Stop(const string &name)
{
  FsmTimer<Fsm>* pTimer = m_timerMap[name];
  if(pTimer){
    cout << "FsmTimerList<Fsm>::Stop() timer " << name << " had " << m_timerRunningSet.size() << " timer(s)" << endl;
    pTimer->Stop();
    m_timerRunningSet.erase(pTimer);
    cout << "FsmTimerList<Fsm>::Stop() timer " << name << " has now " <<  m_timerRunningSet.size() << " timer(s)" << endl;
    return true;
  } else {
    cerr << "FsmTimerList<Fsm>::Stop() timer " << name << " does not exist" << endl;
    assert(0);
    return false;
  }
};

template <class Fsm>
void FsmTimerList<Fsm>::Process(long &nextTick)
{
  cout << "FsmTimerList::Process has " <<  m_timerRunningSet.size() << " timer(s) in total" << endl;

  for (TimerSetIterator it = m_timerRunningSet.begin(); it != m_timerRunningSet.end();) {
    FsmTimer<Fsm>* pTimer = (*it);
    long nextTickTimer = pTimer->expiredIn();
    if(nextTickTimer < 0){
      size_t timersRunning = m_timerRunningSet.size();
      if(timersRunning == 1){
          m_timerRunningSet.clear();
          //cout << "FsmTimerList::Process no more running timers " << endl;
      } else {
        m_timerRunningSet.erase(it);
      }

      //cout << "FsmTimerList::Process timer invoking " << pTimer->GetName() << " delay " << nextTickTimer << endl;
      pTimer->Invoke(); 
      //cout << "FsmTimerList::Process timer invoked " << pTimer->GetName()
      //    << " has now " << m_timerRunningSet.size() << " timers, before " << timersRunning << endl;

      it = m_timerRunningSet.begin();
    } else {
      cout << "FsmTimerList::Process timer " <<  pTimer->GetName() << " expires in " << nextTickTimer << endl;
      nextTick = (std::min)(nextTickTimer, nextTick);
      break;
    }
  }
  //cout << "FsmTimerList::Process has now " <<  m_timerRunningSet.size() << " timer(s) nextTick " << nextTick << endl;
}

template <class Fsm>
void FsmTimerList<Fsm>::TimerAdd(FsmTimer<Fsm> *pFsmTimerToAdd)
{
  FsmTimer<Fsm> *pFsmTimer = m_timerMap[pFsmTimerToAdd->GetName()];
  if(pFsmTimer){
    cerr << "FsmTimerList::TimerAdd " <<  pFsmTimer->GetName() << " already exists" << endl;
    delete pFsmTimer;
  }
  m_timerMap[pFsmTimerToAdd->GetName()] = pFsmTimerToAdd;
};

template <class Fsm>
void FsmTimerList<Fsm>::DeleteAll(void)
{
  for (TimerMapIterator it = m_timerMap.begin(); it != m_timerMap.end(); ++it ) {
    delete it->second;
  }
  m_timerMap.clear(); 
  m_timerRunningSet.clear();
};

/** FsmTimer
 */
template <class Fsm>
FsmTimer<Fsm>::FsmTimer():
      m_eState(STOPPED)
    , m_pTimerList(NULL)
    , m_pFsm(NULL)
    , m_method(0)
    , m_startTime(ptime(pos_infin))
    , m_duration()  
{};

template <class Fsm>
FsmTimer<Fsm>::FsmTimer(const string &name,
                        FsmTimerList<Fsm> *pTimerList,
                        Fsm *pFsm,
                        Method method):
    m_eState(STOPPED),
    m_name(name),
    m_pTimerList(pTimerList),
    m_pFsm(pFsm),
    m_method(method)
{
  if (m_pTimerList != NULL) {
    m_pTimerList->TimerAdd(this);
  }
};

template <class Fsm>
FsmTimer<Fsm>::FsmTimer(const FsmTimer & fsmTimer) :
    m_eState(fsmTimer.GetState()),
    m_name(fsmTimer.GetName()),
    m_pTimerList(fsmTimer.GetTimerList()),
    m_pFsm(fsmTimer.GetFsm()),
    m_method(fsmTimer.GetMethod())
{}

template <class Fsm>
FsmTimer<Fsm>::~FsmTimer()
{};

template <class Fsm>
void FsmTimer<Fsm>::Start(long lDuration)
{
  cout << "FsmTimer Start" << lDuration << endl;
  m_duration = milliseconds(lDuration);
  m_startTime = microsec_clock::local_time();
  m_expiredTime = m_startTime + m_duration;
  SetState(STARTED);
};

template <class Fsm>
void FsmTimer<Fsm>::Stop()
{
  SetState(STOPPED);
};

template <class Fsm>
void FsmTimer<Fsm>::Invoke()
{
  assert(m_pFsm);
  assert(m_method);
  //Set state before invoking the method
  //TODO do not stop for autoshot timer
  SetState(STOPPED);
  (m_pFsm->*m_method)();
};

template <class Fsm>
long FsmTimer<Fsm>::expiredIn()
{
  time_duration timeDuration = m_expiredTime - microsec_clock::local_time();
  return timeDuration.total_milliseconds();
};

#endif //#ifdef FSM_TIMERS_INTERNAL

#endif // #ifndef _FSM_TIMER_HPP_

// End of File ///////////////////////////////////////////////////////////////
