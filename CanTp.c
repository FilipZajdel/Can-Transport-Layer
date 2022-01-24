
#include <CanIf.h>
#include <CanTp.h>
#include <CanTp_Types.h>
#include <Det.h>
#include <PduR_CanTp.h>

#define CANTP_IS_ON() (CanTp_State.activation == CANTP_ON)
#define PARAM_UNUSED(param) (void)param
#define PDUID_VALID(pduid) (pduid < CONFIG_CAN_TP_MAX_CHANNELS_COUNT)

#define ARR_SIZE(arr) sizeof(arr) / (sizeof(*arr))

#define CANTP_N_PCI_TYPE_SF 0x00
#define CANTP_N_PCI_TYPE_FF 0x01
#define CANTP_N_PCI_TYPE_CF 0x02
#define CANTP_N_PCI_TYPE_FC 0x03

typedef enum
{
    CANTP_NSDU_DIRECTION_TX,
    CANTP_NSDU_DIRECTION_RX
} CanTp_NSduDirection_t;

static CanTp_State_t CanTp_State;
static CanTp_ConfigType config = {
    .channels = {
        {// Channel 0
         .rxNSdu = {{.id = 1}, {.id = 2}, {.id = 3}, {.id = 4}, {.id = 5}},
         .rxNSduCount = 5,
         .txNSdu = {{.id = 1}},
         .txNSduCount = 1},
        {// Channel 1
         .rxNSdu = {{.id = 6}, {.id = 7}, {.id = 8}},
         .rxNSduCount = 3,
         .txNSdu = {{.id = 6}, {.id = 7}, {.id = 8}, {.id = 9}, {.id = 10}},
         .txNSduCount = 5},
        {// Channel 2
         .rxNSduCount = 0,
         .txNSdu = {{.id = 11}, {.id = 12}},
         .txNSduCount = 2},
        {// Channel 3
         .rxNSdu = {{.id = 13}, {.id = 14}, {.id = 15}},
         .rxNSduCount = 3,
         .txNSduCount = 0},
        {// Channel 4
         .rxNSdu = {{.id = 16}, {.id = 17}, {.id = 18}, {.id = 19}},
         .rxNSduCount = 4,
         .txNSdu = {{.id = 16}, {.id = 20}, {.id = 21}, {.id = 22}},
         .txNSduCount = 4},
        {// Channel 5
         .rxNSduCount = 0,
         .txNSdu = {{.id = 23}, {.id = 24}, {.id = 25}, {.id = 26}, {.id = 27}},
         .txNSduCount = 5},
        {// Channel 6
         .rxNSdu = {{.id = 28}},
         .rxNSduCount = 1,
         .txNSdu = {{.id = 29}, {.id = 30}},
         .txNSduCount = 2},
        {// Channel 7
         .rxNSdu = {{.id = 31}, {.id = 32}, {.id = 33}, {.id = 34}},
         .rxNSduCount = 4,
         .txNSdu = {{.id = 34}, {.id = 35}, {.id = 36}},
         .txNSduCount = 3},
    }};

