//------------------------------------------------------------------------------
// <copyright file="Context.hpp" company="StateForge">
//      Copyright (c) 2010-2015 StateForge.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#ifndef FSM_CONTEXT_HPP
#define FSM_CONTEXT_HPP

#include <cassert> // assert
#include <algorithm> //std::remove
#include <memory> // std::auto_ptr

#include <fsm/State.h>
#include <fsm/IContext.h>
#include <fsm/Context.h>
#include <fsm/IManagement.h>

namespace fsm {

    template<class TState, class TContextParent>
    Context<TState, TContextParent>::Context(TContextParent *pContextParent) :
        m_pState(0),
        m_pStatePrevious(0),
        m_pStateNext(0),
        m_pStateHistory(0),
        m_pContextParent(pContextParent),
#ifdef STATEFORGE_OBSERVER_NO
#else
        m_pObserver(0),
#endif
        m_pLock(0),
        m_pParallel(0),
        m_pManagement(0),
        m_pName(0),
        m_id(0),
        m_pTransitionName(0),
        m_transitionId(0)
    {
        if(pContextParent != NULL){
            pContextParent->AddChild(this);
        }
    }

    template<class TState, class TContextParent>
    Context<TState, TContextParent>::~Context() 
    {
    }

    template<class TState, class TContextParent>
    const TState& Context<TState, TContextParent>::GetState() const {
        assert(m_pState);
        return *m_pState;
    }

    template<class TState, class TContextParent>
    void Context<TState, TContextParent>::SetState(const TState* pState) {
        m_pState = pState;
    }

    template<class TState, class TContextParent>
    const TState* Context<TState, TContextParent>::GetStatePrevious() const {
        return m_pStatePrevious;
    }

    template<class TState, class TContextParent>
    void Context<TState, TContextParent>::SetStatePrevious(const TState* pStatePrevious) {
        m_pStatePrevious = pStatePrevious;
    }

    template<class TState, class TContextParent>
    const TState* Context<TState, TContextParent>::GetStateNext() const {
        return m_pStateNext;
    }

    template<class TState, class TContextParent>
    void Context<TState, TContextParent>::SetStateNext(const TState* pStateNext) {
        m_pStateNext = pStateNext;
    }

    template<class TState, class TContextParent>
    const TState* Context<TState, TContextParent>::GetStateHistory() const {
        return m_pStateHistory;
    }

    template<class TState, class TContextParent>
    void Context<TState, TContextParent>::SaveState() {
        m_pStateHistory = m_pStatePrevious;
    }

    template<class TState, class TContextParent>
    void Context<TState, TContextParent>::SetInitialState(const TState& state) {
        m_pState = &state;
        m_pStatePrevious = &state;
    }

    template<class TState, class TContextParent>
    void Context<TState, TContextParent>::SetStatePreviousFromCurrent() {
        m_pStatePrevious = m_pState;
        m_pState = NULL;
    }

    template<class TState, class TContextParent>
    TContextParent& Context<TState, TContextParent>::GetParentContext() const {
        //TODO add exception
        assert(m_pContextParent != NULL);
        return *m_pContextParent;
    }

#ifdef STATEFORGE_OBSERVER_NO
#elif defined STATEFORGE_OBSERVER_INT
    template<class TState, class TContextParent>
    void Context<TState, TContextParent>::SetObserverInt(IObserverInt &observer)
    {
        m_pObserver = &observer;
        for(ContextList::iterator it = m_contextList.begin(); it != m_contextList.end(); ++it)
        {
            (*it)->SetObserverInt(observer);
        }
    }
    template<class TState, class TContextParent>
    IObserverInt* Context<TState, TContextParent>::GetObserver() const
    {
        return m_pObserver;
    }
#else
    template<class TState, class TContextParent>
    void Context<TState, TContextParent>::SetObserver(IObserver &observer)
    {
        m_pObserver = &observer;
        for(ContextList::iterator it = m_contextList.begin(); it != m_contextList.end(); ++it)
        {
            (*it)->SetObserver(observer);
        }
    }

    template<class TState, class TContextParent>
    IObserver* Context<TState, TContextParent>::GetObserver() const
    {
        return m_pObserver;
    }
#endif
    template<class TState, class TContextParent>
    bool Context<TState, TContextParent>::EnterState(const char* /*stateName*/)
    {
        // add serialize="true" in the settings tag
        assert(false);
        return false;
    }



    template<class TState, class TContextParent>
    void Context<TState, TContextParent>::SetLock(ILock* pLock)
    {
        m_pLock = pLock;
        for(ContextList::iterator it = m_contextList.begin(); it != m_contextList.end(); ++it)
        {
            (*it)->SetLock(pLock);
        }
    }

    template<class TState, class TContextParent>
    void Context<TState, TContextParent>::Lock(void)
    {
        if(m_pLock != NULL) m_pLock->Lock();
    }

