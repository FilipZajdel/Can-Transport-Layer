#define TEST_NO_MAIN

#include <fff.h>
#include <acutest.h>
#include <Test_Stub.h>
#include <CanTp.c>

void CanTp_Shutdown_stub(void)
{
    TEST_CHECK(0);
}

TEST_LINKED_LIST_ENTRY CanTp_Shutdown_TEST_LIST[] = {
    { "CanTp_Shutdown_stub", CanTp_Shutdown_stub },
    {NULL, NULL}};

