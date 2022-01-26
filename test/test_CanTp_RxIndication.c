#define TEST_NO_MAIN

#include <fff.h>
#include <acutest.h>
#include <Test_Stub.h>
#include <CanTp.c>
#include <stdlib.h>

#define TEST_1_PDU_PAYLOAD_LEN 6
#define TEST_1_PDU_ID 102

#define TEST_2_PDU_PAYLOAD_LEN 7
#define TEST_2_PDU_ID 103

#define TEST_3_PDU_PAYLOAD_LEN 6
#define TEST_3_PDU_ID 104

uint8 testBuffer[8];

FAKE_STATIC_VALUE_FUNC3(BufReq_ReturnType, PduR_CanTpCopyRxData, PduIdType, const PduInfoType *,
                        PduLengthType *);

FAKE_STATIC_VALUE_FUNC4(BufReq_ReturnType, PduR_CanTpStartOfReception, PduIdType,
                        const PduInfoType *, PduLengthType, PduLengthType *);
FAKE_STATIC_VOID_FUNC2(PduR_CanTpRxIndication, PduIdType, Std_ReturnType);

static BufReq_ReturnType fake_impl_PduR_CanTpCopyRxData(PduIdType rxPduId,
                                                        const PduInfoType *pPduInfo,
                                                        PduLengthType *pBuffer)
{
    for (PduLengthType i = 0; i < pPduInfo->SduLength; i++) {
        testBuffer[i] = pPduInfo->SduDataPtr[i];
    }
    return BUFREQ_OK;
}

static BufReq_ReturnType fake_impl_PduR_CanTpStartOfReception(PduIdType pduId,
                                                              const PduInfoType *pPduInfo,
                                                              PduLengthType tpSduLength,
                                                              PduLengthType *pBufferSize)
{

    *pBufferSize = tpSduLength;
    return BUFREQ_OK;
}
static BufReq_ReturnType fake_impl_2_PduR_CanTpStartOfReception(PduIdType pduId,
                                                                const PduInfoType *pPduInfo,
                                                                PduLengthType tpSduLength,
                                                                PduLengthType *pBufferSize)
{

    return BUFREQ_E_NOT_OK;
}
void printRxConn(CanTp_RxConnection *conn)
{
    printf("\r\nact:%d, abs:%d, bs:%d, s:%d\r\n", conn->activation, conn->aquiredBuffSize,
           conn->buffSize, conn->state);
}

void CanTp_RxIndication_SF_NormalAddressing(void)
{

    BufReq_ReturnType (*fakeStartRFunc[])(PduIdType, const PduInfoType, PduLengthType,
                                          PduLengthType) = {fake_impl_PduR_CanTpStartOfReception};
    BufReq_ReturnType (*fakeCopyRData[])(PduIdType, const PduInfoType,
                                         PduLengthType) = {fake_impl_PduR_CanTpCopyRxData};
    SET_CUSTOM_FAKE_SEQ(PduR_CanTpStartOfReception, fakeStartRFunc, 1);
    SET_CUSTOM_FAKE_SEQ(PduR_CanTpCopyRxData, fakeCopyRData, 1);

    uint8 pduPayload[TEST_1_PDU_PAYLOAD_LEN] = {
        CANTP_N_PCI_TYPE_SF << 4 | 5, 'T', 'E', 'S', 'T', 0,
    };
    PduInfoType pdu = {
        .SduDataPtr = pduPayload,
        .MetaDataPtr = NULL,
        .SduLength = TEST_1_PDU_PAYLOAD_LEN,
    };
    CanTp_RxNSduType test_nsdu = {
        .id = TEST_1_PDU_ID, .paddingActivation = CANTP_OFF, .addressingFormat = CANTP_STANDARD};
    config.channels[0].rxNSdu[1] = test_nsdu;

    CanTp_RxIndication(102, &pdu);

    TEST_CHECK(PduR_CanTpStartOfReception_fake.call_count == 1);
    TEST_CHECK(PduR_CanTpStartOfReception_fake.arg0_val == TEST_1_PDU_ID);
    TEST_CHECK((PduR_CanTpStartOfReception_fake.arg2_val) == TEST_1_PDU_PAYLOAD_LEN - 1);
    TEST_CHECK(*(PduLengthType *)(PduR_CanTpStartOfReception_fake.arg3_val) ==
               TEST_1_PDU_PAYLOAD_LEN - 1);

    TEST_CHECK(PduR_CanTpCopyRxData_fake.call_count == 1);
    TEST_CHECK(strcmp(testBuffer, "TEST") == 0);
    TEST_CHECK(PduR_CanTpCopyRxData_fake.arg0_val == TEST_1_PDU_ID);

    TEST_CHECK(PduR_CanTpRxIndication_fake.call_count == 1);
    TEST_CHECK(PduR_CanTpRxIndication_fake.arg0_val == TEST_1_PDU_ID);
    TEST_CHECK(PduR_CanTpRxIndication_fake.arg1_val == E_OK);
}

