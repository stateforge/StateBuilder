################################################################################
#Installation
################################################################################

set(FSMCPP_INCLUDE_FILE 
  Context.h
  Context.hpp  
  ContextBase.h  
  ContextAsync.h
  ContextAsync.hpp
  IContextAsync.h    
  Definitions.h  
  Event.h
  Event.hpp
  Exception.h 
  Fsm.h 
  Fsm.hpp
  FsmAsync.h 
  FsmAsync.hpp
  IObserver.h
  ObserverStd.h
  Parallel.h
  StateBase.h
  State.h
  State.hpp
  Timer.h
  Timer.hpp
  ILock.h   
)

# Typically to C:\Program Files\StateBuilderCpp-1.2.3\include\fsm on windows
# Typically to /usr/include/StateBuilderCpp-1.2.3/fsm on Linux
# INSTALL(FILES ${FSMCPP_INCLUDE_FILE} 
#         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fsm 
#         COMPONENT Devel)


