
#include <CanTp.h>
#include <CanTp_Types.h>

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

typedef struct
{
    const CanTp_NSduChannelType *nsduChannel;
} CanTp_ChannelType;

static CanTp_ChannelType CanTp_Channels[CONFIG_CAN_TP_MAX_CHANNELS_COUNT];
static uint32_t CanTp_RxChannelsRef[CONFIG_CAN_TP_MAX_CHANNELS_COUNT];
static uint32_t CanTp_TxChannelsRef[CONFIG_CAN_TP_MAX_CHANNELS_COUNT];
static CanTp_ChannelCountType CanTp_ChannelsIterator;
static CanTp_ChannelCountType CanTp_RxChannelsIterator;
static CanTp_ChannelCountType CanTp_TxChannelsIterator;

static inline void memzero(uint8_t *ptr, uint32_t size)
{
    for (uint32_t i=0; i<size; i++) {
        *(ptr+i) = 0;
    }
}

static void CanTp_CleanGlobals(void)
{
    memzero((uint8_t*)CanTp_Channels, sizeof(CanTp_Channels));
    memzero((uint8_t*)CanTp_RxChannelsRef, sizeof(CanTp_RxChannelsRef));
    memzero((uint8_t*)CanTp_TxChannelsRef, sizeof(CanTp_TxChannelsRef));
    CanTp_ChannelsIterator = 0;
    CanTp_RxChannelsIterator = 0;
    CanTp_TxChannelsIterator = 0;
}

void CanTp_Init(const CanTp_ConfigType* CfgPtr)
{
    CanTp_CleanGlobals();

    if ((CfgPtr != MEMPTR_NULL) && (CfgPtr->channelCount <= CONFIG_CAN_TP_MAX_CHANNELS_COUNT)) {
        uint32_t i;

        for (i=0; i<CfgPtr->channelCount; i++) {
            if (CfgPtr->channels[i].direction == CANTP_CHANNEL_DIRECTION_TX) {
                CanTp_Channels[CanTp_ChannelsIterator].nsduChannel = &CfgPtr->channels[i];
                CanTp_TxChannelsRef[CanTp_TxChannelsIterator] = CanTp_ChannelsIterator;

                CanTp_ChannelsIterator += 1;
                CanTp_TxChannelsIterator += 1;
            } else if (CfgPtr->channels[i].direction == CANTP_CHANNEL_DIRECTION_RX) {
                CanTp_Channels[CanTp_ChannelsIterator].nsduChannel = &CfgPtr->channels[i];
                CanTp_RxChannelsRef[CanTp_RxChannelsIterator] = CanTp_ChannelsIterator;

                CanTp_ChannelsIterator += 1;
                CanTp_RxChannelsIterator += 1;
            } else {
                // @TODO: Report an error
            }
        }

        CanTp_State.general = CANTP_ON;
        CanTp_State.tx = CANTP_TX_WAIT;
        CanTp_State.rx = CANTP_RX_WAIT;
    } else {
        // @TODO: Report an error
        CanTp_State.general = CANTP_OFF;
        CanTp_State.tx = CANTP_TX_WAIT;
        CanTp_State.rx = CANTP_RX_WAIT;
    }
}

void CanTp_Shutdown(void)
{
    CanTp_State.general = CANTP_OFF;
}
