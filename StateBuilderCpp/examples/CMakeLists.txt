
enable_testing()

add_subdirectory(unitest)

add_subdirectory(stdcpp)

#Boost
option(STATEBUILDERCPP_EXAMPLE_BOOST "Build Boost examples" OFF)
if(STATEBUILDERCPP_EXAMPLE_BOOST)
  find_package(Boost COMPONENTS date_time filesystem system)
  
  if(STATEBUILDERCPP_EXAMPLE_BOOST AND Boost_FOUND)
    add_subdirectory(boost)
  endif(STATEBUILDERCPP_EXAMPLE_BOOST AND Boost_FOUND)
endif()
#Qt
option(STATEBUILDERCPP_EXAMPLE_QT "Build Qt examples" OFF)
if(STATEBUILDERCPP_EXAMPLE_QT)
  find_package(Qt4 COMPONENTS QtCore QtGui)

  if(STATEBUILDERCPP_EXAMPLE_QT AND QT_FOUND)
    add_subdirectory(qt)
  else()
    message(STATUS "Qt not found, Qt examples will not be compled")
  endif(STATEBUILDERCPP_EXAMPLE_QT AND QT_FOUND)
endif()

#Libev
option(STATEBUILDERCPP_EXAMPLE_LIBEV "Build libev examples" OFF)

if(STATEBUILDERCPP_EXAMPLE_LIBEV)
  add_subdirectory(libev)
endif()