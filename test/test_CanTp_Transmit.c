#define TEST_NO_MAIN

#include <fff.h>
#include <acutest.h>
#include <string.h>
#include <Test_Stub.h>

#define PDU_INVALID (PduIdType)0xFFFFFFFF

/** Place fake functions before including CanTp.c to avoid
 * 'warning: ‘Det_ReportRuntimeError’ used but never defined'
 */

FAKE_STATIC_VALUE_FUNC2(Std_ReturnType, CanIf_Transmit, PduIdType, const PduInfoType *);

FAKE_STATIC_VALUE_FUNC4(BufReq_ReturnType, PduR_CanTpCopyTxData, PduIdType, const PduInfoType *,
                        const RetryInfoType *, PduLengthType *);

FAKE_VOID_FUNC2(PduR_CanTpTxConfirmation, PduIdType, Std_ReturnType);

#include <CanTp.c>

static PduIdType findNextValidTxPduId(void)
{
    static uint32 connItr = 0;

    PduIdType pduId = PDU_INVALID;

    for (connItr = 0; pduId == PDU_INVALID; connItr++) {
        if (CanTp_State.txConnections[connItr].nsdu) {
            pduId = CanTp_State.txConnections[connItr].nsdu->id;
            break;
        }

        connItr = connItr % ARR_SIZE(CanTp_State.txConnections);
    }

    TEST_ASSERT(pduId != PDU_INVALID);
    TEST_MSG("Could not find any PduId -> Modify CanTp config so it have at least one tx nsdu\n");

    return pduId;
}

static void InvalidPduIdTest(void)
{
    boolean PduInvalid = FALSE;

    uint8 data[6] = {1};
    PduInfoType pduInfo = {.SduLength = 6, .SduDataPtr = data, .MetaDataPtr = NULL};
    PduIdType randomPduId;

    Std_ReturnType result;

    srand(time(0));

    do {
        PduIdType randomPduId = rand() % 128;
        PduInvalid = TRUE;

        /** Assure that there is no configured nsdu with randomized pduId */
        for (uint32 connItr = 0; connItr < ARR_SIZE(CanTp_State.txConnections); connItr++) {
            if (CanTp_State.txConnections[connItr].nsdu) {
                if (CanTp_State.txConnections[connItr].nsdu->id == randomPduId) {
                    PduInvalid = FALSE;
                }
            }
        }
    } while (!PduInvalid);

    result = CanTp_Transmit(randomPduId, &pduInfo);
    TEST_CHECK(result == E_NOT_OK);
}

static void InvalidDataPtrTest(void)
{
    Std_ReturnType result;
    PduInfoType pduInfo = {.SduLength = 6, .SduDataPtr = NULL, .MetaDataPtr = NULL};
    PduIdType PduId = findNextValidTxPduId();

    result = CanTp_Transmit(PduId, &pduInfo);
    TEST_CHECK(result == E_NOT_OK);
}

