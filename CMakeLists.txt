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

# VS Code Integration

add_subdirectory(vscode)

# ------------------------------------------------------------------------------
# Function overrides
# ------------------------------------------------------------------------------
function(add_executable target)
  # Forward all arguments to the CMake add_executable
  _add_executable(${target} ${ARGN})

  # Add flash command for each executable

  add_custom_target(
    flash_${target}
    "${CMAKE_BINARY_DIR}/uniflash_linux/ccs_base/DebugServer/bin/DSLite" load -c "${CMAKE_BINARY_DIR}/uniflash_linux/user_files/configs/rm57l8xx.ccxml" -f "$<TARGET_FILE:${target}>"
  )

  add_dependencies(flash_${target} ${target})

  # Add debug launch command for each executable

  add_custom_target(launch_debug_${target}
                    "${CMAKE_C_DEBUGGER}" -ex "target extended-remote :3333" "$<TARGET_FILE:${target}>")

  # Add VS Code launch configurations for each executable

  add_dependencies(launch_debug_${target} ${target})

  get_property(VSCODE_LAUNCH_IN GLOBAL PROPERTY VSCODE_LAUNCH_IN_PROP)
  get_property(DEBUGGER_DIR GLOBAL PROPERTY DEBUGGER_DIR_PROP)
  get_property(DEBUGGER_CONFIG_PATH GLOBAL PROPERTY DEBUGGER_CONFIG_PATH_PROP)

  set(EXECUTABLE_NAME ${target})

  set(EXECUTABLE_PATH $<TARGET_FILE:${target}>)

  configure_file("${VSCODE_LAUNCH_IN}" "${CMAKE_SOURCE_DIR}/.vscode/launch.json" @ONLY)

  file(GENERATE OUTPUT "${CMAKE_SOURCE_DIR}/.vscode/launch.json" INPUT "${CMAKE_SOURCE_DIR}/.vscode/launch.json" )

  # Add VS code build tasks for each executable

  get_property(VSCODE_TASKS_IN GLOBAL PROPERTY VSCODE_TASKS_IN_PROP)

  set(BUILD_TARGET flash_${target})

  configure_file("${VSCODE_TASKS_IN}" "${CMAKE_SOURCE_DIR}/.vscode/tasks.json" @ONLY)

endfunction()
