cmake_minimum_required(VERSION 3.20.1)

project(LibCopper LANGUAGES ASM C VERSION 1.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AR "llvm-ar")
set(CMAKE_RANLIB "llvm-ranlib")

add_compile_options(-Wall -Wextra -Wpedantic -Werror -ffreestanding -std=c11)

add_library(copper STATIC src/copper.c)
target_include_directories(copper PUBLIC include)

add_executable(main src/main.c)
target_link_libraries(main copper)
target_include_directories(main PUBLIC include)
