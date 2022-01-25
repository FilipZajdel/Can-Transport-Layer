#define TEST_NO_MAIN

#include <fff.h>
#include <acutest.h>
#include <Test_Stub.h>
#include <CanTp.c>

void CanTp_Main_stub(void)
{
    TEST_CHECK(0);
}

TEST_LINKED_LIST_ENTRY CanTp_MainFunction_TEST_LIST[] = {
    { "CanTp_Main_stub", CanTp_Main_stub },
    { NULL, NULL }
};


