#define TEST_NO_MAIN

#include <fff.h>
#include <acutest.h>
#include <Test_Stub.h>
#include <CanTp.c>


void CanTp_Change_stub(void)
{
    TEST_CHECK(0);
}

TEST_LINKED_LIST_ENTRY CanTp_ChangeParameter_TEST_LIST[] = {
    { "CanTp_ChangeParam_stub", CanTp_Change_stub },
    { NULL, NULL }
};
