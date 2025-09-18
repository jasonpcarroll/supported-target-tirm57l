/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* HalCoGen includes. */
#include "system.h"
#include "gio.h"
#include "het.h"
#include "reg_vim.h"
#include "sci.h"
#include "sys_vim.h"
#include "reg_system.h"

#define configEOI_ADDRESS           0xFFFFFE70UL

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


/* Registers used by the Vectored Interrupt Manager */
typedef void ( * ISRFunction_t )( void );
#define portVIM_IRQ_INDEX        ( *( ( volatile uint32_t * ) 0xFFFFFE00 ) )
#define portVIM_IRQ_VEC_REG      ( *( ( volatile ISRFunction_t * ) 0xFFFFFE70 ) )

#define portVIM_FIQ_INDEX        ( *( ( volatile uint32_t * ) 0xFFFFFE04 ) )
#define portVIM_FIQ_VEC_REG      ( *( ( volatile ISRFunction_t * ) 0xFFFFFE74 ) )

#define portSSI_INT_REG_BASE     ( ( ( volatile uint32_t * ) 0xFFFFFFB0 ) )

#define portSSI_INT_REG_ONE      ( ( ( volatile uint32_t * ) 0xFFFFFFB0 ) )
#define portSSI_ONE_KEY          0x7500UL

#define portSSI_INT_REG_TWO      ( ( ( volatile uint32_t * ) 0xFFFFFFB4 ) )
#define portSSI_TWO_KEY          0x8400UL

#define portSSI_INT_REG_THREE    ( ( ( volatile uint32_t * ) 0xFFFFFFB8 ) )
#define portSSI_THREE_KEY        0x9300UL

#define portSSI_INT_REG_FOUR     ( ( ( volatile uint32_t * ) 0xFFFFFFBC ) )
#define portSSI_FOUR_KEY         0xA200UL

#define portSSI_VEC_REG          ( *( ( volatile uint32_t * ) 0xFFFFFFF4 ) )
#define portSSI_INTFLAG_REG      ( *( ( volatile uint32_t * ) 0xFFFFFFF8 ) )

/** @brief Statically declared TCB Used by the Idle Task */
PRIVILEGED_DATA static StaticTask_t xTimerTaskTCB;

/** @brief Statically declared MPU aligned stack used by the timer task */
PRIVILEGED_DATA static StackType_t uxTimerTaskStack[ configMINIMAL_STACK_SIZE ]
__attribute__( ( aligned( configMINIMAL_STACK_SIZE * 0x4U ) ) );

/** @brief Statically declared TCB Used by the Idle Task */
PRIVILEGED_DATA static StaticTask_t xIdleTaskTCB;

/** @brief Statically declared MPU aligned stack used by the idle task */
PRIVILEGED_DATA static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE * 2 ]
__attribute__( ( aligned( configMINIMAL_STACK_SIZE * 2 * 0x4U ) ) );

/** @brief Simple variable to show how the idle tick hook can be used */
PRIVILEGED_DATA static volatile TickType_t ulIdleTickHookCount = 0x0;

extern PRIVILEGED_DATA volatile uint32_t ulPortYieldRequired;


extern void vRunTest( void );

int main( void )
{
    sciInit();

    vRunTest();

    return 0;
}

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

    /* Enable RTI0 in VIM hardware. */
    vimEnableInterrupt( 2, SYS_IRQ );
}
/*---------------------------------------------------------------------------*/

/** @brief Default IRQ Handler used in the ARM_Cortex_RX ports.
 * @note This Handler is directly tied to the Texas Instrument's Hercules
 * Vectored Interrupt Manager (VIM). For more information about what
 * this is and how it operates please refer to their document:
 * https://www.ti.com/lit/pdf/spna218
 */
