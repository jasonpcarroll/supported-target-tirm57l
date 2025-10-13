#include <stdint.h>
#include "FreeRTOSConfig.h"
#include "reg_vim.h"
#include "sys_vim.h"

/* Registers required to configure the Real Time Interrupt (RTI). */
#define portRTI_GCTRL_REG           ( *( ( volatile uint32_t * ) 0xFFFFFC00UL ) )
#define portRTI_TBCTRL_REG          ( *( ( volatile uint32_t * ) 0xFFFFFC04UL ) )
#define portRTI_COMPCTRL_REG        ( *( ( volatile uint32_t * ) 0xFFFFFC0CUL ) )
#define portRTI_CNT0_FRC0_REG       ( *( ( volatile uint32_t * ) 0xFFFFFC10UL ) )
#define portRTI_CNT0_UC0_REG        ( *( ( volatile uint32_t * ) 0xFFFFFC14UL ) )
#define portRTI_CNT0_CPUC0_REG      ( *( ( volatile uint32_t * ) 0xFFFFFC18UL ) )
#define portRTI_CNT0_COMP0_REG      ( *( ( volatile uint32_t * ) 0xFFFFFC50UL ) )
#define portRTI_CNT0_UDCP0_REG      ( *( ( volatile uint32_t * ) 0xFFFFFC54UL ) )
#define portRTI_SETINTENA_REG       ( *( ( volatile uint32_t * ) 0xFFFFFC80UL ) )
#define portRTI_CLEARINTENA_REG     ( *( ( volatile uint32_t * ) 0xFFFFFC84UL ) )
#define portRTI_INTFLAG_REG         ( *( ( volatile uint32_t * ) 0xFFFFFC88UL ) )
#define portEND_OF_INTERRUPT_REG    ( ( ( volatile uint32_t * ) configEOI_ADDRESS ) )

typedef void ( * ISRFunction_t )( void );
#define portVIM_IRQ_INDEX           ( *( ( volatile uint32_t * ) 0xFFFFFE00 ) )
#define portVIM_IRQ_VEC_REG         ( *( ( volatile ISRFunction_t * ) 0xFFFFFE70 ) )

extern volatile uint32_t ulPortYieldRequired;

void vMainSetupTimerInterrupt( void )
{
    /* Disable timer 0. */
    portRTI_GCTRL_REG &= 0xFFFFFFFEUL;

    /* Use the internal counter. */
    portRTI_TBCTRL_REG = 0x00000000U;

    /* COMPSEL0 will use the RTIFRC0 counter. */
    portRTI_COMPCTRL_REG = 0x00000000U;

    /* Initialise the counter and the prescale counter registers. */
    portRTI_CNT0_UC0_REG = 0x00000000U;
    portRTI_CNT0_FRC0_REG = 0x00000000U;

    /* Set Prescalar for RTI clock. */
    portRTI_CNT0_CPUC0_REG = 0x00000001U;
    portRTI_CNT0_COMP0_REG = ( configCPU_CLOCK_HZ / 2 ) / configTICK_RATE_HZ;
    portRTI_CNT0_UDCP0_REG = ( configCPU_CLOCK_HZ / 2 ) / configTICK_RATE_HZ;

    /* Clear interrupts. */
    portRTI_INTFLAG_REG = 0x0007000FU;
    portRTI_CLEARINTENA_REG = 0x00070F0FU;

    /* Enable the compare 0 interrupt. */
    portRTI_SETINTENA_REG = 0x00000001U;
    portRTI_GCTRL_REG |= 0x00000001U;
}

/** @brief Default IRQ Handler used in the ARM_Cortex_RX ports.
 * @note This Handler is directly tied to the Texas Instrument's Hercules
 * Vectored Interrupt Manager (VIM). For more information about what
 * this is and how it operates please refer to their document:
 * https://www.ti.com/lit/pdf/spna218
 */
