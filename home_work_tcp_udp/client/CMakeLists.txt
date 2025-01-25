cmake_minimum_required(VERSION 3.15)
project(BoostExample)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BOOST_ROOT "${CMAKE_SOURCE_DIR}/../../external/boost")

set(BOOST_INCLUDE_LIBRARIES system)
set(BOOST_ENABLE_CMAKE ON)
add_subdirectory(${BOOST_ROOT} "${CMAKE_BINARY_DIR}/boost-build" EXCLUDE_FROM_ALL)

set(Boost_DEBUG ON)
set(Boost_DETAILED_FAILURE_MSG ON)

find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)

if(Boost_FOUND)
    message(STATUS "Boost found:")
    message(STATUS "  Version: ${Boost_VERSION}")
    message(STATUS "  Include dirs: ${Boost_INCLUDE_DIRS}")
    message(STATUS "  Library dirs: ${Boost_LIBRARY_DIRS}")
    message(STATUS "  Libraries: ${Boost_LIBRARIES}")
    message(STATUS "  OpenSSL Include dirs: ${OPENSSL_INCLUDE_DIR}")
    message(STATUS "  OpenSSL Libraries: ${OPENSSL_LIBRARIES}")
    include_directories(${Boost_INCLUDE_DIRS})
    include_directories(${OPENSSL_INCLUDE_DIR})
else()
    message(FATAL_ERROR "Could not find Boost or OpenSSL!")
endif()

add_executable(TCPClient client_tcp.cpp)

target_link_libraries(TCPClient
        PRIVATE
        Boost::system
        OpenSSL::SSL
        OpenSSL::Crypto
)
target_include_directories(TCPClient
        PRIVATE
        ${Boost_INCLUDE_DIRS}
)
