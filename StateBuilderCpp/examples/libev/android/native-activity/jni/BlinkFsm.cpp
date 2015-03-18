// BlinkFsm.cpp has been generated automatically by StateBuilderCpp 1.3
// Executable: 
// file:/C:/Users/frederic/AppData/Local/StateForge/StateBuilderCpp/bin/StateBuilderCpp.exe
// Command line options: 
//  C:\Users\frederic\Documents\projects\StateForge\dev\StateBuilder\StateBuilderCpp\examples\libev\android\native-activity\jni\Blink.fsmcpp
// Date: 20-gen-2012 12.10.37

// Includes
#include <fsm/FsmAsync.hpp>
#include <Blink.h>
#include "BlinkFsm.h"

using namespace fsm;

// Disable some compiler warnings
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4355) // 'this' : used in base member initialiser list
#pragma warning(disable:4189) // local variable is initialised but not referenced
#pragma warning(disable:4100) // unreferenced formal parameter
#endif

// State Instantiation
BlinkTopState const *BlinkTopState::m_pTop = NULL;
BlinkIdleState const *BlinkIdleState::m_pIdle = NULL;
BlinkRunningState const *BlinkRunningState::m_pRunning = NULL;
BlinkEndState const *BlinkEndState::m_pEnd = NULL;

/**************************************************************
  * Contexts implementations  
  *************************************************************/
/**************************************************************
  * Context BlinkContext
  *************************************************************/
BlinkContext::BlinkContext(Blink& blink, ev::default_loop &loop) : 
  fsm::ContextAsync<BlinkTopState, BlinkContext, BlinkContext>(*this, NULL)
  , m_blink(blink)
  , m_timerProcess(loop)
  , m_timerTimerRetry(loop)
{
  SetName("BlinkContext");
  // Timers registration
  m_timerProcess.set<BlinkContext,&BlinkContext::process>(this);
  m_timerTimerRetry.set<BlinkContext,&BlinkContext::EvTimerRetry>(this);

  SetInitialState(BlinkIdleState::GetInstance());
};
void BlinkContext::EnterInitialState()
{
  fsm::WalkChainEntry<BlinkContext, BlinkTopState>(*this, &BlinkTopState::GetInstance() , &BlinkIdleState::GetInstance());
  SetInitialState(BlinkIdleState::GetInstance());
};

void BlinkContext::LeaveCurrentState()
{
  fsm::WalkChainExit<BlinkContext, BlinkTopState>(*this, &GetState(), &BlinkTopState::GetInstance());
};

// BlinkEvent synchronous events 
void BlinkContext::EvStartSync()
{
  GetState().EvStart(*this);
};

void BlinkContext::EvStopSync()
{
  GetState().EvStop(*this);
};

// Timers synchronous events 
// Timer's event implementation
void BlinkContext::EvTimerRetry()
{
  GetState().EvTimerRetry(*this);
};



// Timer start implementation
void BlinkContext::TimerStartTimerRetry(long duration)
{
  GetObserver().OnTimerStart("BlinkContext", "TimerRetry", duration);
  m_timerTimerRetry.start(((double)duration)/1000);
}


// Timer stop implementation
void BlinkContext::TimerStopTimerRetry()
{
  GetObserver().OnTimerStop("BlinkContext", "TimerRetry");
  m_timerTimerRetry.stop();
}

void BlinkContext::process()
{
  long nextTick = 0;
  switch (ProcessEvent(nextTick))
  {
  case fsm::ProcessRunning:
      break;
  case fsm::ProcessEnd:
      m_timerProcess.stop();
      break;
  default:
      break;
  }
}
void BlinkContext::Schedule(long nextTick)
{
  m_timerProcess.start(0);
}

/**************************************************************
  * States implementations  
 **************************************************************/
/**************************************************************
 * State implementation for BlinkTopState
 **************************************************************/

// State Constructor
BlinkTopState::BlinkTopState(const string &name) : fsm::State<BlinkContext, BlinkTopState>(name)
{
};

// State Destructor
BlinkTopState::~BlinkTopState()
{
  delete m_pTop;
};

// State GetInstance
const BlinkTopState& BlinkTopState::GetInstance()
{
  if(m_pTop == NULL){
    m_pTop = new BlinkTopState("Top");
  }
  return *m_pTop;
};

// OnEntry
void BlinkTopState::OnEntry(BlinkContext &context) const
{
  // OnEntry for state BlinkTopState
  context.GetObserver().OnEntry(context.GetName(), BlinkTopState::GetInstance().GetName());
};

// OnExit
void BlinkTopState::OnExit(BlinkContext &context) const
{
  // OnExit for state BlinkTopState
  context.GetObserver().OnExit(context.GetName(), BlinkTopState::GetInstance().GetName());
};

// Events implementation for state  BlinkTopState
void BlinkTopState::EvStart(BlinkContext &context) const
{
  // 0 transition(s)
}

void BlinkTopState::EvStop(BlinkContext &context) const
{
  // 1 transition(s)
  Blink& blink = context.getBlink();
  context.SetTransitionName("EvStop");
  fsm::ProcessTransitionPre<BlinkContext, BlinkTopState>(context, &BlinkEndState::GetInstance());
  // No action to do
  fsm::ProcessTransitionPost<BlinkContext, BlinkTopState>(context, &BlinkEndState::GetInstance());
  context.OnEnd();
  return;
}

