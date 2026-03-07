cmake_minimum_required(VERSION 3.20.1)

project(LibCopper LANGUAGES ASM C VERSION 1.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic -Werror -ffreestanding -std=c11 -fno-stack-protector)
add_link_options(-nostdlib)

add_library(copper STATIC src/copper.c src/standard.c)
target_include_directories(copper PUBLIC include)

add_executable(main src/main.c)
target_link_libraries(main copper)
target_include_directories(main PRIVATE include)
