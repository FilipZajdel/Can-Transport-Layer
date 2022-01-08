#include <fff.h>

void CanTp_CancelReceive_stub(void)
{
    TEST_CHECK(0);
}

#define CanTp_CancelReceive_TEST_LIST \
    { "CanTp_CancelReceive_stub", CanTp_CancelReceive_stub }
    // ,{ "next_test", next_test } // and so on
