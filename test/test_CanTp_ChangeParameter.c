#define TEST_NO_MAIN

#include <fff.h>
#include <acutest.h>
#include <Test_Stub.h>
#include <CanTp.c>

#define TEST_1_PDU_PAYLOAD_LEN 6
#define TEST_1_PDU_ID 102

void CanTp_ChangeParameter_valid(void)
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
    CanTp_State.rxConnections[1].activation = CANTP_RX_WAIT;

    uint16 value = 123;

    TEST_CHECK(CanTp_ChangeParameter(TEST_1_PDU_ID, TP_BS, value) == E_OK);
    TEST_CHECK(CanTp_State.rxConnections[1].nsdu->bs == value);
}

void CanTp_ChangeParameter_invalid_state(void)
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
    CanTp_State.rxConnections[1].activation = CANTP_RX_PROCESSING;

    uint16 value = 123;

    TEST_CHECK(CanTp_ChangeParameter(TEST_1_PDU_ID, TP_BS, value) == E_NOT_OK);
    TEST_CHECK(CanTp_State.rxConnections[1].nsdu->bs == 100);
}

void CanTp_ChangeParameter_invalid_value(void)
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
                                  .STmin = 100};
    config.channels[0].rxNSdu[1] = test_nsdu;
    CanTp_State.rxConnections[1].activation = CANTP_RX_WAIT;

    uint16 value = 567;

    TEST_CHECK(CanTp_ChangeParameter(TEST_1_PDU_ID, TP_STMIN, value) == E_NOT_OK);
    TEST_CHECK(CanTp_State.rxConnections[1].nsdu->STmin == 100);
}

TEST_LINKED_LIST_ENTRY CanTp_ChangeParameter_TEST_LIST[] = {
    {"CanTp_ChangeParameter_valid", CanTp_ChangeParameter_valid},
    {"CanTp_ChangeParameter_invalid_state", CanTp_ChangeParameter_invalid_state},
    {"CanTp_ChangeParameter_invalid_value", CanTp_ChangeParameter_invalid_value},
    {NULL, NULL}};
