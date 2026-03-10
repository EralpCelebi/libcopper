cmake_minimum_required(VERSION 3.20.1)
project(LibCopper LANGUAGES ASM C VERSION 1.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CMakeDependentOption)

option(
        CONFIG_PROVIDE_ALLOCATOR 
        "Feature: Expose allocator bindings and provide local arena allocator." ON)

cmake_dependent_option(
        CONFIG_PROVIDE_STANDARD 
        "Feature: Provide standalone standard library features." ON 
        "CONFIG_PROVIDE_ALLOCATOR" OFF)

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Set the build type.")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Release;Debug")

set(
        CONFIG_ALLOCATOR_LOCAL_SIZE "4096"
        CACHE STRING "Option: Sets the default local allocator buffer size.")
set_property(CACHE CONFIG_ALLOCATOR_LOCAL_SIZE PROPERTY STRINGS "512;1024;2048;4096")

configure_file(include/copper.h.in include/defconfig.h)

add_compile_options(-Wall -Wextra -Wpedantic -Werror -ffreestanding -std=c11 -fno-stack-protector)

add_library(copper STATIC src/copper.c)
target_include_directories(copper PUBLIC include)
target_include_directories(copper PRIVATE ${PROJECT_BINARY_DIR}/include)

if(CONFIG_PROVIDE_ALLOCATOR)
        target_sources(copper PRIVATE src/alloc.c)
endif()

if(CONFIG_PROVIDE_STANDARD)
        target_sources(copper PRIVATE src/slibc.c)
endif()

add_executable(main src/main.c)
target_link_libraries(main copper)
target_include_directories(main PRIVATE ${PROJECT_BINARY_DIR}/include)
