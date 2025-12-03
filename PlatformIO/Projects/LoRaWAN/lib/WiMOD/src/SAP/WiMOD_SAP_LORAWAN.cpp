//------------------------------------------------------------------------------
//! @file WiMOD_SAP_LORAWAN.cpp
//! @ingroup WiMOD_SAP_LORAWAN
//! <!------------------------------------------------------------------------->
//! @brief Implementation of the commands of the LoRaWAN SericeAccessPoint
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
//! @author (FB), IMST
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
 * THIS IS AN EXAMPLE IMPLEMENTATION ACCORDING THE THE HCI SPEC: V1.26
 * FOR FIRMWARE: LoRaWAN_EndNode_Modem
 *
 * SEE FILE: WiMOD_LoRaWAN_EndNode_Modem_HCI_Spec_V1_26.pdf
 */


//------------------------------------------------------------------------------
//
// Section Includes Files
//
//------------------------------------------------------------------------------

#include "WiMOD_SAP_LORAWAN.h"
#include <string.h>

//------------------------------------------------------------------------------
//
// Section public functions
//
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 * @brief Constructor
 *
 * @param hci       Pointer to HCI processor object
 *
 * @param buffer    pointer to storage area for building tx frames; MUST BE VALID
 *
 * @param bufferSize    size of the buffer
 *
 */
WiMOD_SAP_LoRaWAN::WiMOD_SAP_LoRaWAN(TWiMODLRHCI* hci, UINT8* buffer, UINT16 bufferSize):
    HciParser(hci)
{
    JoinTxIndCallback    = NULL;
    NoDataIndCallback    = NULL;
    TxCDataIndCallback   = NULL;
    TxUDataIndCallback   = NULL;
    RxUDataIndCallback   = NULL;
    RxCDataIndCallback   = NULL;
    RxMacCmdIndCallback  = NULL;
    JoinedNwkIndCallback = NULL;
    RxAckIndCallback     = NULL;

    txPayload = buffer;
    txPayloadSize = bufferSize;

    region = LoRaWAN_Region_EU868; // default init
}

//-----------------------------------------------------------------------------
/**
 * @brief Destructor
 */

WiMOD_SAP_LoRaWAN::~WiMOD_SAP_LoRaWAN(void) {

}

//-----------------------------------------------------------------------------
/**
 * @brief Setup regional settings for the LoRaWAN Firmware of the WiMOD module
 *
 *
 * @param regionalSetting region code for the firmware
 *
 */

void WiMOD_SAP_LoRaWAN::setRegion(TLoRaWANregion regionalSetting) {
	region = regionalSetting;
}

//-----------------------------------------------------------------------------
/**
 * @brief Activates the device via the ABP procedure
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param activationData   structure holding the necessary parameters
 *
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_LoRaWAN::ActivateDevice(TWiMODLORAWAN_ActivateDeviceData& activationData,
        UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = 0;

    if ( statusRsp &&
            (txPayloadSize >= (4+ WiMODLORAWAN_NWK_SESSION_KEY_LEN + WiMODLORAWAN_APP_SESSION_KEY_LEN)))
    {
        HTON32(&txPayload[offset], activationData.DeviceAddress);
        offset += 0x04;
        memcpy(&txPayload[offset], activationData.NwkSKey, WiMODLORAWAN_NWK_SESSION_KEY_LEN);
        offset += WiMODLORAWAN_NWK_SESSION_KEY_LEN;
        memcpy(&txPayload[offset], activationData.AppSKey, WiMODLORAWAN_APP_SESSION_KEY_LEN);
        offset += WiMODLORAWAN_APP_SESSION_KEY_LEN;

        result = HciParser->SendHCIMessage(LORAWAN_SAP_ID,
                                           LORAWAN_MSG_ACTIVATE_DEVICE_REQ,
                                           LORAWAN_MSG_ACTIVATE_DEVICE_RSP,
                                           txPayload, offset);
        // copy response status
        if (result == WiMODLR_RESULT_OK) {
            *statusRsp = HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_STATUS_POS];
        }
    }
    return result;
}

//-----------------------------------------------------------------------------
/**
 * @brief Re-Activates the device via the ABP procedure
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param devAdr    pointer where to store the "received" device address
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_LoRaWAN::ReactivateDevice(UINT32* devAdr,
        UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = 0;

    if ( statusRsp && devAdr)
    {

        result = HciParser->SendHCIMessage(LORAWAN_SAP_ID,
                                           LORAWAN_MSG_REACTIVATE_DEVICE_REQ,
                                           LORAWAN_MSG_REACTIVATE_DEVICE_RSP,
                                           txPayload, offset);

        if (result == WiMODLR_RESULT_OK) {
            const TWiMODLR_HCIMessage& rx = HciParser->GetRxMessage();

            *devAdr = NTOH32(&rx.Payload[WiMODLR_HCI_RSP_CMD_PAYLOAD_POS]);

            // copy response status
            *statusRsp = rx.Payload[WiMODLR_HCI_RSP_STATUS_POS];
       }  else {
    	   result = WiMODLR_RESULT_PAYLOAD_PTR_ERROR;
       }
    }
    return result;
}

//-----------------------------------------------------------------------------
/**
 * @brief Sets the parameters used for the OTAA activation procedure
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param joinParams   structure holding the necessary parameters
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_LoRaWAN::SetJoinParameter(TWiMODLORAWAN_JoinParams& joinParams,
        UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = 0;

    if ( statusRsp && (txPayloadSize >= WiMODLORAWAN_APP_EUI_LEN + WiMODLORAWAN_APP_KEY_LEN)) {
        memcpy(&txPayload[offset], joinParams.AppEUI, WiMODLORAWAN_APP_EUI_LEN);
        offset += WiMODLORAWAN_APP_EUI_LEN;
        memcpy(&txPayload[offset], joinParams.AppKey, WiMODLORAWAN_APP_KEY_LEN);
        offset += WiMODLORAWAN_APP_KEY_LEN;

        result = HciParser->SendHCIMessage(LORAWAN_SAP_ID,
                                           LORAWAN_MSG_SET_JOIN_PARAM_REQ,
                                           LORAWAN_MSG_SET_JOIN_PARAM_RSP,
                                           txPayload, offset);
        // copy response status
        if (result == WiMODLR_RESULT_OK) {
            *statusRsp = HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_STATUS_POS];
        }
    }
    return result;
}

//-----------------------------------------------------------------------------
/**
 * @brief Start joining the network via the OTAA procedure. Asynchronous process
 *
 * This functions starts an asynchronous process of joining to a network.
 * It will take a time until a (final) result is ready. Therefore the
 * callback interface (e.g. RegisterJoinedNwkIndicationClient) should be used.
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_LoRaWAN::JoinNetwork(UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;

    if ( statusRsp) {
        result = HciParser->SendHCIMessage(LORAWAN_SAP_ID,
                                           LORAWAN_MSG_JOIN_NETWORK_REQ,
                                           LORAWAN_MSG_JOIN_NETWORK_RSP,
                                           NULL, 0);
        // copy response status
        if (result == WiMODLR_RESULT_OK) {
            *statusRsp = HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_STATUS_POS];
        }
    }
    return result;
}

//-----------------------------------------------------------------------------
/**
 * @brief Tries to send transmit U-Data to network server via RF link
 *
 *
 * @param data       pointer to data structure containing the TX-data and options.
 *                   @see TWiMODLORAWAN_TX_Data for details
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_LoRaWAN::SendUData(const TWiMODLORAWAN_TX_Data* data,
        UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = 0;
    UINT8              tmpSize;

    if ( data && (data->Length > 0) && statusRsp) {

        tmpSize = MIN((WiMOD_LORAWAN_TX_PAYLOAD_SIZE-1), data->Length);

        if (txPayloadSize >= tmpSize) {
            txPayload[offset++] = data->Port;
            memcpy(&txPayload[offset], data->Payload, MIN((WiMOD_LORAWAN_TX_PAYLOAD_SIZE-1), tmpSize));
            offset += tmpSize;

            result = HciParser->SendHCIMessage(LORAWAN_SAP_ID,
                                               LORAWAN_MSG_SEND_UDATA_REQ,
                                               LORAWAN_MSG_SEND_UDATA_RSP,
                                               txPayload, offset);
            // copy response status
            if (result == WiMODLR_RESULT_OK) {
                *statusRsp = HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_STATUS_POS];
            }
        } else {
            result = WiMODLR_RESULT_PAYLOAD_LENGTH_ERROR;
        }

    } else {
        result = WiMODLR_RESULT_PAYLOAD_PTR_ERROR;
    }
    return result;

}

//-----------------------------------------------------------------------------
/**
 * @brief Tries to send transmit C-Data to network server via RF link
 *
 *
 * @param data       pointer to data structure containing the TX-data and options.
 *                   @see TWiMODLORAWAN_TX_Data for details
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_LoRaWAN::SendCData(const TWiMODLORAWAN_TX_Data* data,
        UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = 0;

    if ( data && (data->Length > 0) && statusRsp) {

        txPayload[offset++] = data->Port;
        memcpy(&txPayload[offset], data->Payload, MIN((WiMOD_LORAWAN_TX_PAYLOAD_SIZE-1), data->Length));
        offset += MIN((WiMOD_LORAWAN_TX_PAYLOAD_SIZE-1), data->Length);

        result = HciParser->SendHCIMessage(LORAWAN_SAP_ID,
                                           LORAWAN_MSG_SEND_CDATA_REQ,
                                           LORAWAN_MSG_SEND_CDATA_RSP,
                                           txPayload, offset);
        // copy response status
        if (result == WiMODLR_RESULT_OK) {
            *statusRsp = HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_STATUS_POS];
        }
    } else {
        result = WiMODLR_RESULT_PAYLOAD_PTR_ERROR;
    }
    return result;

}

//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the event "TX Join Indication"
 *
 * This registered callback is called when the specified event is called by
 * the WiMOD stack.
 *
 * @param   cb          pointer to a callback function that should be called
 *                      if the event occurs.
 */

