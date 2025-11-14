add_library(transport STATIC ${CMAKE_CURRENT_LIST_DIR}/transport.c)
target_include_directories(transport PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(transport PUBLIC board_support_package)