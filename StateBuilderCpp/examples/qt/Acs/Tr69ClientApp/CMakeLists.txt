

include_directories( ${QTSOAP_INCLUDE_DIR}
                     ${QSLOG_INCLUDE_DIR}
                     ${SMQTUTILS_INCLUDE_DIR}
                     ${TR69CLIENTCORE_INCLUDE_DIR}
)

set(APPNAME Tr69ClientApp)
set(SRCS Tr69ClientApp.cpp )
set(HDRS Tr69ClientApp.h )
qt4_wrap_cpp(MOC_SRCS ${HDRS})
add_executable(${APPNAME} ${SRCS} ${MOC_SRCS})
target_link_libraries(${APPNAME} ${QT_LIBRARIES} ${TR69CLIENTCORE_LIBRARY})  
install(TARGETS ${APPNAME} RUNTIME DESTINATION bin)
