// BlinkFsm.h has been generated automatically by StateBuilderCpp 1.3
// Executable: 
// file:/C:/Users/frederic/AppData/Local/StateForge/StateBuilderCpp/bin/StateBuilderCpp.exe
// Command line options: 
//  C:\Users\frederic\Documents\projects\StateForge\dev\StateBuilder\StateBuilderCpp\examples\libev\android\native-activity\jni\Blink.fsmcpp
// Date: 20-gen-2012 12.10.37

#ifndef BLINKFSM_H
#define BLINKFSM_H


// Includes
#include <ev++.h>
#include <fsm/FsmAsync.h>

// Object classes forward declaration
class Blink;
// Context classes forward declaration
class BlinkContext;
// Parallel classes forward declaration
// State classes forward declaration
class BlinkEndState;
class BlinkTopState;
class BlinkIdleState;
class BlinkRunningState;

// Context declaration for state Top
class BlinkContext : public fsm::ContextAsync<BlinkTopState, BlinkContext, BlinkContext>, public fsm::ISchedule {
public:
  // Constructor
  BlinkContext(Blink& blink, ev::default_loop &loop);

  // BlinkEvent's events 
  void EvStart();
  void EvStop();

  // Timers's events 

  // Timers declarations
  void TimerStartTimerRetry(long duration);
  void TimerStopTimerRetry();



  // Enter the initial state: walk the onEntry chain from the top state to the initial state.
  void EnterInitialState();

  // Leave the current state: walk the onExit chain from the current state to the top state.
  void LeaveCurrentState();

  Blink& getBlink(){return m_blink;};

private:
  Blink& m_blink;

  // BlinkEvent's events 
  void EvStartSync();
  void EvStopSync();

  // Timers's events 

  // Timer variables declaration
  ev::timer m_timerProcess;
  ev::timer m_timerTimerRetry;

  // Timer's event declarations
  void EvTimerRetry();

  // process is called by the libev loop
  void process();

  // Overrides void ISchedule::Schedule(long);
  virtual void Schedule(long);


};


// State BlinkTopState
class BlinkTopState : public fsm::State<BlinkContext, BlinkTopState> {
public:
  // Constructor
  BlinkTopState(const string &name);

  // Singleton pattern
  static const BlinkTopState& GetInstance();

  // Destructor
  virtual ~BlinkTopState();

  // OnEntry and OnExit
  virtual void OnEntry(BlinkContext& context) const;
  virtual void OnExit(BlinkContext& context) const;

  // Events
  virtual void EvStart(BlinkContext &context) const ;
  virtual void EvStop(BlinkContext &context) const ;
  virtual void EvTimerRetry(BlinkContext &context) const ;

protected:
  // static class 
  static BlinkTopState const *m_pTop;

};


// State BlinkIdleState
class BlinkIdleState : public BlinkTopState {
public:
  // Constructor
  BlinkIdleState(const string &name);

  // Singleton pattern
  static const BlinkIdleState& GetInstance();

  // Destructor
  virtual ~BlinkIdleState();

  // OnEntry and OnExit
  virtual void OnEntry(BlinkContext& context) const;
  virtual void OnExit(BlinkContext& context) const;

  // Events
  virtual void EvStart(BlinkContext &context) const ;

protected:
  // static class 
  static BlinkIdleState const *m_pIdle;

};


// State BlinkRunningState
class BlinkRunningState : public BlinkTopState {
public:
  // Constructor
  BlinkRunningState(const string &name);

  // Singleton pattern
  static const BlinkRunningState& GetInstance();

  // Destructor
  virtual ~BlinkRunningState();

  // OnEntry and OnExit
  virtual void OnEntry(BlinkContext& context) const;
  virtual void OnExit(BlinkContext& context) const;

  // Events
  virtual void EvTimerRetry(BlinkContext &context) const ;

protected:
  // static class 
  static BlinkRunningState const *m_pRunning;

};


// State BlinkEndState
class BlinkEndState : public BlinkTopState {
public:
  // Constructor
  BlinkEndState(const string &name);

  // Singleton pattern
  static const BlinkEndState& GetInstance();

  // Destructor
  virtual ~BlinkEndState();

  // OnEntry and OnExit
  virtual void OnEntry(BlinkContext& context) const;
  virtual void OnExit(BlinkContext& context) const;

  // Events
  virtual void EvStart(BlinkContext &context) const ;
  virtual void EvStop(BlinkContext &context) const ;
  virtual void EvTimerRetry(BlinkContext &context) const ;

protected:
  // static class 
  static BlinkEndState const *m_pEnd;

};

#endif //BLINKFSM_H
