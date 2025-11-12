#include <stdint.h>
#include "HL_reg_vim.h"
#include "HL_sys_core.h"

typedef void (* ISRFunction_t)( void );

/** @brief Default IRQ Handler used in the ARM_Cortex_RX ports.
 * @note This Handler is directly tied to the Texas Instrument's Hercules
 * Vectored Interrupt Manager (VIM). For more information about what
 * this is and how it operates please refer to their document:
 * https://www.ti.com/lit/pdf/spna218
 */
void vApplicationIRQHandler( void )
{
    /* Load the IRQ Channel Number and Function PTR from the VIM. */
    ISRFunction_t xIRQFncPtr = ( ISRFunction_t ) vimREG->IRQVECREG;

    /* Save current IRQ mask. */
    uint32_t ulSaveIRQMask0 = vimREG->REQMASKSET0;
    uint32_t ulSaveIRQMask1 = vimREG->REQMASKSET1;
    uint32_t ulSaveIRQMask2 = vimREG->REQMASKSET2;
    uint32_t ulSaveIRQMask3 = vimREG->REQMASKSET3;

    /* Mask all lower priority IRQs and self. */
    if( vimREG->IRQINDEX <= 32U )
    {
        vimREG->REQMASKCLR0 = 0xFFFFFFFFU << ( vimREG->IRQINDEX - 1 );
        vimREG->REQMASKCLR1 = 0xFFFFFFFFU;
        vimREG->REQMASKCLR2 = 0xFFFFFFFFU;
        vimREG->REQMASKCLR3 = 0xFFFFFFFFU;
    }
    else if( vimREG->IRQINDEX <= 64U )
    {
        vimREG->REQMASKCLR1 = 0xFFFFFFFFU << ( vimREG->IRQINDEX - 33U );
        vimREG->REQMASKCLR2 = 0xFFFFFFFFU;
        vimREG->REQMASKCLR3 = 0xFFFFFFFFU;
    }
    else if( vimREG->IRQINDEX <= 96U )
    {
        vimREG->REQMASKCLR2 = 0xFFFFFFFFU << ( vimREG->IRQINDEX - 65U );
        vimREG->REQMASKCLR3 = 0xFFFFFFFFU;
    }
    else
    {
        vimREG->REQMASKCLR3 = 0xFFFFFFFFU << ( vimREG->IRQINDEX - 96U );
    }

    /* Re-enable interrupts to allow nesting. */
    _enable_IRQ_interrupt_();

    xIRQFncPtr();

    /* Disable interrupts to restore IRQ mask. */
    _disable_IRQ_interrupt_();

    /* Restore IRQ mask prior to entry. */
    vimREG->REQMASKSET0 = ulSaveIRQMask0;
    vimREG->REQMASKSET1 = ulSaveIRQMask1;
    vimREG->REQMASKSET2 = ulSaveIRQMask2;
    vimREG->REQMASKSET3 = ulSaveIRQMask3;
}
