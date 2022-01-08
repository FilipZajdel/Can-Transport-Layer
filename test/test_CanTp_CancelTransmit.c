#include <fff.h>

void CanTp_CancelTransmit_stub(void)
{
    TEST_CHECK(0);
}

#define CanTp_CancelTransmit_TEST_LIST \
    { "CanTp_CancelTransmit_stub", CanTp_CancelTransmit_stub }
     // ,{ "next_test", next_test } // and so on
