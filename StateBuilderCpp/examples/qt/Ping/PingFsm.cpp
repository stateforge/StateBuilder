// PingFsm.cpp has been generated automatically by StateBuilderCpp 2.0
// Executable: 
// file:/C:/Users/frederic/AppData/Local/StateForge/StateBuilderCpp/bin/StateBuilderCpp.exe
// Command line options: 
//  C:/Users/frederic/Documents/projects/StateForge/dev/StateBuilder/StateBuilderCpp/examples/qt/Ping/Ping.fsmcpp
// Date: 28-gen-2013 1.19.00

// Includes
#include <fsm/FsmAsync.hpp>
#include <Ping.h>
#include "PingFsm.h"



namespace stateforge { namespace network { 
// Disable some MS compiler warnings
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4355) // 'this' : used in base member initialiser list
#pragma warning(disable:4189) // local variable is initialised but not referenced
#pragma warning(disable:4100) // unreferenced formal parameter
#endif

// Disable some GCC compiler warnings
#if ((__GNUC__ * 100) + __GNUC_MINOR__) >= 402
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

// State Instantiation
static const PingPingState kPingPingState("Ping");
static const PingIdleState kPingIdleState("Idle");
static const PingSendPingAndWaitForReplyState kPingSendPingAndWaitForReplyState("SendPingAndWaitForReply");
static const PingEndState kPingEndState("End");

/**************************************************************
  * Contexts implementations  
  *************************************************************/
/**************************************************************
  * Context PingContext
  *************************************************************/
PingContext::PingContext(stateforge::network::Ping& ping, QObject *pParent) : 
  QObject(pParent), 
  fsm::ContextAsync<PingPingState, PingContext, PingContext>(*this, NULL)
  , m_ping(ping)
{
  SetName("PingContext");
  SetManagement(this);

  // Timers registration
  m_pTimerTimer = new QTimer(this);
  m_pTimerTimer->setSingleShot(true);
  QObject::connect(m_pTimerTimer, SIGNAL(timeout()), this, SLOT(EvTimer()));

  SetInitialState(PingIdleState::GetInstance());
};

PingContext::~PingContext(){}

void PingContext::EnterInitialState()
{
  fsm::WalkChainEntry<PingContext, PingPingState>(*this, &PingPingState::GetInstance() , &GetState());
};

void PingContext::LeaveCurrentState()
{
  fsm::WalkChainExit<PingContext, PingPingState>(*this, &GetState(), &PingPingState::GetInstance());
};

// Ping synchronous events 
void PingContext::EvStartSync()
{
  GetState().EvStart(*this);
};

void PingContext::EvCancelSync()
{
  GetState().EvCancel(*this);
};

// Internal synchronous events 
void PingContext::EvErrorSync()
{
  GetState().EvError(*this);
};

void PingContext::EvPingReplySync()
{
  GetState().EvPingReply(*this);
};

// Timer's event implementation
void PingContext::EvTimer()
{
  GetState().EvTimer(*this);
};



// Timer start implementation
void PingContext::TimerStartTimer(long duration)
{
  if(m_pObserver){
    m_pObserver->OnTimerStart("PingContext", "Timer", duration);
  }
  m_pTimerTimer->start(duration);
}


// Timer stop implementation
void PingContext::TimerStopTimer()
{
  if(m_pObserver){
    m_pObserver->OnTimerStop("PingContext", "Timer");
  }
  m_pTimerTimer->stop();
}

void PingContext::timerEvent(QTimerEvent *pTimerEvent)
{
  killTimer(pTimerEvent->timerId());
  long nextTick = 0;
  ProcessEvent(nextTick);
}

void PingContext::Schedule(long nextTick)
{
  startTimer(nextTick);
}

/**************************************************************
  * States implementations  
 **************************************************************/
/**************************************************************
 * State implementation for PingPingState
 **************************************************************/

// State Constructor
PingPingState::PingPingState(const char* pName) : fsm::State<PingContext, PingPingState>(pName)
{
};

// State Destructor
PingPingState::~PingPingState()
{
};

// State GetInstance
const PingPingState& PingPingState::GetInstance()
{
  return kPingPingState;
};

// OnEntry
void PingPingState::OnEntry(PingContext &context) const
{
  // OnEntry for state PingPingState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnEntry(context.GetName(), PingPingState::GetInstance().GetName());
  }
};