static CanTp_State_t CanTp_State = {
    .activation = CANTP_OFF,
    .currentTime = 0,
    .rxConnections =
        {
            {.nsdu = &config.channels[0].rxNSdu[0], .activation = CANTP_RX_WAIT},
            {.nsdu = &config.channels[0].rxNSdu[1], .activation = CANTP_RX_WAIT},
            {.nsdu = &config.channels[0].rxNSdu[2], .activation = CANTP_RX_WAIT},
            {.nsdu = &config.channels[0].rxNSdu[3], .activation = CANTP_RX_WAIT},
            {.nsdu = &config.channels[0].rxNSdu[4], .activation = CANTP_RX_WAIT},
            {.nsdu = &config.channels[1].rxNSdu[0], .activation = CANTP_RX_WAIT},
            {.nsdu = &config.channels[1].rxNSdu[1], .activation = CANTP_RX_WAIT},
            {.nsdu = &config.channels[1].rxNSdu[2], .activation = CANTP_RX_WAIT},
            {.nsdu = &config.channels[3].rxNSdu[0], .activation = CANTP_RX_WAIT},
            {.nsdu = &config.channels[3].rxNSdu[1], .activation = CANTP_RX_WAIT},
            {.nsdu = &config.channels[3].rxNSdu[2], .activation = CANTP_RX_WAIT},
            {.nsdu = &config.channels[4].rxNSdu[0], .activation = CANTP_RX_WAIT},
            {.nsdu = &config.channels[4].rxNSdu[1], .activation = CANTP_RX_WAIT},
            {.nsdu = &config.channels[4].rxNSdu[2], .activation = CANTP_RX_WAIT},
            {.nsdu = &config.channels[4].rxNSdu[3], .activation = CANTP_RX_WAIT},
            {.nsdu = &config.channels[6].rxNSdu[0], .activation = CANTP_RX_WAIT},
            {.nsdu = &config.channels[7].rxNSdu[0], .activation = CANTP_RX_WAIT},
            {.nsdu = &config.channels[7].rxNSdu[1], .activation = CANTP_RX_WAIT},
            {.nsdu = &config.channels[7].rxNSdu[2], .activation = CANTP_RX_WAIT},
            {.nsdu = &config.channels[7].rxNSdu[3], .activation = CANTP_RX_WAIT},
        },
    .txConnections = {
        {.nsdu = &config.channels[0].txNSdu[0], .activation = CANTP_TX_WAIT},
        {.nsdu = &config.channels[1].txNSdu[0], .activation = CANTP_TX_WAIT},
        {.nsdu = &config.channels[1].txNSdu[1], .activation = CANTP_TX_WAIT},
        {.nsdu = &config.channels[1].txNSdu[2], .activation = CANTP_TX_WAIT},
        {.nsdu = &config.channels[1].txNSdu[3], .activation = CANTP_TX_WAIT},
        {.nsdu = &config.channels[1].txNSdu[4], .activation = CANTP_TX_WAIT},
        {.nsdu = &config.channels[2].txNSdu[0], .activation = CANTP_TX_WAIT},
        {.nsdu = &config.channels[2].txNSdu[1], .activation = CANTP_TX_WAIT},
        {.nsdu = &config.channels[4].txNSdu[0], .activation = CANTP_TX_WAIT},
        {.nsdu = &config.channels[4].txNSdu[1], .activation = CANTP_TX_WAIT},
        {.nsdu = &config.channels[4].txNSdu[2], .activation = CANTP_TX_WAIT},
        {.nsdu = &config.channels[4].txNSdu[3], .activation = CANTP_TX_WAIT},
        {.nsdu = &config.channels[5].txNSdu[0], .activation = CANTP_TX_WAIT},
        {.nsdu = &config.channels[5].txNSdu[1], .activation = CANTP_TX_WAIT},
        {.nsdu = &config.channels[5].txNSdu[2], .activation = CANTP_TX_WAIT},
        {.nsdu = &config.channels[5].txNSdu[3], .activation = CANTP_TX_WAIT},
        {.nsdu = &config.channels[5].txNSdu[4], .activation = CANTP_TX_WAIT},
        {.nsdu = &config.channels[6].txNSdu[0], .activation = CANTP_TX_WAIT},
        {.nsdu = &config.channels[6].txNSdu[1], .activation = CANTP_TX_WAIT},
        {.nsdu = &config.channels[7].txNSdu[0], .activation = CANTP_TX_WAIT},
        {.nsdu = &config.channels[7].txNSdu[1], .activation = CANTP_TX_WAIT},
        {.nsdu = &config.channels[7].txNSdu[2], .activation = CANTP_TX_WAIT},
    }};

static inline void memzero(uint8_t *ptr, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++) {
        *(ptr + i) = 0;
    }
}

static CanTp_TxConnection *getTxConnection(PduIdType PduId)
{
    CanTp_ChannelType *channels = config.channels;
    CanTp_TxConnection *txConnection = NULL;

    for (uint32 connItr = 0; connItr < ARR_SIZE(CanTp_State.txConnections) && !txConnection;
         connItr++) {
        if (CanTp_State.txConnections[connItr].nsdu->id == PduId) {
            txConnection = &CanTp_State.txConnections[connItr];
        }
    }

    return txConnection;
}

static CanTp_RxConnection *getRxConnection(PduIdType PduId)
{
    CanTp_ChannelType *channels = config.channels;
    CanTp_RxConnection *rxConnection = NULL;

    for (uint32 connItr = 0; connItr < ARR_SIZE(CanTp_State.rxConnections) && !rxConnection;
         connItr++) {
        if (CanTp_State.rxConnections[connItr].nsdu->id == PduId) {
            rxConnection = &CanTp_State.rxConnections[connItr];
        }
    }

    return rxConnection;
}

static uint32 determineMaxTxNsduLength(CanTp_TxNSduType *nsdu)
{
    uint32 maxLen = 0;

#if defined(CONFIG_CAN_2_0_OR_CAN_FD)
    if (nsdu) {
        if ((nsdu->addressingFormat == CANTP_STANDARD) ||
            (nsdu->addressingFormat == CANTP_NORMALFIXED)) {
            maxLen = 7;
        } else {
            maxLen = 6;
        }
#elif defined(CONFIG_CAN_FD_ONLY)
    // @TODO: Implement determineMaxTxNsduLength for CAN_FD
#endif
    }
    return maxLen;
}

