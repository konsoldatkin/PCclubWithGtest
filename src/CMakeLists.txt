cmake_minimum_required(VERSION 3.16)

project(PCclub VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
    message(STATUS "CMAKE_BUILD_TYPE is not defined. ${CMAKE_BUILD_TYPE} will be used.")
endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/bin)

add_executable(PCclub main.cpp)

add_subdirectory(core)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    add_subdirectory(test_PCclub)
endif()

if (NOT googletest)
    target_link_libraries(${PROJECT_NAME} core)
    
    include(FetchContent)
    FetchContent_Declare(googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG main)

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

endif()

