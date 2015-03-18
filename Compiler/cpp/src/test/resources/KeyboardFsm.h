/*******************************************************************************
 * YouCompany, All right reserved
 ******************************************************************************/
// KeyboardFsm.h has been generated automatically by StateBuilderCpp x.y.z
// Executable: 
// /C:/Users/frederic/Documents/projects/StateForge/dev/StateBuilder/Compiler/cpp/target/classes/
// Command line options: 
//  src/test/resources/statemachineOutputPathNotExist.cfg
// Date: 22-gen-2013 23.21.16

#ifndef KEYBOARDFSM_H
#define KEYBOARDFSM_H


// Includes
#include <cstring>
#include <fsm/Fsm.h>

// Object classes forward declaration
class BusinessObject;

// Context classes forward declaration
class KeyboardNumericKeypadContext;
class KeyboardContext;
class KeyboardMainKeypadContext;
// Parallel classes forward declaration
class KeyboardIdleParallel;
// State classes forward declaration
class KeyboardDefaultState;
class KeyboardEndState;
class KeyboardIdleState;
class KeyboardNumericKeypadState;
class KeyboardArrowsState;
class KeyboardNumbersState;
class KeyboardCapLockState;
class KeyboardKeyboardState;
class KeyboardMainKeypadState;

// Context declaration for state Keyboard
class KeyboardContext : public fsm::Context<KeyboardKeyboardState, KeyboardContext> {
public:
  // Constructor
  KeyboardContext(BusinessObject& myBusinessObject);

  // Destructor
  virtual ~KeyboardContext();

  // MainEvent's events 
  void evStart();
  void evStop();

  KeyboardIdleParallel& GetParallelIdle() {return *m_parallelIdle;};

  // Enter the initial state: walk the onEntry chain from the top state to the initial state.
  void EnterInitialState();

  // Leave the current state: walk the onExit chain from the current state to the top state.
  void LeaveCurrentState();

  BusinessObject& getBusinessObject(){return m_myBusinessObject;};

private:
  BusinessObject& m_myBusinessObject;
  std::auto_ptr<KeyboardIdleParallel> m_parallelIdle;


};

// Context declaration for state MainKeypad
class KeyboardMainKeypadContext : public fsm::Context<KeyboardMainKeypadState, KeyboardContext> {
public:
  // Constructor
  KeyboardMainKeypadContext(BusinessObject& myBusinessObject, KeyboardContext *pContext);

  // Destructor
  virtual ~KeyboardMainKeypadContext();

  // MainEvent's events 
  void evStart();
  void evStop();


  // Enter the initial state: walk the onEntry chain from the top state to the initial state.
  void EnterInitialState();

  // Leave the current state: walk the onExit chain from the current state to the top state.
  void LeaveCurrentState();

  BusinessObject& getBusinessObject(){return m_myBusinessObject;};

private:
  BusinessObject& m_myBusinessObject;


};

// Context declaration for state NumericKeypad
class KeyboardNumericKeypadContext : public fsm::Context<KeyboardNumericKeypadState, KeyboardContext> {
public:
  // Constructor
  KeyboardNumericKeypadContext(BusinessObject& myBusinessObject, KeyboardContext *pContext);

  // Destructor
  virtual ~KeyboardNumericKeypadContext();

  // MainEvent's events 
  void evStart();
  void evStop();


  // Enter the initial state: walk the onEntry chain from the top state to the initial state.
  void EnterInitialState();

  // Leave the current state: walk the onExit chain from the current state to the top state.
  void LeaveCurrentState();

  BusinessObject& getBusinessObject(){return m_myBusinessObject;};

private:
  BusinessObject& m_myBusinessObject;


};

// Parallel declaration for Idle
class KeyboardIdleParallel : public fsm::Parallel {
  public:
    // Constructor
    KeyboardIdleParallel(BusinessObject& myBusinessObject, KeyboardContext *pContext);

    KeyboardMainKeypadContext& GetContextMainKeypad() {return m_contextMainKeypad;};
    KeyboardNumericKeypadContext& GetContextNumericKeypad() {return m_contextNumericKeypad;};
    void TransitionToNextState();

  private:
    KeyboardMainKeypadContext m_contextMainKeypad;
    KeyboardNumericKeypadContext m_contextNumericKeypad;

    KeyboardContext &context;
};


// State KeyboardKeyboardState
class KeyboardKeyboardState : public fsm::State<KeyboardContext, KeyboardKeyboardState> {
public:
  // Constructor
  KeyboardKeyboardState(const char* pName);

  // Singleton pattern
  static const KeyboardKeyboardState& GetInstance();

  // Destructor
  virtual ~KeyboardKeyboardState();

  // OnEntry and OnExit
  virtual void OnEntry(KeyboardContext& context) const;
  virtual void OnExit(KeyboardContext& context) const;

