set(QHTTPSERVER_LIBRARY QHttpServer CACHE INTERNAL "QHttpServer library name")
set(QHTTPSERVER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "QHttpServer INCLUDE DIRECTORY")

find_package(Qt4 REQUIRED)
set(QT_DONT_USE_QTGUI 1)
set(QT_USE_QTNETWORK 1)
include(${QT_USE_FILE})

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ../http-parser  
)

set(QT_MOC_HDRS 
  qhttpconnection.h
  qhttprequest.h
  qhttpresponse.h
  qhttpserver.h
)

set(QT_SRCS 
  qhttpconnection.cpp
  qhttprequest.cpp
  qhttpresponse.cpp
  qhttpserver.cpp
  ../http-parser/http_parser.c
)

qt4_wrap_cpp(QT_MOC_SRCS ${QT_MOC_HDRS})

add_library(${QHTTPSERVER_LIBRARY} ${QT_SRCS} ${QT_MOC_SRCS})

target_link_libraries(${QHTTPSERVER_LIBRARY} ${QT_LIBRARIES})

install(TARGETS ${QHTTPSERVER_LIBRARY}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

