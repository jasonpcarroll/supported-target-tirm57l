/* Standard library includes. */
#include <stdint.h>

/* HALCoGen includes. */
#include "HL_sys_vim.h"
#include "HL_reg_rti.h"

/* FreeRTOS-Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

extern uint32_t ulPortYieldRequired;

void vHandleTickInterrupt( void )
{
    ulPortYieldRequired = xTaskIncrementTick();

    /* Clear RTI Compare 0 interrupt. */
    rtiREG1->INTFLAG = 1U;
}

void vMainSetupTimerInterrupt( void )
{
    /* Disable timer 0. */
    rtiREG1->GCTRL &= 0xFFFFFFFEUL;

    /* Use the internal counter. */
    rtiREG1->TBCTRL = 0x00000000U;

    /* COMPSEL0 will use the RTIFRC0 counter. */
    rtiREG1->COMPCTRL = 0x00000000U;

    /* Initialise the counter and the prescale counter registers. */
    rtiREG1->CNT[ 0 ].UCx = 0x00000000U;
    rtiREG1->CNT[ 0 ].FRCx = 0x00000000U;

    /* Set Prescalar for RTI clock. */
    rtiREG1->CNT[ 0 ].CPUCx = 0x00000001U;
    rtiREG1->CMP[ 0 ].COMPx = ( configCPU_CLOCK_HZ / 2 ) / configTICK_RATE_HZ;
    rtiREG1->CMP[ 0 ].UDCPx = ( configCPU_CLOCK_HZ / 2 ) / configTICK_RATE_HZ;

    /* Clear interrupts. */
    rtiREG1->INTFLAG = 0x0007000FU;
    rtiREG1->CLEARINTENA = 0x00070F0FU;

    /* Enable the compare 0 interrupt. */
    rtiREG1->SETINTENA = 0x00000001U;
    rtiREG1->GCTRL |= 0x00000001U;

    /* Map tick interrupt handler to RTI compare 0 interrupt channel. */
    vimChannelMap( 2, 2, ( t_isrFuncPTR ) vHandleTickInterrupt );
}
