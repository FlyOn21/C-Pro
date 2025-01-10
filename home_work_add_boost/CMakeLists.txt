cmake_minimum_required(VERSION 3.15)
project(BoostExample)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BOOST_ROOT "${CMAKE_SOURCE_DIR}/../external/boost")

set(BOOST_INCLUDE_LIBRARIES program_options)
set(BOOST_ENABLE_CMAKE ON)
add_subdirectory(${BOOST_ROOT} "${CMAKE_BINARY_DIR}/boost-build" EXCLUDE_FROM_ALL)

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
