#include <fff.h>

void CanTp_Init_CheckVariablesInitialization(void)
{
    CanTp_Init(NULL);

    TEST_CHECK(CanTp_State.activation == CANTP_ON);

    TEST_CHECK(CanTp_State.txState == CANTP_TX_WAIT);

    TEST_CHECK(CanTp_State.rxState == CANTP_RX_WAIT);
}

#define CanTp_Init_TEST_LIST \
    { "CanTp_Init() Check variables initialization", CanTp_Init_CheckVariablesInitialization }
    // ,{ "next_test", next_test } // and so on
