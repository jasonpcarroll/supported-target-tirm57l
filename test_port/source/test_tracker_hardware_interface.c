#include <stdint.h>
#include "reg_system.h"
#include "sci.h"
#include "sys_vim.h"
#include "test_tracker_hardware_interface.h"
#include "FreeRTOS.h"
#include "task.h"

/* This interrupt channel supports software interrupts. */
#define REGISTERED_INTERRUPT_CHANNEL    21U

static void prvHardwareRegisterGenericSoftwareInterrupt( void * handlerPtr,
                                                         systemInterrupt_t interruptType );

void vHardwareOutputBuffer( const uint8_t * pucInBuffer,
                            uint32_t ulSize )
{
    uint32_t ulCurByte;

    for( ulCurByte = 0; ulCurByte < ulSize; ++ulCurByte )
    {
        sciSendByte( scilinREG, ( uint8_t ) pucInBuffer[ ulCurByte ] );
    }
}

void vHardwareEnterCriticalSection( void )
{
    if( xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED )
    {
        taskENTER_CRITICAL();
    }
}

void vHardwareExitCriticalSection( void )
{
    if( xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED )
    {
        taskEXIT_CRITICAL();
    }
}

void vHardwareDisableInterrupts( void )
{
    portDISABLE_INTERRUPTS();
}

__attribute__( ( weak ) ) void vApplicationSafeAssertHandler( void )
{
    sci_print( "vSafeAssertError called." );

    while( 1 )
    {
    }
}

__attribute__( ( weak ) ) void vApplicationSVCHandler( void )
{
    sci_print( "vApplicationSVCHandler called." );

    while( 1 )
    {
    }
}

void vHardwareRegisterSoftwareInterrupt( void * handlerPtr )
{
    prvHardwareRegisterGenericSoftwareInterrupt( handlerPtr, SYS_IRQ );
}

void vHardwareRegisterSoftwareFastInterrupt( void * handlerPtr )
{
    prvHardwareRegisterGenericSoftwareInterrupt( handlerPtr, SYS_FIQ );
}

void vHardwareTriggerSoftwareInterrupt( void )
{
    /* Set the interrupt to 'triggered'. */
    systemREG1->SSIR1 = 0x7500;
}

static void prvHardwareRegisterGenericSoftwareInterrupt( void * handlerPtr,
                                                         systemInterrupt_t interruptType )
{
    /* Add the ISR handler function to request channel 21 (software interrupt). */
    vimChannelMap( REGISTERED_INTERRUPT_CHANNEL, REGISTERED_INTERRUPT_CHANNEL, ( t_isrFuncPTR ) handlerPtr );
    /* Now enable the channel to be automatically routed to. */
    vimEnableInterrupt( REGISTERED_INTERRUPT_CHANNEL, interruptType );
}

void vHardwareClearSoftwareInterrupt( void )
{
    /* Register read is needed to mark the end of the IRQ/FIQ */
    volatile uint32_t ulEndOfIntRegVal = systemREG1->SSIVEC;
    
    ( void ) ulEndOfIntRegVal;
}
