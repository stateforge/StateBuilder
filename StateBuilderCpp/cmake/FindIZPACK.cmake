# - Locate IzPack, a cross-platform setup generator
# IzPack can be found at izpack.org
# Written by Frederic Heem, frederic.heem _at_ gmail.com

# This module defines
# IZPACK_COMPILE, where is the executable compile
# IZPACK2APP , where is izpack2app to generate a Mac app
# IZPACK_FOUND, If false, don't try to use IzPack 

FIND_PROGRAM(IZPACK_COMPILE 
  NAMES 
    compile
  PATHS
    "/Applications/IzPack/bin/"
    "C:/Program Files/IzPack/bin/"
    "C:/Program Files (x86)/IzPack/bin"
)

FIND_PROGRAM(IZPACK2APP 
  NAMES 
    izpack2app.py
    izpack2app.exe
  PATHS
    "/Applications/IzPack/utils/wrappers/izpack2app"
    "C:/Program Files/IzPack/utils/wrappers/izpack2app"
)

if("${IZPACK2APP}" MATCHES "izpack2app.py")
  set(IZPACK2APP_PYTHON python)
endif()

if(IZPACK_COMPILE)
  set(IZPACK_FOUND "YES")
endif(IZPACK_COMPILE)

MARK_AS_ADVANCED(
  IZPACK_COMPILE
) 

