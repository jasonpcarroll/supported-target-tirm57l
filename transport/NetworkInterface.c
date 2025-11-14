/* Standard includes. */
#include <stdio.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* HALCoGen generated header files */
#include "HL_emac.h"
#include "HL_mdio.h"
#include "HL_phy_dp83640.h"
#include "HL_sys_vim.h"
#include "HL_gio.h"
#include "HL_reg_het.h"

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_IP.h"
#include "FreeRTOS_IP_Private.h"
#include "NetworkBufferManagement.h"

extern hdkif_t hdkif_data[ MAX_EMAC_INSTANCE ];

static uint8_t ucMACAddress[ 6 ] = { 0x00U, 0x08U, 0xEEU, 0x03U, 0xA6U, 0x6CU };

TaskHandle_t xEMACRXTaskHandle = NULL;

#define EMAC_RX_PULSE_INT_VIM_CHANNEL    79
#define EMAC_TX_PULSE_INT_VIM_CHANNEL    77

void vFreeRTOSEMACTXInterruptHandler( void )
{
    hdkif_t * hdkif;

    hdkif = &hdkif_data[ 0U ];
    EMACTxIntHandler( hdkif );
    EMACCoreIntAck( hdkif->emac_base, ( uint32 ) EMAC_INT_CORE0_TX );
}

void vFreeRTOSEMACRXInterruptHandler( void )
{
}

static void prvEMACRXTask( void * pvParameters )
{
    uint16_t xPacketLength;
    NetworkBufferDescriptor_t * pxBufferDescriptor;
    IPStackEvent_t xRxEvent;

    while( 1 )
    {
        /* Wait for interrupt handler to say data is ready to be received. */
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
        
    }
}

static BaseType_t xHercules_NetworkInterfaceInitialise( NetworkInterface_t * pxInterface )
{
    uint32_t ulEMACHWInitReturn;
    BasetType_t xResult = pdPASS;
    BaseType_t xEMACIndex = ( BaseType_t ) pxInterface->pvArgument;

    /* Map interrupts to VIM. */
    vimChannelMap( EMAC_RX_PULSE_INT_VIM_CHANNEL, EMAC_RX_PULSE_INT_VIM_CHANNEL, vFreeRTOSEMACRXInterruptHandler );
    vimChannelMap( EMAC_TX_PULSE_INT_VIM_CHANNEL, EMAC_TX_PULSE_INT_VIM_CHANNEL, vFreeRTOSEMACTXInterruptHandler );
    vimEnableInterrupt( EMAC_RX_PULSE_INT_VIM_CHANNEL );
    vimEnableInterrupt( EMAC_TX_PULSE_INT_VIM_CHANNEL );

    ulEMACHWInitReturn = EMACHWInit( ucMACAddress );

    if( ulEMACHWInitReturn != EMAC_ERR_OK )
    {
        xResult = pdFAIL;
    }

    if( xResult == pdPASS )
    {
    }

    return xResult;
}

static BaseType_t xHercules_NetworkInterfaceOutput( NetworkInterface_t * pxInterface,
                                                    NetworkBufferDescriptor_t * const pxNetworkBuffer,
                                                    BaseType_t xReleaseAfterSend )
{
    BaseType_t xResult = pdPASS;
    boolean xEMACTransmitReturn;
    BaseType_t xEMACIndex = ( BaseType_t ) pxInterface->pvArgument;

    pbuf_t xOutputBuffer;

    xOutputBuffer.next = NULL;

    xOutputBuffer.payload = pxNetworkBuffer->pucEthernetBuffer;

    if( pxNetworkBuffer->xDataLength < MIN_PKT_LEN )
    {
        xOutputBuffer.len = MIN_PKT_LEN;
        xOutputBuffer.tot_len = MIN_PKT_LEN;
    }
    else
    {
        xOutputBuffer.len = pxNetworkBuffer->xDataLength;
        xOutputBuffer.tot_len = pxNetworkBuffer->xDataLength;
    }

    /* Disable EMAC interrupts. */
    vimDisableInterrupt( EMAC_RX_PULSE_INT_VIM_CHANNEL );
    vimDisableInterrupt( EMAC_TX_PULSE_INT_VIM_CHANNEL );

    xEMACTransmitReturn = EMACTransmit( &hdkif_data[ xEMACIndex ], &xOutputBuffer );

    vimEnableInterrupt( EMAC_RX_PULSE_INT_VIM_CHANNEL );
    vimEnableInterrupt( EMAC_TX_PULSE_INT_VIM_CHANNEL );

    if( xEMACTransmitReturn == false )
    {
        xResult = pdFAIL;
    }

    if( xReleaseAfterSend == pdTRUE )
    {
        vReleaseNetworkBufferAndDescriptor( pxNetworkBuffer );
    }

    return xResult;
}

static BaseType_t xHercules_GetPhyLinkStatus( NetworkInterface_t * pxInterface )
{
    BaseType_t xEMACIndex = ( BaseType_t ) pxInterface->pvArgument;

    boolean xPhyStatus = Dp83640LinkStatusGet( hdkif_data[ xEMACIndex ].mdio_base, EMAC_PHYADDRESS, 10 );

    return xPhyStatus: pdTRUE ? pdFALSE;
}

NetworkInterface_t * pxHercules_FillInterfaceDescriptor( BaseType_t xEMACIndex,
                                                         NetworkInterface_t * pxInterface )
{
    pxInterface->pvArgument = ( void * ) xEMACIndex; /* Has only meaning for the driver functions. */
    pxInterface->pfInitialise = xHercules_NetworkInterfaceInitialise;
    pxInterface->pfOutput = xHercules_NetworkInterfaceOutput;
    pxInterface->pfGetPhyLinkStatus = xHercules_GetPhyLinkStatus;

    FreeRTOS_AddNetworkInterface( pxInterface );

    return pxInterface;
}