void CanTp_Init(const CanTp_ConfigType *CfgPtr)
{
    PARAM_UNUSED(CfgPtr);

    /** Note: rxConnections and txConnections have the same size */
    for (uint32 connItr = 0; connItr < ARR_SIZE(CanTp_State.rxConnections); connItr++) {
        CanTp_State.rxConnections[connItr].activation = CANTP_RX_WAIT;
        CanTp_State.rxConnections[connItr].activation = CANTP_TX_WAIT;
    }

    CanTp_State.activation = CANTP_ON;
    CanTp_State.currentTime = 0;
}

void CanTp_Shutdown(void)
{
    CanTp_State.activation = CANTP_OFF;

    /** Note: rxConnections and txConnections have the same size */
    for (uint32 connItr = 0; connItr < ARR_SIZE(CanTp_State.rxConnections); connItr++) {
        CanTp_State.rxConnections[connItr].activation = CANTP_RX_WAIT;
        CanTp_State.rxConnections[connItr].activation = CANTP_TX_WAIT;
    }
}

Std_ReturnType CanTp_Transmit(PduIdType TxPduId, const PduInfoType *PduInfoPtr)
{
    Std_ReturnType result = E_NOT_OK;
    CanTp_TxNSduType *nsdu = NULL;
    uint32 maxNsduLength = 0;
    CanTp_TxConnection *connection = getTxConnection(TxPduId);

    if (connection != NULL) {
        return result;
    }

    if (connection->activation == CANTP_TX_PROCESSING) {
        return result;
    }

    connection->activation = CANTP_TX_PROCESSING;
    nsdu = connection->nsdu;
    maxNsduLength = determineMaxTxNsduLength(nsdu);
    result = E_OK;

    /** Note: Only SF and FF transmission is triggered here. CF frames and FC
     * are triggered from RX/TX state machines.
     */
    if (PduInfoPtr->SduLength < maxNsduLength) {
        connection->state = CANTP_TX_STATE_SF_SEND_REQ;
    } else {
        connection->state = CANTP_TX_STATE_FF_SEND_REQ;
    }

    return result;
}

void CanTp_RxIndication(PduIdType RxPduId, const PduInfoType *PduInfoPtr)
{
    Std_ReturnType result = E_NOT_OK;
    CanTp_RxNSduType *nsdu = NULL;
    CanTp_RxConnection *connection = getRxConnection(RxPduId);
    uint8 addrModeByteSkip = 0;
    if (connection != NULL) {
        return result;
    }

    if (connection->activation == CANTP_RX_PROCESSING) {
        return result;
    }

    nsdu = connection->nsdu;

    if ((nsdu->paddingActivation == CANTP_ON) && (PduInfoPtr->SduLength < 8)) {
        Det_ReportRuntimeError(CANTP_MODULE_ID, 0, CANTP_RX_INDICATION_API_ID, CANTP_E_PADDING);
        return result;
    }

    connection->activation = CANTP_RX_PROCESSING;

    switch (nsdu->addressingFormat) {
        case CANTP_EXTENDED:
        case CANTP_MIXED:
        case CANTP_MIXED29BIT:
            addrModeByteSkip = 1;
            break;
        case CANTP_STANDARD:
        case CANTP_NORMALFIXED:
            addrModeByteSkip = 0;
            break;
        default:
            return result;
            break;
    }

    switch (((PduInfoPtr->SduDataPtr[addrModeByteSkip]) >> 4) & 0xF) {
        case CANTP_N_PCI_TYPE_FF:
            break;
        case CANTP_N_PCI_TYPE_SF:
            break;
        case CANTP_N_PCI_TYPE_CF:
            break;
        case CANTP_N_PCI_TYPE_FC:
            break;
        default:
            return result;
            break;
    }

    /* put somwhere to get buffer */
    // PduR_CanTpStartOfReception(RxPduId, PduInfoPtr,  );

    return result;
}

/**
 * @brief Implements the TX state machine
 */
static void CanTp_TxIteration(void)
{
    for (uint32 connItr = 0; connItr < ARR_SIZE(CanTp_State.txConnections); connItr++) {
        CanTp_TxConnection *conn = &CanTp_State.txConnections[connItr];
        switch (conn->state) {
            case CANTP_TX_STATE_SF_SEND_REQ:

                break;
            case CANTP_TX_STATE_FF_SEND_REQ:
                break;
            default:
                break;
        }
    }
}

/**
 * @brief Implements the RX state machine
 */
static void CanTp_RxIteration(void)
{
    for (uint32 connItr = 0; connItr < ARR_SIZE(CanTp_State.rxConnections); connItr++) {
        CanTp_RxConnection *conn = &CanTp_State.rxConnections[connItr];
        switch (conn->state) {
            default:
                break;
        }
    }
}

void CanTp_MainFunction(void)
{
    if (CANTP_IS_ON()) {
        CanTp_TxIteration();
        CanTp_RxIteration();
    }

    CanTp_State.currentTime += CONFIG_CANTP_MAIN_FUNCTION_PERIOD;
}
