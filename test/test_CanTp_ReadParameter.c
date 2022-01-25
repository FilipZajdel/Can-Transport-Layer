#define TEST_NO_MAIN

#include <fff.h>
#include <acutest.h>
#include <Test_Stub.h>
#include <CanTp.c>

void CanTp_Read_stub(void)
{
    TEST_CHECK(0);
}

TEST_LINKED_LIST_ENTRY CanTp_ReadParameter_TEST_LIST[] = {
    { "CanTp_Read_stub", CanTp_Read_stub },
    {NULL, NULL}
};