void vApplicationIRQHandler( void )
{
    /* Load the IRQ Channel Number and Function PTR from the VIM. */
    volatile uint32_t ulIRQChannelIndex = portVIM_IRQ_INDEX;
    volatile ISRFunction_t xIRQFncPtr = portVIM_IRQ_VEC_REG;

    /* Setup Bit Mask Clear Values. */
    volatile uint32_t ulPendingIRQMask;

    volatile uint32_t ulPendISRReg0 = vimREG->REQMASKCLR0;
    volatile uint32_t ulPendISRReg1 = vimREG->REQMASKCLR1;
    volatile uint32_t ulPendISRReg2 = vimREG->REQMASKCLR2;
    volatile uint32_t ulPendISRReg3 = vimREG->REQMASKCLR3;

    if( NULL == xIRQFncPtr )
    {
        /* Received a NULL Function Pointer from the IRQ VIM */
        configASSERT( 0 );
    }
    else
    {
        if( 0U != ulIRQChannelIndex )
        {
            ulIRQChannelIndex--;
        }

        if( ulIRQChannelIndex <= 31U )
        {
            ulPendingIRQMask = 0xFFFFFFFFU << ulIRQChannelIndex;
            vimREG->REQMASKCLR0 = ulPendingIRQMask;
            vimREG->REQMASKCLR1 = 0xFFFFFFFFU;
            vimREG->REQMASKCLR2 = 0xFFFFFFFFU;
            vimREG->REQMASKCLR3 = 0xFFFFFFFFU;
        }
        else if( ulIRQChannelIndex <= 63U )
        {
            ulPendingIRQMask = 0xFFFFFFFFU << ( ulIRQChannelIndex - 32U );
            vimREG->REQMASKCLR1 = ulPendingIRQMask;
            vimREG->REQMASKCLR2 = 0xFFFFFFFFU;
            vimREG->REQMASKCLR3 = 0xFFFFFFFFU;
        }
        else if( ulIRQChannelIndex <= 95U )
        {
            ulPendingIRQMask = 0xFFFFFFFFU << ( ulIRQChannelIndex - 64U );
            vimREG->REQMASKCLR2 = ulPendingIRQMask;
            vimREG->REQMASKCLR3 = 0xFFFFFFFFU;
        }
        else
        {
            ulPendingIRQMask = 0xFFFFFFFFU << ( ulIRQChannelIndex - 96U );
            vimREG->REQMASKCLR3 = ulPendingIRQMask;
        }
    }

    /*
     * Channel 0 is the ESM handler, treat this as a special case.
     * phantomInterrupt()
     * Keep interrupts disabled, this function does not return.
     */

    if( 0UL == ulIRQChannelIndex )
    {
        /* Unexpected interrupt from ESM Handler. */
        configASSERT( 0 );
    }
    else if( ( phantomInterrupt == xIRQFncPtr ) )
    {
        /* IRQ With no registered function in sys_vim.c has been raised. */
        configASSERT( 0 );
    }
    else
    {
        /* Information about the mapping of Interrupts in the VIM to their
         * causes can be found in the RM57L843 Data Sheet:
         * https://www.ti.com/document-viewer/RM57L843/datasheet#system_information_and_electrical_specifications/SPNS1607150 */
        /* An IRQ Raised by Channel Two of the VIM is RTI Compare Interrupt 0. */
        if( 2UL == ulIRQChannelIndex )
        {
            /* This is the System Tick Timer Interrupt. */
            ulPortYieldRequired = xTaskIncrementTick();
            /* Acknowledge the System Tick Timer Interrupt. */
            portRTI_INTFLAG_REG = 0x1UL;
        }
        /* An IRQ Raised by Channel 21 of the VIM is a Software Interrupt (SSI). */
        else if( 21UL == ulIRQChannelIndex )
        {

            /* SWI of unknown cause was raised! */
            configASSERT( 0 );

            /* Register read is needed to mark the end of the IRQ. */
            volatile uint32_t ulEndOfIntRegVal = *portEND_OF_INTERRUPT_REG;
            *portEND_OF_INTERRUPT_REG = ulEndOfIntRegVal;
        }
        else
        {
            sci_print( "Unmapped IRQ Channel Number Raised\r\n" );
        }
    }

    vimREG->REQMASKSET0 = ulPendISRReg0;
    vimREG->REQMASKSET1 = ulPendISRReg1;
    vimREG->REQMASKSET2 = ulPendISRReg2;
    vimREG->REQMASKSET3 = ulPendISRReg3;
}
