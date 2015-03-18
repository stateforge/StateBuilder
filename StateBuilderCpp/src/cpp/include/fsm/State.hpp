//------------------------------------------------------------------------------
// <copyright file="State.hpp" company="StateForge">
//      Copyright (c) 2010-2013 StateForge.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#ifndef FSM_STATE_HPP_
#define FSM_STATE_HPP_

#include <fsm/State.h>

namespace fsm
{

template<class TContext, class TState>
State<TContext, TState>::State(const char* pName, int id, StateKind eStateKind) :
    m_pName(pName),
    m_id(id),
    m_eStateKind(eStateKind),
    m_pStateParent(0)
{}

template<class TContext, class TState>
State<TContext, TState>::~State()
{}

template<class TContext, class TState>
const TState* State<TContext, TState>::GetStateParent() const
{
  return m_pStateParent;
}

template<class TState>
bool IsChild(const TState *pStateCurrent, const TState* pState)
{
  const TState *pStateParent = pStateCurrent->GetStateParent();
  return (pStateCurrent == pState) || (pStateParent && (IsChild<TState>(pStateParent, pState)));
}

} // namespace fsm {
#endif // #ifndef _FSM_STATE_HPP_
