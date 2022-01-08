#include <fff.h>

void CanTp_GetVersionInfo_stub(void)
{
    TEST_CHECK(0);
}

#define CanTp_GetVersionInfo_TEST_LIST \
    { "CanTp_GetVersionInfo_stub", CanTp_GetVersionInfo_stub }
     // ,{ "next_test", next_test } // and so on
