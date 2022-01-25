#define TEST_NO_MAIN

#include <fff.h>
#include <acutest.h>
#include <Test_Stub.h>
#include <CanTp.c>

void CanTp_CancelReceive_stub(void)
{
    TEST_CHECK(0);
}

TEST_LINKED_LIST_ENTRY CanTp_CancelReceive_TEST_LIST[] = {
    { "CanTp_CancelReceive_stub", CanTp_CancelReceive_stub },
    { NULL, NULL}
};
