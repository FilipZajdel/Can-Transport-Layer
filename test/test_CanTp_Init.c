#include <fff.h>

void CanTp_Init_CheckVariablesInitialization(void)
{
    CanTp_ConfigType config = {
        0, MEMPTR_NULL
    };

    CanTp_State.general = CANTP_OFF;
    CanTp_State.tx = CANTP_TX_PROCESSING;
    CanTp_State.rx = CANTP_RX_PROCESSING;

    CanTp_Init(&config);

    TEST_CHECK(CanTp_State.general == CANTP_ON);

    TEST_CHECK(CanTp_State.tx == CANTP_TX_WAIT);

    TEST_CHECK(CanTp_State.rx == CANTP_RX_WAIT);
}

#define CanTp_Init_TEST_LIST \
    { "CanTp_Init() Check variables initialization", CanTp_Init_CheckVariablesInitialization }
    // ,{ "next_test", next_test } // and so on
