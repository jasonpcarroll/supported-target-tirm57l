cmake_minimum_required(VERSION 3.15.3)

project(BSP C ASM)

# ------------------------------------------------------------------------------
# Includes
# ------------------------------------------------------------------------------

# Gives the sources (BSP_SOURCES) and include paths (BSP_INCLUDE_DIRS) for the
# board_support_package target below

include(${CMAKE_CURRENT_LIST_DIR}/sources.cmake)

# ------------------------------------------------------------------------------
# Targets
# ------------------------------------------------------------------------------

# Board Support Package (board_support_package)

add_subdirectory(board_support_package)

# FreeRTOS-Kernel configuration

add_subdirectory(freertos_config)

# Logging

add_subdirectory(logging)

target_link_libraries(logging PUBLIC board_support_package)


