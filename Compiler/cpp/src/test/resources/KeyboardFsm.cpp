/*******************************************************************************
 * YouCompany, All right reserved
 ******************************************************************************/
// KeyboardFsm.cpp has been generated automatically by StateBuilderCpp x.y.z
// Executable: 
// /C:/Users/frederic/Documents/projects/StateForge/dev/StateBuilder/Compiler/cpp/target/classes/
// Command line options: 
//  src/test/resources/statemachineOutputPathNotExist.cfg
// Date: 22-gen-2013 23.21.16

// Includes
#include <fsm/Fsm.hpp>
#include <BusinessObject.h>
#include "KeyboardFsm.h"


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
static const KeyboardKeyboardState kKeyboardKeyboardState("Keyboard");
static const KeyboardIdleState kKeyboardIdleState("Idle");
static const KeyboardMainKeypadState kKeyboardMainKeypadState("MainKeypad");
static const KeyboardDefaultState kKeyboardDefaultState("Default");
static const KeyboardCapLockState kKeyboardCapLockState("CapLock");
static const KeyboardNumericKeypadState kKeyboardNumericKeypadState("NumericKeypad");
static const KeyboardNumbersState kKeyboardNumbersState("Numbers");
static const KeyboardArrowsState kKeyboardArrowsState("Arrows");
static const KeyboardEndState kKeyboardEndState("End");

/**************************************************************
  * Contexts implementations  
  *************************************************************/
/**************************************************************
  * Context KeyboardContext
  *************************************************************/
KeyboardContext::KeyboardContext(BusinessObject& myBusinessObject) : 
  fsm::Context<KeyboardKeyboardState, KeyboardContext>()
  , m_myBusinessObject(myBusinessObject),
  m_parallelIdle(new KeyboardIdleParallel(myBusinessObject, this))
{
  SetName("KeyboardContext");
  SetInitialState(KeyboardIdleState::GetInstance());
};

KeyboardContext::~KeyboardContext(){}

void KeyboardContext::EnterInitialState()
{
  fsm::WalkChainEntry<KeyboardContext, KeyboardKeyboardState>(*this, &KeyboardKeyboardState::GetInstance() , &GetState());
};

void KeyboardContext::LeaveCurrentState()
{
  fsm::WalkChainExit<KeyboardContext, KeyboardKeyboardState>(*this, &GetState(), &KeyboardKeyboardState::GetInstance());
};

// MainEvent synchronous events 
void KeyboardContext::evStart()
{
  GetState().evStart(*this);
};

void KeyboardContext::evStop()
{
  GetState().evStop(*this);
};

// Timer start implementation

/**************************************************************
  * Context KeyboardMainKeypadContext
  *************************************************************/
KeyboardMainKeypadContext::KeyboardMainKeypadContext(BusinessObject& myBusinessObject, KeyboardContext *pContext) : 
  fsm::Context<KeyboardMainKeypadState, KeyboardContext>(pContext)
  , m_myBusinessObject(myBusinessObject)
{
  SetName("KeyboardMainKeypadContext");
  SetInitialState(KeyboardDefaultState::GetInstance());
};

KeyboardMainKeypadContext::~KeyboardMainKeypadContext(){}

void KeyboardMainKeypadContext::EnterInitialState()
{
  fsm::WalkChainEntry<KeyboardMainKeypadContext, KeyboardMainKeypadState>(*this, &KeyboardMainKeypadState::GetInstance() , &GetState());
};

void KeyboardMainKeypadContext::LeaveCurrentState()
{
  fsm::WalkChainExit<KeyboardMainKeypadContext, KeyboardMainKeypadState>(*this, &GetState(), &KeyboardMainKeypadState::GetInstance());
};

// MainEvent synchronous events 
void KeyboardMainKeypadContext::evStart()
{
  GetState().evStart(*this);
};

void KeyboardMainKeypadContext::evStop()
{
  GetState().evStop(*this);
};

