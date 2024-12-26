cmake_minimum_required(VERSION 3.10)
project(CustomSharedPTRWithExample)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_library(CustomSharedPTR INTERFACE)
target_include_directories(CustomSharedPTR
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
add_library(CustomSharedPTR_static STATIC
        include/custom_shared_ptr.hpp
        src/custom_shared_ptr.cpp
)
target_include_directories(CustomSharedPTR_static
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
set_target_properties(CustomSharedPTR_static PROPERTIES
        OUTPUT_NAME "custom_shared_ptr_static"
)
add_library(CustomSharedPTR_shared SHARED
        include/custom_shared_ptr.hpp
        src/custom_shared_ptr.cpp
)
target_include_directories(CustomSharedPTR_shared
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
set_target_properties(CustomSharedPTR_shared PROPERTIES
        OUTPUT_NAME "custom_shared_ptr_shared"
)
install(TARGETS CustomSharedPTR CustomSharedPTR_static CustomSharedPTR_shared
        EXPORT CustomSharedPTRTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(FILES include/custom_shared_ptr.hpp
        DESTINATION include
)
add_executable(CustomSharedPTRExample src/main.cpp)
target_link_libraries(CustomSharedPTRExample PRIVATE CustomSharedPTR)