#include <fff.h>

void CanTp_Transmit_stub(void)
{
    TEST_CHECK(0);
}

#define CanTp_Transmit_TEST_LIST \
    { "CanTp_Transmit_stub", CanTp_Transmit_stub }
    // ,{ "next_test", next_test } // and so on