void WiMOD_SAP_LoRaWAN::RegisterJoinTxIndicationClient(TJoinTxIndicationCallback cb)
{
    JoinTxIndCallback =  cb;
}


//-----------------------------------------------------------------------------
/**
 * @brief Convert a received low level HCI-Msg to a high-level tx ind structure
 *
 * This function should be used by the Tx Data Indication Callback functions
 * prior processing the received data message.
 *
 * @param   RxMsg       Reference to low-level HCI message.
 *                      @warning DO NOT MANIPULATE THESE VALUES !!!
 *
 * @param   sendIndData Pointer to the buffer where to store the received data
 *
 * @retval true     if the conversion was successful
 */
bool WiMOD_SAP_LoRaWAN::convert(TWiMODLR_HCIMessage&        RxMsg,
                                TWiMODLORAWAN_TxIndData*    sendIndData)
{
    UINT8 offset = 0;


    if (sendIndData) {
        sendIndData->FieldAvailability = LORAWAN_OPT_TX_IND_INFOS_NOT_AVAILABLE;
    }

    if (sendIndData && RxMsg.Length >= 1) {

        sendIndData->StatusFormat = RxMsg.Payload[offset++];

        if (sendIndData->StatusFormat & LORAWAN_FORMAT_EXT_HCI_OUT_ACTIVE) {

            // check if extra UDATA indication data is available (without TxPktCounter)
            if  (RxMsg.Length > (uint16_t)(offset+2) ){
                sendIndData->FieldAvailability = LORAWAN_OPT_TX_IND_INFOS_EXCL_PKT_CNT;
                sendIndData->ChannelIndex =  RxMsg.Payload[offset++];
                sendIndData->DataRateIndex = RxMsg.Payload[offset++];

                // check if extra TxPktCounter is available
                if (RxMsg.Length > offset) {
                    sendIndData->FieldAvailability = LORAWAN_OPT_TX_IND_INFOS_INCL_PKT_CNT;
                    sendIndData->NumTxPackets = RxMsg.Payload[offset++];
                }

                sendIndData->PowerLevel = 0;
				// check if extra PowerLevel is available
                if (RxMsg.Length > offset) {
                	sendIndData->PowerLevel = RxMsg.Payload[offset++];
                }

				// check if extra AirTimne is available
                sendIndData->RfMsgAirtime = 0;
                if (RxMsg.Length > offset) {
                	sendIndData->RfMsgAirtime = NTOH32(&RxMsg.Payload[offset]);
                	offset += 4;
                }

            }
        }
        return true;
    }
    return false;
}



//-----------------------------------------------------------------------------
/**
 * @brief Convert a received low level HCI-Msg to a high-level Rx Data structure
 *
 * This function should be used by the Rx Data Indication Callback functions
 * prior processing the received data message.
 *
 * @param   RxMsg       Reference to low-level HCI message.
 *                      @warning DO NOT MANIPULATE THESE VALUES !!!
 *
 * @param   loraWanRxData Pointer to the buffer where to store the received data
 *
 * @retval true     if the conversion was successful
 */
