cmake_minimum_required(VERSION 2.6)
project(raytracing)

# Prepare the build environment
set(SHARED_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/shared/")
include_directories(${LIBIGL_INCLUDE_DIRS})
add_definitions(${LIBIGL_DEFINITIONS})
option(USE_SOLUTION "Use solution"  OFF)
option(BUILD_TEST "Build test"  OFF)
# Add your project files
include_directories("include/")
if(USE_SOLUTION)
  set(SRC_DIR "solution/")
else()
  set(SRC_DIR "src/")
endif()
file(GLOB SRCFILES "${SRC_DIR}/*.cpp")

if(BUILD_TEST)
  list(APPEND SRCFILES test/main.cpp)
else()
  list(APPEND SRCFILES main.cpp)
endif()
# create executable
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pthread")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

add_executable(${PROJECT_NAME} ${SRCFILES} ${LIBIGL_EXTRA_SOURCES})
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${SHARED_ROOT}/eigen ${SHARED_ROOT}/json)
target_link_libraries(${PROJECT_NAME} ${LIBIGL_LIBRARIES} ${LIBIGL_EXTRA_LIBRARIES})