static void ValidSingleFrameSimpleTest(void)
{
    uint8 sdu[] = {0xF, 0xA, 0xD, 0xE, 0xD};
    PduInfoType pduInfo = {
        .SduDataPtr = sdu,
        .SduLength = ARR_SIZE(sdu),
    };

    PduIdType pduId = findNextValidTxPduId();
    Std_ReturnType transmitResult;
    uint8 sduLengthPassedToCanIf = ARR_SIZE(sdu) + 1; // 5 payload bytes + 1 CanTp header byte

    CanTp_State.activation = CANTP_ON;

    transmitResult = CanTp_Transmit(pduId, &pduInfo);

    for (int i = 0; i < 2; i++) {
        CanTp_MainFunction();
    }

    /** Verification of CanIf_Transmit usage */
    TEST_CHECK(CanIf_Transmit_fake.call_count == 1);
    TEST_MSG("CanIf_Transmit was called %u/%u times", CanIf_Transmit_fake.call_count, 1);

    TEST_CHECK(CanIf_Transmit_fake.arg0_val == pduId);
    TEST_MSG("CanIf_Transmit was called for other pduId (0x%x)", CanIf_Transmit_fake.arg0_val);

    /** Not: can do that, because the buffer passed by pointer is statically declared */
    TEST_CHECK(((PduInfoType *)CanIf_Transmit_fake.arg1_val)->SduLength == sduLengthPassedToCanIf);
    TEST_MSG("CanIf_Transmit was called with bad sdu length (%u)",
             ((PduInfoType *)CanIf_Transmit_fake.arg1_val)->SduLength);

    /** Verification of PduR_CanTpCopyTxData usage */
    TEST_CHECK(PduR_CanTpCopyTxData_fake.call_count == 1);
    TEST_MSG("PduR_CanTpCopyTxData was called %u/%u times", PduR_CanTpCopyTxData_fake.call_count,
             1);

    TEST_CHECK(PduR_CanTpCopyTxData_fake.arg0_val == pduId);
    TEST_MSG("PduR_CanTpCopyTxData was called for other pduId (0x%x)",
             PduR_CanTpCopyTxData_fake.arg0_val);

    /** Verification of PduR_CanTpTxConfirmation usage */
    TEST_CHECK(PduR_CanTpTxConfirmation_fake.call_count == 1);
    TEST_MSG("PduR_CanTpTxConfirmation was called %u/%u times",
             PduR_CanTpTxConfirmation_fake.call_count, 1);

    TEST_CHECK(PduR_CanTpTxConfirmation_fake.arg0_val == pduId);
    TEST_MSG("PduR_CanTpTxConfirmation was called for other pduId (0x%x)",
             PduR_CanTpTxConfirmation_fake.arg0_val);

    TEST_CHECK(PduR_CanTpTxConfirmation_fake.arg1_val == E_OK);
    TEST_MSG("PduR_CanTpTxConfirmation was called with invalid result",
             PduR_CanTpTxConfirmation_fake.arg1_val);
}

/** Buffer used to exchange data with CanTp */
#define TEST_SF_MESSAGE "ANDRZEJ"
/* don't count termination character*/
#define TEST_SF_MESSAGE_LEN (ARR_SIZE(TEST_SF_MESSAGE) - 1)

#define TEST_FRAGMENTATION_MESSAGE "ANDRZEJWETULA"
/* don't count termination character*/
#define TEST_FRAGMENTATION_MESSAGE_LEN (ARR_SIZE(TEST_FRAGMENTATION_MESSAGE) - 1)

static uint8 PduR_Buffer[64];
static uint8 PduR_BufferUsage;
static uint8 PduR_BufferOffset;

static uint8 CanIf_Buffer[64];
static uint8 CanIf_BufferUsage;

static PduInfoType CopyTxPduInfoHistory[8];
static PduInfoType CanIfTransmitHistory[8];

static uint8 TxPduInfoHistoryCtr;
static uint8 IfTransmitHistoryCtr;

static BufReq_ReturnType PduR_CanTpCopyTxData_1(PduIdType txPduId, const PduInfoType *pPduInfo,
                                                const RetryInfoType *pRetryInfo,
                                                PduLengthType *pAvailableData)
{
    CopyTxPduInfoHistory[TxPduInfoHistoryCtr++].SduLength = pPduInfo->SduLength;

    memcpy(pPduInfo->SduDataPtr, PduR_Buffer + PduR_BufferOffset, pPduInfo->SduLength);

    PduR_BufferUsage -= pPduInfo->SduLength;

    PduR_BufferOffset += pPduInfo->SduLength;

    *pAvailableData = PduR_BufferUsage;

    return BUFREQ_OK;
}

static Std_ReturnType CanIf_Transmit_1(PduIdType txPduId, const PduInfoType *pPduInfo)
{
    CanIfTransmitHistory[IfTransmitHistoryCtr++].SduLength = pPduInfo->SduLength;

    memcpy(CanIf_Buffer + CanIf_BufferUsage, pPduInfo->SduDataPtr, pPduInfo->SduLength);

    CanIf_BufferUsage += pPduInfo->SduLength;

    return E_OK;
}

