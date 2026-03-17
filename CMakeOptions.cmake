include(CMakeDependentOption)

# ================================================================================

option(
        CONFIG_PROVIDE_ALLOCATOR 
        "Feature: Expose allocator bindings and provide local arena allocator." ON
)

# ================================================================================

option(
        CONFIG_PROVIDE_JOURNAL
        "Feature: Expose journaling bindings." ON
)

# ================================================================================

cmake_dependent_option(
        CONFIG_PROVIDE_STANDARD 
        "Feature: Provide standalone standard library features." ON 
        "CONFIG_PROVIDE_ALLOCATOR" OFF
)

# ================================================================================

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Set the build type.")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Release;Debug")

# ================================================================================

set(
        CONFIG_ALLOCATOR_ARENA_SIZE "4096"
        CACHE STRING "Option: Sets the default local allocator buffer size."
)

set_property(CACHE CONFIG_ALLOCATOR_ARENA_SIZE PROPERTY STRINGS "512;1024;2048;4096")

# ================================================================================

set(
        CONFIG_JOURNAL_MAX_CALLBACKS "4"
        CACHE STRING "Option: Sets the maximum number of journaling callbacks."
)

set_property(CACHE CONFIG_JOURNAL_MAX_CALLBACKS PROPERTY STRINGS "1;2;4;8;16")

# ================================================================================

configure_file(include/copper.h.in include/defconfig.h)

