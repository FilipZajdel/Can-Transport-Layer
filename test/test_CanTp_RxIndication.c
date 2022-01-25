#define TEST_NO_MAIN

#include <fff.h>
#include <acutest.h>
#include <Test_Stub.h>
#include <CanTp.c>

void CanTp_RxInd_stub(void)
{
    TEST_CHECK(0);
}

TEST_LINKED_LIST_ENTRY CanTp_RxIndication_TEST_LIST[] = {
    { "CanTp_RxInd_stub", CanTp_RxInd_stub },
    {NULL, NULL}};
    // ,{ "next_test", next_test } // and so on

