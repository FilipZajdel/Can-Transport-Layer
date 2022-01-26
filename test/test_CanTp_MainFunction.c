#define TEST_NO_MAIN

#include <fff.h>
#include <acutest.h>
#include <Test_Stub.h>
#include <CanTp.c>

void CanTp_TestMainFunction(void)
{
    const uint8 mainIterations = 18;
    uint8 mainCounterBefore = CanTp_State.currentTime;

    CanTp_State.activation = CANTP_ON;

    CanTp_MainFunction();

    TEST_CHECK(mainCounterBefore + CONFIG_CANTP_MAIN_FUNCTION_PERIOD == CanTp_State.currentTime);

    mainCounterBefore += CanTp_State.currentTime;
    for (int i = 0; i < mainIterations; i++) {
        CanTp_MainFunction();
    }

    TEST_CHECK((mainCounterBefore + mainIterations * CONFIG_CANTP_MAIN_FUNCTION_PERIOD) ==
               CanTp_State.currentTime);

    mainCounterBefore = mainCounterBefore + mainIterations * CONFIG_CANTP_MAIN_FUNCTION_PERIOD;
    CanTp_State.activation = CANTP_OFF;
    for (int i = 0; i < mainIterations; i++) {
        CanTp_MainFunction();
    }

    TEST_CHECK(mainCounterBefore == CanTp_State.currentTime);
}

TEST_LINKED_LIST_ENTRY CanTp_MainFunction_TEST_LIST[] = {
    {"CanTp_MainFunction Timer Incrementation Test", CanTp_TestMainFunction}, {NULL, NULL}};
