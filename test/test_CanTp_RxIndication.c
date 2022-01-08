#include <fff.h>

void CanTp_RxInd_stub(void)
{
    TEST_CHECK(0);
}

#define CanTp_RxIndication_TEST_LIST \
    { "CanTp_RxInd_stub", CanTp_RxInd_stub }
    // ,{ "next_test", next_test } // and so on

