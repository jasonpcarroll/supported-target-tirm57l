add_library(transport STATIC ${CMAKE_CURRENT_LIST_DIR}/transport.c)
target_include_directories(transport PUBLIC ${CMAKE_CURRENT_LIST_DIR})
