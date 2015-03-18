//------------------------------------------------------------------------------
// <copyright file="Event.h" company="StateForge">
//      Copyright (c) 2010-2013 StateForge.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------
#ifndef FSM_EVENT_H
#define FSM_EVENT_H

namespace fsm {
/**
 * Definitions for queuable event types
 */

template <typename T>
class IEvent;

/** @class IEventBase
  * @brief IEventBase is the base abstract class for all events 
  */
class IEventBase {
  public:
    IEventBase(){}
    virtual ~IEventBase() {}
    virtual void dispatch() = 0;
    virtual int GetEventId() = 0;
};

/** @class IEvent
  * @brief IEvent is the base template class for all events
  */
template <typename TContext>
class IEvent : public IEventBase {
  public:
    IEvent(): m_eventId(0) {}
    virtual ~IEvent() {}
    virtual void dispatch() = 0;

    int GetEventId(){return m_eventId;}
  protected:
    int m_eventId;
};

/** @class Event0
  * @class Event0 is a template class for events without parameter,
  * it is the only event that takes an eventId, event with parameters cannot be serialized
  */
template <typename TContext, typename R>
class Event0 : public IEvent<TContext> {
    typedef R(TContext::*Signature)();

  public:
    Event0(TContext &context, int eventId, R(TContext::*handler)());

    void dispatch();

  protected:
    TContext &m_context; ///< the class instance to use
    Signature m_handler;  ///<the member function to call

  private:
	Event0& operator= (const Event0 &other);
};

/** Factory to create events without parameter
*/
template <class TContext, class R>
inline IEvent<TContext> * Event(TContext &context, int numberId, R(TContext::*handler)()) {
  return new Event0<TContext, R>(context, numberId, handler);
}

/** @class Event1
 *  @class Event1 is a template class for events with 1 parameter p�sed by value
 */
template <class TContext, class R, class P1>
class Event1 : public IEvent<TContext> {
    typedef R(TContext::*Signature)(P1);

  public:
    Event1(TContext &context, R(TContext::*handler)(P1), P1 p1);


    void dispatch();

  protected:
    TContext &m_context; ///< the class instance to use
    Signature m_handler;
    P1 myParam1;
};

/** Factory to create events with one parameter passed as value
*/
template <class TContext, class R, class P1>
inline IEvent<TContext> * Event(TContext &context, R(TContext::*handler)(P1), P1 p1) {
  return new Event1<TContext, R, P1>(context, handler, p1);
}

/** @class Event1Ref
  * @brief Event1Ref is a template class for events with 1 parameter passed by reference
  */
template <class TContext, class R, class P1>
class Event1Ref : public IEvent<TContext> {
    typedef R(TContext::*Signature)(P1&);

  public:
    Event1Ref(TContext &context, R(TContext::*handler)(P1&), P1 &p1);

    void dispatch();

  protected:
    TContext &m_context; ///< the class instance to use
    Signature m_handler;
    P1 &myParam1;

  private:
	Event1Ref& operator= (const Event1Ref &other);
};

/** Factory to create events with one parameter passed as a reference
 */
template <class TContext, class R, class P1>
inline IEvent<TContext> * Event(TContext &context, R(TContext::*handler)(P1&), P1 &p1) {
  return new Event1Ref<TContext, R, P1>(context, handler, p1);
}

/** @class Event2RR
 *  @class Event2RR is a template class for events with 2 parameters passed as references
 */
template <class TContext, class R, class P1, class P2>
class Event2RR : public IEvent<TContext> {
    typedef R(TContext::*Signature)(P1&, P2&);

  public:
    Event2RR(TContext &context, R(TContext::*handler)(P1&, P2&), P1 &p1, P2 &p2);

    void dispatch();

  protected:
    TContext &m_context; ///< the class instance to use
    Signature m_handler;
    P1 &myParam1;
    P2 &myParam2;
};

/** Factory to create events with 2 parameters passed as references
*/
template <class TContext, class R, class P1, class P2>
inline IEvent<TContext> * Event(TContext &context, R(TContext::*handler)(P1&, P2&), P1 &p1, P2 &p2) {
  return new Event2RR<TContext, R, P1, P2>(context, handler, p1, p2);
}


/** @class Event3Rvv
 *  @class Event3Rvv is a template class for events with 3 parameters passed as reference, value and value
 */
template <class TContext, class R, class P1, class P2, class P3>
class Event3Rvv : public IEvent<TContext> {
    typedef R(TContext::*Signature)(P1&, P2, P3);

  public:
    Event3Rvv(TContext &context, R(TContext::*handler)(P1&, P2, P3), P1 &p1, P2 p2, P3 p3);

    void dispatch();

  protected:
    TContext &m_context; ///< the class instance to use
    Signature m_handler;
    P1 &myParam1;
    P2 myParam2;
    P3 myParam3;
};

/** Factory to create events with 3 parameters passed as reference, value and value
*/
template <class TContext, class R, class P1, class P2, class P3>
inline IEvent<TContext> * Event(TContext &context, R(TContext::*handler)(P1&, P2, P3), P1 &p1, P2 p2, P3 p3) {
  return new Event3Rvv<TContext, R, P1, P2, P3>(context, handler, p1, p2, p3);
}


}
// namespace fsm

#endif
