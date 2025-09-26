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

add_library(board_support_package OBJECT ${BSP_SOURCES})

target_include_directories(board_support_package PUBLIC ${BSP_INCLUDE_DIRS})

# FreeRTOS configuration

add_library(freertos_config INTERFACE)

target_include_directories(freertos_config PUBLIC ${CMAKE_CURRENT_LIST_DIR}/freertos_config)

