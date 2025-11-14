/* Standard includes. */
#include <stdint.h>

/* HALCoGen includes. */
#include "HL_emac.h"
#include "transport.h"


static uint8_t ucMACAddress[ 6 ] = { 0x00U, 0x08U, 0xEEU, 0x03U, 0xA6U, 0x6CU };

TransportStatus_t Transport_Init( void )
{
    TransportStatus_t xReturn = TRANSPORT_SUCCESS;

    EMACHWInit( ucMACAddress );
    return xReturn;
}

TransportStatus_t Transport_Connect( NetworkContext_t * pNetworkContext,
                                     const ServerInfo_t * pServerInfo,
                                     const TransportCredentials_t * pCredentials,
                                     uint32_t sendTimeoutMs,
                                     uint32_t recvTimeoutMs )
{
    TransportStatus_t xReturn = TRANSPORT_SUCCESS;

    return xReturn;
}

TransportStatus_t Transport_Disconnect( const NetworkContext_t * pNetworkContext )
{
    TransportStatus_t xReturn = TRANSPORT_SUCCESS;

    return xReturn;
}

int32_t Transport_Recv( NetworkContext_t * pNetworkContext,
                        void * pBuffer,
                        size_t bytesToRecv )
{
    int32_t xReturn = 0;

    return xReturn;
}

int32_t Transport_Send( NetworkContext_t * pNetworkContext,
                        const void * pBuffer,
                        size_t bytesToSend )
{
    int32_t xReturn = 0;

    return xReturn;
}
