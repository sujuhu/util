
cmake_minimum_required(VERSION 2.4)

PROJECT(util)

SET(SRC_LIST
filemap.cpp
pipe.cpp
strhelp.cpp
inject.cpp
memhelp.cpp
)

ADD_LIBRARY(util_static STATIC ${SRC_LIST})
SET_TARGET_PROPERTIES(util_static PROPERTIES OUTPUT_NAME "util" PREFIX "lib")
SET_TARGET_PROPERTIES(util_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

#install
INSTALL(TARGETS util_static ARCHIVE DESTINATION lib)
INSTALL(FILES filemap.h DESTINATION include)
INSTALL(FILES pipe.h DESTINATION include)
INSTALL(FILES strhelp.h DESTINATION include)
INSTALL(FILES pshpack1.h DESTINATION include)
INSTALL(FILES pshpack2.h DESTINATION include)
INSTALL(FILES pshpack4.h DESTINATION include)
INSTALL(FILES pshpack8.h DESTINATION include)
INSTALL(FILES poppack.h DESTINATION include)
