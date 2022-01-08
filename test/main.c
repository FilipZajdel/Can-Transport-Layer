#include <CanTp.c>

#include <acutest.h>
#include <fff.h>

#include <test_CanTp_CancelReceive.c>
#include <test_CanTp_CancelTransmit.c>
#include <test_CanTp_ReadParameter.c>
#include <test_CanTp_ChangeParameter.c>
#include <test_CanTp_GetVersionInfo.c>
#include <test_CanTp_Init.c>
#include <test_CanTp_MainFunction.c>
#include <test_CanTp_RxIndication.c>
#include <test_CanTp_Shutdown.c>
#include <test_CanTp_Transmit.c>
#include <test_CanTp_TxConfirmation.c>


TEST_LIST = {
    CanTp_CancelReceive_TEST_LIST,
    CanTp_CancelTransmit_TEST_LIST,
    CanTp_ReadParameter_TEST_LIST,
    CanTp_ChangeParameter_TEST_LIST,
    CanTp_GetVersionInfo_TEST_LIST,
    CanTp_Init_TEST_LIST,
    CanTp_MainFunction_TEST_LIST,
    CanTp_RxIndication_TEST_LIST,
    CanTp_Shutdown_TEST_LIST,
    CanTp_Transmit_TEST_LIST,
    CanTp_TxConfirmation_TEST_LIST,
    { NULL, NULL }
};
