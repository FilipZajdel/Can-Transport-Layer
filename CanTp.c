
#include <CanTp.h>
#include <CanTp_Types.h>

#define CAN_IS_ON() (CanTp_State.activation == CANTP_ON)
#define PARAM_UNUSED(param) (void)param
#define PDUID_VALID(pduid) (pduid < CONFIG_CAN_TP_MAX_CHANNELS_COUNT)

static CanTp_State_t CanTp_State;
static CanTp_ConfigType config = {
    .channels = {
        {.rxNSdu = {{.id = 1}, {.id = 2}, {.id = 3}, {.id = 4}, {.id = 5}},
         .rxNSduCount = 5,
         .txNSdu = {{.id = 1}},
         .txNSduCount = 1},
        {.rxNSdu = {{.id = 6}, {.id = 7}, {.id = 8}},
         .rxNSduCount = 3,
         .txNSdu = {{.id = 6}, {.id = 7}, {.id = 8}, {.id = 9}, {.id = 10}},
         .txNSduCount = 5},
        {.rxNSduCount = 0, .txNSdu = {{.id = 11}, {.id = 12}}, .txNSduCount = 2},
        {.rxNSdu = {{.id = 13}, {.id = 14}, {.id = 15}}, .rxNSduCount = 3, .txNSduCount = 0},
        {.rxNSdu = {{.id = 16}, {.id = 17}, {.id = 18}, {.id = 19}},
         .rxNSduCount = 4,
         .txNSdu = {{.id = 16}, {.id = 20}, {.id = 21}, {.id = 22}},
         .txNSduCount = 4},
        {.rxNSduCount = 0,
         .txNSdu = {{.id = 23}, {.id = 24}, {.id = 25}, {.id = 26}, {.id = 27}},
         .txNSduCount = 5},
        {.rxNSdu = {{.id = 28}},
         .rxNSduCount = 1,
         .txNSdu = {{.id = 29}, {.id = 30}},
         .txNSduCount = 2},
        {.rxNSdu = {{.id = 31}, {.id = 32}, {.id = 33}, {.id = 34}},
         .rxNSduCount = 4,
         .txNSdu = {{.id = 34}, {.id = 35}, {.id = 36}},
         .txNSduCount = 3},
    }};

static inline void memzero(uint8_t *ptr, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++) {
        *(ptr + i) = 0;
    }
}

static inline const CanTp_ChannelType *getNSduFromPduId(PduIdType PduId)
{
    CanTp_ChannelType *channels = config.channels;

    if (!PDUID_VALID(PduId)) {
        return NULL;
    }

    for (uint32 chan_itr = 0; chan_itr < CONFIG_CAN_TP_MAX_CHANNELS_COUNT; chan_itr++) {
        // if (channels[chan_itr].rx)
    }
}

void CanTp_Init(const CanTp_ConfigType *CfgPtr)
{
    PARAM_UNUSED(CfgPtr);

    CanTp_State.txState = CANTP_TX_WAIT;
    CanTp_State.rxState = CANTP_RX_WAIT;
    CanTp_State.activation = CANTP_ON;
}

void CanTp_Shutdown(void)
{
    CanTp_State.activation = CANTP_OFF;
    CanTp_State.txState = CANTP_TX_WAIT;
    CanTp_State.rxState = CANTP_RX_WAIT;
}