void vApplicationIRQHandler( void )
{
    /* Load the IRQ Channel Number and Function PTR from the VIM */
    volatile uint32_t ulIRQChannelIndex = portVIM_IRQ_INDEX;
    volatile ISRFunction_t xIRQFncPtr = portVIM_IRQ_VEC_REG;

    /* Setup Bit Mask Clear Values */
    volatile uint32_t ulPendingIRQMask;

    volatile uint32_t ulPendISRReg0 = vimREG->REQMASKCLR0;
    volatile uint32_t ulPendISRReg1 = vimREG->REQMASKCLR1;
    volatile uint32_t ulPendISRReg2 = vimREG->REQMASKCLR2;
    volatile uint32_t ulPendISRReg3 = vimREG->REQMASKCLR3;
    volatile uint32_t ulEndOfIntRegVal;

    if( NULL == xIRQFncPtr )
    {
        sci_print( "Received a NULL Function Pointer from the IRQ VIM\r\n" );
        configASSERT( pdFALSE );
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
     * Keep interrupts disabled, this function does not return
     */

    if( 0UL == ulIRQChannelIndex )
    {
        sci_print( "Phantom interrupt?\r\n" );
        configASSERT( pdFALSE );
        ( *xIRQFncPtr )();
    }
    else if( ( phantomInterrupt == xIRQFncPtr ) )
    {
        sci_print( "IRQ With no registered function in sys_vim.c has been raised\r\n" );
        configASSERT( pdFALSE );
    }
    else
    {
        /* Information about the mapping of Interrupts in the VIM to their
         * causes can be found in the RM48L852 Data Sheet:
         * https://www.ti.com/lit/ds/symlink/rm46l852.pdf?ts=1704878833799 */
        /* An IRQ Raised by Channel Two of the VIM is RTI Compare Interrupt 0. */
        if( 2UL == ulIRQChannelIndex )
        {
            /* This is the System Tick Timer Interrupt */
            ulPortYieldRequired = xTaskIncrementTick();
            /* Acknowledge the System Tick Timer Interrupt */
            portRTI_INTFLAG_REG = 0x1UL;
        }
        /* An IRQ Raised by Channel 21 of the VIM is a Software Interrupt (SSI). */
        else if( 21UL == ulIRQChannelIndex )
        {
            _enable_IRQ_interrupt_();
            ( *xIRQFncPtr )();
            _disable_IRQ_interrupt_();

            /* Register read is needed to mark the end of the IRQ */
            ulEndOfIntRegVal = systemREG1->SSIVEC;
            ( void ) ulEndOfIntRegVal;
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
/*---------------------------------------------------------------------------*/

/** @brief Default FIQ Handler used in the ARM_Cortex_RX ports.
 * @note This Handler is directly tied to the Texas Instrument's Hercules
 * Vectored Interrupt Manager (VIM). For more information about what
 * this is and how it operates please refer to their document:
 * https://www.ti.com/lit/pdf/spna218
 */
void vApplicationFIQHandler( void )
{
    /* Load the FIQ Channel Number and Function PTR from the VIM */
    volatile uint32_t ulFIQChannelIndex = portVIM_FIQ_INDEX;
    volatile ISRFunction_t xFIQFncPtr = portVIM_FIQ_VEC_REG;

    /* Setup Bit Mask Clear Values */
    volatile uint32_t ulPendingFIQMask;

    volatile uint32_t ulPendISRReg0 = vimREG->REQMASKCLR0;
    volatile uint32_t ulPendISRReg1 = vimREG->REQMASKCLR1;
    volatile uint32_t ulPendISRReg2 = vimREG->REQMASKCLR2;
    volatile uint32_t ulPendISRReg3 = vimREG->REQMASKCLR3;
    volatile uint32_t ulEndOfIntRegVal;

    if( NULL == xFIQFncPtr )
    {
        sci_print( "Received a NULL Function Pointer from the FIQ VIM\r\n" );
        configASSERT( pdFALSE );
    }
    else
    {
        if( 0U != ulFIQChannelIndex )
        {
            ulFIQChannelIndex--;
        }

        if( ulFIQChannelIndex <= 31U )
        {
            ulPendingFIQMask = 0xFFFFFFFFU << ulFIQChannelIndex;
            vimREG->REQMASKCLR0 = ulPendingFIQMask;
            vimREG->REQMASKCLR1 = 0xFFFFFFFFU;
            vimREG->REQMASKCLR2 = 0xFFFFFFFFU;
            vimREG->REQMASKCLR3 = 0xFFFFFFFFU;
        }
        else if( ulFIQChannelIndex <= 63U )
        {
            ulPendingFIQMask = 0xFFFFFFFFU << ( ulFIQChannelIndex - 32U );
            vimREG->REQMASKCLR1 = ulPendingFIQMask;
            vimREG->REQMASKCLR2 = 0xFFFFFFFFU;
            vimREG->REQMASKCLR3 = 0xFFFFFFFFU;
        }
        else if( ulFIQChannelIndex <= 95U )
        {
            ulPendingFIQMask = 0xFFFFFFFFU << ( ulFIQChannelIndex - 64U );
            vimREG->REQMASKCLR2 = ulPendingFIQMask;
            vimREG->REQMASKCLR3 = 0xFFFFFFFFU;
        }
        else
        {
            ulPendingFIQMask = 0xFFFFFFFFU << ( ulFIQChannelIndex - 96U );
            vimREG->REQMASKCLR3 = ulPendingFIQMask;
        }
    }

    /*
     * Channel 0 is the ESM handler, treat this as a special case.
     * phantomInterrupt()
     * Keep interrupts disabled, this function does not return
     */

    if( 0UL == ulFIQChannelIndex )
    {
        sci_print( "Phantom interrupt?\r\n" );
        configASSERT( pdFALSE );
        ( *xFIQFncPtr )();
    }
    else if( ( phantomInterrupt == xFIQFncPtr ) )
    {
        sci_print( "IRQ With no registered function in sys_vim.c has been raised\r\n" );
        configASSERT( pdFALSE );
    }
    else
    {
        /* Information about the mapping of Interrupts in the VIM to their
         * causes can be found in the RM48L852 Data Sheet:
         * https://www.ti.com/lit/ds/symlink/rm46l852.pdf?ts=1704878833799 */
        /* An IRQ Raised by Channel Two of the VIM is RTI Compare Interrupt 0. */
        if( 2UL == ulFIQChannelIndex )
        {
            /* This is the System Tick Timer Interrupt */
            ulPortYieldRequired = xTaskIncrementTick();
            /* Acknowledge the System Tick Timer Interrupt */
            portRTI_INTFLAG_REG = 0x1UL;
        }
        /* An IRQ Raised by Channel 21 of the VIM is a Software Interrupt (SSI). */
        else if( 21UL == ulFIQChannelIndex )
        {
            _enable_interrupt_();
            ( *xFIQFncPtr )();
            _disable_interrupt_();

            /* Register read is needed to mark the end of the FIQ */
            ulEndOfIntRegVal = systemREG1->SSIVEC;
            ( void ) ulEndOfIntRegVal;
        }
        else
        {
            sci_print( "Unmapped FIQ Channel Number Raised\r\n" );
        }
    }

    vimREG->REQMASKSET0 = ulPendISRReg0;
    vimREG->REQMASKSET1 = ulPendISRReg1;
    vimREG->REQMASKSET2 = ulPendISRReg2;
    vimREG->REQMASKSET3 = ulPendISRReg3;
}
/*---------------------------------------------------------------------------*/

__attribute__( ( weak ) ) void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                                              StackType_t ** ppxIdleTaskStackBuffer,
                                                              uint32_t * pulIdleTaskStackSize )
{
    /* Pass out a pointer to the StaticTask_t structure in which the Idle
     * task's state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE * 2;
}
/*---------------------------------------------------------------------------*/

void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     StackType_t ** ppxTimerTaskStackBuffer,
                                     uint32_t * pulTimerTaskStackSize )
{
    /* Pass out a pointer to the StaticTask_t structure in which the Timer
     * task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulTimerTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*---------------------------------------------------------------------------*/

PRIVILEGED_FUNCTION portDONT_DISCARD void vHandleMemoryFault( uint32_t * pulFaultStackAddress )
{
    sci_print( "Unexpected MPU Fault\r\n" );
    /* This is an unexpected fault - loop forever. */
    configASSERT( 0 );
}

void * xApplicationPrivilegedCallback( void * pxParamters )
{
    sci_print( "Running xApplicationPrivilegedCallback" );
    volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0UL;

    /* These variables can be inspected in a debugger. */
    while( ulSetToNonZeroInDebuggerToContinue == 0UL )
    {
        __asm volatile ( "NOP" );
    }

    return ( void * ) ulSetToNonZeroInDebuggerToContinue;
}

void _close( void )
{
    safePANIC();
}

void _lseek( void )
{
    safePANIC();
}

void _read( void )
{
    safePANIC();
}

void _write( void )
{
    safePANIC();
}

void _fstat( void )
{
    safePANIC();
}

void _getpid( void )
{
    safePANIC();
}

void _isatty( void )
{
    safePANIC();
}

void _kill( void )
{
    safePANIC();
}
