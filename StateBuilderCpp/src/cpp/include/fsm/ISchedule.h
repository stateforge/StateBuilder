//------------------------------------------------------------------------------
// <copyright file="ISchedule.h" company="StateForge">
//      Copyright (c) 2010-2013 StateForge.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#ifndef FSM_SCHEDULE_H
#define FSM_SCHEDULE_H

#include <fsm/Definitions.h>

namespace fsm {

/** @class ISchedule
  * @brief ISchedule is the interface which allow the fsm to inform the application to schedule the machine
  */
class ISchedule {
  public:
    virtual void Schedule(long duration) = 0;
};

}

#endif // namespace fsm {
