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

static PduIdType findValidTxPduId(void)
{
    PduIdType pduId = PDU_INVALID;

    for (uint32 connItr = 0; connItr < ARR_SIZE(CanTp_State.txConnections); connItr++) {
        if (CanTp_State.txConnections[connItr].nsdu) {
            pduId = CanTp_State.txConnections[connItr].nsdu->id;
            break;
        }
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
    PduIdType PduId = findValidTxPduId();

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

    PduIdType pduId = findValidTxPduId();
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
#define TEST_SF_MESSAGE_LEN 7

static uint8 PduR_Buffer[64];
static uint8 PduR_BufferUsage;

static uint8 CanIf_Buffer[64];
static uint8 CanIf_BufferUsage;

static PduInfoType CopyTxPduInfoHistory[8];
static PduInfoType TxConfPduInfoHistory[8];

static BufReq_ReturnType PduR_CanTpCopyTxData_1(PduIdType txPduId, const PduInfoType *pPduInfo,
                                                const RetryInfoType *pRetryInfo,
                                                PduLengthType *pAvailableData)
{
    CopyTxPduInfoHistory[0].SduLength = pPduInfo->SduLength;

    memcpy(pPduInfo->SduDataPtr, PduR_Buffer, pPduInfo->SduLength);

    PduR_BufferUsage -= pPduInfo->SduLength;

    return BUFREQ_OK;
}

static Std_ReturnType CanIf_Transmit_1(PduIdType txPduId, const PduInfoType *pPduInfo)
{
    memcpy(CanIf_Buffer, pPduInfo->SduDataPtr, pPduInfo->SduLength);
    CanIf_BufferUsage = pPduInfo->SduLength;
    return E_OK;
}

static void ValidSingleDetailedTest(void)
{
    PduInfoType pduInfo = {
        .SduLength = TEST_SF_MESSAGE_LEN,
    };

    PduIdType pduId = findValidTxPduId();
    Std_ReturnType transmitResult;
    uint8 sduLengthPassedToCanIf = TEST_SF_MESSAGE_LEN + 1; // 5 payload bytes + 1 CanTp header byte
    uint8 bufSizeRequestedFromPduR = TEST_SF_MESSAGE_LEN;

    CanTp_State.activation = CANTP_ON;

    /** Prepare the buffer for sending the data */
    memcpy(PduR_Buffer, TEST_SF_MESSAGE, TEST_SF_MESSAGE_LEN);
    PduR_BufferUsage = TEST_SF_MESSAGE_LEN;

    memset(CanIf_Buffer, 0, ARR_SIZE(CanIf_Buffer));
    CanIf_BufferUsage = 0;

    /** Use implemented fake functions */
    PduR_CanTpCopyTxData_fake.custom_fake = PduR_CanTpCopyTxData_1;
    CanIf_Transmit_fake.custom_fake = CanIf_Transmit_1;

    transmitResult = CanTp_Transmit(pduId, &pduInfo);

    for (int i = 0; i < 2; i++) {
        CanTp_MainFunction();
    }

    /** Check amount of data */
    TEST_CHECK(PduR_BufferUsage == 0);
    TEST_MSG("CanTp didn't take proper number of bytes");

    TEST_CHECK(CopyTxPduInfoHistory[0].SduLength == bufSizeRequestedFromPduR);
    TEST_MSG("CanTp tried to allocate invalid number of bytes from PduR");

    TEST_CHECK(((PduInfoType *)CanIf_Transmit_fake.arg1_val)->SduLength == sduLengthPassedToCanIf);
    TEST_MSG("CanTp tried to pass an invalid number of bytes to CanIf");

    TEST_CHECK(CanIf_BufferUsage == sduLengthPassedToCanIf);
    TEST_MSG("CanTp passed invalid amount of bytes to CanIf");

    /** Check content of the message */
    TEST_CHECK(0 == memcmp(PduR_Buffer, CanIf_Buffer + 1, bufSizeRequestedFromPduR));

    TEST_CHECK(CanIf_Buffer[0] == CANTP_N_PCI_TYPE_SF);
}

TEST_LINKED_LIST_ENTRY CanTp_Transmit_TEST_LIST[] = {
    {"CanTp_Transmit Attempt to transmit using invalid PduId", InvalidPduIdTest},
    {"CanTp_Transmit Passing invalid data", InvalidDataPtrTest},
    {"CanTp_Transmit SF transmission", ValidSingleFrameSimpleTest},
    {NULL, NULL}};
