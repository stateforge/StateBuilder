//------------------------------------------------------------------------------
// <copyright file="Event.hpp" company="StateForge">
//      Copyright (c) 2010-2013 StateForge.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#ifndef FSM_EVENT_HPP
#define FSM_EVENT_HPP

#include <fsm/Event.h>

namespace fsm
{

/** Event0
  */

template<class TContext, class R>
Event0<TContext, R>::Event0(TContext &context, int eventId, R(TContext::*handler)())
    : m_context(context),
    m_handler(handler)
{
    IEvent<TContext>::m_eventId = eventId;
}

template<class TContext, class R>
void Event0<TContext, R>::dispatch()
{
  ((&m_context)->*m_handler)();
}

/** Event1
  */
template<class TContext, class R, class P1>
Event1<TContext, R, P1>::Event1(TContext &context, R(TContext::*handler)(P1), P1 p1)
    : m_context(context),
    m_handler(handler),
    myParam1(p1)
{}

template<class TContext, class R, class P1>
void Event1<TContext, R, P1>::dispatch()
{
  ((&m_context)->*m_handler)(myParam1);
}

/** Event1Ref
  */
template<class TContext, class R, class P1>
Event1Ref<TContext, R, P1>::Event1Ref(TContext &context, R(TContext::*handler)(P1&), P1 &p1)
    : m_context(context),
    m_handler(handler),
    myParam1(p1)
{}

template<class TContext, class R, class P1>
void Event1Ref<TContext, R, P1>::dispatch()
{
  ((&m_context)->*m_handler)(myParam1);
}

/** Event2RR
  */
template<class TContext, class R, class P1, class P2>
Event2RR<TContext, R, P1, P2>::Event2RR(TContext &context, R(TContext::*handler)(P1&, P2&), P1 &p1, P2& p2)
    : m_context(context),
    m_handler(handler),
    myParam1(p1),
    myParam2(p2)
{}

template<class TContext, class R, class P1, class P2>
void Event2RR<TContext, R, P1, P2>::dispatch()
{
  ((&m_context)->*m_handler)(myParam1, myParam2);
}

/** Event3Rvv
  */
template<class TContext, class R, class P1, class P2, class P3>
Event3Rvv<TContext, R, P1, P2, P3>::Event3Rvv(TContext &context, R(TContext::*handler)(P1&, P2, P3), P1 &p1, P2 p2, P3 p3)
    : m_context(context),
    m_handler(handler),
    myParam1(p1),
    myParam2(p2),
    myParam3(p3)
{}

template<class TContext, class R, class P1, class P2, class P3>
void Event3Rvv<TContext, R, P1, P2, P3>::dispatch()
{
  ((&m_context)->*m_handler)(myParam1, myParam2, myParam3);
}

}

#endif // namespace fsm