// OnExit
void PingPingState::OnExit(PingContext &context) const
{
  // OnExit for state PingPingState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnExit(context.GetName(), PingPingState::GetInstance().GetName());
  }
};

// Events implementation for state  PingPingState
void PingPingState::EvStart(PingContext &context) const
{
  // 0 transition(s)
}

void PingPingState::EvCancel(PingContext &context) const
{
  // 0 transition(s)
}

void PingPingState::EvError(PingContext &context) const
{
  // 0 transition(s)
}

void PingPingState::EvPingReply(PingContext &context) const
{
  // 0 transition(s)
}

void PingPingState::EvTimer(PingContext &context) const
{
  // 0 transition(s)
}


/**************************************************************
 * State implementation for PingIdleState
 **************************************************************/

// State Constructor
PingIdleState::PingIdleState(const char* pName) : PingPingState(pName)
{
  m_pStateParent = &PingPingState::GetInstance();
};

// State Destructor
PingIdleState::~PingIdleState()
{
};

// State GetInstance
const PingIdleState& PingIdleState::GetInstance()
{
  return kPingIdleState;
};

// OnEntry
void PingIdleState::OnEntry(PingContext &context) const
{
  // OnEntry for state PingIdleState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnEntry(context.GetName(), PingIdleState::GetInstance().GetName());
  }
};

// OnExit
void PingIdleState::OnExit(PingContext &context) const
{
  // OnExit for state PingIdleState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnExit(context.GetName(), PingIdleState::GetInstance().GetName());
  }
};

// Events implementation for state  PingIdleState
void PingIdleState::EvStart(PingContext &context) const
{
  // 1 transition(s)
  stateforge::network::Ping& ping = context.getPing();
  context.SetTransitionName("EvStart");
  fsm::ProcessTransitionPre<PingContext, PingPingState>(context, &PingSendPingAndWaitForReplyState::GetInstance());
  // No action to do
  fsm::ProcessTransitionPost<PingContext, PingPingState>(context, &PingSendPingAndWaitForReplyState::GetInstance());
  return;
}



/**************************************************************
 * State implementation for PingSendPingAndWaitForReplyState
 **************************************************************/

// State Constructor
PingSendPingAndWaitForReplyState::PingSendPingAndWaitForReplyState(const char* pName) : PingPingState(pName)
{
  m_pStateParent = &PingPingState::GetInstance();
};

// State Destructor
PingSendPingAndWaitForReplyState::~PingSendPingAndWaitForReplyState()
{
};

// State GetInstance
const PingSendPingAndWaitForReplyState& PingSendPingAndWaitForReplyState::GetInstance()
{
  return kPingSendPingAndWaitForReplyState;
};

// OnEntry
void PingSendPingAndWaitForReplyState::OnEntry(PingContext &context) const
{
  // OnEntry for state PingSendPingAndWaitForReplyState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnEntry(context.GetName(), PingSendPingAndWaitForReplyState::GetInstance().GetName());
  }
  stateforge::network::Ping& ping = context.getPing();
  // 2 action(s) to do
  ping.send();
  context.TimerStartTimer(ping.getTimeout());
};

// OnExit
void PingSendPingAndWaitForReplyState::OnExit(PingContext &context) const
{
  // OnExit for state PingSendPingAndWaitForReplyState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnExit(context.GetName(), PingSendPingAndWaitForReplyState::GetInstance().GetName());
  }
  stateforge::network::Ping& ping = context.getPing();
  // 1 action(s) to do
  context.TimerStopTimer();
};

// Events implementation for state  PingSendPingAndWaitForReplyState
void PingSendPingAndWaitForReplyState::EvCancel(PingContext &context) const
{
  // 1 transition(s)
  stateforge::network::Ping& ping = context.getPing();
  context.SetTransitionName("EvCancel");
  fsm::ProcessTransitionPre<PingContext, PingPingState>(context, &PingEndState::GetInstance());
  // No action to do
  fsm::ProcessTransitionPost<PingContext, PingPingState>(context, &PingEndState::GetInstance());
  context.OnEnd();
  return;
}

