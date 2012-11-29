
cmake_minimum_required(VERSION 2.4)

PROJECT(util)

SET(SRC_LIST
filemap.cpp
strhelp.cpp
memhelp.cpp
strconv.cpp
)

ADD_LIBRARY(util STATIC ${SRC_LIST})
SET_TARGET_PROPERTIES(util PROPERTIES OUTPUT_NAME "util")
SET_TARGET_PROPERTIES(util PROPERTIES CLEAN_DIRECT_OUTPUT 1)

#install
INSTALL(TARGETS util ARCHIVE DESTINATION lib)
INSTALL(FILES filemap.h DESTINATION include)
INSTALL(FILES strconv.h DESTINATION include)
INSTALL(FILES strhelp.h DESTINATION include)
INSTALL(FILES pshpack1.h DESTINATION include)
INSTALL(FILES pshpack2.h DESTINATION include)
INSTALL(FILES pshpack4.h DESTINATION include)
INSTALL(FILES pshpack8.h DESTINATION include)
INSTALL(FILES poppack.h DESTINATION include)
