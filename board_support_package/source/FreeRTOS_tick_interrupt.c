#include "HL_rti.h"

void vMainSetupTimerInterrupt( void )
{
    rtiInit();
    rtiEnableNotification( rtiREG1, rtiNOTIFICATION_COMPARE0 );
    rtiStartCounter( rtiREG1, rtiCOUNTER_BLOCK0 );
}