void PingSendPingAndWaitForReplyState::EvPingReply(PingContext &context) const
{
  // 1 transition(s)
  stateforge::network::Ping& ping = context.getPing();
  if((ping.getTx() == ping.getCount()) == true){
    context.SetTransitionName("EvPingReply[ping.getTx() == ping.getCount()]");
    fsm::ProcessTransitionPre<PingContext, PingPingState>(context, &PingEndState::GetInstance());
    // No action to do
    fsm::ProcessTransitionPost<PingContext, PingPingState>(context, &PingEndState::GetInstance());
    context.OnEnd();
    return;
  }
  // pass the event to the parent state
  PingPingState::GetInstance().EvPingReply(context);
}

void PingSendPingAndWaitForReplyState::EvTimer(PingContext &context) const
{
  // 2 transition(s)
  stateforge::network::Ping& ping = context.getPing();
  if((ping.getTx() == ping.getCount()) == true){
    context.SetTransitionName("EvTimer[ping.getTx() == ping.getCount()]");
    fsm::ProcessTransitionPre<PingContext, PingPingState>(context, &PingEndState::GetInstance());
    // No action to do
    fsm::ProcessTransitionPost<PingContext, PingPingState>(context, &PingEndState::GetInstance());
    context.OnEnd();
    return;
  }
  context.SetTransitionName("EvTimer");
  fsm::ProcessTransitionPre<PingContext, PingPingState>(context, &PingSendPingAndWaitForReplyState::GetInstance());
  // No action to do
  fsm::ProcessTransitionPost<PingContext, PingPingState>(context, &PingSendPingAndWaitForReplyState::GetInstance());
  return;
}



/**************************************************************
 * State implementation for PingEndState
 **************************************************************/

// State Constructor
PingEndState::PingEndState(const char* pName) : PingPingState(pName)
{
  m_eStateKind = fsm::STATE_FINAL;
  m_pStateParent = &PingPingState::GetInstance();
};

// State Destructor
PingEndState::~PingEndState()
{
};

// State GetInstance
const PingEndState& PingEndState::GetInstance()
{
  return kPingEndState;
};

// OnEntry
void PingEndState::OnEntry(PingContext &context) const
{
  // OnEntry for state PingEndState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnEntry(context.GetName(), PingEndState::GetInstance().GetName());
  }
};

// OnExit
void PingEndState::OnExit(PingContext &context) const
{
  // OnExit for state PingEndState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnExit(context.GetName(), PingEndState::GetInstance().GetName());
  }
};

// Events implementation for state  PingEndState
void PingEndState::EvStart(PingContext &context) const
{
  // 0 transition(s)
}

void PingEndState::EvCancel(PingContext &context) const
{
  // 0 transition(s)
}

void PingEndState::EvError(PingContext &context) const
{
  // 0 transition(s)
}

void PingEndState::EvPingReply(PingContext &context) const
{
  // 0 transition(s)
}

void PingEndState::EvTimer(PingContext &context) const
{
  // 0 transition(s)
}




// Ping asynchronous events 
void PingContext::EvStart()
{
  GetEventList().push_back(Event(*this, 1, &PingContext::EvStartSync));
  m_schedule.Schedule(0);
};

void PingContext::EvCancel()
{
  GetEventList().push_back(Event(*this, 2, &PingContext::EvCancelSync));
  m_schedule.Schedule(0);
};

// Internal asynchronous events 
void PingContext::EvError()
{
  GetEventList().push_back(Event(*this, 3, &PingContext::EvErrorSync));
  m_schedule.Schedule(0);
};

void PingContext::EvPingReply()
{
  GetEventList().push_back(Event(*this, 4, &PingContext::EvPingReplySync));
  m_schedule.Schedule(0);
};


// Reenable some compiler warnings
#ifdef _MSC_VER
#pragma warning(pop)
#endif

}}// namespace stateforge::network