bool WiMOD_SAP_LoRaWAN::convert(TWiMODLR_HCIMessage&    RxMsg,
                                TWiMODLORAWAN_RX_Data*  loraWanRxData)
{
    UINT8 offset = 0;

    INT16 dataLen = RxMsg.Length;
    INT16 i;

    if (loraWanRxData) {
        loraWanRxData->Length = 0;
        loraWanRxData->OptionalInfoAvaiable = false;
    }

    if (loraWanRxData && RxMsg.Length >= 1) {

        loraWanRxData->StatusFormat = RxMsg.Payload[offset++];

        if (loraWanRxData->StatusFormat & LORAWAN_FORMAT_EXT_HCI_OUT_ACTIVE) {
            dataLen -= (0x01 + 0x05); // format + rx ch info
            loraWanRxData->OptionalInfoAvaiable = true;
        } else {
            dataLen -= 0x01; // format; only
            loraWanRxData->OptionalInfoAvaiable = false;
        }

        // LoRaWAN port ID
        if (dataLen > 0) {
            loraWanRxData->Port = RxMsg.Payload[offset++];
        }

        // adjust dataLen for the real user defined payload
        dataLen--;

        // copy payload field
        // do not use memcpy here because of potential negative dataLen
        for (i = 0; i < dataLen; i++) {
            loraWanRxData->Payload[i] = RxMsg.Payload[offset++];
            loraWanRxData->Length++;
        }

        // check if optional attributes are present
        if (offset < RxMsg.Length) {
            loraWanRxData->ChannelIndex  = (UINT8) RxMsg.Payload[offset++];
            loraWanRxData->DataRateIndex = (UINT8) RxMsg.Payload[offset++];
            loraWanRxData->RSSI          = (INT8)  RxMsg.Payload[offset++];
            loraWanRxData->SNR           = (INT8)  RxMsg.Payload[offset++];
            loraWanRxData->RxSlot        = (UINT8) RxMsg.Payload[offset++];
        }
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
/**
 * @brief Convert a received low level HCI-Msg to a high-level MAC-Cmd structure
 *
 * This function should be used by the Rx Mac Cmd Indication Callback function
 * prior processing the received data message.
 *
 * @param   RxMsg       Reference to low-level HCI message.
 *                      @warning DO NOT MANIPULATE THESE VALUES !!!
 *
 * @param   loraWanMacCmdData Pointer to the buffer where to store the received data
 *
 * @retval true     if the conversion was successful
 */
bool WiMOD_SAP_LoRaWAN::convert(TWiMODLR_HCIMessage&         RxMsg,
                                TWiMODLORAWAN_RX_MacCmdData* loraWanMacCmdData)
{
    UINT8 offset = 0;

    UINT8 format;
    INT16 dataLen = RxMsg.Length;
    INT16 i;

    if (loraWanMacCmdData) {
        loraWanMacCmdData->Length = 0;
        loraWanMacCmdData->OptionalInfoAvaiable = false;
    }

    if (loraWanMacCmdData && RxMsg.Length >= 1) {

        format = RxMsg.Payload[offset++];

        if (format & LORAWAN_FORMAT_EXT_HCI_OUT_ACTIVE) {
            dataLen -= (0x01 + 0x05); // format + rx ch info
            loraWanMacCmdData->OptionalInfoAvaiable = true;
        } else {
            dataLen -= 0x01; // format; only
            loraWanMacCmdData->OptionalInfoAvaiable = false;
        }

        // copy MAC cmd data field
        // do not use memcpy here because of potential negative dataLen
        for (i = 0; i < dataLen; i++) {
            loraWanMacCmdData->MacCmdData[i] = RxMsg.Payload[offset++];
            loraWanMacCmdData->Length++;
        }

        // check if optional attributes are present
        if (offset < RxMsg.Length) {
            loraWanMacCmdData->ChannelIndex  = (UINT8) RxMsg.Payload[offset++];
            loraWanMacCmdData->DataRateIndex = (UINT8) RxMsg.Payload[offset++];
            loraWanMacCmdData->RSSI          = (INT8)  RxMsg.Payload[offset++];
            loraWanMacCmdData->SNR           = (INT8)  RxMsg.Payload[offset++];
            loraWanMacCmdData->RxSlot        = (UINT8) RxMsg.Payload[offset++];
        }
        return true;
    }
    return false;
}


//-----------------------------------------------------------------------------
/**
 * @brief Convert a received low level HCI-Msg to a high-level NwkJoined structure
 *
 * This function should be used by the JoinedNwk Indication Callback function
 * prior processing the received data message.
 *
 * @param   RxMsg       Reference to low-level HCI message.
 *                      @warning DO NOT MANIPULATE THESE VALUES !!!
 *
 * @param   joinedNwkData Pointer to the buffer where to store the received data
 *
 * @retval true     if the conversion was successful
 */
bool WiMOD_SAP_LoRaWAN::convert(TWiMODLR_HCIMessage&            RxMsg,
                                TWiMODLORAWAN_RX_JoinedNwkData* joinedNwkData)
{
    UINT8 offset = 0;

    INT16 dataLen = RxMsg.Length;

    if (joinedNwkData) {
        joinedNwkData->OptionalInfoAvaiable = false;
    }

    if (joinedNwkData && RxMsg.Length >= 1) {

        joinedNwkData->StatusFormat = RxMsg.Payload[offset++];

        if (joinedNwkData->StatusFormat & LORAWAN_FORMAT_EXT_HCI_OUT_ACTIVE) {
            dataLen -= (0x01 + 0x05); // format + rx ch info
            joinedNwkData->OptionalInfoAvaiable = true;
        } else {
            dataLen -= 0x01; // format; only
            joinedNwkData->OptionalInfoAvaiable = false;
        }

        // get new device address retrieved from NWK server
        joinedNwkData->DeviceAddress = NTOH32(&RxMsg.Payload[offset]);
        offset += 0x04;


        // check if optional attributes are present
        if (offset < RxMsg.Length) {
            joinedNwkData->ChannelIndex  = (UINT8) RxMsg.Payload[offset++];
            joinedNwkData->DataRateIndex = (UINT8) RxMsg.Payload[offset++];
            joinedNwkData->RSSI          = (INT8)  RxMsg.Payload[offset++];
            joinedNwkData->SNR           = (INT8)  RxMsg.Payload[offset++];
            joinedNwkData->RxSlot        = (UINT8) RxMsg.Payload[offset++];
        }
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
/**
 * @brief Convert a received low level HCI-Msg to a high-level AckData structure
 *
 * This function should be used by the Rx Ack Indication Callback function
 * prior processing the received data message.
 *
 * @param   RxMsg       Reference to low-level HCI message.
 *                      @warning DO NOT MANIPULATE THESE VALUES !!!
 *
 * @param   ackData     Pointer to the buffer where to store the received data
 *
 * @retval true     if the conversion was successful
 */
bool WiMOD_SAP_LoRaWAN::convert(TWiMODLR_HCIMessage&        RxMsg,
                                TWiMODLORAWAN_RX_ACK_Data*  ackData)
{
    UINT8 offset = 0;

    INT16 dataLen = RxMsg.Length;

    if (ackData) {
        ackData->OptionalInfoAvaiable = false;
    }

    if (ackData && RxMsg.Length >= 1) {

        ackData->StatusFormat = RxMsg.Payload[offset++];

        if (ackData->StatusFormat & LORAWAN_FORMAT_EXT_HCI_OUT_ACTIVE) {
            dataLen -= (0x01 + 0x05); // format + rx ch info
            ackData->OptionalInfoAvaiable = true;
        } else {
            dataLen -= 0x01; // format; only
            ackData->OptionalInfoAvaiable = false;
        }

        // check if optional attributes are present
        if (offset < RxMsg.Length) {
            ackData->ChannelIndex  = (UINT8) RxMsg.Payload[offset++];
            ackData->DataRateIndex = (UINT8) RxMsg.Payload[offset++];
            ackData->RSSI          = (INT8)  RxMsg.Payload[offset++];
            ackData->SNR           = (INT8)  RxMsg.Payload[offset++];
            ackData->RxSlot        = (UINT8) RxMsg.Payload[offset++];
        }
        return true;
    }
    return false;
}


//-----------------------------------------------------------------------------
/**
 * @brief Convert a received low level HCI-Msg to a high-level NoDataIndication structure
 *
 * This function should be used by the NoData Indication Callback function
 * prior processing the received data message.
 *
 * @param   RxMsg       Reference to low-level HCI message.
 *                      @warning DO NOT MANIPULATE THESE VALUES !!!
 *
 * @param   info        Pointer to the buffer where to store the received data
 *
 * @retval true     if the conversion was successful
 */

bool WiMOD_SAP_LoRaWAN::convert(TWiMODLR_HCIMessage&        RxMsg,
								TWiMODLORAWAN_NoData_Data*  info)
{
    UINT8 offset = 0;

    if (info) {
    	info->ErrorCode = 0;
    }

    if (info && RxMsg.Length >= 1) {

    	info->StatusFormat = RxMsg.Payload[offset++];

        if (info->StatusFormat & LORAWAN_FORMAT_EXT_HCI_OUT_ACTIVE) {
        	info->OptionalInfoAvaiable = true;
        } else {
        	info->OptionalInfoAvaiable = false;
        }

        // check if optional attributes are present
        if (offset < RxMsg.Length) {
        	info->ErrorCode  = (UINT8) RxMsg.Payload[offset++];
        }
        return true;
    }
    return false;
}


//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the event "TX Join Indication"
 *
 * This registered callback is called when the specified event is called by
 * the WiMOD stack.
 *
 * @param   cb          pointer to a callback function that should be called
 *                      if the event occurs.
 */
void WiMOD_SAP_LoRaWAN::RegisterNoDataIndicationClient(TNoDataIndicationCallback cb)
{
    NoDataIndCallback =  cb;
}

//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the event "TX C-Data Indication"
 *
 * This registered callback is called when the specified event is called by
 * the WiMOD stack.
 *
 * @param   cb          pointer to a callback function that should be called
 *                      if the event occurs.
 */
void WiMOD_SAP_LoRaWAN::RegisterTxCDataIndicationClient(TTxCDataIndicationCallback cb)
{
    TxCDataIndCallback =  cb;
}

//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the event "TX U Data Indication"
 *
 * This registered callback is called when the specified event is called by
 * the WiMOD stack.
 *
 * @param   cb          pointer to a callback function that should be called
 *                      if the event occurs.
 */
void WiMOD_SAP_LoRaWAN::RegisterTxUDataIndicationClient(TTxUDataIndicationCallback cb)
{
    TxUDataIndCallback = cb;
}

//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the event "RX U-Data Indication"
 *
 * This registered callback is called when the specified event is called by
 * the WiMOD stack.
 *
 * @param   cb          pointer to a callback function that should be called
 *                      if the event occurs.
 */
void WiMOD_SAP_LoRaWAN::RegisterRxUDataIndicationClient(TRxUDataIndicationCallback cb)
{
    RxUDataIndCallback =  cb;
}

//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the event "RX C-Data Indication"
 *
 * This registered callback is called when the specified event is called by
 * the WiMOD stack.
 *
 * @param   cb          pointer to a callback function that should be called
 *                      if the event occurs.
 */
void WiMOD_SAP_LoRaWAN::RegisterRxCDataIndicationClient(TRxCDataIndicationCallback cb)
{
    RxCDataIndCallback =  cb;
}

//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the event "RX MAC Cmd Indication"
 *
 * This registered callback is called when the specified event is called by
 * the WiMOD stack.
 *
 * @param   cb          pointer to a callback function that should be called
 *                      if the event occurs.
 */
void WiMOD_SAP_LoRaWAN::RegisterRxMacCmdIndicationClient(TRxMacCmdIndicationCallback cb)
{
    RxMacCmdIndCallback =  cb;
}

//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the event "Joined Nwk Indication"
 *
 * This registered callback is called when the specified event is called by
 * the WiMOD stack.
 *
 * @param   cb          pointer to a callback function that should be called
 *                      if the event occurs.
 */
void WiMOD_SAP_LoRaWAN::RegisterJoinedNwkIndicationClient(TJoinedNwkIndicationCallback cb)
{
    JoinedNwkIndCallback = cb;
}

//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the event "RX ACK (data) Indication"
 *
 * This registered callback is called when the specified event is called by
 * the WiMOD stack.
 *
 * @param   cb          pointer to a callback function that should be called
 *                      if the event occurs.
 */
void WiMOD_SAP_LoRaWAN::RegisterRxAckIndicationClient(TRxAckIndicationCallback cb)
{
    RxAckIndCallback = cb;
}


//-----------------------------------------------------------------------------
/**
 * @brief Sets a new radio config parameter set of the WiMOD
 *
 *
 * @param data       pointer to data structure containing the new parameters
 *                   @see TWiMODLORAWAN_TX_Data for details
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_LoRaWAN::SetRadioStackConfig(TWiMODLORAWAN_RadioStackConfig* data,
        UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = 0;

    if ( data && statusRsp) {

        txPayload[offset++] = data->DataRateIndex;
        txPayload[offset++] = data->TXPowerLevel;
        txPayload[offset++] = data->Options;
        txPayload[offset++] = data->PowerSavingMode;
        txPayload[offset++] = data->Retransmissions;
        txPayload[offset++] = data->BandIndex;

        if (region == LoRaWAN_Region_US915) {
			txPayload[offset++] = data->SubBandMask1;
			txPayload[offset++] = data->SubBandMask2;
        }

        result = HciParser->SendHCIMessage(LORAWAN_SAP_ID,
                                           LORAWAN_MSG_SET_RSTACK_CONFIG_REQ,
                                           LORAWAN_MSG_SET_RSTACK_CONFIG_RSP,
                                           txPayload, offset);


        // clear RX field
        data->WrongParamErrCode = 0x00;

        if (result == WiMODLR_RESULT_OK) {
            const TWiMODLR_HCIMessage& rx = HciParser->GetRxMessage();

            offset = WiMODLR_HCI_RSP_STATUS_POS + 1;
            if (rx.Length > 1) {
                data->WrongParamErrCode = rx.Payload[offset++];
            }

            // copy response status
            *statusRsp = rx.Payload[WiMODLR_HCI_RSP_STATUS_POS];
       }
    } else {
        result = WiMODLR_RESULT_PAYLOAD_PTR_ERROR;
    }    return result;

}



//-----------------------------------------------------------------------------
/**
 * @brief Gets the current radio config parameter set of the WiMOD
 *
 *
 * @param data       pointer to data structure for storing the requested information
 *                   @see TWiMODLORAWAN_TX_Data for details
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_LoRaWAN::GetRadioStackConfig(TWiMODLORAWAN_RadioStackConfig* data,
        UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = WiMODLR_HCI_RSP_STATUS_POS + 1;

    if ( data && statusRsp) {
        result = HciParser->SendHCIMessage(LORAWAN_SAP_ID,
                                           LORAWAN_MSG_GET_RSTACK_CONFIG_REQ,
                                           LORAWAN_MSG_GET_RSTACK_CONFIG_RSP,
                                           txPayload, 0x00);

        if (result == WiMODLR_RESULT_OK) {
            const TWiMODLR_HCIMessage& rx = HciParser->GetRxMessage();

            data->DataRateIndex   		= rx.Payload[offset++];
            data->TXPowerLevel    		= rx.Payload[offset++];
            data->Options         		= rx.Payload[offset++];
            data->PowerSavingMode 		= rx.Payload[offset++];
            data->Retransmissions 		= rx.Payload[offset++];
            data->BandIndex       		= rx.Payload[offset++];
            data->HeaderMacCmdCapacity 	= rx.Payload[offset++];

            if (region == LoRaWAN_Region_US915) {
				if (rx.Length > offset) {
					data->SubBandMask1 = rx.Payload[offset++];
				}
				if (rx.Length > offset) {
					data->SubBandMask2 = rx.Payload[offset++];
				}
            }

            // copy response status
            *statusRsp = rx.Payload[WiMODLR_HCI_RSP_STATUS_POS];
       }
    } else {
        result = WiMODLR_RESULT_PAYLOAD_PTR_ERROR;
    }
    return result;

}

//-----------------------------------------------------------------------------
/**
 * @brief Deactivate the device (logical disconnect from lora network)
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_LoRaWAN::DeactivateDevice(UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;

    if ( statusRsp) {

        result = HciParser->SendHCIMessage(LORAWAN_SAP_ID,
                                           LORAWAN_MSG_DEACTIVATE_DEVICE_REQ,
                                           LORAWAN_MSG_DEACTIVATE_DEVICE_RSP,
                                           txPayload, 0x00);
        // copy response status
        if (result == WiMODLR_RESULT_OK) {
            *statusRsp = HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_STATUS_POS];
        }
    }
    return result;

}

//-----------------------------------------------------------------------------
/**
 * @brief Reset all internal settings to default values (incl. DevEUI)
 *
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_LoRaWAN::FactoryReset(UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;

    if ( statusRsp) {

        result = HciParser->SendHCIMessage(LORAWAN_SAP_ID,
                                           LORAWAN_MSG_FACTORY_RESET_REQ,
                                           LORAWAN_MSG_FACTORY_RESET_RSP,
                                           txPayload, 0x00);
        // copy response status
        if (result == WiMODLR_RESULT_OK) {
            *statusRsp = HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_STATUS_POS];
        }
    }
    return result;

}

//-----------------------------------------------------------------------------
/**
 * @brief Sets a new DeviceEUI (aka. IEEE-Address) to the WiMOD
 *
 * Note: Setting a new DeviceEUI is requires to switch to customer operation
 *       mode. In "normal" application mode, this command is locked.
 *
 * @param deviceEUI  pointer to data structure containing the new parameters
 *                   (Must be an pointer of a 64bit address)
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_LoRaWAN::SetDeviceEUI(const UINT8* deviceEUI,
        UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;

    if ( deviceEUI && statusRsp) {

        // copy 64 bit DeviceEUI information into tx buffer
        memcpy(txPayload, deviceEUI, WiMODLORAWAN_DEV_EUI_LEN);
        result = HciParser->SendHCIMessage(LORAWAN_SAP_ID,
                                           LORAWAN_MSG_SET_DEVICE_EUI_REQ,
                                           LORAWAN_MSG_SET_DEVICE_EUI_RSP,
                                           txPayload, WiMODLORAWAN_DEV_EUI_LEN);

        // copy response status
        if (result == WiMODLR_RESULT_OK) {
            *statusRsp = HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_STATUS_POS];
        }
    } else {
        result = WiMODLR_RESULT_PAYLOAD_PTR_ERROR;
    }
    return result;

}


//-----------------------------------------------------------------------------
/**
 * @brief Gets the  DeviceEUI (aka. IEEE-Address) of the WiMOD
 *
 *
 * @param deviceEUI  pointer for storing the received 64bit address
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_LoRaWAN::GetDeviceEUI(UINT8* deviceEUI,
        UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = WiMODLR_HCI_RSP_STATUS_POS + 1;

    if ( deviceEUI && statusRsp) {
        result = HciParser->SendHCIMessage(LORAWAN_SAP_ID,
                                           LORAWAN_MSG_GET_DEVICE_EUI_REQ,
                                           LORAWAN_MSG_GET_DEVICE_EUI_RSP,
                                           txPayload, 0x00);

        if (result == WiMODLR_RESULT_OK) {
            const TWiMODLR_HCIMessage& rx = HciParser->GetRxMessage();

            // just copy the 8 EUI bytes as octet stream
            memcpy(deviceEUI, &rx.Payload[offset], WiMODLORAWAN_DEV_EUI_LEN);
            offset += WiMODLORAWAN_DEV_EUI_LEN;

            // copy response status
            *statusRsp = rx.Payload[WiMODLR_HCI_RSP_STATUS_POS];
       }
    } else {
        result = WiMODLR_RESULT_PAYLOAD_PTR_ERROR;
    }
    return result;

}

//-----------------------------------------------------------------------------
/**
 * @brief Gets the  current status of the network "connection"
 *
 *
 * @param nwkStatus  pointer for storing the requested information
 *                   @see LORAWAN_NWK_STATUS defines; e.g.:
 *                     - LORAWAN_NWK_STATUS_INACTIVE,
 *                     - LORAWAN_NWK_STATUS_ACTIVE_ABP,
 *                     - LORAWAN_NWK_STATUS_ACTIVE_OTAA,
 *                     - LORAWAN_NWK_STATUS_JOINING_OTAA
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */

// pre V1.14 implementation
//TWiMODLRResultCodes WiMOD_SAP_LoRaWAN::GetNwkStatus(UINT8* nwkStatus,
//        UINT8* statusRsp)
//{
//    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
//    UINT8              offset = WiMODLR_HCI_RSP_STATUS_POS + 1;
//
//    if ( nwkStatus && statusRsp) {
//        result = HciParser->SendHCIMessage(LORAWAN_SAP_ID,
//                                           LORAWAN_MSG_GET_NWK_STATUS_REQ,
//                                           LORAWAN_MSG_GET_NWK_STATUS_RSP,
//                                           txPayload, 0x00);
//
//        if (result == WiMODLR_RESULT_OK) {
//            const TWiMODLR_HCIMessage& rx = HciParser->GetRxMessage();
//
//            *nwkStatus = rx.Payload[offset++];
//
//            // copy response status
//            *statusRsp = rx.Payload[WiMODLR_HCI_RSP_STATUS_POS];
//       }
//    } else {
//        result = WiMODLR_RESULT_PAYLOAD_PTR_ERROR;
//    }
//    return result;
//}

// new implemenation for V1.14
TWiMODLRResultCodes WiMOD_SAP_LoRaWAN::GetNwkStatus(TWiMODLORAWAN_NwkStatus_Data* nwkStatus,
        UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = WiMODLR_HCI_RSP_STATUS_POS + 1;

    if ( nwkStatus && statusRsp) {
        result = HciParser->SendHCIMessage(LORAWAN_SAP_ID,
                                           LORAWAN_MSG_GET_NWK_STATUS_REQ,
                                           LORAWAN_MSG_GET_NWK_STATUS_RSP,
                                           txPayload, 0x00);

        if (result == WiMODLR_RESULT_OK) {
            const TWiMODLR_HCIMessage& rx = HciParser->GetRxMessage();

            nwkStatus->NetworkStatus = rx.Payload[offset++];					// @see TLoRaWAN_NwkStatus for meaning

            // set dummy values
            nwkStatus->DeviceAddress  = 0;
            nwkStatus->DataRateIndex  = 0;
            nwkStatus->PowerLevel     = 0;
            nwkStatus->MaxPayloadSize = 0;
            // check if optional fields are present
            if (rx.Length > offset) {
            	nwkStatus->DeviceAddress = NTOH32(&rx.Payload[offset]);
            	offset += 0x04;
            	nwkStatus->DataRateIndex = rx.Payload[offset++];
            	nwkStatus->PowerLevel    = rx.Payload[offset++];
            	nwkStatus->MaxPayloadSize= rx.Payload[offset++];
            }


            // copy response status
            *statusRsp = rx.Payload[WiMODLR_HCI_RSP_STATUS_POS];
       }
    } else {
        result = WiMODLR_RESULT_PAYLOAD_PTR_ERROR;
    }
    return result;
}


//-----------------------------------------------------------------------------
/**
 * @brief Send a MAC command to the server; expert level only
 *
 * @param cmd      pointer containing the MAC command and parameters
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_LoRaWAN::SendMacCmd(const TWiMODLORAWAN_MacCmd* cmd,
        UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = 0;
    UINT8              i      = 0;

    if ( cmd && statusRsp && (cmd->Length <= WiMODLORAWAN_MAC_CMD_PAYLOAD_LENGTH)) {
        txPayload[offset++] = cmd->DataServiceType;
        txPayload[offset++] = cmd->MacCmdID;
        // copy payload / parameters of MAC command
        while (i < cmd->Length) {
            txPayload[offset++] = cmd->Payload[i];
            i++;
        }

        result = HciParser->SendHCIMessage(LORAWAN_SAP_ID,
                                           LORAWAN_MSG_SEND_MAC_CMD_REQ,
                                           LORAWAN_MSG_SEND_MAC_CMD_RSP,
                                           txPayload, offset);

        if (result == WiMODLR_RESULT_OK) {
            // copy response status
            *statusRsp = HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_STATUS_POS];
        }
    } else {
        if (cmd == NULL || statusRsp == NULL) {
            result = WiMODLR_RESULT_PAYLOAD_PTR_ERROR;
        } else {
            result = WiMODLR_RESULT_PAYLOAD_LENGTH_ERROR;
        }
    }
    return result;
}


//-----------------------------------------------------------------------------
/**
 * @brief Setup a custom config for tx power settings; expert level only
 *
 * @param rfGain  new rfGain value for tx power settings
 *
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_LoRaWAN::SetCustomConfig(const INT8 rfGain,
        UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = 0;

    if ( statusRsp) {
        txPayload[offset++] = (UINT8) rfGain;

        result = HciParser->SendHCIMessage(LORAWAN_SAP_ID,
                                           LORAWAN_MSG_SET_CUSTOM_CFG_REQ,
                                           LORAWAN_MSG_SET_CUSTOM_CFG_RSP,
                                           txPayload, offset);

        if (result == WiMODLR_RESULT_OK) {
            // copy response status
            *statusRsp = HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_STATUS_POS];
        }
    }
    return result;

}

//-----------------------------------------------------------------------------
/**
 * @brief Get the current offet for tx power level; expert level only
 *
 * @param rfGain  pointer to store the rfGain info  for tx power settings
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes
WiMOD_SAP_LoRaWAN::GetCustomConfig(INT8* rfGain, UINT8* statusRsp) {
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = WiMODLR_HCI_RSP_STATUS_POS + 1;

    if ( rfGain && statusRsp) {
        result = HciParser->SendHCIMessage(LORAWAN_SAP_ID,
                                           LORAWAN_MSG_GET_CUSTOM_CFG_REQ,
                                           LORAWAN_MSG_GET_CUSTOM_CFG_RSP,
                                           txPayload, 0x00);

        if (result == WiMODLR_RESULT_OK) {
            const TWiMODLR_HCIMessage& rx = HciParser->GetRxMessage();

            *rfGain = (INT8) rx.Payload[offset++];

            // copy response status
            *statusRsp = rx.Payload[WiMODLR_HCI_RSP_STATUS_POS];
       }
    } else {
        result = WiMODLR_RESULT_PAYLOAD_PTR_ERROR;
    }
    return result;
}

//-----------------------------------------------------------------------------
/**
 * @brief Get the supported bands of this firmware
 *
 * @param supportedBands  pointer  to store area for result
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */

TWiMODLRResultCodes
WiMOD_SAP_LoRaWAN::GetSupportedBands(TWiMODLORAWAN_SupportedBands* supportedBands, UINT8* statusRsp) {
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = WiMODLR_HCI_RSP_STATUS_POS + 1;

    if ( supportedBands && statusRsp) {
        result = HciParser->SendHCIMessage(LORAWAN_SAP_ID,
                                           LORAWAN_MSG_GET_SUPPORTED_BANDS_REQ,
										   LORAWAN_MSG_GET_SUPPORTED_BANDS_RSP,
                                           txPayload, 0x00);

        supportedBands->NumOfEntries = 0;

        if (result == WiMODLR_RESULT_OK) {
            const TWiMODLR_HCIMessage& rx = HciParser->GetRxMessage();

            // copy response status
            *statusRsp = rx.Payload[WiMODLR_HCI_RSP_STATUS_POS];

            // get the band infos
            while (offset < rx.Length  && (supportedBands->NumOfEntries < (WiMODLORAWAN_APP_PAYLOAD_LEN / 2) ) ) {
            	supportedBands->BandIndex[supportedBands->NumOfEntries] = rx.Payload[offset++];
            	supportedBands->MaxEIRP[supportedBands->NumOfEntries] = rx.Payload[offset++];
            	supportedBands->NumOfEntries++;
            }

       }
    } else {
        result = WiMODLR_RESULT_PAYLOAD_PTR_ERROR;
    }
    return result;
}

//-----------------------------------------------------------------------------
/**
 * @brief Get the configured TxPowerLimit settings
 *
 * @param txPwrLimitCfg  pointer to store area for result
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */

TWiMODLRResultCodes
WiMOD_SAP_LoRaWAN::GetTxPowerLimitConfig(TWiMODLORAWAN_TxPwrLimitConfig* txPwrLimitCfg, UINT8* statusRsp) {
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = WiMODLR_HCI_RSP_STATUS_POS + 1;

    if (region != LoRaWAN_Region_EU868) {
    	return WiMODLR_RESULT_NO_RESPONSE;
    }

    if ( txPwrLimitCfg && statusRsp) {
        result = HciParser->SendHCIMessage(LORAWAN_SAP_ID,
        								   LORAWAN_MSG_GET_TXPOWER_LIMIT_CONFIG_REQ,
										   LORAWAN_MSG_GET_TXPOWER_LIMIT_CONFIG_RSP,
                                           txPayload, 0x00);

        txPwrLimitCfg->NumOfEntries = 0;

        if (result == WiMODLR_RESULT_OK) {
            const TWiMODLR_HCIMessage& rx = HciParser->GetRxMessage();

            // copy response status
            *statusRsp = rx.Payload[WiMODLR_HCI_RSP_STATUS_POS];

            // get the band infos
            while (offset < rx.Length && (txPwrLimitCfg->NumOfEntries < (WiMODLORAWAN_APP_PAYLOAD_LEN / 3) )) {
            	txPwrLimitCfg->SubBandIndex[txPwrLimitCfg->NumOfEntries] = rx.Payload[offset++];
            	txPwrLimitCfg->TxPwrLimitFlag[txPwrLimitCfg->NumOfEntries] = rx.Payload[offset++];
            	txPwrLimitCfg->TxPwrLimitValue[txPwrLimitCfg->NumOfEntries] = rx.Payload[offset++];
            	txPwrLimitCfg->NumOfEntries++;
            }

       }
    } else {
        result = WiMODLR_RESULT_PAYLOAD_PTR_ERROR;
    }
    return result;
}

//-----------------------------------------------------------------------------
/**
 * @brief Set a SINGLE TxPowerLimit entry
 *
 * @param txPwrLimitCfg  reference that holds the data to write
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */

TWiMODLRResultCodes
WiMOD_SAP_LoRaWAN::SetTxPowerLimitConfig(TWiMODLORAWAN_TxPwrLimitConfig& txPwrLimitCfg, UINT8* statusRsp) {
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = 0;

    if (region != LoRaWAN_Region_EU868) {
    	return WiMODLR_RESULT_NO_RESPONSE;
    }

    if (statusRsp) {
    	/*
    	 * note: exactly one data set !!!!
    	 *
    	 */
        txPwrLimitCfg.NumOfEntries = 1;
        txPayload[offset++] = (UINT8) txPwrLimitCfg.SubBandIndex[0];
        txPayload[offset++] = (UINT8) txPwrLimitCfg.TxPwrLimitFlag[0];
        txPayload[offset++] = (UINT8) txPwrLimitCfg.TxPwrLimitValue[0];


        result = HciParser->SendHCIMessage(LORAWAN_SAP_ID,
        								   LORAWAN_MSG_SET_TXPOWER_LIMIT_CONFIG_REQ,
										   LORAWAN_MSG_SET_TXPOWER_LIMIT_CONFIG_RSP,
                                           txPayload, offset);


        offset = WiMODLR_HCI_RSP_STATUS_POS + 1;

        if (result == WiMODLR_RESULT_OK) {
            const TWiMODLR_HCIMessage& rx = HciParser->GetRxMessage();

            // copy response status
            *statusRsp = rx.Payload[WiMODLR_HCI_RSP_STATUS_POS];

            // try to get an optional error code for wrong parameters
            txPwrLimitCfg.WrongParamErrCode = 0x00;
            if (rx.Length >= offset) {
            	txPwrLimitCfg.WrongParamErrCode =rx.Payload[offset++];
            }
       }
    } else {
        result = WiMODLR_RESULT_PAYLOAD_PTR_ERROR;
    }
    return result;
}


//-----------------------------------------------------------------------------
/**
 * @brief Get the configured LinkAdrReq config setting
 *
 * @param linkAdrReqCfg  pointer to store area for result
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */

TWiMODLRResultCodes
WiMOD_SAP_LoRaWAN::GetLinkAdrReqConfig(TWiMODLORAWAN_LinkAdrReqConfig* linkAdrReqCfg, UINT8* statusRsp) {
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = WiMODLR_HCI_RSP_STATUS_POS + 1;

    if ( linkAdrReqCfg && statusRsp) {
        result = HciParser->SendHCIMessage(LORAWAN_SAP_ID,
        								   LORAWAN_MSG_GET_LINKADRREQ_CONFIG_REQ,
										   LORAWAN_MSG_GET_LINKADRREQ_CONFIG_RSP,
                                           txPayload, 0x00);


        if (result == WiMODLR_RESULT_OK) {
            const TWiMODLR_HCIMessage& rx = HciParser->GetRxMessage();

            // copy response status
            *statusRsp = rx.Payload[WiMODLR_HCI_RSP_STATUS_POS];

            // get the configured option
            *linkAdrReqCfg = (TWiMODLORAWAN_LinkAdrReqConfig) rx.Payload[offset++];

       }
    } else {
        result = WiMODLR_RESULT_PAYLOAD_PTR_ERROR;
    }
    return result;
}

//-----------------------------------------------------------------------------
/**
 * @brief Set LinkAdrReq config setting
 *
 * @param linkAdrReqCfg  reference to the data to set
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */

TWiMODLRResultCodes
WiMOD_SAP_LoRaWAN::SetLinkAdrReqConfig(TWiMODLORAWAN_LinkAdrReqConfig& linkAdrReqCfg, UINT8* statusRsp) {
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = 0;

    if (statusRsp) {
        txPayload[offset++] = (UINT8) linkAdrReqCfg;


        result = HciParser->SendHCIMessage(LORAWAN_SAP_ID,
											LORAWAN_MSG_SET_LINKADRREQ_CONFIG_REQ,
											LORAWAN_MSG_SET_LINKADRREQ_CONFIG_RSP,
                                           txPayload, offset);


        offset = WiMODLR_HCI_RSP_STATUS_POS + 1;

        if (result == WiMODLR_RESULT_OK) {
            const TWiMODLR_HCIMessage& rx = HciParser->GetRxMessage();

            // copy response status
            *statusRsp = rx.Payload[WiMODLR_HCI_RSP_STATUS_POS];

       }
    } else {
        result = WiMODLR_RESULT_PAYLOAD_PTR_ERROR;
    }
    return result;
}

//-----------------------------------------------------------------------------
/**
 * @brief Set Battery Level Status
 *
 * @param battStatus  status value; see documentation for details.
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */

TWiMODLRResultCodes
WiMOD_SAP_LoRaWAN::SetBatteryLevelStatus(UINT8 battStatus, UINT8* statusRsp) {
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = 0;

    if (statusRsp) {
        txPayload[offset++] = (UINT8) battStatus;


        result = HciParser->SendHCIMessage(LORAWAN_SAP_ID,
                                           LORAWAN_MSG_SET_BATTERY_LEVEL_REQ,
                                           LORAWAN_MSG_SET_BATTERY_LEVEL_RSP,
                                           txPayload, offset);


        offset = WiMODLR_HCI_RSP_STATUS_POS + 1;

        if (result == WiMODLR_RESULT_OK) {
            const TWiMODLR_HCIMessage& rx = HciParser->GetRxMessage();

            // copy response status
            *statusRsp = rx.Payload[WiMODLR_HCI_RSP_STATUS_POS];

       }
    } else {
        result = WiMODLR_RESULT_PAYLOAD_PTR_ERROR;
    }
    return result;

}

//------------------------------------------------------------------------------
//
// Section protected functions
//
//------------------------------------------------------------------------------

/**
 * @internal
 *
 * @brief Dispatch messages from the WiMOD (aka indications)
 *
 * @param rxMsg reference to the complete received HCI message; DO NOT MODIFIY it!
 *
 * @endinternal
 */
void
WiMOD_SAP_LoRaWAN::DispatchLoRaWANMessage(TWiMODLR_HCIMessage& rxMsg) {
    switch (rxMsg.MsgID)
    {
        case LORAWAN_MSG_JOIN_NETWORK_TX_IND:
            if (JoinTxIndCallback) {
                JoinTxIndCallback(rxMsg);
            }
            break;
        case LORAWAN_MSG_RECV_NO_DATA_IND:
            if (NoDataIndCallback) {
                NoDataIndCallback();
            }
            break;
        case LORAWAN_MSG_SEND_CDATA_TX_IND:
            if (TxCDataIndCallback) {
                TxCDataIndCallback(rxMsg);
            }
            break;
        case LORAWAN_MSG_SEND_UDATA_TX_IND:
            if (TxUDataIndCallback) {
                TxUDataIndCallback(rxMsg);
            }
            break;
        case LORAWAN_MSG_RECV_UDATA_IND:
            if (RxUDataIndCallback) {
                RxUDataIndCallback(rxMsg);
            }
            break;
        case LORAWAN_MSG_RECV_CDATA_IND:
            if (RxCDataIndCallback) {
                RxCDataIndCallback(rxMsg);
            }
            break;
        case LORAWAN_MSG_RECV_MAC_CMD_IND:
            if (RxMacCmdIndCallback) {
                RxMacCmdIndCallback(rxMsg);
            }
            break;
        case LORAWAN_MSG_JOIN_NETWORK_IND:
            if (JoinedNwkIndCallback) {
                JoinedNwkIndCallback(rxMsg);
            }
            break;
        case LORAWAN_MSG_RECV_ACK_IND:
            if (RxAckIndCallback) {
                RxAckIndCallback(rxMsg);
            }
            break;
        default:

//            if (StackErrorClientCB) {
//                StackErrorClientCB(WIMOD_STACK_ERR_UNKNOWN_RX_CMD_ID);
//            }
            break;
    }
    return;
}



//------------------------------------------------------------------------------
//
// Section private functions
//
//------------------------------------------------------------------------------


