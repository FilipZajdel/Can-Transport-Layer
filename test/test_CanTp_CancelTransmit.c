#define TEST_NO_MAIN

#include <fff.h>
#include <acutest.h>
#include <Test_Stub.h>
#include <CanTp.c>

#define PDU_INVALID (PduIdType)0xFFFFFFFF

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

void CancelTransmitSimpleTest(void)
{
    uint8 data[] = {1,2,3,4,5,6,7,8,9,10};
    PduInfoType pduInfo = {
        .SduDataPtr = data,
        .SduLength = ARR_SIZE(data)
    };

    PduIdType pduId = findNextValidTxPduId();

    CanTp_State.activation = CANTP_ON;

    CanTp_Transmit(pduId, &pduInfo);

    CanTp_MainFunction();

    TEST_CHECK(E_OK == CanTp_CancelTransmit(pduId));

    CanTp_MainFunction();

    TEST_CHECK(getTxConnection(pduId)->activation == CANTP_TX_WAIT);
}

TEST_LINKED_LIST_ENTRY CanTp_CancelTransmit_TEST_LIST[] = {
    { "CanTp_CancelTransmit Simple Test", CancelTransmitSimpleTest },
    {  NULL, NULL  }
};
