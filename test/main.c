#include <fff.h>

DEFINE_FFF_GLOBALS;

#define ACUTEST_USE_LINKED_TEST_LIST
#include <acutest.h>

#include <Std_Types.h>
FAKE_VALUE_FUNC4(Std_ReturnType, Det_ReportRuntimeError, uint16, uint8, uint8, uint8);

extern TEST_LINKED_LIST_ENTRY CanTp_CancelReceive_TEST_LIST[];
extern TEST_LINKED_LIST_ENTRY CanTp_CancelTransmit_TEST_LIST[];
extern TEST_LINKED_LIST_ENTRY CanTp_ReadParameter_TEST_LIST[];
extern TEST_LINKED_LIST_ENTRY CanTp_ChangeParameter_TEST_LIST[];
extern TEST_LINKED_LIST_ENTRY CanTp_GetVersionInfo_TEST_LIST[];
extern TEST_LINKED_LIST_ENTRY CanTp_Init_TEST_LIST[];
extern TEST_LINKED_LIST_ENTRY CanTp_MainFunction_TEST_LIST[];
extern TEST_LINKED_LIST_ENTRY CanTp_RxIndication_TEST_LIST[];
extern TEST_LINKED_LIST_ENTRY CanTp_Shutdown_TEST_LIST[];
extern TEST_LINKED_LIST_ENTRY CanTp_Transmit_TEST_LIST[];
extern TEST_LINKED_LIST_ENTRY CanTp_TxConfirmation_TEST_LIST[];

TEST_LINKED_LIST = {
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
    TEST_LINKED_LIST_END
};
