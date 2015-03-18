//------------------------------------------------------------------------------
// <copyright file="IContextAsync.h" company="StateForge">
//      Copyright (c) 2010-2013 StateForge.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#ifndef _FSM_ICONTEXTASYNC_H_
#define _FSM_ICONTEXTASYNC_H_

#include <fsm/Definitions.h>
#include <fsm/IContext.h>

namespace fsm {

/** @class IContextAsync
  * @brief IContextAsync is the interface for an asynchronous context.
  * It provides the ProcessEvent() function that has to be called on a regular basis
  */
class IContextAsync {
  public:
    virtual ~IContextAsync(){}
    virtual ProcessReturn ProcessEvent(long &nextTick) = 0;
  protected:
      IContextAsync(){}
};

}

#endif // namespace fsm {