  // Events
  virtual void evStart(KeyboardContext &context) const ;
  virtual void evStop(KeyboardContext &context) const ;

};


// State KeyboardIdleState
class KeyboardIdleState : public KeyboardKeyboardState {
public:
  // Constructor
  KeyboardIdleState(const char* pName);

  // Singleton pattern
  static const KeyboardIdleState& GetInstance();

  // Destructor
  virtual ~KeyboardIdleState();

  // OnEntry and OnExit
  virtual void OnEntry(KeyboardContext& context) const;
  virtual void OnExit(KeyboardContext& context) const;

  // Events
  virtual void evStart(KeyboardContext &context) const ;
  virtual void evStop(KeyboardContext &context) const ;

};


// State KeyboardMainKeypadState
class KeyboardMainKeypadState : public fsm::State<KeyboardMainKeypadContext, KeyboardMainKeypadState> {
public:
  // Constructor
  KeyboardMainKeypadState(const char* pName);

  // Singleton pattern
  static const KeyboardMainKeypadState& GetInstance();

  // Destructor
  virtual ~KeyboardMainKeypadState();

  // OnEntry and OnExit
  virtual void OnEntry(KeyboardMainKeypadContext& context) const;
  virtual void OnExit(KeyboardMainKeypadContext& context) const;

  // Events
  virtual void evStart(KeyboardMainKeypadContext &context) const ;
  virtual void evStop(KeyboardMainKeypadContext &context) const ;

};


// State KeyboardDefaultState
class KeyboardDefaultState : public KeyboardMainKeypadState {
public:
  // Constructor
  KeyboardDefaultState(const char* pName);

  // Singleton pattern
  static const KeyboardDefaultState& GetInstance();

  // Destructor
  virtual ~KeyboardDefaultState();

  // OnEntry and OnExit
  virtual void OnEntry(KeyboardMainKeypadContext& context) const;
  virtual void OnExit(KeyboardMainKeypadContext& context) const;

  // Events

};


// State KeyboardCapLockState
class KeyboardCapLockState : public KeyboardMainKeypadState {
public:
  // Constructor
  KeyboardCapLockState(const char* pName);

  // Singleton pattern
  static const KeyboardCapLockState& GetInstance();

  // Destructor
  virtual ~KeyboardCapLockState();

  // OnEntry and OnExit
  virtual void OnEntry(KeyboardMainKeypadContext& context) const;
  virtual void OnExit(KeyboardMainKeypadContext& context) const;

  // Events

};


// State KeyboardNumericKeypadState
class KeyboardNumericKeypadState : public fsm::State<KeyboardNumericKeypadContext, KeyboardNumericKeypadState> {
public:
  // Constructor
  KeyboardNumericKeypadState(const char* pName);

  // Singleton pattern
  static const KeyboardNumericKeypadState& GetInstance();

  // Destructor
  virtual ~KeyboardNumericKeypadState();

  // OnEntry and OnExit
  virtual void OnEntry(KeyboardNumericKeypadContext& context) const;
  virtual void OnExit(KeyboardNumericKeypadContext& context) const;

  // Events
  virtual void evStart(KeyboardNumericKeypadContext &context) const ;
  virtual void evStop(KeyboardNumericKeypadContext &context) const ;

};


// State KeyboardNumbersState
class KeyboardNumbersState : public KeyboardNumericKeypadState {
public:
  // Constructor
  KeyboardNumbersState(const char* pName);

  // Singleton pattern
  static const KeyboardNumbersState& GetInstance();

  // Destructor
  virtual ~KeyboardNumbersState();

  // OnEntry and OnExit
  virtual void OnEntry(KeyboardNumericKeypadContext& context) const;
  virtual void OnExit(KeyboardNumericKeypadContext& context) const;

  // Events

};


// State KeyboardArrowsState
class KeyboardArrowsState : public KeyboardNumericKeypadState {
public:
  // Constructor
  KeyboardArrowsState(const char* pName);

  // Singleton pattern
  static const KeyboardArrowsState& GetInstance();

  // Destructor
  virtual ~KeyboardArrowsState();

  // OnEntry and OnExit
  virtual void OnEntry(KeyboardNumericKeypadContext& context) const;
  virtual void OnExit(KeyboardNumericKeypadContext& context) const;

  // Events

};


// State KeyboardEndState
class KeyboardEndState : public KeyboardKeyboardState {
public:
  // Constructor
  KeyboardEndState(const char* pName);

  // Singleton pattern
  static const KeyboardEndState& GetInstance();

  // Destructor
  virtual ~KeyboardEndState();

  // OnEntry and OnExit
  virtual void OnEntry(KeyboardContext& context) const;
  virtual void OnExit(KeyboardContext& context) const;

  // Events
  virtual void evStart(KeyboardContext &context) const ;
  virtual void evStop(KeyboardContext &context) const ;

};

#endif //KEYBOARDFSM_H