    template<class TState, class TContextParent>
    void Context<TState, TContextParent>::UnLock(void)
    {
        if(m_pLock != NULL) m_pLock->UnLock();
        
    }

    template<class TState, class TContextParent>
    void Context<TState, TContextParent>::OnEnd(void)
    {
        if(m_pContextParent != NULL){
            Parallel* pParallelParent = m_pContextParent->GetParallel();
            if(pParallelParent != NULL){
                pParallelParent->StateActiveDecrement();
                if(pParallelParent->GetStateActive() == 0){
                    m_pContextParent->SetTransitionName("ContextParallelEnd");
                    pParallelParent->TransitionToNextState();
                }
            }
        }
        IManagement *pManagement = GetManagement();
        if(pManagement){
            pManagement->OnEnd();
        }
    }

    template<class TState, class TContextParent>
    void Context<TState, TContextParent>::AddChild(IContext *pContextChild)
    {
        m_contextList.push_back(pContextChild);
    }

    //WalkChain template funtions

    template<class TContext, class TState>
    void WalkChainEntry(TContext &context,
        const TState *pStatePrevious,
        const TState *pStateNext)
    {
        if (pStateNext == NULL){
            return;
        }

        if (pStatePrevious == pStateNext)
        {
            pStateNext->OnEntry(context);
            return;
        } 

        const TState *pStateParent = pStateNext->GetStateParent();
        if (pStateParent == NULL) {
            return;
        }

        if(pStatePrevious == pStateNext){
            pStateNext->OnEntry(context);
        } else if (fsm::IsChild<TState>(pStatePrevious, pStateNext) == false) {
            WalkChainEntry<TContext, TState>(context, pStatePrevious, pStateParent);
            pStateNext->OnEntry(context);
        }
    }

    template<class TContext, class TState>
    void WalkChainExit(TContext &context,
        const TState *pStatePrevious,
        const TState *pStateNext) 
    {
        if (pStatePrevious == pStateNext)
        {
            pStateNext->OnExit(context);
            return;
        }

        const TState *pStateParent = pStatePrevious->GetStateParent();
        if ((pStateParent == NULL) || (pStateNext == NULL)) {
            return;
        }

        if ((pStatePrevious == pStateNext) || (fsm::IsChild<TState>(pStateNext, pStatePrevious) == false)) {
            pStatePrevious->OnExit(context);
            WalkChainExit<TContext, TState>(context, pStateParent, pStateNext);
        }
    }

    template<typename TContext, typename TState>
    void ProcessTransitionPre(TContext &context,
        const TState *pStateNext) 
    {
        context.SetStatePreviousFromCurrent();
        const TState* pStatePrevious = context.GetStatePrevious();
        context.SetStateNext(pStateNext);
#ifdef STATEFORGE_OBSERVER_NO
#elif defined STATEFORGE_OBSERVER_INT
        IObserverInt *pObserver = context.GetObserver();
        if(pObserver){
            pObserver->OnTransitionBegin(
                context.GetId(),
                pStatePrevious->GetId(),
                pStateNext != NULL ? pStateNext->GetId(): pStatePrevious->GetId(),
                context.GetTransitionId());
        }

#else
        IObserver *pObserver = context.GetObserver();
        if(pObserver){
            pObserver->OnTransitionBegin(
                context.GetName(),
                pStatePrevious->GetName(),
                pStateNext != NULL ? pStateNext->GetName(): pStatePrevious->GetName(),
                context.GetTransitionName());
        }
#endif
        WalkChainExit<TContext, TState>(context, pStatePrevious, pStateNext);
    }

    template<class TContext, class TState>
    void ProcessTransitionPost(TContext &context,
        const TState *pStateNext) 
    { 
        const TState* pStatePrevious = context.GetStatePrevious();
        WalkChainEntry<TContext, TState>(context, pStatePrevious, pStateNext);
#ifdef STATEFORGE_OBSERVER_NO
#elif defined STATEFORGE_OBSERVER_INT
        IObserverInt *pObserver = context.GetObserver();
        if(pObserver){
            pObserver->OnTransitionEnd(
                context.GetId(),
                pStatePrevious->GetId(),
                pStateNext != NULL ? pStateNext->GetId(): pStatePrevious->GetId(),
                context.GetTransitionId());  
        }
#else
        IObserver *pObserver = context.GetObserver();
        if(pObserver){
            pObserver->OnTransitionEnd(
                context.GetName(),
                pStatePrevious->GetName(),
                pStateNext != NULL ? pStateNext->GetName(): pStatePrevious->GetName(),
                context.GetTransitionName());  
        }
#endif
        context.SetStatePrevious(NULL);
        context.SetState(pStateNext != NULL ? pStateNext : pStatePrevious);
        context.SetStateNext(NULL);
    }
}

#endif
