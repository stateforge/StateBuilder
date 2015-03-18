//------------------------------------------------------------------------------
// <copyright file="Context.h" company="StateForge">
//      Copyright (c) 2010-2015 StateForge.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#ifndef FSM_CONTEXT_H
#define FSM_CONTEXT_H

#ifndef STATEFORGE_NO_PERSISTENCE
#include <sstream>
#endif 

#include <fsm/Definitions.h>
#include <fsm/IContext.h>
#include <fsm/ILock.h>
#include <fsm/Parallel.h>

namespace fsm {

// Forward declaration
class IObserver;
class IManagement;
class Parallel;

/** @class Context
  * @brief Context knows about the current state, its parent context, the observer, the management interface
  */

template<class TState, class TContextParent>
class Context : public IContext {
  public:

    /** Main constructor
      * @param pContextParent the parent context
      */
    Context(TContextParent *pContextParent = NULL);

    /** Virtual destructor
      */
    virtual ~Context();

    /** Get the current state
      */
    const TState& GetState() const;

    /** Set the current state to the specified state.
      */
    void SetState(const TState* pState);

    /** Get the previous state.
      */
    const TState* GetStatePrevious() const;
        
    /** Set the previous state to the specified state.
      */
    void SetStatePrevious(const TState* pState);
    
    /** Get the next state.
      */
    const TState* GetStateNext() const;
        
    /** Set the next state to the specified state.
      */
    void SetStateNext(const TState* pState);

    /** Get the history state.
      */
    const TState* GetStateHistory() const;

    /**
     * Se the initial and previous state to the given state
      */ 
    void SetInitialState(const TState& pState);

    /**
     * Save the current state to the history state.
      */ 
    void SaveState();

    /** Set the previous state to the current state and nullify the current state
      */
    void SetStatePreviousFromCurrent();

    /** Get the parent context
     */
    TContextParent& GetParentContext() const;

    /** get the context name
     */
    const char* GetName() const {return m_pName;}

    /** set the context name
      */
    void SetName(const char* pName) {m_pName = pName;}

    /** get the context name
     */
    int GetId() const {return m_id;}

    /** set the context name
      */
    void SetId(int id) {m_id = id;}


    /** Get the transition name
      */
    const char* GetTransitionName() const {return m_pTransitionName;}

    /** Set the transition name
      */
    void SetTransitionName(const char* pTransitionName) {m_pTransitionName = pTransitionName;}

    /** Get the transition id
      */
    int GetTransitionId() const {return m_transitionId;}

    /** Set the transition id
      */
    void SetTransitionId(int transitionId) {m_transitionId = transitionId;}

    /** Add a child context.
      */
    virtual void AddChild(IContext *pContextChild);

#ifdef STATEFORGE_OBSERVER_NO
#elif defined STATEFORGE_OBSERVER_INT
    /** Set the observer with integer
      */
    virtual void SetObserverInt(IObserverInt &observer);

    /** Get the observer with integer
      */
    IObserverInt* GetObserver() const;
#else
    /** Set the observer
      */
    virtual void SetObserver(IObserver &observer);

    /** Get the observer
      */
    IObserver* GetObserver() const;
#endif

    /** Set the lock interface, must be called after the context creation
      */
    void SetLock(ILock *pLock);

    /** Set the management interface, may be called after the context creation
      */
    void SetManagement(IManagement *pManagement){m_pManagement = pManagement;}

    /** Get the management interface
      */
    IManagement* GetManagement() const {return m_pManagement;}

    /** Set the parallel context
      */
    void SetParallel(Parallel *pParallel) {m_pParallel = pParallel;}

    /** Get the parallel context
      */
    Parallel* GetParallel() const {return m_pParallel;}

    /** Callback when the context ends, especially for subcontext.
       */
    void OnEnd(void);

    virtual bool EnterState(const char* /*stateName*/);

#ifndef STATEFORGE_NO_PERSISTENCE
    virtual void Serialize(std::ostream &os) const {
        SerializeState(os);
        SerializeParallel(os);
    }

    virtual void UnSerialize(std::istream &is) {
        UnSerializeState(is);
        UnSerializeParallel(is);
    }
#endif /*STATEFORGE_NO_PERSISTENCE*/

  protected:

#ifndef STATEFORGE_NO_PERSISTENCE

