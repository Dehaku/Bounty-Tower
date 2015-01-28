project(Galaxy)
cmake_minimum_required(VERSION 2.8.12)

# Detect compiler
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(USING_GCC TRUE)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(USING_CLANG TRUE)
endif()

# OS specific configuration
if(UNIX)
    add_definitions(-DGALAXY_LINUX)
endif()

# For GCC and Clang, enable C++14 support and add some other flags
if(USING_GCC OR USING_CLANG)
  add_compile_options(-std=c++14 -pedantic -Wall)
  if(SANITIZE)
    add_compile_options(-fsanitize=${SANITIZE})
    set(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=${SANITIZE}")
  endif()
endif()

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)
find_package(SFML 2 COMPONENTS graphics window audio system REQUIRED)
include_directories(${SFML_INCLUDE_DIR})

file(GLOB_RECURSE SOURCE_FILES src/*)
add_executable(galaxy ${SOURCE_FILES})

target_link_libraries(galaxy ${SFML_LIBRARIES})