/**************************************************************
  * Context KeyboardNumericKeypadContext
  *************************************************************/
KeyboardNumericKeypadContext::KeyboardNumericKeypadContext(BusinessObject& myBusinessObject, KeyboardContext *pContext) : 
  fsm::Context<KeyboardNumericKeypadState, KeyboardContext>(pContext)
  , m_myBusinessObject(myBusinessObject)
{
  SetName("KeyboardNumericKeypadContext");
  SetInitialState(KeyboardNumbersState::GetInstance());
};

KeyboardNumericKeypadContext::~KeyboardNumericKeypadContext(){}

void KeyboardNumericKeypadContext::EnterInitialState()
{
  fsm::WalkChainEntry<KeyboardNumericKeypadContext, KeyboardNumericKeypadState>(*this, &KeyboardNumericKeypadState::GetInstance() , &GetState());
};

void KeyboardNumericKeypadContext::LeaveCurrentState()
{
  fsm::WalkChainExit<KeyboardNumericKeypadContext, KeyboardNumericKeypadState>(*this, &GetState(), &KeyboardNumericKeypadState::GetInstance());
};

// MainEvent synchronous events 
void KeyboardNumericKeypadContext::evStart()
{
  GetState().evStart(*this);
};

void KeyboardNumericKeypadContext::evStop()
{
  GetState().evStop(*this);
};

KeyboardIdleParallel::KeyboardIdleParallel(BusinessObject& myBusinessObject, KeyboardContext *pContext) : 
  fsm::Parallel(),
  m_contextMainKeypad(myBusinessObject, pContext), 
  m_contextNumericKeypad(myBusinessObject, pContext), 
  context(*pContext)
{
  m_contextList.push_back(&m_contextMainKeypad);
  m_contextList.push_back(&m_contextNumericKeypad);
}

void KeyboardIdleParallel::TransitionToNextState()
{
  fsm::ProcessTransitionPre<KeyboardContext, KeyboardKeyboardState>(context, &KeyboardEndState::GetInstance());
  fsm::ProcessTransitionPost<KeyboardContext, KeyboardKeyboardState>(context, &KeyboardEndState::GetInstance());
  context.OnEnd();
  m_contextMainKeypad.SetInitialState(KeyboardDefaultState::GetInstance());
  m_contextNumericKeypad.SetInitialState(KeyboardNumbersState::GetInstance());
}

/**************************************************************
  * States implementations  
 **************************************************************/
/**************************************************************
 * State implementation for KeyboardKeyboardState
 **************************************************************/

// State Constructor
KeyboardKeyboardState::KeyboardKeyboardState(const char* pName) : fsm::State<KeyboardContext, KeyboardKeyboardState>(pName)
{
};

// State Destructor
KeyboardKeyboardState::~KeyboardKeyboardState()
{
};

// State GetInstance
const KeyboardKeyboardState& KeyboardKeyboardState::GetInstance()
{
  return kKeyboardKeyboardState;
};

// OnEntry
void KeyboardKeyboardState::OnEntry(KeyboardContext &context) const
{
  // OnEntry for state KeyboardKeyboardState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnEntry(context.GetName(), KeyboardKeyboardState::GetInstance().GetName());
  }
};

// OnExit
void KeyboardKeyboardState::OnExit(KeyboardContext &context) const
{
  // OnExit for state KeyboardKeyboardState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnExit(context.GetName(), KeyboardKeyboardState::GetInstance().GetName());
  }
};

// Events implementation for state  KeyboardKeyboardState
void KeyboardKeyboardState::evStart(KeyboardContext &context) const
{
  // 0 transition(s)
}

void KeyboardKeyboardState::evStop(KeyboardContext &context) const
{
  // 0 transition(s)
}


/**************************************************************
 * State implementation for KeyboardIdleState
 **************************************************************/

