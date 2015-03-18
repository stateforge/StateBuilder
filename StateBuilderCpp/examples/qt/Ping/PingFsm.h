// PingFsm.h has been generated automatically by StateBuilderCpp 2.0
// Executable: 
// file:/C:/Users/frederic/AppData/Local/StateForge/StateBuilderCpp/bin/StateBuilderCpp.exe
// Command line options: 
//  C:/Users/frederic/Documents/projects/StateForge/dev/StateBuilder/StateBuilderCpp/examples/qt/Ping/Ping.fsmcpp
// Date: 28-gen-2013 1.19.00

#ifndef PINGFSM_H
#define PINGFSM_H


// Includes
#include <cstring>
#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QTimerEvent>
#include <fsm/FsmAsync.h>

// Object classes forward declaration
namespace stateforge { 
  namespace network { 
    class Ping;
}}

namespace stateforge { namespace network { 
// Context classes forward declaration
class PingContext;
// Parallel classes forward declaration
// State classes forward declaration
class PingEndState;
class PingIdleState;
class PingSendPingAndWaitForReplyState;
class PingPingState;
}}// namespace stateforge::network



namespace stateforge { namespace network { 
// Context declaration for state Ping
class PingContext :  public QObject, public fsm::ContextAsync<PingPingState, PingContext, PingContext>, public fsm::ISchedule, public fsm::IManagement {
  Q_OBJECT

public:
  // Constructor
  PingContext(stateforge::network::Ping& ping, QObject *pParent = NULL);

  // Destructor
  virtual ~PingContext();

public slots:
  // Ping's events 
  void EvStart();
  void EvCancel();

  // Internal's events 
  void EvError();
  void EvPingReply();

public:
  // Timers declarations
  void TimerStartTimer(long duration);
  void TimerStopTimer();



  // Enter the initial state: walk the onEntry chain from the top state to the initial state.
  void EnterInitialState();

  // Leave the current state: walk the onExit chain from the current state to the top state.
  void LeaveCurrentState();

  // Wrap OnEnd()
  void OnEnd() {emit end();};

  stateforge::network::Ping& getPing(){return m_ping;};

private:
  stateforge::network::Ping& m_ping;

  // Ping's events 
  void EvStartSync();
  void EvCancelSync();

  // Internal's events 
  void EvErrorSync();
  void EvPingReplySync();

  // Timer variables declaration
  QTimer *m_pTimerTimer;

  // Timer's event declarations
private slots:
  void EvTimer();

  // Overrides virtual void QObject::timerEvent(QTimerEvent *pTimerEvent)
  virtual void timerEvent(QTimerEvent *pTimerEvent);

  // Overrides void ISchedule::Schedule(long);
  virtual void Schedule(long);


signals:
  void end();

};


// State PingPingState
class PingPingState : public fsm::State<PingContext, PingPingState> {
public:
  // Constructor
  PingPingState(const char* pName);

  // Singleton pattern
  static const PingPingState& GetInstance();

  // Destructor
  virtual ~PingPingState();

  // OnEntry and OnExit
  virtual void OnEntry(PingContext& context) const;
  virtual void OnExit(PingContext& context) const;

  // Events
  virtual void EvStart(PingContext &context) const ;
  virtual void EvCancel(PingContext &context) const ;
  virtual void EvError(PingContext &context) const ;
  virtual void EvPingReply(PingContext &context) const ;
  virtual void EvTimer(PingContext &context) const ;

};


// State PingIdleState
class PingIdleState : public PingPingState {
public:
  // Constructor
  PingIdleState(const char* pName);

  // Singleton pattern
  static const PingIdleState& GetInstance();

  // Destructor
  virtual ~PingIdleState();

  // OnEntry and OnExit
  virtual void OnEntry(PingContext& context) const;
  virtual void OnExit(PingContext& context) const;

  // Events
  virtual void EvStart(PingContext &context) const ;

};


// State PingSendPingAndWaitForReplyState
class PingSendPingAndWaitForReplyState : public PingPingState {
public:
  // Constructor
  PingSendPingAndWaitForReplyState(const char* pName);

  // Singleton pattern
  static const PingSendPingAndWaitForReplyState& GetInstance();

  // Destructor
  virtual ~PingSendPingAndWaitForReplyState();

  // OnEntry and OnExit
  virtual void OnEntry(PingContext& context) const;
  virtual void OnExit(PingContext& context) const;

  // Events
  virtual void EvCancel(PingContext &context) const ;
  virtual void EvPingReply(PingContext &context) const ;
  virtual void EvTimer(PingContext &context) const ;

};


// State PingEndState
class PingEndState : public PingPingState {
public:
  // Constructor
  PingEndState(const char* pName);

  // Singleton pattern
  static const PingEndState& GetInstance();

  // Destructor
  virtual ~PingEndState();

  // OnEntry and OnExit
  virtual void OnEntry(PingContext& context) const;
  virtual void OnExit(PingContext& context) const;

  // Events
  virtual void EvStart(PingContext &context) const ;
  virtual void EvCancel(PingContext &context) const ;
  virtual void EvError(PingContext &context) const ;
  virtual void EvPingReply(PingContext &context) const ;
  virtual void EvTimer(PingContext &context) const ;

};

}}// namespace stateforge::network

#endif //PINGFSM_H
