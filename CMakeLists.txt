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

# Flashing

add_subdirectory(flashing)

# Debugging

add_subdirectory(debugging)

# ------------------------------------------------------------------------------
# Function overrides
# ------------------------------------------------------------------------------
function(add_executable target)
  # Forward all arguments to the CMake add_executable
  _add_executable(${target} ${ARGN})

  add_custom_target(
    flash_${target}
    "${CMAKE_BINARY_DIR}/uniflash_linux/ccs_base/DebugServer/bin/DSLite" load -c "${CMAKE_BINARY_DIR}/uniflash_linux/user_files/configs/rm57l8xx.ccxml" -f "$<TARGET_FILE:${target}>"
  )

  message("CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")

  add_dependencies(flash_${target} ${target})

  add_custom_target(launch_debug_${target}
                    "${CMAKE_C_DEBUGGER}" -ex "target extended-remote :3333" "$<TARGET_FILE:${target}>")

  add_dependencies(launch_debug_${target} flash_${target})

endfunction()
