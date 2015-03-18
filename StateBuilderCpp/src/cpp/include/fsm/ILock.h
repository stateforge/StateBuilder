//------------------------------------------------------------------------------
// <copyright file="ILock.h" company="StateForge">
//      Copyright (c) 2010-2013 StateForge.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#ifndef _FSM_ILOCK_H_
#define _FSM_ILOCK_H_

#include <fsm/Definitions.h>

namespace fsm {

/** @class ILock
  * @brief ILock is an interface that all controlled object must implement in case multithreading is used
    */

class ILock {
  public:
    virtual void Lock(void) = 0;
    virtual void UnLock(void) = 0; 
};

}

#endif
