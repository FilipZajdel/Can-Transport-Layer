#define TEST_NO_MAIN

#include <fff.h>
#include <acutest.h>
#include <Test_Stub.h>
#include <CanTp.c>

void CanTp_TxConf_stub(void) { TEST_CHECK(0); }

TEST_LINKED_LIST_ENTRY CanTp_TxConfirmation_TEST_LIST[] = {{"CanTp_TxConf_stub", CanTp_TxConf_stub},
                                                           {NULL, NULL}};
