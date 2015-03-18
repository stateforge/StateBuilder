project(ACS)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# Where to find all FindXXX.cmake
set(CMAKE_MODULE_PATH $ENV{STATEBUILDERCPP_HOME}/share/cmake)

set(QT_USE_QTNETWORK TRUE)
set(QT_DONT_USE_QTGUI FALSE)
set(QT_DONT_USE_QTXML FALSE)

find_package(Qt4 REQUIRED) # find and setup Qt4
include(${QT_USE_FILE})

#ADD_DEFINITIONS(-Wall -Werror)

add_subdirectory(QHttpServer)
add_subdirectory(QtSoap)
add_subdirectory(Tr69Common)
add_subdirectory(AcsCore)
add_subdirectory(Tr69ClientCore)
add_subdirectory(Tr69ClientApp)
add_subdirectory(AcsApp)
add_subdirectory(AcsTest)
