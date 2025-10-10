set(BSP_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/source/adc.c
    ${CMAKE_CURRENT_LIST_DIR}/source/can.c
    ${CMAKE_CURRENT_LIST_DIR}/source/dabort.S
    ${CMAKE_CURRENT_LIST_DIR}/source/emac.c
    ${CMAKE_CURRENT_LIST_DIR}/source/emif.c
    ${CMAKE_CURRENT_LIST_DIR}/source/errata_SSWF021_45.c
    ${CMAKE_CURRENT_LIST_DIR}/source/esm.c
    ${CMAKE_CURRENT_LIST_DIR}/source/gio.c
    ${CMAKE_CURRENT_LIST_DIR}/source/het.c
    ${CMAKE_CURRENT_LIST_DIR}/source/lin.c
    ${CMAKE_CURRENT_LIST_DIR}/source/mdio.c
    ${CMAKE_CURRENT_LIST_DIR}/source/mibspi.c
    ${CMAKE_CURRENT_LIST_DIR}/source/notification.c
    ${CMAKE_CURRENT_LIST_DIR}/source/phy_dp83640.c
    ${CMAKE_CURRENT_LIST_DIR}/source/pinmux.c
    ${CMAKE_CURRENT_LIST_DIR}/source/sci.c
    ${CMAKE_CURRENT_LIST_DIR}/source/sys_core.S
    ${CMAKE_CURRENT_LIST_DIR}/source/sys_dma.c
    ${CMAKE_CURRENT_LIST_DIR}/source/sys_intvecs.S
    ${CMAKE_CURRENT_LIST_DIR}/source/sys_pcr.c
    ${CMAKE_CURRENT_LIST_DIR}/source/sys_phantom.c
    ${CMAKE_CURRENT_LIST_DIR}/source/sys_pmm.c
    ${CMAKE_CURRENT_LIST_DIR}/source/sys_pmu.S
    ${CMAKE_CURRENT_LIST_DIR}/source/sys_startup.c
    ${CMAKE_CURRENT_LIST_DIR}/source/system.c
    ${CMAKE_CURRENT_LIST_DIR}/source/sys_vim.c)

set(BSP_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(board_support_package OBJECT ${BSP_SOURCES})

target_include_directories(board_support_package PUBLIC ${BSP_INCLUDE_DIRS})

target_link_options(board_support_package PUBLIC
                    "-Wl,-T${CMAKE_CURRENT_LIST_DIR}/source/sys_link.ld")
