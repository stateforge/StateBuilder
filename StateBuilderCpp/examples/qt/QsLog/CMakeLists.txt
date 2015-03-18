set(QSLOG_LIBRARY QsLog CACHE INTERNAL "QsLog library name")
set(QSLOG_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "QsLog include directory")
find_package(Qt4 COMPONENTS QtCore REQUIRED)

set(QSLOG_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

include(${QT_USE_FILE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${QT_INCLUDE_DIRS})

set(QSLOG_SRCS QsDebugOutput.cpp QsLog.cpp QsLogDest.cpp)

qt4_automoc(${QSLOG_SRCS})

link_directories(${QT_LIBRARY_DIRS})

add_library(${QSLOG_LIBRARY} STATIC ${QSLOG_SRCS})

target_link_libraries(${QSLOG_LIBRARY} ${QT_LIBRARIES})
