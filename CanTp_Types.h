#ifndef CAN_TP_TYPES_H
#define CAN_TP_TYPES_H

/*====================================================================================================================*\
 \@file CAN Transport Layer Types definitions

 FilipZajdel (c) 2022
\*====================================================================================================================*/

typedef uint32_t CanTp_NSduId;

typedef enum
{
    CANTP_CHANNEL_DIRECTION_TX,
    CANTP_CHANNEL_DIRECTION_RX
} CanTp_ChannelDirection;

typedef struct
{
} CanTp_TxChannelConfigType;

typedef struct
{
} CanTp_RxChannelConfigType;

typedef union
{
    CanTp_TxChannelConfigType tx;
    CanTp_RxChannelConfigType rx;
} CanTp_ChannelConfigType;

typedef struct
{
    CanTp_ChannelDirection  direction;
    CanTp_ChannelConfigType config;
    CanTp_NSduId            nSduId;
} CanTp_NSduChannelType;

typedef uint32_t CanTp_ChannelCountType;

typedef struct
{
    CanTp_ChannelCountType channelCount;
    CanTp_NSduChannelType  *channels;
} CanTp_ConfigType;


#endif /* CAN_TP_TYPES_H */
