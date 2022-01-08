#include <fff.h>

void CanTp_Read_stub(void)
{
    TEST_CHECK(0);
}

#define CanTp_ReadParameter_TEST_LIST \
    { "CanTp_Read_stub", CanTp_Read_stub }
    // ,{ "next_test", next_test } // and so on
