#include <fff.h>

void CanTp_Init_stub(void)
{
    TEST_CHECK(0);
}

#define CanTp_Init_TEST_LIST \
    { "CanTp_Init_stub", CanTp_Init_stub }
    // ,{ "next_test", next_test } // and so on
