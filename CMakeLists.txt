cmake_minimum_required(VERSION 3.20.1)
project(LibCopper LANGUAGES ASM C VERSION 1.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CMakeOptions.cmake)

add_compile_options(-Wall -Wextra -Wpedantic -Werror -ffreestanding -std=c11 -fno-stack-protector)

add_library(copper STATIC src/copper.c)
target_include_directories(copper PUBLIC include)
target_include_directories(copper PRIVATE ${PROJECT_BINARY_DIR}/include)

if(CONFIG_PROVIDE_ALLOCATOR)
        target_sources(copper PRIVATE src/alloc.c)
endif()

if(CONFIG_PROVIDE_STANDARD)
        target_sources(copper PRIVATE src/standard.c)
endif()

if(CONFIG_PROVIDE_JOURNAL)
        target_sources(copper PRIVATE src/journal.c)
endif()

add_executable(main src/main.c)
target_link_libraries(main copper)
target_include_directories(main PRIVATE ${PROJECT_BINARY_DIR}/include)
