//------------------------------------------------------------------------------
// <copyright file="IManagement.h" company="StateForge">
//      Copyright (c) 2010-2013 StateForge.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#ifndef FSM_IMANAGEMENT
#define FSM_IMANAGEMENT

#include <fsm/Definitions.h>

namespace fsm {

/** @class IManagement
 *  @brief IManagement is a abstract class use to monitor the fsm.
 *         The  main purpose is to find out when the fsm ends with success or error
*/
class FSM_EXPORT IManagement {
  public:

    virtual void OnEnd() = 0;
};


}

#endif