// State Constructor
KeyboardIdleState::KeyboardIdleState(const char* pName) : KeyboardKeyboardState(pName)
{
  m_pStateParent = &KeyboardKeyboardState::GetInstance();
};

// State Destructor
KeyboardIdleState::~KeyboardIdleState()
{
};

// State GetInstance
const KeyboardIdleState& KeyboardIdleState::GetInstance()
{
  return kKeyboardIdleState;
};

// OnEntry
void KeyboardIdleState::OnEntry(KeyboardContext &context) const
{
  // OnEntry for state KeyboardIdleState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnEntry(context.GetName(), KeyboardIdleState::GetInstance().GetName());
  }
  KeyboardIdleParallel &parallelIdle = context.GetParallelIdle();
  context.SetParallel(&parallelIdle);
  parallelIdle.SetStateActive(2);
  parallelIdle.GetContextMainKeypad().EnterInitialState();
  parallelIdle.GetContextNumericKeypad().EnterInitialState();
};

// OnExit
void KeyboardIdleState::OnExit(KeyboardContext &context) const
{
  KeyboardIdleParallel &parallelKeyboardIdleState = context.GetParallelIdle();
  context.SetParallel(NULL);
  parallelKeyboardIdleState.GetContextMainKeypad().LeaveCurrentState();
  parallelKeyboardIdleState.GetContextNumericKeypad().LeaveCurrentState();
  // OnExit for state KeyboardIdleState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnExit(context.GetName(), KeyboardIdleState::GetInstance().GetName());
  }
};

// Events implementation for state  KeyboardIdleState
void KeyboardIdleState::evStart(KeyboardContext &context) const
{
  // 0 transition(s)
  // State Idle is parallel
  // Forward events to all orthogonal states
  KeyboardIdleParallel &parallelKeyboardIdleState = context.GetParallelIdle();
  parallelKeyboardIdleState.GetContextMainKeypad().GetState().evStart(parallelKeyboardIdleState.GetContextMainKeypad());
  parallelKeyboardIdleState.GetContextNumericKeypad().GetState().evStart(parallelKeyboardIdleState.GetContextNumericKeypad());
  // pass the event to the parent state
  KeyboardKeyboardState::GetInstance().evStart(context);
}

void KeyboardIdleState::evStop(KeyboardContext &context) const
{
  // 0 transition(s)
  // State Idle is parallel
  // Forward events to all orthogonal states
  KeyboardIdleParallel &parallelKeyboardIdleState = context.GetParallelIdle();
  parallelKeyboardIdleState.GetContextMainKeypad().GetState().evStop(parallelKeyboardIdleState.GetContextMainKeypad());
  parallelKeyboardIdleState.GetContextNumericKeypad().GetState().evStop(parallelKeyboardIdleState.GetContextNumericKeypad());
  // pass the event to the parent state
  KeyboardKeyboardState::GetInstance().evStop(context);
}


/**************************************************************
 * State implementation for KeyboardMainKeypadState
 **************************************************************/

// State Constructor
KeyboardMainKeypadState::KeyboardMainKeypadState(const char* pName) : fsm::State<KeyboardMainKeypadContext, KeyboardMainKeypadState>(pName)
{
};

// State Destructor
KeyboardMainKeypadState::~KeyboardMainKeypadState()
{
};

// State GetInstance
const KeyboardMainKeypadState& KeyboardMainKeypadState::GetInstance()
{
  return kKeyboardMainKeypadState;
};

// OnEntry
void KeyboardMainKeypadState::OnEntry(KeyboardMainKeypadContext &context) const
{
  // OnEntry for state KeyboardMainKeypadState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnEntry(context.GetName(), KeyboardMainKeypadState::GetInstance().GetName());
  }
};

// OnExit
void KeyboardMainKeypadState::OnExit(KeyboardMainKeypadContext &context) const
{
  // OnExit for state KeyboardMainKeypadState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnExit(context.GetName(), KeyboardMainKeypadState::GetInstance().GetName());
  }
};

