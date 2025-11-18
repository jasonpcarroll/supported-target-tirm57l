/* Standard includes. */
#include <stdint.h>

/* HALCoGen includes. */
#include "HL_emac.h"
#include "HL_sys_vim.h"
#include "transport.h"

/* Include */
#include "FreeRTOS_IP.h"

static uint8_t ucMACAddress[ 6 ] = { 0x00U, 0x08U, 0xEEU, 0x03U, 0xA6U, 0x6CU };
static const uint8_t ucIPAddress[ 4 ] = { 10, 128, 56, 101 };
static const uint8_t ucNetMask[ 4 ] = { 255, 255, 255, 0 };
static const uint8_t ucGatewayAddress[ 4 ] = { 10, 128, 56, 1 };
static const uint8_t ucDNSServerAddress[ 4 ] = { 8, 8, 8, 8 };

/* Only one network interface. */
static NetworkInterface_t xInterfaces[ 1 ];
static NetworkEndPoint_t xEndPoints[ 4 ];

TransportStatus_t Transport_Init( void )
{
    TransportStatus_t xReturn = TRANSPORT_SUCCESS;
    BaseType_t xFreeRTOSIPInitReturn;

    extern NetworkInterface_t * pxHercules_FillInterfaceDescriptor( BaseType_t xEMACIndex,
                                                                    NetworkInterface_t * pxInterface );

    pxHercules_FillInterfaceDescriptor( 0, &( xInterfaces[ 0 ] ) );

    /* === End-point 0 === */
    FreeRTOS_FillEndPoint( &( xInterfaces[ 0 ] ), &( xEndPoints[ 0 ] ), ucIPAddress, ucNetMask, ucGatewayAddress, ucDNSServerAddress, ucMACAddress );
    #if ( ipconfigUSE_DHCP != 0 )
    {
        /* End-point 0 wants to use DHCPv4. */
        xEndPoints[ 0 ].bits.bWantDHCP = pdTRUE;
    }
    #endif /* ( ipconfigUSE_DHCP != 0 ) */

    xFreeRTOSIPInitReturn = FreeRTOS_IPInit_Multi();

    if( xFreeRTOSIPInitReturn != pdPASS )
    {
        xReturn = TRANSPORT_INIT_FAIL;
    }

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
