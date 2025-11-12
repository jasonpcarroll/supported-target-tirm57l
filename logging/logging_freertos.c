/* Standard includes. */
#include <stdio.h>
#include <stdarg.h>

/* FreeRTOS-Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Logging includes. */
#include "logging.h"

/* Board Support Package includes. */
#include "HL_sci.h"

#define LOGGING_OUTPUT_BUFFER_SIZE    512

void vLoggingInit( void )
{
    sciInit();
}

void prvUARTSend( const char * pucInBuffer,
                  unsigned int ulSize )
{
    unsigned int ulCurByte;

    for( ulCurByte = 0; ulCurByte < ulSize; ++ulCurByte )
    {
        sciSendByte( sciREG1, ( uint8_t ) pucInBuffer[ ulCurByte ] );
    }
}

void vLoggingPrintf( const char * pcFormatString,
                     ... )
{
    static char pcOutputBuffer[ LOGGING_OUTPUT_BUFFER_SIZE ];
    va_list xArgs;
    int32_t lSnprintfRet;

    va_start( xArgs, pcFormatString );

    taskENTER_CRITICAL();
    {
        lSnprintfRet = vsprintf( pcOutputBuffer,
                                 pcFormatString,
                                 xArgs );

        va_end( xArgs );

        configASSERT( lSnprintfRet > 0 && lSnprintfRet < LOGGING_OUTPUT_BUFFER_SIZE );

        prvUARTSend( pcOutputBuffer, lSnprintfRet );
    }
    taskEXIT_CRITICAL();
}
