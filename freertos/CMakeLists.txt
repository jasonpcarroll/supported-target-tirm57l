add_library(freertos_config STATIC ${CNAKE_CURRENT_LIST_DIR}/tick_interrupt.c)

target_include_directories(freertos_config PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(freertos_config PUBLIC board_support_package)
