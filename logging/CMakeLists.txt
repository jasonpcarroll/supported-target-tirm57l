add_library(logging STATIC ${CMAKE_CURRENT_LIST_DIR}/logging_freertos.c)

target_include_directories(logging PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(logging PUBLIC board_support_package freertos_kernel)
