FetchContent_Declare(
  freertos_kernel
  GIT_REPOSITORY https://github.com/jasonpcarroll/FreeRTOS-Kernel.git
  GIT_TAG main
  GIT_PROGRESS TRUE)

add_library(freertos_config INTERFACE)

target_include_directories(freertos_config INTERFACE ${CMAKE_CURRENT_LIST_DIR})

FetchContent_MakeAvailable(freertos_kernel)

add_library(default_freertos_hooks STATIC
            ${CMAKE_CURRENT_LIST_DIR}/default_freertos_hooks.c)

target_link_libraries(default_freertos_hooks PUBLIC freertos_kernel)
target_link_libraries(freertos_kernel PUBLIC default_freertos_hooks)

add_library(default_freertos_assert STATIC
            ${CMAKE_CURRENT_LIST_DIR}/default_freertos_assert.c)

target_link_libraries(default_freertos_assert PUBLIC freertos_kernel)
target_link_libraries(freertos_kernel PUBLIC default_freertos_assert)
