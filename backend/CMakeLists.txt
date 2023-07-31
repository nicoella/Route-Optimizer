cmake_minimum_required(VERSION 3.0)

project(server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

SET (BOOST_ROOT "/usr/local/Cellar/boost/1.82.0")
SET (BOOST_INCLUDEDIR "/usr/local/Cellar/boost/1.82.0/include")
SET (BOOST_LIBRARYDIR "/usr/local/Cellar/boost/1.82.0/lib")

SET (BOOST_MIN_VERSION "1.82.0")
set (Boost_NO_BOOST_CMAKE ON)
FIND_PACKAGE(Boost ${BOOST_MIN_VERSION} REQUIRED)
if (NOT Boost_FOUND)
  message(FATAL_ERROR "Fatal error: Boost (version >= 1.82) required.")
else()
  message(STATUS "Setting up BOOST")
  message(STATUS " Includes - ${Boost_INCLUDE_DIRS}")
  message(STATUS " Library  - ${Boost_LIBRARY_DIRS}")
  include_directories(${Boost_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIRS})
endif (NOT Boost_FOUND)

add_executable(server src/main.cpp src/http_server.hpp src/held_karp.cpp)
target_link_libraries(server PRIVATE ${BOOST_BEAST_INCLUDE_DIRS})
