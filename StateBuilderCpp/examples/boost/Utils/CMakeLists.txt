set(SMBOOSTUTILS_LIBRARY SmBoostUtils CACHE INTERNAL "SMQTUTILS LIBRARY NAME")
set(SMBOOSTUTILS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "SMBOOSTRUTILS INCLUDE DIRECTORY")


if(APPLE OR WIN32)
  set(Boost_USE_STATIC_LIBS ON)
endif()

find_package(Boost COMPONENTS date_time filesystem thread system REQUIRED)

set(SMBOOSTUTILS_SRCS
  SmBoostObserver.cpp
)

include_directories(${STATEBUILDERCPP_INCLUDE_DIR} ${LOGLITE_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})

link_directories(${Boost_LIBRARY_DIRS} )


add_library(${SMBOOSTUTILS_LIBRARY} STATIC ${SMBOOSTUTILS_SRCS} ) 

target_link_libraries(${SMBOOSTUTILS_LIBRARY} 
  ${Boost_THREAD_LIBRARY}
  ${Boost_DATE_TIME_LIBRARY}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
)


# A warning is an error
if(MSVC) 
  add_definitions( /W4 ) 
else (MSVC)  
  add_definitions( -Wall)
endif (MSVC)

