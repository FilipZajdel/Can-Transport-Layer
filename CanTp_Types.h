#ifndef CAN_TP_TYPES_H
#define CAN_TP_TYPES_H

/*====================================================================================================================*\
 \@file CAN Transport Layer Types definitions

 FilipZajdel & michal-szymocha (c) 2022
\*====================================================================================================================*/

#include "ComStack_Types.h"

#define CONFIG_CAN_TP_MAX_CHANNELS_COUNT (uint32)8
#define CONFIG_CANTP_MAX_TX_NSDU_PER_CHANNEL (uint32)5
#define CONFIG_CANTP_MAX_RX_NSDU_PER_CHANNEL (uint32)5
#define CONFIG_CANTP_MAIN_FUNCTION_PERIOD (uint32)1

/** If can 2.0 and can FD is used */
#define FRAME_SF_MAX_LEN (8)
#define FRAME_FF_MAX_LEN (4095)

typedef uint32_t CanTp_NSduId;

typedef enum
{
    CANTP_STANDARD = 0,
    CANTP_NORMALFIXED,
    CANTP_MIXED29BIT,
    CANTP_MIXED,
    CANTP_EXTENDED
} CanTp_AddressingFormatType;

typedef enum
{
    CANTP_ON = 0,
    CANTP_OFF
} CanTp_PaddingActivationType;

typedef enum
{
    CANTP_TX_WAIT,
    CANTP_TX_PROCESSING
} CanTp_TxNSduState;

typedef enum
{
    CANTP_RX_WAIT,
    CANTP_RX_PROCESSING
} CanTp_RxNSduState;

typedef enum
{
    CANTP_PHYSICAL = 0,
    CANTP_FUNCTIONAL
} CanTp_TaTypeType;

typedef struct
{
    uint8 nAe;
} CanTp_NAeType;

typedef struct
{
    uint8 nSa;
} CanTp_NSaType;

typedef struct
{
    uint8 nTa;
} CanTp_NTaType;

typedef struct
{
    /**
     * @brief Handle Id to be used by the CanIf to confirm the transmission of
     * the CanTpTxNPdu to the CanIf module.
     */
    uint16 id;

    /**
     * @brief Reference to a Pdu in the COM-Stack.
     */
    PduIdType ref;
} CanTp_NPduType;

typedef struct
{
    uint16 nPduConfirmationPduId;
    PduIdType ref;
} CanTp_FcNPduType;

typedef struct
{
    uint8 bs;
    uint32 nar;
    uint32 nbr;
    uint32 ncr;
    CanTp_AddressingFormatType addressingFormat;
    uint16 id;
    CanTp_PaddingActivationType paddingActivation;
    CanTp_TaTypeType taType;
    uint16 wftMax;
    uint32 STmin;
    PduIdType ref;
    const CanTp_NAeType *pNAe;
    const CanTp_NSaType *pNSa;
    const CanTp_NTaType *pNTa;
    const CanTp_NPduType *rxNPdu;
    const CanTp_FcNPduType *txFcNPdu;
} CanTp_RxNSduType;

typedef struct
{
    /**
     * @brief Value in second of the N_As timeout. N_As is the time for
     * transmission of a CAN frame (any N_PDU) on the part of the sender.
    */
    uint32 nas;

    /**
     * @brief Value in seconds of the N_Bs timeout. N_Bs is the time of
     * transmission until reception of the next Flow Control N_PDU.
     */
    uint32 nbs;

    /**
     * @brief Value in seconds of the performance requirements relating to N_Cs.
     * CanTpNcs is the time in which CanTp is allowed to request from PduR the
     * Tx data of a Consecutive Frame N_PDU.
     */
    uint32 ncs;

    /**
     * @brief Switch for enabling Transmit Cancellation.
     */
    boolean tc;

    /**
     * @brief Declares which communication addressing format is supported
     * for this TxNSdu.
     */
    CanTp_AddressingFormatType addressingFormat;

    /**
     * @brief Unique identifier to a structure that contains all useful
     * information to process the transmission of a TxNsdu.
     * ECUC_CanTp_00268
     */
    uint16 id;

    /**
     * @brief Defines if the transmit frame use padding or not. This parameter
     * is restricted to 8 byte N-PDUs.
     */
    CanTp_PaddingActivationType paddingActivation;

    /**
     * @brief Declares the communication type of this TxNsdu.
     * CanTpPhysical. Used for 1:1 communication.
     * CanTpFunctional. Used for 1:n communication.
     */
    CanTp_TaTypeType taType;

    /**
     * @brief Reference to a Pdu in the COM-Stack.
     */
    PduIdType ref;

    /**
     * @brief Used for grouping of the ID of a PDU and the Reference to a
     * PDU. This N-PDU produces a meta data item of type CAN_ID_32.
     */
    const CanTp_NPduType txNPdu;
    const CanTp_NAeType *pNAe;
    const CanTp_NSaType *pNSa;
    const CanTp_NTaType *pNTa;
    const CanTp_FcNPduType *rxFcNPdu;
} CanTp_TxNSduType;

typedef struct
{
    CanTp_RxNSduType rxNSdu[CONFIG_CANTP_MAX_RX_NSDU_PER_CHANNEL];
    uint32 rxNSduCount;
    CanTp_TxNSduType txNSdu[CONFIG_CANTP_MAX_TX_NSDU_PER_CHANNEL];
    uint32 txNSduCount;
} CanTp_ChannelType;

typedef struct
{
    CanTp_ChannelType channels[CONFIG_CAN_TP_MAX_CHANNELS_COUNT];
} CanTp_ConfigType;

/**
 * @brief Type for holding a RX connection info on a given RxNsdu
 */
typedef struct
{
    CanTp_RxNSduState state;
    /** Pointer to nsdu in CanTp_Config.channels */
    CanTp_RxNSduType *nsdu;

    /**
     * Fill it with other necessary variables
     * .
     * .
     */
} CanTp_RxConnection;

/**
 * @brief Type for holding a TX connection info on a given RxNsdu
 */
typedef struct
{
    CanTp_TxNSduState state;
    /** Pointer to nsdu in CanTp_Config.channels */
    CanTp_TxNSduType *nsdu;

    /**
     * Fill it with other necessary variables
     * .
     * .
     */
} CanTp_TxConnection;

typedef struct
{
    CanTp_PaddingActivationType activation;
    uint32 currentTime;

    /**
     * @brief Any time a new nsdu is started to be received
     * an entry corresponding to its id is modified
     */
    CanTp_RxConnection rxConnections[CONFIG_CAN_TP_MAX_CHANNELS_COUNT*CONFIG_CANTP_MAX_RX_NSDU_PER_CHANNEL];

    /**
     * @brief Any time a new nsdu is started to be received
     * an entry corresponding to its id is modified
     */
    CanTp_TxConnection txConnections[CONFIG_CAN_TP_MAX_CHANNELS_COUNT*CONFIG_CANTP_MAX_TX_NSDU_PER_CHANNEL];
} CanTp_State_t;

#endif /* CAN_TP_TYPES_H */
