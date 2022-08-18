#if !defined(_SKLHDAUDBUS_FDO_H_)
#define _SKLHDAUDBUS_FDO_H_

union baseaddr {
    PVOID Base;
    UINT8* baseptr;
};

typedef struct _PCI_BAR {
    union baseaddr Base;
    ULONG Len;
} PCI_BAR, * PPCI_BAR;

#define HDA_UNSOL_QUEUE_SIZE	64
#define HDA_MAX_CODECS		8	/* limit by controller side */

typedef struct _HDAC_RB {
    UINT32 *buf;
    PHYSICAL_ADDRESS addr;
    USHORT rp, wp;
    int cmds[HDA_MAX_CODECS];
    UINT32 res[HDA_MAX_CODECS];
} HDAC_RB, *PHDAC_RB;

typedef struct _FDO_CONTEXT
{
    WDFDEVICE WdfDevice;

    UINT16 venId;
    UINT16 devId;

    PCI_BAR m_BAR0; //required
    PCI_BAR m_BAR4; //Intel AudioDSP
    BUS_INTERFACE_STANDARD BusInterface; //PCI Bus Interface
    WDFINTERRUPT Interrupt;

    UINT32 captureIndexOff;
    UINT32 playbackIndexOff;
    UINT32 numStreams;

    //bit flags of detected codecs
    UINT16 codecMask;

    HDAC_RB corb;
    HDAC_RB rirb;
    unsigned int last_cmd[HDA_MAX_CODECS];
    WDFWAITLOCK cmdLock;

    PVOID rb; //CORB and RIRB buffers
    PVOID posbuf;
} FDO_CONTEXT, * PFDO_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(FDO_CONTEXT, Fdo_GetContext)

NTSTATUS
Fdo_Create(
	_Inout_ PWDFDEVICE_INIT DeviceInit
);

#endif
