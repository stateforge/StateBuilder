//------------------------------------------------------------------------------
// <copyright file="IContext.h" company="StateForge">
//      Copyright (c) 2010-2015 StateForge.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#ifndef FSM_ICONTEXT_H
#define FSM_ICONTEXT_H

#ifndef STATEFORGE_NO_PERSISTENCE
#include <sstream>
#endif 

#include <list>   // std::list

#ifdef STATEFORGE_OBSERVER_NO
#elif defined STATEFORGE_OBSERVER_INT
#include <fsm/IObserverInt.h>    
#else
#include <fsm/IObserver.h>    
#endif

#include <fsm/Definitions.h>
#include <fsm/ILock.h>
#include <fsm/IManagement.h>


namespace fsm {

/** @class IContext 
*/

class IContext {

public:
    virtual ~IContext() {}
    virtual ProcessReturn ProcessEvent(long &nextTick) {nextTick = -1;return ProcessError;}

    virtual void AddChild(IContext *pContextChild) = 0;

#ifdef STATEFORGE_OBSERVER_NO
#elif defined STATEFORGE_OBSERVER_INT
    virtual void SetObserverInt(fsm::IObserverInt &observer) = 0;
#else
    virtual void SetObserver(fsm::IObserver &observer) = 0;
#endif

    virtual void Lock(){}
    virtual void UnLock(){}
    virtual void SetLock(ILock*) = 0;

#ifndef STATEFORGE_NO_PERSISTENCE    
    virtual void Serialize(std::ostream &os) const = 0;
    virtual void UnSerialize(std::istream &is) = 0;
#endif /* STATEFORGE_NO_PERSISTENCE */
};

} //namespace fsm {
#endif
