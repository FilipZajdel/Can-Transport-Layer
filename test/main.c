#include <fff.h>

DEFINE_FFF_GLOBALS;

#define ACUTEST_USE_LINKED_TEST_LIST
#include <acutest.h>

#include <Std_Types.h>
#include <ComStack_Types.h>

/**
 * Definitions of all APIs used by CanTp.c as weak to fake only those functions
 * that are needed for particular test.
 */
Std_ReturnType __attribute__((weak))
Det_ReportRuntimeError(uint16 moduleId, uint8 instanceId, uint8 apiId, uint8 errorId)
{
    (void)moduleId;
    (void)instanceId;
    (void)apiId;
    (void)errorId;
    return E_OK;
}

Std_ReturnType __attribute__((weak)) CanIf_Transmit(PduIdType txPduId, const PduInfoType *pPduInfo)
{
    (void)txPduId;
    (void)pPduInfo;
    return E_OK;
}

BufReq_ReturnType __attribute__((weak))
PduR_CanTpCopyRxData(PduIdType rxPduId, const PduInfoType *pPduInfo, PduLengthType *pBuffer)
{
    (void)rxPduId;
    (void)pPduInfo;
    (void)pBuffer;
    return BUFREQ_OK;
}

BufReq_ReturnType __attribute__((weak))
PduR_CanTpCopyTxData(PduIdType txPduId, const PduInfoType *pPduInfo,
                     const RetryInfoType *pRetryInfo, PduLengthType *pAvailableData)
{
    (void)txPduId;
    (void)pPduInfo;
    (void)pRetryInfo;
    (void)pAvailableData;
    return BUFREQ_OK;
}

void __attribute__((weak)) PduR_CanTpRxIndication(PduIdType rxPduId, Std_ReturnType result)
{
    (void)rxPduId;
    (void)result;
}

BufReq_ReturnType __attribute__((weak))
PduR_CanTpStartOfReception(PduIdType pduId, const PduInfoType *pPduInfo, PduLengthType tpSduLength,
                           PduLengthType *pBufferSize)
{
    (void)pduId;
    (void)pPduInfo;
    (void)tpSduLength;
    (void)pBufferSize;
    return BUFREQ_OK;
}

void __attribute__((weak)) PduR_CanTpTxConfirmation(PduIdType txPduId, Std_ReturnType result)
{
    (void)txPduId;
    (void)result;
}

extern TEST_LINKED_LIST_ENTRY CanTp_CancelReceive_TEST_LIST[];
extern TEST_LINKED_LIST_ENTRY CanTp_CancelTransmit_TEST_LIST[];
extern TEST_LINKED_LIST_ENTRY CanTp_ReadParameter_TEST_LIST[];
extern TEST_LINKED_LIST_ENTRY CanTp_ChangeParameter_TEST_LIST[];
extern TEST_LINKED_LIST_ENTRY CanTp_GetVersionInfo_TEST_LIST[];
extern TEST_LINKED_LIST_ENTRY CanTp_Init_TEST_LIST[];
extern TEST_LINKED_LIST_ENTRY CanTp_MainFunction_TEST_LIST[];
extern TEST_LINKED_LIST_ENTRY CanTp_RxIndication_TEST_LIST[];
extern TEST_LINKED_LIST_ENTRY CanTp_Shutdown_TEST_LIST[];
extern TEST_LINKED_LIST_ENTRY CanTp_Transmit_TEST_LIST[];
extern TEST_LINKED_LIST_ENTRY CanTp_TxConfirmation_TEST_LIST[];

TEST_LINKED_LIST = {
    CanTp_CancelReceive_TEST_LIST,   CanTp_CancelTransmit_TEST_LIST, CanTp_ReadParameter_TEST_LIST,
    CanTp_ChangeParameter_TEST_LIST, CanTp_Init_TEST_LIST,
    CanTp_MainFunction_TEST_LIST,    CanTp_RxIndication_TEST_LIST,   CanTp_Shutdown_TEST_LIST,
    CanTp_Transmit_TEST_LIST,        TEST_LINKED_LIST_END};
