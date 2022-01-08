
#include <CanTp.h>

#include <CanIf.h>

#define CAN_IS_ON() (CanTp_State.general == CANTP_ON)


static struct CanTp_State_s {
    enum General {
        CANTP_OFF,
        CANTP_ON
    } general;

    enum TX {
        CANTP_TX_WAIT,
        CANTP_TX_PROCESSING
    } tx;

    enum RX {
        CANTP_RX_WAIT,
        CANTP_RX_PROCESSING
    } rx;
} CanTp_State;


void CanTp_Init(const CanTp_ConfigType* CfgPtr)
{
    CanIf_ConfigType canIf_Config;

    if (CAN_IS_ON())
    {
        return;
    }

    CanTp_State.general = CANTP_ON;
    CanTp_State.tx = CANTP_TX_WAIT;
    CanTp_State.rx = CANTP_RX_WAIT;


    CanIf_Init()
}

void CanTp_Shutdown(void)
{

    CanTp_State.general = CANTP_OFF;
}
