#include <fff.h>

void CanTp_Change_stub(void)
{
    TEST_CHECK(0);
}

#define CanTp_ChangeParameter_TEST_LIST \
    { "CanTp_ChangeParam_stub", CanTp_Change_stub }
    // ,{ "next_test", next_test } // and so on
