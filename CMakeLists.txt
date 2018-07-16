cmake_minimum_required(VERSION 2.6)
project(mydumper)
set(VERSION 0.9.5)
set(ARCHIVE_NAME "${CMAKE_PROJECT_NAME}-${VERSION}")

#Required packages
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
find_package(MySQL)
find_package(ZLIB)
find_package(GLIB2)
find_package(PCRE)

option(BUILD_DOCS "Build the documentation" ON)

if (BUILD_DOCS)
  add_subdirectory(docs)
endif (BUILD_DOCS)

option(WITH_BINLOG "Build binlog dump options" OFF)
option(WITH_SSL "Build SSL support" ON)

set(CMAKE_C_FLAGS "-Wall -Wno-deprecated-declarations -Wunused -Wwrite-strings -Wno-strict-aliasing -Wextra -Wshadow -Werror -O3 -g ${MYSQL_CFLAGS}")

include_directories(${MYDUMPER_SOURCE_DIR} ${MYSQL_INCLUDE_DIR} ${GLIB2_INCLUDE_DIR} ${PCRE_INCLUDE_DIR} ${ZLIB_INCLUDE_DIRS})

if (NOT CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local" CACHE STRING "Install path" FORCE)
endif (NOT CMAKE_INSTALL_PREFIX)
MARK_AS_ADVANCED(CMAKE)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h)


if (WITH_BINLOG)

  add_executable(__exec_mydumper mydumper.c binlog.c server_detect.c g_unix_signal.c connection.c getPassword.c parser.c)
else (WITH_BINLOG)
  add_executable(__exec_mydumper mydumper.c server_detect.c g_unix_signal.c connection.c getPassword.c parser.c)
endif (WITH_BINLOG)
target_link_libraries(__exec_mydumper ${MYSQL_LIBRARIES} ${GLIB2_LIBRARIES} ${GTHREAD2_LIBRARIES} ${PCRE_PCRE_LIBRARY} ${ZLIB_LIBRARIES} stdc++)


add_executable(__exec_myloader myloader.c connection.c getPassword.c)
target_link_libraries(__exec_myloader ${MYSQL_LIBRARIES} ${GLIB2_LIBRARIES} ${GTHREAD2_LIBRARIES} ${PCRE_PCRE_LIBRARY} ${ZLIB_LIBRARIES} stdc++)

INSTALL(TARGETS __exec_mydumper __exec_myloader
  RUNTIME DESTINATION bin
)

add_custom_target(dist
  COMMAND bzr export --root=${ARCHIVE_NAME}
    ${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.tar.gz
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

OPTION(RUN_CPPCHECK "Run cppcheck" OFF)

IF(RUN_CPPCHECK)
  include(CppcheckTargets)
  add_cppcheck(__exec_mydumper)
  add_cppcheck(__exec_myloader)
ENDIF(RUN_CPPCHECK)


MESSAGE(STATUS "------------------------------------------------")
MESSAGE(STATUS "MYSQL_CONFIG = ${MYSQL_CONFIG}")
MESSAGE(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
MESSAGE(STATUS "BUILD_DOCS = ${BUILD_DOCS}")
MESSAGE(STATUS "WITH_BINLOG = ${WITH_BINLOG}")
MESSAGE(STATUS "RUN_CPPCHECK = ${RUN_CPPCHECK}")
MESSAGE(STATUS "Change a values with: cmake -D<Variable>=<Value>")
MESSAGE(STATUS "------------------------------------------------")
MESSAGE(STATUS)

