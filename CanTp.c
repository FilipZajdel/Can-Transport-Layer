
#include <CanIf.h>
#include <CanTp.h>
#include <CanTp_Types.h>
#include <Det.h>
#include <PduR_CanTp.h>

#define CANTP_IS_ON() (CanTp_State.activation == CANTP_ON)
#define PARAM_UNUSED(param) (void)param
#define ARR_SIZE(arr) sizeof(arr) / (sizeof(*arr))

typedef enum
{
    CANTP_N_PCI_TYPE_SF = 0x00,
    CANTP_N_PCI_TYPE_FF = 0x01,
    CANTP_N_PCI_TYPE_CF = 0x02,
    CANTP_N_PCI_TYPE_FC = 0x03,
} CanTp_PciType;

typedef enum
{
    /**
     * No pending operations on a connection.
     */
    CANTP_TX_STATE_FREE,
    /**
     * Getting the data from PduR and composing the frame for lower layer.
     */
    CANTP_TX_STATE_SF_SEND_REQ,
    /**
     * Sending the data to CanIf.
     */
    CANTP_TX_STATE_SF_SEND_PROCESS,
    CANTP_TX_STATE_FF_SEND_REQ,
    CANTP_TX_STATE_CF_SEND_REQ,
    CANTP_TX_STATE_WAIT_FC,
    CANTP_TX_STATE_WAIT_CANIF_CONFIRM
} CanTp_TxConnectionState;

typedef enum
{
    CANTP_RX_STATE_FREE,
    CANTP_RX_STATE_SF_RCV,
    CANTP_RX_STATE_FF_RCV,
    CANTP_RX_STATE_CF_SEND_REQ,
    CANTP_RX_STATE_WAIT_FC,
    CANTP_RX_STATE_WAIT_CANIF_CONFIRM
} CanTp_RxConnectionState;

/**
 * @brief Type for holding a RX connection info on a given RxNsdu
 */
typedef struct
{
    CanTp_RxNSduState activation;
    /** Points to nsdu in CanTp_Config.channels */
    CanTp_RxNSduType *nsdu;
    CanTp_RxConnectionState state;

    /**
     * Fill it with other necessary variables
     * .
     * .
     */
    struct
    {
        uint32 ar;
        uint32 br;
        uint32 cr;
    } timer;

} CanTp_RxConnection;

typedef struct
{
#if defined(CONFIG_CAN_2_0_OR_CAN_FD)
    uint8 data[8];
#elif defined(CONFIG_CAN_FD_ONLY)
    uint8 data[64];
#endif
    uint8 payloadOffset;
    uint8 payloadLength;
} CanTp_ConnectionBuffer;

/**
 * @brief Type for holding a TX connection info on a given RxNsdu
 */
typedef struct
{
    CanTp_TxNSduState activation;
    /** Points to nsdu in CanTp_Config.channels */
    CanTp_TxNSduType *nsdu;
    CanTp_TxConnectionState state;
    PduInfoType pduInfo;
    CanTp_ConnectionBuffer buf;

    struct
    {
        uint32 as;
        uint32 bs;
        uint32 cs;
    } timer;

    uint8 sequenceNumber;
} CanTp_TxConnection;

