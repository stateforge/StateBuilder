//------------------------------------------------------------------------------
// <copyright file="Definitions.h" company="StateForge">
//      Copyright (c) 2010-2013 StateForge.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------
#ifndef _FSM_DEFINITION_H_
#define _FSM_DEFINITION_H_

namespace fsm {
#if defined(WIN32) && defined(FSMCPP_BUILD_SHARED_LIBS)  
   #if defined(fsmcpp_EXPORTS) 
     #define FSM_EXPORT __declspec(dllexport) 
   #else
     #ifdef _MSC_VER
     #pragma comment (lib, "fsmcpp.lib")
     #endif // #ifdef _MSC_VER
     #define FSM_EXPORT __declspec(dllimport) 
   #endif /* fsmcpp_EXPORTS */ 
#else 
   #define FSM_EXPORT 
#endif 

    
/** ProcessReturn enumerates all possibile return values for ProcessEvent()
  */
typedef enum {
  ProcessError = -1,
  ProcessRunning = 0,
  ProcessEnd = 1
} ProcessReturn;

}
// namespace fsm

#endif // #ifndef FSM_DEFINITION_H
