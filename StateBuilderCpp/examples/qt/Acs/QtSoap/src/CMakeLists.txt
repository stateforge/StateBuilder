set(QTSOAP_LIBRARY QtSoap CACHE INTERNAL "QtSoap library name")
set(QTSOAP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "QtSoap INCLUDE DIRECTORY")

find_package(Qt4 REQUIRED)
set(QT_DONT_USE_QTGUI 1)
set(QT_USE_QTNETWORK 1)
set(QT_USE_QTXML 1)
include(${QT_USE_FILE})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(QT_MOC_HDRS 
  qtsoap.h
)

set(QT_SRCS 
  qtsoap.cpp
)

qt4_wrap_cpp(QT_MOC_SRCS ${QT_MOC_HDRS})

add_library(${QTSOAP_LIBRARY} ${QT_SRCS} ${QT_MOC_SRCS})

target_link_libraries(${QTSOAP_LIBRARY} ${QT_LIBRARIES})

install(TARGETS ${QTSOAP_LIBRARY}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