void BlinkTopState::EvTimerRetry(BlinkContext &context) const
{
  // 0 transition(s)
}


/**************************************************************
 * State implementation for BlinkIdleState
 **************************************************************/

// State Constructor
BlinkIdleState::BlinkIdleState(const string &name) : BlinkTopState(name)
{
  m_pStateParent = &BlinkTopState::GetInstance();
};

// State Destructor
BlinkIdleState::~BlinkIdleState()
{
  delete m_pIdle;
};

// State GetInstance
const BlinkIdleState& BlinkIdleState::GetInstance()
{
  if(m_pIdle == NULL){
    m_pIdle = new BlinkIdleState("Idle");
  }
  return *m_pIdle;
};

// OnEntry
void BlinkIdleState::OnEntry(BlinkContext &context) const
{
  // OnEntry for state BlinkIdleState
  context.GetObserver().OnEntry(context.GetName(), BlinkIdleState::GetInstance().GetName());
};

// OnExit
void BlinkIdleState::OnExit(BlinkContext &context) const
{
  // OnExit for state BlinkIdleState
  context.GetObserver().OnExit(context.GetName(), BlinkIdleState::GetInstance().GetName());
};

// Events implementation for state  BlinkIdleState
void BlinkIdleState::EvStart(BlinkContext &context) const
{
  // 1 transition(s)
  Blink& blink = context.getBlink();
  context.SetTransitionName("EvStart");
  fsm::ProcessTransitionPre<BlinkContext, BlinkTopState>(context, &BlinkRunningState::GetInstance());
  // No action to do
  fsm::ProcessTransitionPost<BlinkContext, BlinkTopState>(context, &BlinkRunningState::GetInstance());
  return;
}



/**************************************************************
 * State implementation for BlinkRunningState
 **************************************************************/

// State Constructor
BlinkRunningState::BlinkRunningState(const string &name) : BlinkTopState(name)
{
  m_pStateParent = &BlinkTopState::GetInstance();
};

// State Destructor
BlinkRunningState::~BlinkRunningState()
{
  delete m_pRunning;
};

// State GetInstance
const BlinkRunningState& BlinkRunningState::GetInstance()
{
  if(m_pRunning == NULL){
    m_pRunning = new BlinkRunningState("Running");
  }
  return *m_pRunning;
};

// OnEntry
void BlinkRunningState::OnEntry(BlinkContext &context) const
{
  // OnEntry for state BlinkRunningState
  context.GetObserver().OnEntry(context.GetName(), BlinkRunningState::GetInstance().GetName());
  Blink& blink = context.getBlink();
  // 2 action(s) to do
  context.TimerStartTimerRetry(1000);
  // TODO helloWorld.Print("Ciao");
};

// OnExit
void BlinkRunningState::OnExit(BlinkContext &context) const
{
  // OnExit for state BlinkRunningState
  context.GetObserver().OnExit(context.GetName(), BlinkRunningState::GetInstance().GetName());
};

// Events implementation for state  BlinkRunningState
void BlinkRunningState::EvTimerRetry(BlinkContext &context) const
{
  // 1 transition(s)
  Blink& blink = context.getBlink();
  context.SetTransitionName("EvTimerRetry");
  fsm::ProcessTransitionPre<BlinkContext, BlinkTopState>(context, &BlinkRunningState::GetInstance());
  // 1 action(s) to do
  //TODO helloWorld.IncrementCounter();;
  fsm::ProcessTransitionPost<BlinkContext, BlinkTopState>(context, &BlinkRunningState::GetInstance());
  return;
}



/**************************************************************
 * State implementation for BlinkEndState
 **************************************************************/

// State Constructor
BlinkEndState::BlinkEndState(const string &name) : BlinkTopState(name)
{
  m_eStateKind = STATE_FINAL;
  m_pStateParent = &BlinkTopState::GetInstance();
};

// State Destructor
BlinkEndState::~BlinkEndState()
{
  delete m_pEnd;
};

// State GetInstance
const BlinkEndState& BlinkEndState::GetInstance()
{
  if(m_pEnd == NULL){
    m_pEnd = new BlinkEndState("End");
  }
  return *m_pEnd;
};

// OnEntry
void BlinkEndState::OnEntry(BlinkContext &context) const
{
  // OnEntry for state BlinkEndState
  context.GetObserver().OnEntry(context.GetName(), BlinkEndState::GetInstance().GetName());
};

// OnExit
void BlinkEndState::OnExit(BlinkContext &context) const
{
  // OnExit for state BlinkEndState
  context.GetObserver().OnExit(context.GetName(), BlinkEndState::GetInstance().GetName());
};

// Events implementation for state  BlinkEndState
void BlinkEndState::EvStart(BlinkContext &context) const
{
  // 0 transition(s)
}

void BlinkEndState::EvStop(BlinkContext &context) const
{
  // 0 transition(s)
}

void BlinkEndState::EvTimerRetry(BlinkContext &context) const
{
  // 0 transition(s)
}




// BlinkEvent asynchronous events 
void BlinkContext::EvStart()
{
  GetEventList().push_back(Event(*this, &BlinkContext::EvStartSync));
  m_schedule.Schedule(0);
};

void BlinkContext::EvStop()
{
  GetEventList().push_back(Event(*this, &BlinkContext::EvStopSync));
  m_schedule.Schedule(0);
};

// Timers asynchronous events 

// Reenable some compiler warnings
#ifdef _MSC_VER
#pragma warning(pop)
#endif