static void ValidSingleDetailedTest(void)
{
    PduInfoType pduInfo = {
        .SduDataPtr = PduR_Buffer,
        .SduLength = TEST_SF_MESSAGE_LEN,
    };

    PduIdType pduId = findNextValidTxPduId();
    Std_ReturnType transmitResult;
    uint8 sduLengthPassedToCanIf = TEST_SF_MESSAGE_LEN + 1; // 5 payload bytes + 1 CanTp header byte
    uint8 bufSizeRequestedFromPduR = TEST_SF_MESSAGE_LEN;
    const uint8 expectedTpHeader = (CANTP_N_PCI_TYPE_SF << 4) | (TEST_SF_MESSAGE_LEN & 0x0F);

    /** Prepare the buffer for sending the data */
    memcpy(PduR_Buffer, TEST_SF_MESSAGE, TEST_SF_MESSAGE_LEN);
    PduR_BufferUsage = TEST_SF_MESSAGE_LEN;

    memset(CanIf_Buffer, 0, ARR_SIZE(CanIf_Buffer));
    CanIf_BufferUsage = 0;

    RESET_FAKE(PduR_CanTpCopyTxData);
    RESET_FAKE(CanIf_Transmit);
    /** Use implemented fake functions */
    PduR_CanTpCopyTxData_fake.custom_fake = PduR_CanTpCopyTxData_1;
    CanIf_Transmit_fake.custom_fake = CanIf_Transmit_1;

    TxPduInfoHistoryCtr = 0;
    IfTransmitHistoryCtr = 0;

    CanTp_State.activation = CANTP_ON;
    transmitResult = CanTp_Transmit(pduId, &pduInfo);

    for (int i = 0; i < 3; i++) {
        CanTp_MainFunction();
    }

    TEST_CHECK(transmitResult == E_OK);

    TEST_CHECK(CanIf_Transmit_fake.call_count == 1);
    TEST_MSG("CanIf_Transmit was called %u/%u times", CanIf_Transmit_fake.call_count, 1);

    /** Check amount of data */
    TEST_CHECK(PduR_BufferUsage == 0);
    TEST_MSG("CanTp didn't take proper number of bytes (%u remained)", PduR_BufferUsage);

    TEST_CHECK(CopyTxPduInfoHistory[0].SduLength == bufSizeRequestedFromPduR);
    TEST_MSG("CanTp tried to allocate invalid number of bytes from PduR (got %u)",
             CopyTxPduInfoHistory[0].SduLength);

    TEST_CHECK(CanIfTransmitHistory[0].SduLength == sduLengthPassedToCanIf);
    TEST_MSG("CanTp tried to pass an invalid number of bytes to CanIf");

    TEST_CHECK(CanIf_BufferUsage == sduLengthPassedToCanIf);
    TEST_MSG("CanTp passed invalid amount of bytes to CanIf");

    /** Check content of the message */
    TEST_CHECK(0 == memcmp(PduR_Buffer, CanIf_Buffer + 1, bufSizeRequestedFromPduR));

    TEST_CHECK(CanIf_Buffer[0] == expectedTpHeader);
    TEST_MSG("CanTp Header is (0x%x), expected(0x%x)", expectedTpHeader);
}

