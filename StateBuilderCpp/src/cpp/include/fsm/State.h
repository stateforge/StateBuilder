//------------------------------------------------------------------------------
// <copyright file="State.h" company="StateForge">
//      Copyright (c) 2010-2013 StateForge.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------
#ifndef FSM_STATE_H
#define FSM_STATE_H

namespace fsm {

///<  the type of state
typedef enum StateKind {
    STATE_LEAF,
    STATE_FINAL,
    STATE_ERROR,
    STATE_HISTORY,
    STATE_TOP
} StateKind;

///< Map a state name to an id
typedef struct {
    const char* pcName;
    int iId;
} StateNameToId;

///< Map a state name to an id
typedef struct {
    const char* pcName;
    int iId;
} TransitionNameToId;

template<class TContext, class TState>
class State {
  public:

    /** Main Constructor
      */
    State(const char* pName, int id, StateKind eStateKind = STATE_LEAF);

    /** Virtual destructor because StateBase is abstract base class
      */
    virtual ~State();

    /** Get the state name
      */
    const char* GetName() const {
      return m_pName;
    }

   /** Get the state id
      */
    int GetId() const {
      return m_id;
    }

    /** Get the state kind
      */
    StateKind getKind() const {
        return m_eStateKind;
    }

    /** Get the parent state or NULL is this state is a top state
      */
    const TState* GetStateParent() const;

    /** Perform all necessary action upon entering this state
     */
    virtual void OnEntry(TContext &/*context*/) const = 0;

    /** Perform all necessary action upon leaving this state
     */
    virtual void OnExit(TContext &/*context*/) const = 0;

protected:
    ///< state name
    const char* m_pName;
    ///< state id
    int m_id;

    ///< the type of state, e.g normal, final or error
    StateKind m_eStateKind;
    ///< the eventual parent state
    const TState *m_pStateParent;

  private:
    /** Default contructor, copy and assignment operator are private to prevent their use*/
    State();
    State(const State&);
    State& operator=(const State&);
};

/** return true is stateChild is a child of stateCurrent
  */
template<class TState>
bool IsChild(const TState &stateCurrent, const TState &stateChild);

}

#endif
