#define TEST_NO_MAIN

#include <fff.h>
#include <acutest.h>
#include <Test_Stub.h>
#include <CanTp.c>

#define TEST_1_PDU_PAYLOAD_LEN 6
#define TEST_1_PDU_ID 102

void CanTp_ReadParameter_valid(void)
{
    uint8 pduPayload[TEST_1_PDU_PAYLOAD_LEN] = {
        CANTP_N_PCI_TYPE_SF << 4 | 5, 'T', 'E', 'S', 'T', 0,
    };
    PduInfoType pdu = {
        .SduDataPtr = pduPayload,
        .MetaDataPtr = NULL,
        .SduLength = TEST_1_PDU_PAYLOAD_LEN,
    };
    CanTp_RxNSduType test_nsdu = {.id = TEST_1_PDU_ID,
                                  .paddingActivation = CANTP_OFF,
                                  .addressingFormat = CANTP_STANDARD,
                                  .bs = 100};
    config.channels[0].rxNSdu[1] = test_nsdu;
    CanTp_State.rxConnections[1].aquiredBuffSize = 0;

    uint16 readVal = 0;

    TEST_CHECK(CanTp_ReadParameter(TEST_1_PDU_ID, TP_BS, &readVal) == E_OK);
    TEST_CHECK(readVal == 100);
}

void CanTp_ReadParameter_invalid_param(void)
{
    uint8 pduPayload[TEST_1_PDU_PAYLOAD_LEN] = {
        CANTP_N_PCI_TYPE_SF << 4 | 5, 'T', 'E', 'S', 'T', 0,
    };
    PduInfoType pdu = {
        .SduDataPtr = pduPayload,
        .MetaDataPtr = NULL,
        .SduLength = TEST_1_PDU_PAYLOAD_LEN,
    };
    CanTp_RxNSduType test_nsdu = {.id = TEST_1_PDU_ID,
                                  .paddingActivation = CANTP_OFF,
                                  .addressingFormat = CANTP_STANDARD,
                                  .bs = 100};
    config.channels[0].rxNSdu[1] = test_nsdu;
    CanTp_State.rxConnections[1].aquiredBuffSize = 0;

    uint16 readVal = 0;

    TEST_CHECK(CanTp_ReadParameter(TEST_1_PDU_ID, TP_BC, &readVal) == E_NOT_OK);
    TEST_CHECK(readVal == 0);
}

void CanTp_ReadParameter_invalid_value(void)
{
    uint8 pduPayload[TEST_1_PDU_PAYLOAD_LEN] = {
        CANTP_N_PCI_TYPE_SF << 4 | 5, 'T', 'E', 'S', 'T', 0,
    };
    PduInfoType pdu = {
        .SduDataPtr = pduPayload,
        .MetaDataPtr = NULL,
        .SduLength = TEST_1_PDU_PAYLOAD_LEN,
    };
    CanTp_RxNSduType test_nsdu = {.id = TEST_1_PDU_ID,
                                  .paddingActivation = CANTP_OFF,
                                  .addressingFormat = CANTP_STANDARD,
                                  .STmin = 300};
    config.channels[0].rxNSdu[1] = test_nsdu;
    CanTp_State.rxConnections[1].aquiredBuffSize = 0;

    uint16 readVal = 0;

    TEST_CHECK(CanTp_ReadParameter(TEST_1_PDU_ID, TP_STMIN, &readVal) == E_NOT_OK);
    TEST_CHECK(readVal == 0);
}

TEST_LINKED_LIST_ENTRY CanTp_ReadParameter_TEST_LIST[] = {
    {"CanTp_ReadParameter_valid", CanTp_ReadParameter_valid},
    {"CanTp_ReadParameter_invalid_param", CanTp_ReadParameter_invalid_param},
    {"CanTp_ReadParameter_invalid_value", CanTp_ReadParameter_invalid_value},
    {NULL, NULL}};
