//------------------------------------------------------------------------------
//! @file WiMOD_SAP_RadioLink_IDs.h
//! @ingroup WiMOD_SAP_RadioLink
//! <!------------------------------------------------------------------------->
//! @brief Supporting IDs and definitions for the RadioLink ServiceAccessPoint
//! @version 0.1
//! <!------------------------------------------------------------------------->
//!
//!
//!
//! <!--------------------------------------------------------------------------
//! Copyright (c) 2016
//! IMST GmbH
//! Carl-Friedrich Gauss Str. 2-4
//! 47475 Kamp-Lintfort
//! --------------------------------------------------------------------------->
//! @author  (FB), IMST
//! <!--------------------------------------------------------------------------
//! Target OS:    none
//! Target CPU:   tbd
//! Compiler:     tbd
//! --------------------------------------------------------------------------->
//! @internal
//! @par Revision History:
//! <PRE>
//!-----------------------------------------------------------------------------
//! Version | Date       | Author | Comment
//!-----------------------------------------------------------------------------
//!
//! </PRE>
//------------------------------------------------------------------------------


/*
 * THIS IS AN EXAMPLE IMPLEMENTATION ACCORDING THE THE HCI SPEC: V1.10
 * FOR FIRMWARE: LR-BASE
 *
 * SEE FILE: WiMOD_LR_Base_HCI_Spec_V1_10.pdf for detailed information
 */


#ifndef ARDUINO_SAP_WIMOD_SAP_RADIOLINK_IDS_H_
#define ARDUINO_SAP_WIMOD_SAP_RADIOLINK_IDS_H_

//------------------------------------------------------------------------------
//
// Section Includes Files
//
//------------------------------------------------------------------------------

#include "utils/WMDefs.h"

//------------------------------------------------------------------------------
//
// Service Access Point Identifier
//
//------------------------------------------------------------------------------

/** SAP ID for the RadioLink service */
#define RADIOLINK_SAP_ID                      0x03



//------------------------------------------------------------------------------
//
//  Radio Link Services Identifier
//
//------------------------------------------------------------------------------
//! @cond Doxygen_Suppress
#define RADIOLINK_MSG_SEND_U_DATA_REQ                   0x01
#define RADIOLINK_MSG_SEND_U_DATA_RSP                   0x02
#define RADIOLINK_MSG_U_DATA_RX_IND                     0x04
#define RADIOLINK_MSG_U_DATA_TX_IND                     0x06
#define RADIOLINK_MSG_RAW_DATA_RX_IND                   0x08
#define RADIOLINK_MSG_SEND_C_DATA_REQ                   0x09
#define RADIOLINK_MSG_SEND_C_DATA_RSP                   0x0A
#define RADIOLINK_MSG_C_DATA_RX_IND                     0x0C
#define RADIOLINK_MSG_C_DATA_TX_IND                     0x0E
#define RADIOLINK_MSG_ACK_RX_IND                        0x10
#define RADIOLINK_MSG_ACK_TIMEOUT_IND                   0x12
#define RADIOLINK_MSG_ACK_TX_IND                        0x14
#define RADIOLINK_MSG_SET_ACK_DATA_REQ                  0x15
#define RADIOLINK_MSG_SET_ACK_DATA_RSP                  0x16
//! @endcond

//------------------------------------------------------------------------------
//
//  Radio Link Status Codes
//
//------------------------------------------------------------------------------
//! @cond Doxygen_Suppress

#define RADIOLINK_STATUS_OK                             0x00
#define RADIOLINK_STATUS_ERROR                          0x01
#define RADIOLINK_STATUS_CMD_NOT_SUPPORTED              0x02
#define RADIOLINK_STATUS_WRONG_PARAMETER                0x03
#define RADIOLINK_STATUS_WRONG_RADIO_MODE               0x04
#define RADIOLINK_STATUS_MEDIA_BUSY                     0x05
#define RADIOLINK_STATUS_BUFFER_FULL                    0x07
#define RADIOLINK_STATUS_LENGTH_ERROR                   0x08
//! @endcond

//------------------------------------------------------------------------------
//
//  Format / Status Field defines
//
//------------------------------------------------------------------------------
//! @cond Doxygen_Suppress
#define RADIOLINK_FORMAT_EXTENDED_OUTPUT                (1 << 0) // bit field
#define RADIOLINK_FORMAT_DECRYPTED_DATA                 (1 << 5) // bit field
#define RADIOLINK_FORMAT_DECRYPTION_ERROR               (1 << 6) // bit field
#define RADIOLINK_FORMAT_ENCRYPTED_DATA                 (1 << 7) // bit field
//! @endcond
//------------------------------------------------------------------------------
//
//  Misc. Defines
//
//------------------------------------------------------------------------------
//! @cond Doxygen_Suppress
#define RADIOLINK_BROADCAST_GROUP_ADR                  0xFF
#define RADIOLINK_BROADCAST_DEVICE_ADR                 0xFFFF
//! @endcond