static void FragmentationSimpleTest(void)
{
    PduIdType pduId = findNextValidTxPduId();
    PduInfoType pduInfo = {
        .SduDataPtr = PduR_Buffer,
        .SduLength = TEST_FRAGMENTATION_MESSAGE_LEN,
    };

    memcpy(PduR_Buffer, TEST_FRAGMENTATION_MESSAGE, TEST_FRAGMENTATION_MESSAGE_LEN);
    PduR_BufferUsage = TEST_FRAGMENTATION_MESSAGE_LEN;

    /** Reset fakes and buffers */
    RESET_FAKE(PduR_CanTpCopyTxData);
    RESET_FAKE(CanIf_Transmit);
    /** Use implemented fake functions */
    PduR_CanTpCopyTxData_fake.custom_fake = PduR_CanTpCopyTxData_1;
    CanIf_Transmit_fake.custom_fake = CanIf_Transmit_1;
    TxPduInfoHistoryCtr = 0;
    IfTransmitHistoryCtr = 0;

    CanTp_State.activation = CANTP_ON;
    TEST_CHECK(CanTp_Transmit(pduId, &pduInfo) == E_OK);
    TEST_MSG("CanTp_Transmit(%u, %p) didn't returned E_OK", pduId, &pduInfo);

    /** Check FF */

    /** Loop to have it in state of waiting for FC */
    CanTp_MainFunction();

    /** Verification of PduR_CanTpCopyTxData usage */
    TEST_CHECK(PduR_CanTpCopyTxData_fake.call_count == 1);
    TEST_MSG("PduR_CanTpCopyTxData was called %u/%u times", PduR_CanTpCopyTxData_fake.call_count,
             1);

    TEST_CHECK(PduR_CanTpCopyTxData_fake.arg0_val == pduId);
    TEST_MSG("PduR_CanTpCopyTxData was called for other pduId (0x%x)",
             PduR_CanTpCopyTxData_fake.arg0_val);

    /** First Frame should pass 6 bytes of data from PduR */
    TEST_CHECK(CopyTxPduInfoHistory[0].SduLength == 6);
    TEST_MSG("PduR_CanTpCopyTxData was called with non proper sdu length (%u)",
             CopyTxPduInfoHistory[0].SduLength);

    CanTp_MainFunction();

    /** Verification of CanIf_Transmit usage */
    TEST_CHECK(CanIf_Transmit_fake.call_count == 1);
    TEST_MSG("CanIf_Transmit was called %u/%u times", CanIf_Transmit_fake.call_count, 1);

    TEST_CHECK(CanIf_Transmit_fake.arg0_val == pduId);
    TEST_MSG("CanIf_Transmit was called for other pduId (0x%x)", CanIf_Transmit_fake.arg0_val);

    /** First frame should use the whole payload */
    TEST_CHECK(CanIfTransmitHistory[0].SduLength == 8);
    TEST_MSG("CanIf_Transmit was called with bad sdu length (%u)",
             CanIfTransmitHistory[0].SduLength);

    CanTp_RxIndFC(getTxConnection(pduId), NULL, 0);
    CanTp_MainFunction();

    /** Check CF */

    /** Verification of PduR_CanTpCopyTxData usage */
    TEST_CHECK(PduR_CanTpCopyTxData_fake.call_count == 2);
    TEST_MSG("PduR_CanTpCopyTxData was called %u/%u times", PduR_CanTpCopyTxData_fake.call_count,
             2);

    TEST_CHECK(PduR_CanTpCopyTxData_fake.arg0_val == pduId);
    TEST_MSG("PduR_CanTpCopyTxData was called for other pduId (0x%x)",
             PduR_CanTpCopyTxData_fake.arg0_val);

    /** Consecutive frame should pass 7 bytes of data from PduR */
    TEST_CHECK(CopyTxPduInfoHistory[1].SduLength == 7);
    TEST_MSG("PduR_CanTpCopyTxData was called with non proper sdu length (%u)",
             CopyTxPduInfoHistory[1].SduLength);

    CanTp_MainFunction();

    /** Verification of CanIf_Transmit usage */
    TEST_CHECK(CanIf_Transmit_fake.call_count == 2);
    TEST_MSG("CanIf_Transmit was called %u/%u times", CanIf_Transmit_fake.call_count, 2);

    TEST_CHECK(CanIf_Transmit_fake.arg0_val == pduId);
    TEST_MSG("CanIf_Transmit was called for other pduId (0x%x)", CanIf_Transmit_fake.arg0_val);

    /** Second frame should use the whole payload */
    TEST_CHECK(CanIfTransmitHistory[1].SduLength == 8);
    TEST_MSG("CanIf_Transmit was called with bad sdu length (%u)",
             CanIfTransmitHistory[1].SduLength);

    TEST_CHECK(getTxConnection(pduId)->activation == CANTP_TX_STATE_FREE);
    TEST_MSG("The connection was not freed after transmission");
}

#define FRAGMENTATION_DETAILED_MESSAGE "THIS MESSAGE NEEDS TO BE FRAGMENTED"
#define FRAGMENTATION_DETAILED_MESSAGE_LEN (ARR_SIZE(FRAGMENTATION_DETAILED_MESSAGE)-1)

