#include <fff.h>

void CanTp_TxConf_stub(void)
{
    TEST_CHECK(0);
}

#define CanTp_TxConfirmation_TEST_LIST \
    { "CanTp_TxConf_stub", CanTp_TxConf_stub }
    // ,{ "next_test", next_test } // and so on
