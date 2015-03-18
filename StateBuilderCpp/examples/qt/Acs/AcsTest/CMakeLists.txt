
find_package(Qt4 COMPONENTS QtCore QtGui QtXml QtTest REQUIRED )
include(${QT_USE_FILE})

include_directories( ${QTSOAP_INCLUDE_DIR}
                     ${QSLOG_INCLUDE_DIR}
                     ${SMQTUTILS_INCLUDE_DIR}
                     ${TR69COMMON_INCLUDE_DIR}
                     ${TR69CLIENTCORE_INCLUDE_DIR}
                     ${ACSCORE_INCLUDE_DIR}
)

# add the target test: "make test" or ctest
set(CTEST_OUTPUT_ON_FAILURE TRUE)
enable_testing()

set(APPNAME AcsTest)
set(SRCS AcsTest.cpp )
set(HDRS AcsTest.h )
qt4_wrap_cpp(MOC_SRCS ${HDRS})
add_executable(${APPNAME} ${SRCS} ${QT_MOC_HDRS} ${MOC_SRCS})
target_link_libraries(${APPNAME} 
    ${QT_LIBRARIES}
    ${ACSCORE_LIBRARY}
    ${TR69COMMON_LIBRARY}
    ${TR69CLIENTCORE_LIBRARY}
    ${QSLOG_LIBRARY} 
    ${SMQTUTILS_LIBRARY} 
)  
install(TARGETS ${APPNAME} RUNTIME DESTINATION bin)
add_test(${APPNAME} ${APPNAME})

