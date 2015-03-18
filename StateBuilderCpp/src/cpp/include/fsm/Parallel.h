//------------------------------------------------------------------------------
// <copyright file="Parallel.h" company="StateForge">
//      Copyright (c) 2010-2013 StateForge.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#ifndef FSM_PARALLEL_H
#define FSM_PARALLEL_H

#include <cassert>
#include <list>   // std::list
#include <algorithm> //std::min

#include <fsm/Definitions.h>
#include <fsm/ILock.h>
#include <fsm/IContext.h>

namespace fsm {

class IObserver;
class ILock;

/** @class Parallel
* @brief Parallel implements IContext.
*       
*        ProcessEvent() shall invoke  ProcessEvent() for all orthogonal states owns by the parallel state
*/

class Parallel {

public:

    virtual ~Parallel() {}

    /** Process events for all subcontexts, 
    * as soon as a context returns an error, sucessive contexts are not processed
    * @return  ProcessRunning or ProcessError
    */
    ProcessReturn ProcessEvent(long &nextTick) {
        for (ContextList::iterator it = m_contextList.begin(); it != m_contextList.end(); ++it) {
            long nextTickChild = nextTick;
            ProcessReturn eProcessReturn = (*it)->ProcessEvent(nextTickChild);
            nextTick = (std::min)(nextTickChild, nextTick);
            if (eProcessReturn == ProcessError) {
                //Do not try to process sucessive context as the parallel pointer will be set to NULL  
                return ProcessError;
            }
        }
        return ProcessRunning;
    }

    /** Get the number of active state
    */
    int GetStateActive() const {
        return m_iStateActive;
    }

    /** Set the number of active state
    */
    void SetStateActive(int iStateActive) {
        m_iStateActive = iStateActive;
    }

    /** Decrement the number of active state
    */
    void StateActiveDecrement() {
        assert(m_iStateActive > 0);
        m_iStateActive--;
    }

    virtual void TransitionToNextState() = 0;

protected :

    int m_iStateActive;    ///< number of active state
    //List of orthogonal contexts
    //TODO use IContextAsync instead of IContext
    typedef std::list<IContext*> ContextList;
    ContextList m_contextList; /// List of subcontext
};

}

#endif
