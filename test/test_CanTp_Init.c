#define TEST_NO_MAIN

#include <fff.h>
#include <acutest.h>
#include <Test_Stub.h>
#include <CanTp.c>

void CanTp_Init_CheckVariablesInitialization(void)
{
    CanTp_Init(NULL);

    TEST_CHECK(CanTp_State.activation == CANTP_ON);
}

TEST_LINKED_LIST_ENTRY CanTp_Init_TEST_LIST[] = {
    {"CanTp_Init() Check variables initialization", CanTp_Init_CheckVariablesInitialization},
    {NULL, NULL}};