static void FragmentationDetailedTest(void)
{
    PduIdType pduId = findNextValidTxPduId();
    PduInfoType pduInfo = {
        .SduDataPtr = PduR_Buffer,
        .SduLength = FRAGMENTATION_DETAILED_MESSAGE_LEN,
    };

    memcpy(PduR_Buffer, FRAGMENTATION_DETAILED_MESSAGE, FRAGMENTATION_DETAILED_MESSAGE_LEN);
    PduR_BufferUsage = FRAGMENTATION_DETAILED_MESSAGE_LEN;
    CanIf_BufferUsage = 0;

    /** Reset fakes and buffers */
    RESET_FAKE(PduR_CanTpCopyTxData);
    RESET_FAKE(CanIf_Transmit);
    /** Use implemented fake functions */
    PduR_CanTpCopyTxData_fake.custom_fake = PduR_CanTpCopyTxData_1;
    CanIf_Transmit_fake.custom_fake = CanIf_Transmit_1;
    TxPduInfoHistoryCtr = 0;
    IfTransmitHistoryCtr = 0;

    CanTp_State.activation = CANTP_ON;
    TEST_CHECK(CanTp_Transmit(pduId, &pduInfo) == E_OK);
    TEST_MSG("CanTp_Transmit(%u, %p) didn't returned E_OK", pduId, &pduInfo);

    /** Check FF */

    /** Loop to have it in state of waiting for FC */
    CanTp_MainFunction();
    CanTp_MainFunction();

    const uint16 expectedHeader = (CANTP_N_PCI_TYPE_FF << 12) | (pduInfo.SduLength);
    TEST_CHECK(*((uint16*)&CanIf_Buffer[0]) == expectedHeader);
    TEST_MSG("FF header (0x%x) differs from expected (0x%x)", *((uint16*)&CanIf_Buffer[0]), expectedHeader);

    CanTp_RxIndFC(getTxConnection(pduId), NULL, 0);
    CanTp_MainFunction();
    CanTp_MainFunction();

    /** Check CF */
    uint8 sequenceNumber = CANTP_SEQUENCE_NUMBER_START_VALUE;
    const uint8 expectedHeaderCF0 = (CANTP_N_PCI_TYPE_CF << 4) | sequenceNumber;

    const uint8 expectedCFHeaders[] = {
        (CANTP_N_PCI_TYPE_CF << 4) | (sequenceNumber + 0 * CANTP_SEQUENCE_NUMBER_INCREMENT_VALUE),
        (CANTP_N_PCI_TYPE_CF << 4) | (sequenceNumber + 1 * CANTP_SEQUENCE_NUMBER_INCREMENT_VALUE),
        (CANTP_N_PCI_TYPE_CF << 4) | (sequenceNumber + 2 * CANTP_SEQUENCE_NUMBER_INCREMENT_VALUE)
    };

    for(int i=0; i<ARR_SIZE(expectedCFHeaders); i++) {
        CanTp_MainFunction();
        CanTp_MainFunction();
        TEST_CHECK(CanIf_Buffer[8 + i*8] == expectedCFHeaders[i]);
        TEST_MSG("CF header (0x%x) differs from expected (0x%x)", CanIf_Buffer[8 + i*8], expectedCFHeaders[i]);
    }

    CanTp_MainFunction();
    CanTp_MainFunction();

    TEST_CHECK(getTxConnection(pduId)->activation == CANTP_TX_STATE_FREE);
    TEST_MSG("The connection was not freed after transmission");
}

TEST_LINKED_LIST_ENTRY CanTp_Transmit_TEST_LIST[] = {
    {"CanTp_Transmit Attempt to transmit using invalid PduId", InvalidPduIdTest},
    {"CanTp_Transmit Passing invalid data", InvalidDataPtrTest},
    {"CanTp_Transmit SF Simple", ValidSingleFrameSimpleTest},
    {"CanTp_Transmit SF Detailed", ValidSingleDetailedTest},
    {"CanTp_Transmit Simple Fragmentation", FragmentationSimpleTest},
    {"CanTp_Transmit Detailed Fragmentation", FragmentationDetailedTest},
    {NULL, NULL}};
