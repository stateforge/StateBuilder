
enable_testing()

add_subdirectory(unitest)

add_subdirectory(stdcpp)

#Boost
option(STATEBUILDERCPP_EXAMPLE_BOOST "Build Boost examples" ON)

find_package(Boost COMPONENTS date_time filesystem system)

if(STATEBUILDERCPP_EXAMPLE_BOOST AND Boost_FOUND)
  add_subdirectory(boost)
endif(STATEBUILDERCPP_EXAMPLE_BOOST AND Boost_FOUND)

#Qt
option(STATEBUILDERCPP_EXAMPLE_QT "Build Qt examples" ON)

find_package(Qt4 COMPONENTS QtCore QtGui)

if(STATEBUILDERCPP_EXAMPLE_QT AND QT_FOUND)
  add_subdirectory(qt)
else()
  message(STATUS "Qt not found, Qt examples will not be compled")
endif(STATEBUILDERCPP_EXAMPLE_QT AND QT_FOUND)
 
#Libev
option(STATEBUILDERCPP_EXAMPLE_LIBEV "Build libev examples" OFF)

if(STATEBUILDERCPP_EXAMPLE_LIBEV)
  add_subdirectory(libev)
endif()