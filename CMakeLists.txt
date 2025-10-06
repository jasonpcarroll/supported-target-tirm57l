cmake_minimum_required(VERSION 3.15.3)

project(TI-RM57-Support-Package C ASM)

# ------------------------------------------------------------------------------
# Targets
# ------------------------------------------------------------------------------

# Board Support Package (board_support_package)

add_subdirectory(board_support_package)

# FreeRTOS-Kernel configuration

add_subdirectory(freertos_config)

# Logging

add_subdirectory(logging)

target_link_libraries(logging_freertos PUBLIC board_support_package)
