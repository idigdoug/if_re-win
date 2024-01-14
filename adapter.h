#pragma once

#include "if_re_bsd.h"
#include "bsdexport.h"

#define RT_MAX_TX_QUEUES (2)
#define RT_MAX_RX_QUEUES (4)
#define RT_MAX_QUEUES RT_MAX_RX_QUEUES

typedef enum REG_SPEED_SETTING {
    RtSpeedDuplexModeAutoNegotiation = 0,
    RtSpeedDuplexMode10MHalfDuplex = 1,
    RtSpeedDuplexMode10MFullDuplex = 2,
    RtSpeedDuplexMode100MHalfDuplex = 3,
    RtSpeedDuplexMode100MFullDuplex = 4,
    RtSpeedDuplexMode1GHalfDuplex = 5,
    RtSpeedDuplexMode1GFullDuplex = 6,
    RtSpeedDuplexMode2GFullDuplex = 7,
    RtSpeedDuplexMode5GFullDuplex = 8
} REG_SPEED_SETTING;

typedef enum FLOW_CTRL {
    FlowControl,
    NoFlowControl
};

typedef struct _RT_ADAPTER
{
    // WDF handles associated with this context
    NETADAPTER NetAdapter;
    WDFDEVICE WdfDevice;

    //Handle to default Tx and Rx Queues
    NETPACKETQUEUE TxQueues[RT_MAX_TX_QUEUES];
    NETPACKETQUEUE RxQueues[RT_MAX_RX_QUEUES];

    // spin locks
    WDFSPINLOCK Lock;

    WDFDMAENABLER DmaEnabler;

    // MMIO
    PVOID MMIOAddress;
    SIZE_T MMIOSize;
    BUS_INTERFACE_STANDARD PciConfig;

    // Pointer to interrupt object
    RT_INTERRUPT* Interrupt;

    // Configuration
    REG_SPEED_SETTING SpeedDuplex;
    NET_ADAPTER_LINK_LAYER_ADDRESS PermanentAddress;
    NET_ADAPTER_LINK_LAYER_ADDRESS CurrentAddress;
    BOOLEAN OverrideAddress;

    BOOLEAN isRTL8125;
    ULONG64 MaxSpeed;

    FLOW_CTRL reqFlowControl;

    struct re_softc bsdData;
} RT_ADAPTER, * PRT_ADAPTER;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(RT_ADAPTER, RtGetAdapterContext);

EVT_NET_ADAPTER_CREATE_TXQUEUE   EvtAdapterCreateTxQueue;
EVT_NET_ADAPTER_CREATE_RXQUEUE   EvtAdapterCreateRxQueue;

NTSTATUS
RtInitializeAdapterContext(
    _In_ RT_ADAPTER* adapter,
    _In_ WDFDEVICE device,
    _In_ NETADAPTER netAdapter);

NTSTATUS
RtAdapterStart(
    _In_ RT_ADAPTER* adapter);

UINT8 ConfigRead8(_In_ RT_ADAPTER* adapter, UINT32 reg);
UINT16 ConfigRead16(_In_ RT_ADAPTER* adapter, UINT32 reg);
void ConfigWrite8(_In_ RT_ADAPTER* adapter, UINT32 reg, UINT8 val);
void ConfigWrite16(_In_ RT_ADAPTER* adapter, UINT32 reg, UINT16 val);

void RtResetQueues(_In_ RT_ADAPTER* adapter);