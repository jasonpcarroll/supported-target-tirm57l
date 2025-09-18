cmake_minimum_required(VERSION 3.15.3)

#project(BSP C ASM)

#-------------------------------------------------------------------------------
# Includes
#-------------------------------------------------------------------------------

# Sets the build flags
include(${CMAKE_CURRENT_LIST_DIR}/build_flags.cmake)

# Sets the sources and include paths for the BSP target below
include(${CMAKE_CURRENT_LIST_DIR}/sources.cmake)

#-------------------------------------------------------------------------------
# Board Support Package Target
#-------------------------------------------------------------------------------

add_library(HERCULES_RM57 OBJECT
    ${BSP_SOURCES}
)

target_include_directories(HERCULES_RM57 PUBLIC
    ${BSP_INCLUDE_DIRS}
)
