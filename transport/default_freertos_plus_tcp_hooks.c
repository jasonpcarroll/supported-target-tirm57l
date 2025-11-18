#include <stdlib.h>
#include <stdint.h>

#include "FreeRTOS.h"

BaseType_t xApplicationGetRandomNumber( uint32_t * pulNumber )
{
    *pulNumber = ( uint32_t ) rand();

    return pdTRUE;
}

uint32_t ulApplicationGetNextSequenceNumber( uint32_t ulSourceAddress,

                                             uint16_t usSourcePort,

                                             uint32_t ulDestinationAddress,

                                             uint16_t usDestinationPort )
{
    return ( uint32_t ) rand();
}
