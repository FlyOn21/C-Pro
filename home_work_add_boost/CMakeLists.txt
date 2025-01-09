cmake_minimum_required(VERSION 3.15)
project(BoostExample)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)
#set(Boost_DEBUG ON)
#set(Boost_DETAILED_FAILURE_MSG ON)

set(BOOST_ROOT "/usr/local/boost")
#set(BOOST_INCLUDEDIR "${BOOST_ROOT}")
#set(BOOST_LIBRARYDIR "${BOOST_ROOT}/stage/lib")

#include_directories(${BOOST_INCLUDEDIR})
set(BOOST_INCLUDE_LIBRARIES program_options)
set(BOOST_ENABLE_CMAKE ON)
add_subdirectory(${BOOST_ROOT} ${CMAKE_BINARY_DIR}/boost-build EXCLUDE_FROM_ALL)

set(Boost_DEBUG ON)
set(Boost_DETAILED_FAILURE_MSG ON)

find_package(Boost REQUIRED COMPONENTS program_options)

if(Boost_FOUND)
    message(STATUS "Boost found:")
    message(STATUS "  Version: ${Boost_VERSION}")
    message(STATUS "  Include dirs: ${Boost_INCLUDE_DIRS}")
    message(STATUS "  Library dirs: ${Boost_LIBRARY_DIRS}")
    message(STATUS "  Libraries: ${Boost_LIBRARIES}")
    include_directories(${Boost_INCLUDE_DIRS})
#    link_directories(${Boost_LIBRARY_DIRS})
else()
    message(FATAL_ERROR "Could not find Boost!")
endif()

add_executable(SystemInfo main.cpp)

target_link_libraries(SystemInfo
        PRIVATE
        Boost::program_options
)

target_include_directories(SystemInfo
        PRIVATE
        ${Boost_INCLUDE_DIRS}
)

#cmake_minimum_required(VERSION 3.15)
#project(BoostExample)
#
#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#
## Boost configuration flags
#set(BOOST_INCLUDE_LIBRARIES program_options)
#set(BOOST_ENABLE_CMAKE ON)
#set(Boost_DEBUG ON)
#
## Set correct path to Boost
#set(BOOST_ROOT "/home/flyon21/CLionProjects/C-Pro/external/boost")
#
## Add Boost from submodule with correct path
#add_subdirectory(${BOOST_ROOT} ${CMAKE_BINARY_DIR}/boost-build  EXCLUDE_FROM_ALL)
#
## Create executable
#add_executable(SystemInfo main.cpp)
#
## Link and include
#target_link_libraries(SystemInfo
#        PRIVATE
#        Boost::program_options
#)
#
## Status messages for debugging
#if(TARGET Boost::boost)
#    get_target_property(BOOST_VERSION Boost::boost VERSION)
#    message(STATUS "Boost Status:")
#    message(STATUS "  Version: ${BOOST_VERSION}")
#
#    if(TARGET Boost::program_options)
#        get_target_property(OPTS_INCLUDE_DIRS Boost::program_options INTERFACE_INCLUDE_DIRECTORIES)
#        get_target_property(OPTS_LIBRARY_NAME Boost::program_options IMPORTED_LOCATION_RELEASE)
#        message(STATUS "  Program Options Include dirs: ${OPTS_INCLUDE_DIRS}")
#        message(STATUS "  Program Options Library: ${OPTS_LIBRARY_NAME}")
#    endif()
#endif()