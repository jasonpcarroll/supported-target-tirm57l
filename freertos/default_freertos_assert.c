#include "FreeRTOS.h"
#include "task.h"

__attribute__( ( weak ) ) void vAssertCalled( const char * const pcFileName,
                                              unsigned long ulLine )
{
    volatile unsigned long ulSetToNonZeroInDebuggerToContinue = 0;

    taskENTER_CRITICAL();
    {
        /* You can step out of this function to debug the assertion by using
         * the debugger to set ulSetToNonZeroInDebuggerToContinue to a non-zero
         * value. */
        while( ulSetToNonZeroInDebuggerToContinue == 0 )
        {
        }
    }
    taskEXIT_CRITICAL();
}
