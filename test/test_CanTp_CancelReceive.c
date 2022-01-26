#define TEST_NO_MAIN

#include <fff.h>
#include <acutest.h>
#include <Test_Stub.h>
#include <CanTp.c>

#define TEST_1_PDU_PAYLOAD_LEN 6
#define TEST_1_PDU_ID 102

FAKE_STATIC_VOID_FUNC2(PduR_CanTpRxIndication, PduIdType, Std_ReturnType);

void CanTp_CancelReceive_valid_connection(void)
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
    CanTp_State.rxConnections[1].activation = CANTP_RX_PROCESSING;
    CanTp_State.rxConnections[1].state = CANTP_RX_STATE_FC_TX_REQ;

    TEST_CHECK(CanTp_CancelReceive(TEST_1_PDU_ID) == E_OK);

    TEST_CHECK(PduR_CanTpRxIndication_fake.call_count == 1);
    TEST_CHECK(PduR_CanTpRxIndication_fake.arg0_val == TEST_1_PDU_ID);
    TEST_CHECK(PduR_CanTpRxIndication_fake.arg1_val == E_NOT_OK);

    TEST_CHECK(CanTp_State.rxConnections[1].activation == CANTP_RX_WAIT);
    TEST_CHECK(CanTp_State.rxConnections[1].state == CANTP_RX_STATE_ABORT);
}

void CanTp_CancelReceive_invalid_connection_state(void)
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
    CanTp_State.rxConnections[1].state = CANTP_RX_STATE_FC_TX_REQ;

    TEST_CHECK(CanTp_CancelReceive(TEST_1_PDU_ID) == E_NOT_OK);

    TEST_CHECK(PduR_CanTpRxIndication_fake.call_count == 0);

    TEST_CHECK(CanTp_State.rxConnections[1].activation == CANTP_RX_WAIT);
    TEST_CHECK(CanTp_State.rxConnections[1].state == CANTP_RX_STATE_FC_TX_REQ);
    getRxConnection(300);
}

void CanTp_CancelReceive_invalid_connection(void)
{
    TEST_CHECK(CanTp_CancelReceive(300) == E_NOT_OK); // invalid pduid

    TEST_CHECK(PduR_CanTpRxIndication_fake.call_count == 0);
}

TEST_LINKED_LIST_ENTRY CanTp_CancelReceive_TEST_LIST[] = {
    {"CanTp_CancelReceive_valid_connection", CanTp_CancelReceive_valid_connection},
    {"CanTp_CancelReceive_invalid_connection_state", CanTp_CancelReceive_invalid_connection_state},
    {"CanTp_CancelReceive_invalid_connection", CanTp_CancelReceive_invalid_connection},
    {NULL, NULL}};
