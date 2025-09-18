#-------------------------------------------------------------------------------
# Set compilation, assembler, and linker flags
#-------------------------------------------------------------------------------

set(CMAKE_BUILD_TYPE Debug)
set(OBJ_GEN_FLAGS "-std=c89 -Wall -Wextra -g -O0 -mthumb-interwork -ffunction-sections -fdata-sections -Wno-unused-parameter -mcpu=cortex-r5 -mfpu=vfpv3-d16 -marm -mfloat-abi=softfp -specs=\"nosys.specs\"")
set(CMAKE_ASM_FLAGS ${OBJ_GEN_FLAGS})
set(CMAKE_C_FLAGS ${OBJ_GEN_FLAGS})
set(CMAKE_CXX_FLAGS ${OBJ_GEN_FLAGS})


set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections -Wl,--no-warn-rwx-segment -Wl,-Map,\"FreeRTOSSystemTest.map\" -Wl,-T\"${CMAKE_CURRENT_LIST_DIR}/board_support_package/source/sys_link.ld\" -Wl,--no-wchar-size-warning -lc")

#-------------------------------------------------------------------------------
# Set executable output flags.
#-------------------------------------------------------------------------------

set(CMAKE_EXECUTABLE_SUFFIX ".out")