typedef struct
{
    CanTp_PaddingActivationType activation;
    uint32 currentTime;

    /**
     * @brief Any time a new nsdu is started to be received
     * an entry corresponding to its id is modified
     */
    CanTp_RxConnection
        rxConnections[CONFIG_CAN_TP_MAX_CHANNELS_COUNT * CONFIG_CANTP_MAX_RX_NSDU_PER_CHANNEL];

    /**
     * @brief Any time a new nsdu is started to be received
     * an entry corresponding to its id is modified
     */
    CanTp_TxConnection
        txConnections[CONFIG_CAN_TP_MAX_CHANNELS_COUNT * CONFIG_CANTP_MAX_TX_NSDU_PER_CHANNEL];
} CanTp_State_t;

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

        },
    .txConnections =
        {
            {.nsdu = &config.channels[0].txNSdu[0], .activation = CANTP_TX_WAIT},
            {.nsdu = &config.channels[1].txNSdu[0], .activation = CANTP_TX_WAIT},
            {.nsdu = &config.channels[1].txNSdu[1], .activation = CANTP_TX_WAIT},
            {.nsdu = &config.channels[1].txNSdu[2], .activation = CANTP_TX_WAIT},
            {.nsdu = &config.channels[1].txNSdu[3], .activation = CANTP_TX_WAIT},
            {.nsdu = &config.channels[1].txNSdu[4], .activation = CANTP_TX_WAIT},
            {.nsdu = &config.channels[2].txNSdu[0], .activation = CANTP_TX_WAIT},
            {.nsdu = &config.channels[2].txNSdu[1], .activation = CANTP_TX_WAIT},

        },
};

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

static inline uint8 CanTp_DecodeFrameType(uint8 *sdu) { return (((sdu[0]) >> 4) & 0xF); }

static inline uint8 CanTp_GetAddrFieldLen(CanTp_AddressingFormatType af)
{
    uint8 extAddrFieldLen = 0;

    switch (af) {
        case CANTP_EXTENDED:
        case CANTP_MIXED:
        case CANTP_MIXED29BIT:
            extAddrFieldLen = 1;
            break;
        case CANTP_STANDARD:
        case CANTP_NORMALFIXED:
            extAddrFieldLen = 0;
            break;
        default:
            break;
    }
}

static uint32 determineMaxTxNsduLength(CanTp_TxNSduType *nsdu)
{
    uint32 maxLen = 0;

#if defined(CONFIG_CAN_2_0_OR_CAN_FD)
    maxLen = 8 - 1 - CanTp_GetAddrFieldLen(nsdu->addressingFormat);
#elif defined(CONFIG_CAN_FD_ONLY)
    maxLen = 64 - 2 - CanTp_GetAddrFieldLen(nsdu->addressingFormat);
#endif

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
        connection->pduInfo.SduLength = PduInfoPtr->SduLength;
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
    uint8 sduDataBeginIndex = 0;
    uint8 *sduData = NULL;

    if (connection == NULL) {
        return;
    }

    if (connection->activation == CANTP_RX_PROCESSING) {
        return;
    }

    nsdu = connection->nsdu;

    if ((nsdu->paddingActivation == CANTP_ON) && (PduInfoPtr->SduLength < 8)) {
        Det_ReportRuntimeError(CANTP_MODULE_ID, 0, CANTP_RX_INDICATION_API_ID, CANTP_E_PADDING);
        return;
    }

    connection->activation = CANTP_RX_PROCESSING;

    sduDataBeginIndex = CanTp_GetAddrFieldLen(nsdu->addressingFormat);

    sduData = &(PduInfoPtr->SduDataPtr[sduDataBeginIndex]);

    switch (CanTp_DecodeFrameType(sduData)) {
        case CANTP_N_PCI_TYPE_FF:
            break;
        case CANTP_N_PCI_TYPE_SF:
            break;
        case CANTP_N_PCI_TYPE_CF:
            break;
        case CANTP_N_PCI_TYPE_FC:
            break;
        default:
            return;
            break;
    }

    /* put somwhere to get buffer */
    // PduR_CanTpStartOfReception(RxPduId, PduInfoPtr,  );

    return;
}

/**
 * @brief Fills the connection's buffer header with data specific for a given pciType
 * @note Only connection's buffer is modified.
 */
