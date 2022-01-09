
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
static CanTp_ChannelCountType CanTp_ChannelsIterator;

static inline void memzero(uint8_t *ptr, uint32_t size)
{
    for (uint32_t i=0; i<size; i++) {
        *(ptr+i) = 0;
    }
}

static inline const CanTp_NSduChannelType *findChannelnSdu(CanTp_NSduId nSduId)
{
    CanTp_ChannelCountType i;
    const CanTp_NSduChannelType *nSduChannel = MEMPTR_NULL;

    for (i=0; i<CanTp_ChannelsIterator; i++) {
        if (CanTp_Channels[i].nsduChannel->nSduId == nSduId) {
            nSduChannel = CanTp_Channels[i].nsduChannel;
        }
    }

    return nSduChannel;
}

static void CanTp_CleanGlobals(void)
{
    memzero((uint8_t*)CanTp_Channels, sizeof(CanTp_Channels));
    CanTp_ChannelsIterator = 0;
}

void CanTp_Init(const CanTp_ConfigType* CfgPtr)
{
    boolean initOk = FALSE;

    CanTp_CleanGlobals();

    if ((CfgPtr != MEMPTR_NULL) && (CfgPtr->channelCount <= CONFIG_CAN_TP_MAX_CHANNELS_COUNT)) {
        uint32_t i;

        for (i=0, initOk=TRUE; i < (CfgPtr->channelCount) && initOk; i++) {

            if (findChannelnSdu(CfgPtr->channels[i].nSduId) != MEMPTR_NULL) {
                initOk = FALSE;
            } else if (CfgPtr->channels[i].direction == CANTP_CHANNEL_DIRECTION_TX
                      || CfgPtr->channels[i].direction == CANTP_CHANNEL_DIRECTION_RX) {

                CanTp_Channels[CanTp_ChannelsIterator].nsduChannel = &CfgPtr->channels[i];
                CanTp_ChannelsIterator += 1;
            } else {
                initOk = FALSE;
            }
        }
    }

    if (initOk) {
        CanTp_State.general = CANTP_ON;
    } else {
        CanTp_State.general = CANTP_OFF;
    }

    CanTp_State.tx = CANTP_TX_WAIT;
    CanTp_State.rx = CANTP_RX_WAIT;
}

void CanTp_Shutdown(void)
{
    CanTp_State.general = CANTP_OFF;
}
