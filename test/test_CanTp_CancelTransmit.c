#define TEST_NO_MAIN

#include <fff.h>
#include <acutest.h>
#include <Test_Stub.h>
#include <CanTp.c>

void CanTp_CancelTransmit_stub(void)
{
    TEST_CHECK(0);
}

TEST_LINKED_LIST_ENTRY CanTp_CancelTransmit_TEST_LIST[] = {
    { "CanTp_CancelTransmit_stub", CanTp_CancelTransmit_stub },
    {  NULL, NULL  }
};
