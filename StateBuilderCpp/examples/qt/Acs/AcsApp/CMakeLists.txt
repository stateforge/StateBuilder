# A collection of Acs application: reboot, download, get/value etc ...

include_directories( ${QTSOAP_INCLUDE_DIR}
                     ${ACSCORE_INCLUDE_DIR}
                     ${TR69COMMON_INCLUDE_DIR}
                     ${TR69CLIENTCORE_INCLUDE_DIR}
)

# add the target test: "make test" or ctest
set(CTEST_OUTPUT_ON_FAILURE TRUE)
enable_testing()

#Build the common application library 
set(ACSAPPCOMMON_SRCS AcsAppBase.cpp)
set(ACSAPPCOMMON_MOC_HDRS AcsAppBase.h)
qt4_wrap_cpp(ACSAPPCOMMON_MOC_SRCS ${ACSAPPCOMMON_MOC_HDRS})
add_library(acsapp ${ACSAPPCOMMON_SRCS} ${ACSAPPCOMMON_MOC_SRCS} )
target_link_libraries(acsapp 
                      ${ACSCORE_LIBRARY} 
                      ${TR69COMMON_LIBRARY}
                      ${TR69CLIENTCORE_LIBRARY}
                      ${QT_LIBRARIES})  

install(TARGETS acsapp
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

#Reboot
set(ACSREBOOT_APPNAME AcsReboot)
set(ACSREBOOT_SRCS AcsReboot.cpp )
set(ACSREBOOT_HDRS AcsReboot.h )
qt4_wrap_cpp(ACSREBOOT_MOC_SRCS ${ACSREBOOT_HDRS})
add_executable(${ACSREBOOT_APPNAME} ${ACSREBOOT_SRCS} ${ACSREBOOT_MOC_SRCS})
target_link_libraries(${ACSREBOOT_APPNAME} ${QT_LIBRARIES} acsapp)  
install(TARGETS ${ACSREBOOT_APPNAME} RUNTIME DESTINATION bin)

#Download
set(ACSDOWNLOAD_APPNAME AcsDownload)
set(ACSDOWNLOAD_SRCS AcsDownload.cpp)
set(ACSDOWNLOAD_HDRS AcsDownload.h )
qt4_wrap_cpp(ACSDOWNLOAD_MOC_SRCS ${ACSDOWNLOAD_HDRS})
add_executable(${ACSDOWNLOAD_APPNAME} ${ACSDOWNLOAD_SRCS} ${ACSDOWNLOAD_MOC_SRCS})
target_link_libraries(${ACSDOWNLOAD_APPNAME} ${QT_LIBRARIES} acsapp)  
install(TARGETS ${ACSDOWNLOAD_APPNAME} RUNTIME DESTINATION bin)

# #SetValue
# set(ACSSETVALUE_SRCS AcsSetValue.cpp)
# qt4_automoc(${ACSSETVALUE_SRCS})
# add_executable(AcsSetValue ${ACSSETVALUE_SRCS})
# target_link_libraries(AcsSetValue ${QT_LIBRARIES} acs)  
# install(TARGETS AcsSetValue RUNTIME DESTINATION bin)
# 
# #GetValue
# set(ACSGETVALUE_SRCS AcsGetValue.cpp)
# set(ACSGETVALUE_MOC_HDRS AcsGetValue.h)
# qt4_wrap_cpp(ACSGETVALUE_MOC_SRCS ${ACSGETVALUE_MOC_HDRS})
# add_executable(AcsGetValue ${ACSGETVALUE_SRCS} ${ACSGETVALUE_MOC_SRCS})
# target_link_libraries(AcsGetValue ${QT_LIBRARIES} acs)
# install(TARGETS AcsGetValue RUNTIME DESTINATION bin)