// Events implementation for state  KeyboardMainKeypadState
void KeyboardMainKeypadState::evStart(KeyboardMainKeypadContext &context) const
{
  // 0 transition(s)
}

void KeyboardMainKeypadState::evStop(KeyboardMainKeypadContext &context) const
{
  // 0 transition(s)
}


/**************************************************************
 * State implementation for KeyboardDefaultState
 **************************************************************/

// State Constructor
KeyboardDefaultState::KeyboardDefaultState(const char* pName) : KeyboardMainKeypadState(pName)
{
  m_pStateParent = &KeyboardMainKeypadState::GetInstance();
};

// State Destructor
KeyboardDefaultState::~KeyboardDefaultState()
{
};

// State GetInstance
const KeyboardDefaultState& KeyboardDefaultState::GetInstance()
{
  return kKeyboardDefaultState;
};

// OnEntry
void KeyboardDefaultState::OnEntry(KeyboardMainKeypadContext &context) const
{
  // OnEntry for state KeyboardDefaultState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnEntry(context.GetName(), KeyboardDefaultState::GetInstance().GetName());
  }
};

// OnExit
void KeyboardDefaultState::OnExit(KeyboardMainKeypadContext &context) const
{
  // OnExit for state KeyboardDefaultState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnExit(context.GetName(), KeyboardDefaultState::GetInstance().GetName());
  }
};

// Events implementation for state  KeyboardDefaultState


/**************************************************************
 * State implementation for KeyboardCapLockState
 **************************************************************/

// State Constructor
KeyboardCapLockState::KeyboardCapLockState(const char* pName) : KeyboardMainKeypadState(pName)
{
  m_pStateParent = &KeyboardMainKeypadState::GetInstance();
};

// State Destructor
KeyboardCapLockState::~KeyboardCapLockState()
{
};

// State GetInstance
const KeyboardCapLockState& KeyboardCapLockState::GetInstance()
{
  return kKeyboardCapLockState;
};

// OnEntry
void KeyboardCapLockState::OnEntry(KeyboardMainKeypadContext &context) const
{
  // OnEntry for state KeyboardCapLockState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnEntry(context.GetName(), KeyboardCapLockState::GetInstance().GetName());
  }
};

// OnExit
void KeyboardCapLockState::OnExit(KeyboardMainKeypadContext &context) const
{
  // OnExit for state KeyboardCapLockState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnExit(context.GetName(), KeyboardCapLockState::GetInstance().GetName());
  }
};

// Events implementation for state  KeyboardCapLockState



/**************************************************************
 * State implementation for KeyboardNumericKeypadState
 **************************************************************/

// State Constructor
KeyboardNumericKeypadState::KeyboardNumericKeypadState(const char* pName) : fsm::State<KeyboardNumericKeypadContext, KeyboardNumericKeypadState>(pName)
{
};

// State Destructor
KeyboardNumericKeypadState::~KeyboardNumericKeypadState()
{
};

// State GetInstance
const KeyboardNumericKeypadState& KeyboardNumericKeypadState::GetInstance()
{
  return kKeyboardNumericKeypadState;
};

// OnEntry
void KeyboardNumericKeypadState::OnEntry(KeyboardNumericKeypadContext &context) const
{
  // OnEntry for state KeyboardNumericKeypadState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnEntry(context.GetName(), KeyboardNumericKeypadState::GetInstance().GetName());
  }
};

// OnExit
void KeyboardNumericKeypadState::OnExit(KeyboardNumericKeypadContext &context) const
{
  // OnExit for state KeyboardNumericKeypadState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnExit(context.GetName(), KeyboardNumericKeypadState::GetInstance().GetName());
  }
};

// Events implementation for state  KeyboardNumericKeypadState
void KeyboardNumericKeypadState::evStart(KeyboardNumericKeypadContext &context) const
{
  // 0 transition(s)
}

