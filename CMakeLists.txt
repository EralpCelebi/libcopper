cmake_minimum_required(VERSION 3.20.1)
project(LibCopper LANGUAGES ASM C VERSION 1.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CMakeDependentOption)
option(CONFIG_PROVIDE_STANDARD "Feature: Provide standalone standard library features." ON)

configure_file(include/copper.h.in include/copper.h)

add_compile_options(-Wall -Wextra -Wpedantic -Werror -ffreestanding -std=c11 -fno-stack-protector)

add_library(libcopper STATIC src/copper.c)
target_include_directories(libcopper PUBLIC ${PROJECT_BINARY_DIR}/include)

if(CONFIG_PROVIDE_STANDARD)
        target_sources(libcopper PRIVATE src/slibc.c)
endif()

add_executable(main src/main.c)
target_link_libraries(main libcopper)
target_include_directories(main PRIVATE ${PROJECT_BINARY_DIR}/include)