    void SerializeState(std::ostream &os) const {
        os << GetState().GetName() << std::endl;
    }

    void UnSerializeState(std::istream &is) {
        std::string stateName; 
        is >> stateName;
        EnterState(stateName.c_str());
    }

    void SerializeParallel(std::ostream &os) const {
        bool hasChild = !m_contextList.empty();
        os << hasChild << std::endl;
        if(hasChild){
            std::list<IContext*>::const_iterator contextIt = m_contextList.begin();
            for(;contextIt != m_contextList.end(); ++contextIt){
                (*contextIt)->Serialize(os);
            }        
        }
    }

    void UnSerializeParallel(std::istream &is) {
        bool hasChild;
        is >> hasChild;
        if(hasChild){
            std::list<IContext*>::iterator contextIt = m_contextList.begin();
            for(;contextIt != m_contextList.end(); ++contextIt){
                (*contextIt)->UnSerialize(is);
            }
        }
    }
#endif /*STATEFORGE_NO_PERSISTENCE*/

    /** Lock the context
      */
    void Lock();

    /** Unlock the context
      */
    void UnLock();

    ///<The current state
    TState const *m_pState;

    ///< The current state m_pState is save in m_pStatePrevious when entering in a transition
    TState const *m_pStatePrevious;

    ///< The next state is saved when entering in a transition
    TState const *m_pStateNext;
    
    ///< The current state is saved into the history state.
    TState const *m_pStateHistory;

    ///< The parent context
    TContextParent *m_pContextParent;

#ifdef STATEFORGE_OBSERVER_NO
#elif defined STATEFORGE_OBSERVER_INT
    IObserverInt *m_pObserver;
#else
    ///< observe the context
    IObserver *m_pObserver;
#endif
    ///< Lock interface
    ILock *m_pLock;

    ///< Current parallel context
    Parallel *m_pParallel;

    ///< Management:
    IManagement *m_pManagement;

    ///< context name
    const char* m_pName;

    ///< context id
    int m_id;

    ///< The current transition name
    const char *m_pTransitionName;

    ///< The current transition id
    int m_transitionId;

    typedef std::list<IContext*> ContextList;
    ContextList m_contextList;

  private:
   /** empty copy contructor
     */
    Context(const Context&);

    /** empty copy assignment contructor
     */
    Context& operator=(const Context&);
    
#ifndef STATEFORGE_NO_PERSISTENCE
    template<class TState_, class TContextParent_>
    friend std::ostream & operator<<(std::ostream &os, const Context<TState_,TContextParent_> &context);
    
    template<class TState_, class TContextParent_>
    friend std::istream & operator>>(std::istream &os, Context<TState_,TContextParent_> &context);
#endif /* STATEFORGE_NO_PERSISTENCE */
};

#ifndef STATEFORGE_NO_PERSISTENCE

template<class TState, class TContextParent>
std::ostream & operator<<(std::ostream &os, const Context<TState,TContextParent> &context)
{
    context.Serialize(os);
    return os;
}

template<class TState, class TContextParent>
std::istream & operator>>(std::istream &is, Context<TState,TContextParent> &context)
{
    context.UnSerialize(is);
    return is;
}

#endif /* STATEFORGE_NO_PERSISTENCE */

/** Walk the entry chain, from the state parent of both stateNext and statePrevious, to stateNext
  */
template<class TContext, class TState>
void WalkChainEntry(TContext &context,
                    const TState &statePrevious,
                    const TState &stateNext
                   );

/** Walk the exit chain, from statePrevious to the state parent of both stateNext and statePrevious to stateNext
  */
template<class TContext, class TState>
void WalkChainExit(TContext &context,
                   const TState &statePrevious,
                   const TState &stateNext
                  );

/** ProcessTransitionPre:
*  - set the previous state to current state
*  - notify the pre transition
*  - walk the exit chain
*  
*/
template<class TContext, class TState>
 void ProcessTransitionPre(TContext &context,
                          const TState* pStateNext);

/** ProcessTransitionPost:
*  - walk the entry chain
*  - notify the post transition
*  - set current state to next state
*  
*/
template<class TContext, class TState>
void ProcessTransitionPost(TContext &context,
                           const TState* pStateNext);
} /* namespace fsm */

#endif
