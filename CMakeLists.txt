cmake_minimum_required(VERSION 2.6)
project(Infotaxis C CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBSERIAL REQUIRED libserial)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${LIBSERIAL_INCLUDE_DIRS}
)

LINK_DIRECTORIES(${LIBSERIAL_LIBRARY_DIRS})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall")
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED true)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_library(libmipex STATIC "src/libmipex.cpp")
target_link_libraries(libmipex ${LIBSERIAL_LIBRARIES})

add_executable (main "src/main.cpp")
target_link_libraries(main libmipex)
