include(FetchContent)

FetchContent_Declare(
  freertos_plus_tcp
  GIT_REPOSITORY https://github.com/FreeRTOS/FreeRTOS-Plus-TCP.git
  GIT_TAG main # Note: Best practice to use specific git-hash or tagged version
  GIT_SUBMODULES "" # Don't grab any submodules since not latest
)

set(FREERTOS_PLUS_TCP_NETWORK_IF A_CUSTOM_NETWORK_IF)

FetchContent_MakeAvailable(freertos_plus_tcp)

add_library(transport STATIC ${CMAKE_CURRENT_LIST_DIR}/transport.c)
target_include_directories(transport PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(transport PUBLIC board_support_package)
