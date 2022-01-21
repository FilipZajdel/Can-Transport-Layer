#ifndef COM_STACK_TYPES_H
#define COM_STACK_TYPES_H

/*====================================================================================================================*\
 \@file Communication Stack Types


\*====================================================================================================================*/

#include "Std_Types.h"

#define NULL (void*)0

typedef uint16 PduIdType;
typedef uint32 PduLengthType;

typedef struct {
    uint8 *SduDataPtr;
    uint8 *MetaDataPtr;
    PduLengthType SduLength;
} PduInfoType;

typedef uint8 PNCHandleType;

typedef enum {
	TP_STMIN=0x00,
	TP_BS=0x01,
	TP_BC=0x02
} TPParameterType;

typedef enum {
	BUFREQ_OK=0x00,
	BUFREQ_E_NOT_OK=0x01,
	BUFREQ_E_BUSY=0x02,
	BUFREQ_E_OVFL=0x03
} BufReq_ReturnType;

typedef enum {
	TP_DATACONF=0x00,
	TP_DATARETRY=0x01,
	TP_CONFPENDING=0x02
} TpDataStateType;

typedef struct {
	TpDataStateType TpDataState;
	PduLengthType TxTpDataCnt;
} RetryInfoType;

typedef uint8 NetworkHandleType;


#endif /* COM_STACK_TYPES_H */
