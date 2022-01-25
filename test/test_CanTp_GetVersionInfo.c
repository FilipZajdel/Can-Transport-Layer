#define TEST_NO_MAIN

#include <fff.h>
#include <acutest.h>
#include <Test_Stub.h>
#include <CanTp.c>


void CanTp_GetVersionInfo_stub(void)
{
    TEST_CHECK(0);
}

TEST_LINKED_LIST_ENTRY CanTp_GetVersionInfo_TEST_LIST[] = {
    { "CanTp_GetVersionInfo_stub", CanTp_GetVersionInfo_stub },
    { NULL, NULL }
};