static inline CanTp_FillTpHeader(CanTp_TxConnection *conn, CanTp_PciType pciType)
{
    uint16 *header = NULL;
    uint8 addressingInfoOffset = CanTp_GetAddrFieldLen(conn->nsdu->addressingFormat);
    uint8 *buf = &conn->buf.data[addressingInfoOffset];

    buf = ((uint8)pciType << 4);

    // @TODO: Fill Addressing Info

    switch (pciType) {
        case CANTP_N_PCI_TYPE_SF:
#if defined(CONFIG_CAN_2_0_OR_CAN_FD)
            buf[0] &= (uint8)0xF0;
            buf[0] |= (uint8)(conn->pduInfo.SduLength & 0x0F);
            conn->buf.payloadOffset = 1;
#elif defined(CONFIG_CAN_FD_ONLY)
#error "Implement CanTp_FillTpHeader for CAN_FD"
#endif
            break;

        case CANTP_N_PCI_TYPE_FF:
#if defined(CONFIG_CAN_2_0_OR_CAN_FD)
            buf[0] &= (uint8)0xF0;
            header = (uint16 *)&buf[0];
            *header |= ((uint16)conn->pduInfo.SduLength & (uint16)0x0FFF);
            conn->buf.payloadOffset = 2;
#elif defined(CONFIG_CAN_FD_ONLY)
#error "Implement CanTp_FillTpHeader for CAN_FD"
#endif
            break;

        case CANTP_N_PCI_TYPE_CF:
#if defined(CONFIG_CAN_2_0_OR_CAN_FD)
            buf[0] &= (uint8)0xF0;
            buf[0] |= (uint8)(conn->sequenceNumber & 0x0F);
            conn->buf.payloadOffset = 1;
#elif defined(CONFIG_CAN_FD_ONLY)
#error "Implement CanTp_FillTpHeader for CAN_FD"
#endif
        default:
            break;
    }
}

static CanTp_TxConnectionState CanTp_TxStateFsSendReq(CanTp_TxConnection *conn)
{
    CanTp_FillTpHeader(conn, CANTP_N_PCI_TYPE_SF);

    // Lock the data within PduR
    // PduR_CanTpCopyTxData()
}

/**
 * @brief Implements the TX state machine
 */
static void CanTp_TxIteration(void)
{
    for (uint32 connItr = 0; connItr < ARR_SIZE(CanTp_State.txConnections); connItr++) {
        CanTp_TxConnection *conn = &CanTp_State.txConnections[connItr];
        CanTp_TxConnectionState nextState = conn->state;

        switch (conn->state) {
            case CANTP_TX_STATE_SF_SEND_REQ:
                // Get the data from PduR
                nextState = CanTp_TxStateFsSendReq(conn);
                break;
            case CANTP_TX_STATE_SF_SEND_PROCESS:

                break;
            case CANTP_TX_STATE_FF_SEND_REQ:
                break;
            default:
                break;
        }

        conn->state = nextState;
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

/**
 * @brief Increments timers for each tx and rx connection
 */
static void CanTp_ConnectionsTimersInc(void)
{
    for (uint32 connItr = 0; connItr < ARR_SIZE(CanTp_State.txConnections); connItr++) {
        CanTp_TxConnection *conn = &CanTp_State.txConnections[connItr];
        conn->timer.as += CONFIG_CANTP_MAIN_FUNCTION_PERIOD;
        conn->timer.bs += CONFIG_CANTP_MAIN_FUNCTION_PERIOD;
        conn->timer.cs += CONFIG_CANTP_MAIN_FUNCTION_PERIOD;
    }

    for (uint32 connItr = 0; connItr < ARR_SIZE(CanTp_State.rxConnections); connItr++) {
        CanTp_RxConnection *conn = &CanTp_State.rxConnections[connItr];
        conn->timer.ar += CONFIG_CANTP_MAIN_FUNCTION_PERIOD;
        conn->timer.br += CONFIG_CANTP_MAIN_FUNCTION_PERIOD;
        conn->timer.cr += CONFIG_CANTP_MAIN_FUNCTION_PERIOD;
    }
}

void CanTp_MainFunction(void)
{
    if (CANTP_IS_ON()) {
        CanTp_TxIteration();
        CanTp_RxIteration();
        CanTp_ConnectionsTimersInc();
        CanTp_State.currentTime += CONFIG_CANTP_MAIN_FUNCTION_PERIOD;
    }
}
