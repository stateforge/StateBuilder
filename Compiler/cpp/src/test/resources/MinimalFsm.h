// MinimalFsm.h has been generated automatically by StateBuilderCpp x.y.z
// Executable: 
// /Users/frederic/StateBuilder/Compiler/cpp/target/classes/
// Command line options: 
//  src/test/resources/Minimal.fsmcpp
// Date: 23-mar-2015 0.07.17

#ifndef MINIMALFSM_H
#define MINIMALFSM_H


// Includes
#include <cstring>
#include <fsm/Fsm.h>

// Object classes forward declaration

// Context classes forward declaration
class MinimalContext;
// Parallel classes forward declaration
// State classes forward declaration
class MinimalTopState;
class MinimalOffState;
class MinimalOnState;

// Object classes forward declaration within the custom namepace
class Minimal;


// Context declaration for state Top
class MinimalContext : public fsm::Context<MinimalTopState, MinimalContext> {
public:
  // Constructor
  MinimalContext(Minimal& minimal);

  // Destructor
  virtual ~MinimalContext();


  static const fsm::StateNameToId* GetStateNameToId();
  // MinimalEvent's events 
  void EvStart();


  // Enter the initial state: walk the onEntry chain from the top state to the initial state.
  void EnterInitialState();

  // Leave the current state: walk the onExit chain from the current state to the top state.
  void LeaveCurrentState();

  Minimal& getMinimal(){return m_minimal;}

private:
  Minimal& m_minimal;


};


// State MinimalTopState
class MinimalTopState : public fsm::State<MinimalContext, MinimalTopState> {
public:
  // Constructor
  MinimalTopState(const char* pName, int id);

  // Singleton pattern
  static const MinimalTopState& GetInstance();

  // Destructor
  virtual ~MinimalTopState();

  // OnEntry and OnExit
  virtual void OnEntry(MinimalContext& context) const;
  virtual void OnExit(MinimalContext& context) const;

  // Events
  virtual void EvStart(MinimalContext &context) const ;

};


// State MinimalOnState
class MinimalOnState : public MinimalTopState {
public:
  // Constructor
  MinimalOnState(const char* pName, int id);

  // Singleton pattern
  static const MinimalOnState& GetInstance();

  // Destructor
  virtual ~MinimalOnState();

  // OnEntry and OnExit
  virtual void OnEntry(MinimalContext& context) const;
  virtual void OnExit(MinimalContext& context) const;

  // Events

};


// State MinimalOffState
class MinimalOffState : public MinimalTopState {
public:
  // Constructor
  MinimalOffState(const char* pName, int id);

  // Singleton pattern
  static const MinimalOffState& GetInstance();

  // Destructor
  virtual ~MinimalOffState();

  // OnEntry and OnExit
  virtual void OnEntry(MinimalContext& context) const;
  virtual void OnExit(MinimalContext& context) const;

  // Events

};

#endif //MINIMALFSM_H
