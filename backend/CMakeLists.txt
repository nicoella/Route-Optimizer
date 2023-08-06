cmake_minimum_required(VERSION 3.0)

project(server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BOOST_ROOT "/usr/local/Cellar/boost/1.82.0") # update path if required
set(OPENSSL_ROOT "/usr/local/Cellar/openssl@3/3.1.2") # update path if required
set(BOOST_INCLUDE ${BOOST_ROOT}/include) 
set(OPENSSL_INCLUDE ${OPENSSL_ROOT}/include) 
set(BOOST_LIBS ${BOOST_ROOT}/lib) 
set(OPENSSL_LIBS ${OPENSSL_ROOT}/lib) 

find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)

include_directories(${BOOST_INCLUDE})
include_directories(${OPENSSL_INCLUDE})

file(GLOB BOOST_LIBRARIES ${BOOST_LIBS}/*.a)
file(GLOB OPENSSL_LIBRARIES ${OPENSSL_LIBS}/*.dylib)

add_executable(server src/main.cpp src/http_server.hpp src/held_karp.cpp src/utils.hpp src/search.hpp)
target_link_libraries(server PRIVATE 
  ${BOOST_LIBRARIES}
  ${OPENSSL_LIBRARIES}
)