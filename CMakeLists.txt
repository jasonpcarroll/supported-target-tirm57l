cmake_minimum_required(VERSION 3.15.3)

project(BSP C ASM)

#-------------------------------------------------------------------------------
# Includes
#-------------------------------------------------------------------------------

# Gives the sources (BSP_SOURCES) and include paths (BSP_INCLUDE_DIRS) for the 
# board_support_package target below
include(${CMAKE_CURRENT_LIST_DIR}/sources.cmake)

#-------------------------------------------------------------------------------
# Board Support Package Target
#-------------------------------------------------------------------------------

add_library(board_support_package OBJECT
    ${BSP_SOURCES}
)

target_include_directories(board_support_package PUBLIC
    ${BSP_INCLUDE_DIRS}
)
