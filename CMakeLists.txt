cmake_minimum_required(VERSION 3.15.3)

project(TI-RM57-Support-Package C ASM)

# ------------------------------------------------------------------------------
# Targets
# ------------------------------------------------------------------------------

# Board Support Package (board_support_package)

add_subdirectory(board_support_package)

# FreeRTOS-Kernel configuration

add_subdirectory(freertos)

# Logging

add_subdirectory(logging)

# Debugging

add_subdirectory(debugging)

# ------------------------------------------------------------------------------
# Function overrides
# ------------------------------------------------------------------------------
function(add_executable target)

  get_property(DEBUG_COMMAND GLOBAL PROPERTY debug_command_prop)
  # Forward all arguments to the CMake add_executable
  _add_executable(${target} ${ARGN})

  message("${DEBUG_COMMAND}")
endfunction()
