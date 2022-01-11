#ifndef CAN_TP_TYPES_H
#define CAN_TP_TYPES_H

/*====================================================================================================================*\
 \@file CAN Transport Layer Types definitions

 FilipZajdel & michal-szymocha (c) 2022
\*====================================================================================================================*/

#include "ComStack_Types.h"

typedef uint32_t CanTp_NSduId;

typedef enum
{
    CANTP_STANDARD = 0,
    CANTP_NORMALFIXED,
    CANTP_MIXED29BIT,
    CANTP_MIXED,
    CANTP_EXTENDED
} CanTp_AddressingFormatType;

typedef enum
{
    CANTP_ON = 0,
    CANTP_OFF
} CanTp_PaddingActivationType;

typedef enum
{
    CANTP_PHYSICAL = 0,
    CANTP_FUNCTIONAL
} CanTp_TaTypeType;

typedef struct
{
    uint8 nAe;
} CanTp_NAeType;

typedef struct
{
    uint8 nSa;
} CanTp_NSaType;

typedef struct
{
    uint8 nTa;
} CanTp_NTaType;

typedef struct
{
    uint16 id;
    PduIdType ref;
} CanTp_NPduType;

typedef struct
{
    uint16 nPduConfirmationPduId;
    PduIdType ref;
} CanTp_FcNPduType;

typedef struct
{
    uint8 bs;
    uint32 nar;
    uint32 nbr;
    uint32 ncr;
    CanTp_AddressingFormatType addressingFormat;
    uint16 id;
    CanTp_PaddingActivationType paddingActivation;
    CanTp_TaTypeType taType;
    uint16 wftMax;
    uint32 STmin;
    PduIdType ref;
    const CanTp_NAeType *pNAe;
    const CanTp_NSaType *pNSa;
    const CanTp_NTaType *pNTa;
    const CanTp_NPduType *rxNPdu;
    const CanTp_FcNPduType *txFcNPdu;
} CanTp_RxNSduType;

typedef struct
{
    uint32 nas;
    uint32 nbs;
    uint32 ncs;
    boolean tc;
    CanTp_AddressingFormatType addressingFormat;
    uint16 id;
    CanTp_PaddingActivationType paddingActivation;
    CanTp_TaTypeType taType;
    PduIdType ref;
    const CanTp_NAeType *pNAe;
    const CanTp_NSaType *pNSa;
    const CanTp_NTaType *pNTa;
    const CanTp_NPduType *txNPdu;
    const CanTp_FcNPduType *rxFcNPdu;
} CanTp_txNSduType;

typedef struct
{
    CanTp_RxNSduType *rxNSdu;
    uint32 rxNSduCount;
    CanTp_txNSduType *txNSdu;
    uint32 txNSduCount;
} CanTp_ChannelType;

typedef struct
{
    uint32 mainFunctionPeriod;
    uint32 maxChannelCnt;
    CanTp_ChannelType *channels;
} CanTp_ConfigType;

#endif /* CAN_TP_TYPES_H */