void CanTp_RxIndication_SF_ExtendedAddressing(void)
{

    BufReq_ReturnType (*fakeStartRFunc[])(PduIdType, const PduInfoType, PduLengthType,
                                          PduLengthType) = {fake_impl_PduR_CanTpStartOfReception};
    BufReq_ReturnType (*fakeCopyRData[])(PduIdType, const PduInfoType,
                                         PduLengthType) = {fake_impl_PduR_CanTpCopyRxData};
    SET_CUSTOM_FAKE_SEQ(PduR_CanTpStartOfReception, fakeStartRFunc, 1);
    SET_CUSTOM_FAKE_SEQ(PduR_CanTpCopyRxData, fakeCopyRData, 1);

    uint8 pduPayload[TEST_2_PDU_PAYLOAD_LEN] = {
        NULL, CANTP_N_PCI_TYPE_SF << 4 | 5, 'M', 'T', 'M', '5', 0,
    };
    PduInfoType pdu = {
        .SduDataPtr = pduPayload,
        .MetaDataPtr = NULL,
        .SduLength = TEST_2_PDU_PAYLOAD_LEN,
    };
    CanTp_RxNSduType test_nsdu = {
        .id = TEST_2_PDU_ID, .paddingActivation = CANTP_OFF, .addressingFormat = CANTP_EXTENDED};
    config.channels[0].rxNSdu[1] = test_nsdu;

    CanTp_RxIndication(TEST_2_PDU_ID, &pdu);

    TEST_CHECK(PduR_CanTpStartOfReception_fake.call_count == 1);
    TEST_CHECK(PduR_CanTpStartOfReception_fake.arg0_val == TEST_2_PDU_ID);
    TEST_CHECK((PduR_CanTpStartOfReception_fake.arg2_val) == TEST_2_PDU_PAYLOAD_LEN - 2);
    TEST_CHECK(*(PduLengthType *)(PduR_CanTpStartOfReception_fake.arg3_val) ==
               TEST_2_PDU_PAYLOAD_LEN - 2);

    TEST_CHECK(PduR_CanTpCopyRxData_fake.call_count == 1);
    TEST_CHECK(strcmp(testBuffer, "MTM5") == 0);
    TEST_CHECK(PduR_CanTpCopyRxData_fake.arg0_val == TEST_2_PDU_ID);

    TEST_CHECK(PduR_CanTpRxIndication_fake.call_count == 1);
    TEST_CHECK(PduR_CanTpRxIndication_fake.arg0_val == TEST_2_PDU_ID);
    TEST_CHECK(PduR_CanTpRxIndication_fake.arg1_val == E_OK);
}

