include(FetchContent)

add_library(freertos_plus_tcp_network_if STATIC
            ${CMAKE_CURRENT_LIST_DIR}/NetworkInterface.c)

target_link_libraries(freertos_plus_tcp_network_if PUBLIC board_support_package)

FetchContent_Declare(
  freertos_plus_tcp
  GIT_REPOSITORY https://github.com/FreeRTOS/FreeRTOS-Plus-TCP.git
  GIT_TAG main
  GIT_SUBMODULES "")

target_include_directories(freertos_config INTERFACE ${CMAKE_CURRENT_LIST_DIR})
set(FREERTOS_PLUS_TCP_NETWORK_IF A_CUSTOM_NETWORK_IF)

FetchContent_MakeAvailable(freertos_plus_tcp)

target_link_libraries(
  freertos_plus_tcp_network_if
  PUBLIC freertos_plus_tcp_port freertos_plus_tcp_network_if_common
  PRIVATE freertos_kernel freertos_plus_tcp)

add_library(default_freertos_plus_tcp_hooks STATIC
            ${CMAKE_CURRENT_LIST_DIR}/default_freertos_plus_tcp_hooks.c)

target_link_libraries(default_freertos_plus_tcp_hooks PUBLIC freertos_kernel)

target_link_libraries(freertos_plus_tcp PUBLIC default_freertos_plus_tcp_hooks)

add_library(transport STATIC ${CMAKE_CURRENT_LIST_DIR}/transport.c)
target_include_directories(transport PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(transport PUBLIC board_support_package freertos_plus_tcp)
