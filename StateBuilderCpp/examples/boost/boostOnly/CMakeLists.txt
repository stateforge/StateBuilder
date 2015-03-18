project(BOOSTONLY)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#Verbose make
#set(CMAKE_VERBOSE_MAKEFILE ON)

set(PROGRAM_NAME boostOnly)

if(APPLE OR WIN32)
  set(Boost_USE_STATIC_LIBS ON)
endif()

find_package(Boost COMPONENTS date_time filesystem system thread REQUIRED)

#ADD_DEFINITIONS(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})

include_directories(${Boost_INCLUDE_DIRS}
                    ${LOGLITE_INCLUDE_DIR})

link_directories(${Boost_LIBRARY_DIRS})

add_executable(${PROGRAM_NAME} main.cpp)

target_link_libraries(${PROGRAM_NAME}
  ${Boost_LIBRARIES}
)

# A warning is an error
if(MSVC) 
  ADD_DEFINITIONS( /W4 ) 
else(MSVC)  
  ADD_DEFINITIONS( -Wall)
endif(MSVC)

#Cmake bug with MinGW and Boost asio
if(MINGW)
  target_link_libraries(${PROGRAM_NAME} ws2_32)
ENDif()
