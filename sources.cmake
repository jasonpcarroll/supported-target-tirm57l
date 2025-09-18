set(BSP_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/board_support_package/source/adc.c
    ${CMAKE_CURRENT_LIST_DIR}/board_support_package/source/can.c
    ${CMAKE_CURRENT_LIST_DIR}/board_support_package/source/dabort.S
    ${CMAKE_CURRENT_LIST_DIR}/board_support_package/source/emac.c
    ${CMAKE_CURRENT_LIST_DIR}/board_support_package/source/emif.c
    ${CMAKE_CURRENT_LIST_DIR}/board_support_package/source/errata_SSWF021_45.c
    ${CMAKE_CURRENT_LIST_DIR}/board_support_package/source/esm.c
    ${CMAKE_CURRENT_LIST_DIR}/board_support_package/source/gio.c
    ${CMAKE_CURRENT_LIST_DIR}/board_support_package/source/het.c
    ${CMAKE_CURRENT_LIST_DIR}/board_support_package/source/lin.c
    ${CMAKE_CURRENT_LIST_DIR}/board_support_package/source/mdio.c
    ${CMAKE_CURRENT_LIST_DIR}/board_support_package/source/mibspi.c
    ${CMAKE_CURRENT_LIST_DIR}/board_support_package/source/notification.c
    ${CMAKE_CURRENT_LIST_DIR}/board_support_package/source/phy_dp83640.c
    ${CMAKE_CURRENT_LIST_DIR}/board_support_package/source/pinmux.c
    ${CMAKE_CURRENT_LIST_DIR}/board_support_package/source/sci.c
    ${CMAKE_CURRENT_LIST_DIR}/board_support_package/source/sys_core.S
    ${CMAKE_CURRENT_LIST_DIR}/board_support_package/source/sys_dma.c
    ${CMAKE_CURRENT_LIST_DIR}/board_support_package/source/sys_intvecs.S
    ${CMAKE_CURRENT_LIST_DIR}/board_support_package/source/sys_pcr.c
    ${CMAKE_CURRENT_LIST_DIR}/board_support_package/source/sys_phantom.c
    ${CMAKE_CURRENT_LIST_DIR}/board_support_package/source/sys_pmm.c
    ${CMAKE_CURRENT_LIST_DIR}/board_support_package/source/sys_pmu.S
    ${CMAKE_CURRENT_LIST_DIR}/board_support_package/source/sys_startup.c
    ${CMAKE_CURRENT_LIST_DIR}/board_support_package/source/system.c
    ${CMAKE_CURRENT_LIST_DIR}/board_support_package/source/sys_vim.c
)

set(BSP_INCLUDE_DIRS
    ${CMAKE_CURRENT_LIST_DIR}/board_support_package/include
)

set(BSP_TEST_PORT_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/test_port/source/main.c
    ${CMAKE_CURRENT_LIST_DIR}/test_port/source/test_tracker_hardware_interface.c
)

set(BSP_TEST_PORT_INCLUDE_DIRS
    ${CMAKE_CURRENT_LIST_DIR}/test_port/include
)

set(BSP_FREERTOS_CONFIG_INCLUDE_DIR
    ${CMAKE_CURRENT_LIST_DIR}/freertos_config
)

