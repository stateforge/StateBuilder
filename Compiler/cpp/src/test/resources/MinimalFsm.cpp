// MinimalFsm.cpp has been generated automatically by StateBuilderCpp x.y.z
// Executable: 
// /Users/frederic/StateBuilder/Compiler/cpp/target/classes/
// Command line options: 
//  src/test/resources/Minimal.fsmcpp
// Date: 23-mar-2015 0.07.17

// Includes
#include "MinimalFsm.h"

#include <fsm/Fsm.hpp>
#include <Minimal.h>

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

// State names and ids
const fsm::StateNameToId kStateNameToIdMinimalContext[] = 
{
  {"Top", 0},
  {"On", 1},
  {"Off", 2},
  {NULL, 0}
};

// Transition names and ids
const fsm::TransitionNameToId kTransitionNameToIdMinimalContext[] = 
{
  {NULL, 0}
};

// State Instantiation
static const MinimalTopState kMinimalTopState(kStateNameToIdMinimalContext[0].pcName, 0);
static const MinimalOnState kMinimalOnState(kStateNameToIdMinimalContext[1].pcName, 1);
static const MinimalOffState kMinimalOffState(kStateNameToIdMinimalContext[2].pcName, 2);

/**************************************************************
  * Contexts implementations  
  *************************************************************/
/**************************************************************
  * Context MinimalContext
  *************************************************************/
MinimalContext::MinimalContext(Minimal& minimal) : 
  fsm::Context<MinimalTopState, MinimalContext>()
  , m_minimal(minimal)
{
  SetName("MinimalContext");
  SetInitialState(MinimalOnState::GetInstance());
}

MinimalContext::~MinimalContext(){}


const fsm::StateNameToId* MinimalContext::GetStateNameToId()
{
  return kStateNameToIdMinimalContext;
}

void MinimalContext::EnterInitialState()
{
  fsm::WalkChainEntry<MinimalContext, MinimalTopState>(*this, &MinimalTopState::GetInstance() , &GetState());
}

void MinimalContext::LeaveCurrentState()
{
  fsm::WalkChainExit<MinimalContext, MinimalTopState>(*this, &GetState(), &MinimalTopState::GetInstance());
}

// MinimalEvent synchronous events 
void MinimalContext::EvStart()
{
  GetState().EvStart(*this);
}

// Timer start implementation

/**************************************************************
  * States implementations  
 **************************************************************/
/**************************************************************
 * State implementation for MinimalTopState
 **************************************************************/

// State Constructor
MinimalTopState::MinimalTopState(const char* pName, int id) : fsm::State<MinimalContext, MinimalTopState>(pName, id)
{
}

// State Destructor
MinimalTopState::~MinimalTopState()
{
}

// State GetInstance
const MinimalTopState& MinimalTopState::GetInstance()
{
  return kMinimalTopState;
}

// OnEntry
void MinimalTopState::OnEntry(MinimalContext &context) const
{
  // OnEntry for state MinimalTopState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnEntry(context.GetName(), MinimalTopState::GetInstance().GetName());
  }
}

// OnExit
void MinimalTopState::OnExit(MinimalContext &context) const
{
  // OnExit for state MinimalTopState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnExit(context.GetName(), MinimalTopState::GetInstance().GetName());
  }
}

// Events implementation for state  MinimalTopState
void MinimalTopState::EvStart(MinimalContext &context) const
{
  // 0 transition(s)
}


/**************************************************************
 * State implementation for MinimalOnState
 **************************************************************/

// State Constructor
MinimalOnState::MinimalOnState(const char* pName, int id) : MinimalTopState(pName, id)
{
  m_pStateParent = &MinimalTopState::GetInstance();
}

// State Destructor
MinimalOnState::~MinimalOnState()
{
}

// State GetInstance
const MinimalOnState& MinimalOnState::GetInstance()
{
  return kMinimalOnState;
}

// OnEntry
void MinimalOnState::OnEntry(MinimalContext &context) const
{
  // OnEntry for state MinimalOnState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnEntry(context.GetName(), MinimalOnState::GetInstance().GetName());
  }
}

// OnExit
void MinimalOnState::OnExit(MinimalContext &context) const
{
  // OnExit for state MinimalOnState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnExit(context.GetName(), MinimalOnState::GetInstance().GetName());
  }
}

// Events implementation for state  MinimalOnState


/**************************************************************
 * State implementation for MinimalOffState
 **************************************************************/

// State Constructor
MinimalOffState::MinimalOffState(const char* pName, int id) : MinimalTopState(pName, id)
{
  m_pStateParent = &MinimalTopState::GetInstance();
}

// State Destructor
MinimalOffState::~MinimalOffState()
{
}

// State GetInstance
const MinimalOffState& MinimalOffState::GetInstance()
{
  return kMinimalOffState;
}

// OnEntry
void MinimalOffState::OnEntry(MinimalContext &context) const
{
  // OnEntry for state MinimalOffState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnEntry(context.GetName(), MinimalOffState::GetInstance().GetName());
  }
}

// OnExit
void MinimalOffState::OnExit(MinimalContext &context) const
{
  // OnExit for state MinimalOffState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnExit(context.GetName(), MinimalOffState::GetInstance().GetName());
  }
}

// Events implementation for state  MinimalOffState



// Reenable some compiler warnings
#ifdef _MSC_VER
#pragma warning(pop)
#endif

