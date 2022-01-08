#include <fff.h>

void CanTp_Shutdown_stub(void)
{
    TEST_CHECK(0);
}

#define CanTp_Shutdown_TEST_LIST \
    { "CanTp_Shutdown_stub", CanTp_Shutdown_stub }
    // ,{ "next_test", next_test } // and so on