void CanTp_RxIndication_SF_PaddingOnWrongLen(void)
{
    PduR_CanTpStartOfReception_reset();
    PduR_CanTpCopyRxData_reset();

    BufReq_ReturnType (*fakeStartRFunc[])(PduIdType, const PduInfoType, PduLengthType,
                                          PduLengthType) = {fake_impl_PduR_CanTpStartOfReception};
    BufReq_ReturnType (*fakeCopyRData[])(PduIdType, const PduInfoType,
                                         PduLengthType) = {fake_impl_PduR_CanTpCopyRxData};
    SET_CUSTOM_FAKE_SEQ(PduR_CanTpStartOfReception, fakeStartRFunc, 1);
    SET_CUSTOM_FAKE_SEQ(PduR_CanTpCopyRxData, fakeCopyRData, 1);

    uint8 pduPayload[TEST_3_PDU_PAYLOAD_LEN] = {
        CANTP_N_PCI_TYPE_SF << 4 | 5, 'T', 'E', 'S', 'T', 0,
    };
    PduInfoType pdu = {
        .SduDataPtr = pduPayload,
        .MetaDataPtr = NULL,
        .SduLength = TEST_3_PDU_PAYLOAD_LEN,
    };
    CanTp_RxNSduType test_nsdu = {
        .id = TEST_3_PDU_ID, .paddingActivation = CANTP_ON, .addressingFormat = CANTP_STANDARD};
    config.channels[0].rxNSdu[1] = test_nsdu;

    CanTp_RxIndication(TEST_3_PDU_ID, &pdu);

    TEST_CHECK(PduR_CanTpStartOfReception_fake.call_count == 0);
    TEST_CHECK(PduR_CanTpCopyRxData_fake.call_count == 0);

    TEST_CHECK(PduR_CanTpRxIndication_fake.call_count == 1);
    TEST_CHECK(PduR_CanTpRxIndication_fake.arg0_val == TEST_3_PDU_ID);
    TEST_CHECK(PduR_CanTpRxIndication_fake.arg1_val == E_NOT_OK);
}

void CanTp_RxIndication_SF_BufferNotAlocated(void)
{
    PduR_CanTpStartOfReception_reset();
    PduR_CanTpCopyRxData_reset();

    BufReq_ReturnType (*fakeStartRFunc[])(PduIdType, const PduInfoType, PduLengthType,
                                          PduLengthType) = {fake_impl_2_PduR_CanTpStartOfReception};
    BufReq_ReturnType (*fakeCopyRData[])(PduIdType, const PduInfoType,
                                         PduLengthType) = {fake_impl_PduR_CanTpCopyRxData};
    SET_CUSTOM_FAKE_SEQ(PduR_CanTpStartOfReception, fakeStartRFunc, 1);
    SET_CUSTOM_FAKE_SEQ(PduR_CanTpCopyRxData, fakeCopyRData, 1);

    uint8 pduPayload[TEST_1_PDU_PAYLOAD_LEN] = {
        CANTP_N_PCI_TYPE_SF << 4 | 5, 'T', 'E', 'S', 'T', 0,
    };
    PduInfoType pdu = {
        .SduDataPtr = pduPayload,
        .MetaDataPtr = NULL,
        .SduLength = TEST_1_PDU_PAYLOAD_LEN,
    };
    CanTp_RxNSduType test_nsdu = {
        .id = TEST_1_PDU_ID, .paddingActivation = CANTP_OFF, .addressingFormat = CANTP_STANDARD};
    config.channels[0].rxNSdu[1] = test_nsdu;
    CanTp_State.rxConnections[1].aquiredBuffSize = 0;

    CanTp_RxIndication(TEST_1_PDU_ID, &pdu);

    TEST_CHECK(PduR_CanTpStartOfReception_fake.call_count == 1);
    TEST_CHECK(PduR_CanTpStartOfReception_fake.arg0_val == TEST_1_PDU_ID);
    TEST_CHECK((PduR_CanTpStartOfReception_fake.arg2_val) == TEST_1_PDU_PAYLOAD_LEN - 1);
    TEST_CHECK(*(PduLengthType *)(PduR_CanTpStartOfReception_fake.arg3_val) == 0);

    TEST_CHECK(PduR_CanTpCopyRxData_fake.call_count == 0);

    TEST_CHECK(PduR_CanTpRxIndication_fake.call_count == 1);
    TEST_CHECK(PduR_CanTpRxIndication_fake.arg0_val == TEST_1_PDU_ID);
    TEST_CHECK(PduR_CanTpRxIndication_fake.arg1_val == E_NOT_OK);
}

TEST_LINKED_LIST_ENTRY CanTp_RxIndication_TEST_LIST[] = {
    {"CanTp_RxIndication_SF_NormalAddressing", CanTp_RxIndication_SF_NormalAddressing},
    {"CanTp_RxIndication_SF_ExtendedAddressing", CanTp_RxIndication_SF_ExtendedAddressing},
    {"CanTp_RxIndication_SF_PaddingOnWrongLen", CanTp_RxIndication_SF_PaddingOnWrongLen},
    {"CanTp_RxIndication_SF_BufferNotAlocated", CanTp_RxIndication_SF_BufferNotAlocated},
    {NULL, NULL}};
// ,{ "next_test", next_test } // and so on
