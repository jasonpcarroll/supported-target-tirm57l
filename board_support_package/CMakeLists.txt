set(BSP_SOURCES
    # HalCoGen generated files.
    ${CMAKE_CURRENT_LIST_DIR}/source/HL_emac.c
    ${CMAKE_CURRENT_LIST_DIR}/source/HL_epc.c
    ${CMAKE_CURRENT_LIST_DIR}/source/HL_errata_SSWF021_45.c
    ${CMAKE_CURRENT_LIST_DIR}/source/HL_errata.c
    ${CMAKE_CURRENT_LIST_DIR}/source/HL_esm.c
    ${CMAKE_CURRENT_LIST_DIR}/source/HL_gio.c
    ${CMAKE_CURRENT_LIST_DIR}/source/HL_mdio.c
    ${CMAKE_CURRENT_LIST_DIR}/source/HL_nmpu.c
    ${CMAKE_CURRENT_LIST_DIR}/source/HL_notification.c
    ${CMAKE_CURRENT_LIST_DIR}/source/HL_phy_dp83640.c
    ${CMAKE_CURRENT_LIST_DIR}/source/HL_phy_tlk111.c
    ${CMAKE_CURRENT_LIST_DIR}/source/HL_pinmux.c
    ${CMAKE_CURRENT_LIST_DIR}/source/HL_rti.c
    ${CMAKE_CURRENT_LIST_DIR}/source/HL_sci.c
    ${CMAKE_CURRENT_LIST_DIR}/source/HL_sys_core.s
    ${CMAKE_CURRENT_LIST_DIR}/source/HL_sys_dma.c
    ${CMAKE_CURRENT_LIST_DIR}/source/HL_sys_intvecs.s
    ${CMAKE_CURRENT_LIST_DIR}/source/HL_sys_link.ld
    ${CMAKE_CURRENT_LIST_DIR}/source/HL_sys_main.c
    ${CMAKE_CURRENT_LIST_DIR}/source/HL_sys_mpu.s
    ${CMAKE_CURRENT_LIST_DIR}/source/HL_sys_pcr.c
    ${CMAKE_CURRENT_LIST_DIR}/source/HL_sys_phantom.c
    ${CMAKE_CURRENT_LIST_DIR}/source/HL_sys_pmm.c
    ${CMAKE_CURRENT_LIST_DIR}/source/HL_sys_pmu.s
    ${CMAKE_CURRENT_LIST_DIR}/source/HL_sys_startup.c
    ${CMAKE_CURRENT_LIST_DIR}/source/HL_sys_vim.c
    ${CMAKE_CURRENT_LIST_DIR}/source/HL_system.c
    # vApplicationIRQHandler for nested IRQs
    ${CMAKE_CURRENT_LIST_DIR}/source/FreeRTOS_IRQ_handling.c
    # FreeRTOS tick interrupt setup
    ${CMAKE_CURRENT_LIST_DIR}/source/FreeRTOS_tick_interrupt.c)

set(BSP_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(board_support_package OBJECT ${BSP_SOURCES})

target_include_directories(board_support_package PUBLIC ${BSP_INCLUDE_DIRS})

target_link_libraries(board_support_package PRIVATE m)

target_link_options(
  board_support_package PUBLIC
  "-Wl,-T${CMAKE_CURRENT_LIST_DIR}/source/HL_sys_link.ld" "--specs=nosys.specs")