/*
 * Number of payload bytes for a user defined message
 *
 * Notes:
 *
 * 1) the actual number of payload bytes can be less, if the air frame
 * contains a list of MAC commands.
 *
 * 2) The value of the WiMODLRBASE_APP_PAYLOAD_LEN taken here is a tradeoff.
 * The actual size of the payload is mostly bigger than 100 and depends on the
 * settings of the radio and its firmware itself. Please consult the according
 * firmware HCI documentation to figure out the exact value possible.
 * But the amount of memory must be reserved on the host MCU system, too. Some
 * memory contrained Arduino boards (e.g.) the UNO have got only ~2kB of memory.
 * So the default message size has been reduced here. If you want to use the
 * maximal payload size possible you can change this define here.
 */
//! @cond Doxygen_Suppress
#define WiMODLRBASE_APP_PAYLOAD_LEN                       100

#define RADIOLINK_HEADER_SIZE                             (1+1+2+1+2+2+1+4)
//! @endcond
//------------------------------------------------------------------------------
//
// structures
//
//------------------------------------------------------------------------------
//! @cond Doxygen_Suppress
#define WIMOD_RADIOLINK_PAYLOAD_LEN                      (WiMODLRBASE_APP_PAYLOAD_LEN - RADIOLINK_HEADER_SIZE)

#define WIMOD_RADIOLINK_ACK_DATA_LEN                     ( 0x08 )
//! @endcond

/**
 * @brief Structure containing a general RadioLink HCI message
 */
typedef struct
{
    // only for RX
    UINT8       StatusFormat;                                                   /*!< Status/Format Field set by WiMOD; (WiMOD --> Host only) */

    // RX + TX
    UINT8       DestinationGroupAddress;                                        /*!< Address of destination group this message is targeting to  */
    UINT16      DestinationDeviceAddress;                                       /*!< Address of destination device this message is targeting to */

    //only for RX
    UINT8       SourceGroupAddress;                                             /*!< Address of source group this message was send from */
    UINT16      SourceDeviceAddress;                                            /*!< Address of source device this message was send from */

    // RX + TX
    UINT8       Length;                                                         /*!< length of the (user) payload data field */
    UINT8       Payload[WIMOD_RADIOLINK_PAYLOAD_LEN];                           /*!< (user) payload field */
    // note: in case of encrypted data the sequence number and MIC
    //       is included in this payload field. The user must check the
    //       StatusFormat field

    bool        OptionalInfoAvaiable;                                           /*!< Flag indicating if optional receiver infomation is available*/
    // optional fields; only for RX
    INT16       RSSI;                                                           /*!< RSSI value (WiMOD --> host only) */
    INT8        SNR;                                                            /*!< SNR value (WiMOD --> host only) */
    INT32       RxTime;                                                         /*!< RX timestamp of this message (WiMOD --> host only) */

    //optional: only if decryption error occurred
    INT16		MIC;															/*!< Message Integrity Code for decryption */
} TWiMODLR_RadioLink_Msg;

/**
 * @brief Structure containing information about a C-Data indication (TX only)
 */
typedef struct
{
    UINT8       Status;                                                         /*!< Status code for this information packet */
    UINT16      TxEventCounter;                                                 /*!< TX event counter value */
    UINT32		AirTime;														/*!< Air time of last transmission in ms (new SpeV 1V10 */
} TWiMODLR_RadioLink_CdataInd;


/**
 * @brief Structure containing information about a U-Data indication (TX only)
 */
typedef struct
{
    UINT8       Status;                                                         /*!< Status code for this information packet */
    UINT16      TxEventCounter;                                                 /*!< TX event counter value */
    UINT32		AirTime;														/*!< Air time of last transmission in ms (new SpeV 1V10 */
} TWiMODLR_RadioLink_UdataInd;


/**
 * @brief Structure containing information about an acknowledgment transmission
 */
typedef struct
{
    UINT8       Status;                                                         /*!< Status code for the transmission */
    UINT16      TxEventCounter;                                                 /*!< TX event counter value */
    UINT32      AirTime;                                                        /*!< Air time of last transmission in ms (new SpeV 1V10 */
} TWiMODLR_AckTxInd_Msg;

#endif /* ARDUINO_SAP_WIMOD_SAP_RADIOLINK_IDS_H_ */