void KeyboardNumericKeypadState::evStop(KeyboardNumericKeypadContext &context) const
{
  // 0 transition(s)
}


/**************************************************************
 * State implementation for KeyboardNumbersState
 **************************************************************/

// State Constructor
KeyboardNumbersState::KeyboardNumbersState(const char* pName) : KeyboardNumericKeypadState(pName)
{
  m_pStateParent = &KeyboardNumericKeypadState::GetInstance();
};

// State Destructor
KeyboardNumbersState::~KeyboardNumbersState()
{
};

// State GetInstance
const KeyboardNumbersState& KeyboardNumbersState::GetInstance()
{
  return kKeyboardNumbersState;
};

// OnEntry
void KeyboardNumbersState::OnEntry(KeyboardNumericKeypadContext &context) const
{
  // OnEntry for state KeyboardNumbersState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnEntry(context.GetName(), KeyboardNumbersState::GetInstance().GetName());
  }
};

// OnExit
void KeyboardNumbersState::OnExit(KeyboardNumericKeypadContext &context) const
{
  // OnExit for state KeyboardNumbersState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnExit(context.GetName(), KeyboardNumbersState::GetInstance().GetName());
  }
};

// Events implementation for state  KeyboardNumbersState


/**************************************************************
 * State implementation for KeyboardArrowsState
 **************************************************************/

// State Constructor
KeyboardArrowsState::KeyboardArrowsState(const char* pName) : KeyboardNumericKeypadState(pName)
{
  m_pStateParent = &KeyboardNumericKeypadState::GetInstance();
};

// State Destructor
KeyboardArrowsState::~KeyboardArrowsState()
{
};

// State GetInstance
const KeyboardArrowsState& KeyboardArrowsState::GetInstance()
{
  return kKeyboardArrowsState;
};

// OnEntry
void KeyboardArrowsState::OnEntry(KeyboardNumericKeypadContext &context) const
{
  // OnEntry for state KeyboardArrowsState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnEntry(context.GetName(), KeyboardArrowsState::GetInstance().GetName());
  }
};

// OnExit
void KeyboardArrowsState::OnExit(KeyboardNumericKeypadContext &context) const
{
  // OnExit for state KeyboardArrowsState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnExit(context.GetName(), KeyboardArrowsState::GetInstance().GetName());
  }
};

// Events implementation for state  KeyboardArrowsState




/**************************************************************
 * State implementation for KeyboardEndState
 **************************************************************/

// State Constructor
KeyboardEndState::KeyboardEndState(const char* pName) : KeyboardKeyboardState(pName)
{
  m_eStateKind = fsm::STATE_FINAL;
  m_pStateParent = &KeyboardKeyboardState::GetInstance();
};

// State Destructor
KeyboardEndState::~KeyboardEndState()
{
};

// State GetInstance
const KeyboardEndState& KeyboardEndState::GetInstance()
{
  return kKeyboardEndState;
};

// OnEntry
void KeyboardEndState::OnEntry(KeyboardContext &context) const
{
  // OnEntry for state KeyboardEndState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnEntry(context.GetName(), KeyboardEndState::GetInstance().GetName());
  }
};

// OnExit
void KeyboardEndState::OnExit(KeyboardContext &context) const
{
  // OnExit for state KeyboardEndState
  fsm::IObserver *pObserver = context.GetObserver();
  if(pObserver){
    pObserver->OnExit(context.GetName(), KeyboardEndState::GetInstance().GetName());
  }
};

// Events implementation for state  KeyboardEndState
void KeyboardEndState::evStart(KeyboardContext &context) const
{
  // 0 transition(s)
}

void KeyboardEndState::evStop(KeyboardContext &context) const
{
  // 0 transition(s)
}




// Reenable some compiler warnings
#ifdef _MSC_VER
#pragma warning(pop)
#endif

