//------------------------------------------------------------------------------
//! @file WiMOD_SAP_RadioLink.cpp
//! @ingroup WiMOD_SAP_RadioLink
//! <!------------------------------------------------------------------------->
//! @brief Implementation of the commands of the RadioLink SericeAccessPoint
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
 * THIS IS AN EXAMPLE IMPLEMENTATION ACCORDING THE THE HCI SPEC: V1.10
 * FOR FIRMWARE: LR-BASE
 */


//------------------------------------------------------------------------------
//
// Section Includes Files
//
//------------------------------------------------------------------------------

#include "WiMOD_SAP_RadioLink.h"
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
WiMOD_SAP_RadioLink::WiMOD_SAP_RadioLink(TWiMODLRHCI* hci, UINT8* buffer, UINT16 bufferSize) :
    HciParser(hci)
{
    UDataRxIndCallback      = NULL;
    UDataTxIndCallback      = NULL;
    RawDataRxIndCallback    = NULL;
    CDataRxIndCallback      = NULL;
    CDataTxIndCallback      = NULL;
    AckRxDataIndCallback    = NULL;
    AckRxTimeoutIndCallback = NULL;
    AckTxIndCallack         = NULL;

    txPayload = buffer;
    txPayloadSize = bufferSize;
}

//-----------------------------------------------------------------------------
/**
 * @brief Destructor
 *
 */
WiMOD_SAP_RadioLink::~WiMOD_SAP_RadioLink(void)
{

}
//-----------------------------------------------------------------------------
/**
 * @brief Tries to send transmit U-Data to peer module via RF link
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param txMsg      Data structure containing the TX-data and options.
 *                   @see TWiMODLR_RadioLink_Msg for details
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */

TWiMODLRResultCodes WiMOD_SAP_RadioLink::SendUData(const TWiMODLR_RadioLink_Msg* txMsg,
        UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8 offset = 0;

    if (statusRsp && txMsg && (txMsg->Length <= WIMOD_RADIOLINK_PAYLOAD_LEN)
            && (txPayloadSize >= (1+2+ txMsg->Length))) {
        txPayload[offset++] = txMsg->DestinationGroupAddress;
        HTON16(&txPayload[offset], txMsg->DestinationDeviceAddress);
        offset += 0x02;
        memcpy(&txPayload[offset], txMsg->Payload, txMsg->Length);
        offset += txMsg->Length;

        result = HciParser->SendHCIMessage(RADIOLINK_SAP_ID,
                                           RADIOLINK_MSG_SEND_U_DATA_REQ,
                                           RADIOLINK_MSG_SEND_U_DATA_RSP,
                                           txPayload, offset);

        if (result == WiMODLR_RESULT_OK) {
            *statusRsp = HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_STATUS_POS];
        }
    } else {
        result = WiMODLR_RESULT_PAYLOAD_PTR_ERROR;
        if (txMsg && txMsg->Length > WIMOD_RADIOLINK_PAYLOAD_LEN) {
            result = WiMODLR_RESULT_PAYLOAD_LENGTH_ERROR;
        }
    }
    return result;
}

//-----------------------------------------------------------------------------
/**
 * @brief Convert a received low level HCI-Msg to a high-level RadioLink-Msg
 *
 * This function should be used by the RxUData / RxCData callback functions
 * prior processing the received data message.
 *
 * @param   RxMsg       Reference to low-level HCI message.
 *                      @warning DO NOT MANIPULATE THESE VALUES !!!
 *
 * @param   radioLinkMsg Pointer to the buffer where to store the received data
 *
 * @retval true     if the conversion was successful
 */

bool WiMOD_SAP_RadioLink::convert(TWiMODLR_HCIMessage& RxMsg,
        TWiMODLR_RadioLink_Msg* radioLinkMsg)
{
    UINT8 offset = 0;

    INT16 dataLen = RxMsg.Length;
    INT16 i;

    if (radioLinkMsg) {
        radioLinkMsg->Length = 0;
        radioLinkMsg->OptionalInfoAvaiable = false;
    }

    if (radioLinkMsg && RxMsg.Length >= 1) {

        radioLinkMsg->StatusFormat = RxMsg.Payload[offset++];

        if (radioLinkMsg->StatusFormat & RADIOLINK_FORMAT_EXTENDED_OUTPUT) {
            dataLen -= (0x01 + 0x07); // format + rx info
            radioLinkMsg->OptionalInfoAvaiable = true;
        } else {
            dataLen -= 0x01; // format; only
            radioLinkMsg->OptionalInfoAvaiable = false;
        }

        // dest. group adr
        radioLinkMsg->DestinationGroupAddress = RxMsg.Payload[offset++];
        dataLen--;

        // dest. device adr
        radioLinkMsg->DestinationDeviceAddress = NTOH16(&RxMsg.Payload[offset]);
        offset += 0x02;
        dataLen -= 0x02;

        // src. group adr
        radioLinkMsg->SourceGroupAddress = RxMsg.Payload[offset++];
        dataLen--;

        // src. device adr
        radioLinkMsg->SourceDeviceAddress = NTOH16(&RxMsg.Payload[offset]);
        offset += 0x02;
        dataLen -= 0x02;


        // copy payload field
        // do not use memcpy here because of potential negative dataLen
        for (i = 0; i < dataLen; i++) {
            radioLinkMsg->Payload[i] = RxMsg.Payload[offset++];
            radioLinkMsg->Length++;
        }

        // check if optional attributes are present
        if (offset < RxMsg.Length) {
        	if (radioLinkMsg->StatusFormat & RADIOLINK_FORMAT_ENCRYPTED_DATA) {
        		radioLinkMsg->MIC = (INT16) NTOH16(&RxMsg.Payload[offset]);
        		offset += 0x02;
        	}
            radioLinkMsg->RSSI   = (INT16) NTOH16(&RxMsg.Payload[offset]);
            offset += 0x02;
            radioLinkMsg->SNR    = (INT8)   RxMsg.Payload[offset++];
            radioLinkMsg->RxTime = (UINT32) NTOH32(&RxMsg.Payload[offset]);
            offset += 0x04;
        }
        return true;
    }
    return false;
}


//-----------------------------------------------------------------------------
/**
 * @brief Tries to send transmit C-Data to peer module via RF link
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param txMsg      Data structure containing the TX-data and options.
 *                   @see TWiMODLR_RadioLink_Msg for details
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_RadioLink::SendCData(const TWiMODLR_RadioLink_Msg* txMsg,
        UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8 offset = 0;

    if (statusRsp && txMsg && (txMsg->Length <= WIMOD_RADIOLINK_PAYLOAD_LEN)
            && (txPayloadSize >= (1+2+txMsg->Length))) {
        txPayload[offset++] = txMsg->DestinationGroupAddress;
        HTON16(&txPayload[offset], txMsg->DestinationDeviceAddress);
        offset += 0x02;
        memcpy(&txPayload[offset], txMsg->Payload, txMsg->Length);
        offset += txMsg->Length;

        result = HciParser->SendHCIMessage(RADIOLINK_SAP_ID,
                                           RADIOLINK_MSG_SEND_C_DATA_REQ,
                                           RADIOLINK_MSG_SEND_C_DATA_RSP,
                                           txPayload, offset);

        if (result == WiMODLR_RESULT_OK) {
            *statusRsp = HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_STATUS_POS];
        }
    } else {
        result = WiMODLR_RESULT_PAYLOAD_PTR_ERROR;
        if (txMsg && txMsg->Length > WIMOD_RADIOLINK_PAYLOAD_LEN) {
            result = WiMODLR_RESULT_PAYLOAD_LENGTH_ERROR;
        }
    }
    return result;
}

//-----------------------------------------------------------------------------
/**
 * @brief Convert a received low level HCI-Msg to a high-level CDataTx Info
 *
 * This function should be used by the Tx-C-Data Indication Callback functions
 * prior processing the received data message.
 *
 * @param   RxMsg       Reference to low-level HCI message.
 *                      @warning DO NOT MANIPULATE THESE VALUES !!!
 *
 * @param   cDataTxInfo Pointer to the buffer where to store the received data
 *
 * @retval true     if the conversion was successful
 */
bool WiMOD_SAP_RadioLink::convert(TWiMODLR_HCIMessage& RxMsg,
        TWiMODLR_RadioLink_CdataInd* cDataTxInfo)
{
    UINT8 offset = 0;

    if (cDataTxInfo && RxMsg.Length >= 3) {

        cDataTxInfo->Status = RxMsg.Payload[offset++];
        cDataTxInfo->TxEventCounter = NTOH16(&RxMsg.Payload[offset]);
        offset += 0x02;

        cDataTxInfo->AirTime = 0;
        if (RxMsg.Length >= 7) {
			cDataTxInfo->AirTime = NTOH32(&RxMsg.Payload[offset]);
			offset += 0x04;
        }
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
/**
 * @brief Convert a received low level HCI-Msg to a high-level UDataTx Info
 *
 * This function should be used by the Tx-U-Data Indication Callback functions
 * prior processing the received data message.
 *
 * @param   RxMsg       Reference to low-level HCI message.
 *                      @warning DO NOT MANIPULATE THESE VALUES !!!
 *
 * @param   uDataTxInfo Pointer to the buffer where to store the received data
 *
 * @retval true     if the conversion was successful
 */
bool WiMOD_SAP_RadioLink::convert(TWiMODLR_HCIMessage& RxMsg,
        TWiMODLR_RadioLink_UdataInd* uDataTxInfo)
{
    UINT8 offset = 0;

    if (uDataTxInfo && RxMsg.Length >= 3) {

        uDataTxInfo->Status = RxMsg.Payload[offset++];
        uDataTxInfo->TxEventCounter = NTOH16(&RxMsg.Payload[offset]);
        offset += 0x02;

        uDataTxInfo->AirTime = 0;
        if (RxMsg.Length >= 7) {
			uDataTxInfo->AirTime = NTOH32(&RxMsg.Payload[offset]);
			offset += 0x04;
        }
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
/**
 * @brief Convert a received low level HCI-Msg to a high-level AckTxInd Info
 *
 * This function should be used by the AckTx Indication Callback functions
 * prior processing the received data message.
 *
 * @param   RxMsg       Reference to low-level HCI message.
 *                      @warning DO NOT MANIPULATE THESE VALUES !!!
 *
 * @param   txAckIndInfo Pointer to the buffer where to store the received data
 *
 * @retval true     if the conversion was successful
 */
bool WiMOD_SAP_RadioLink::convert(TWiMODLR_HCIMessage& RxMsg,
        TWiMODLR_AckTxInd_Msg* txAckIndInfo)
{
    UINT8 offset = 0;

    if (txAckIndInfo && RxMsg.Length >= 1) {

        txAckIndInfo->Status = RxMsg.Payload[offset++];

        txAckIndInfo->TxEventCounter = 0;
        txAckIndInfo->AirTime = 0;
        if (RxMsg.Length >= 7) {
            txAckIndInfo->TxEventCounter = NTOH16(&RxMsg.Payload[offset]);
            offset+= 0x02;
            txAckIndInfo->AirTime = NTOH32(&RxMsg.Payload[offset]);
            offset+= 0x04;
        }
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
/**
 * @brief Sets additional user payload for the next ack frame to send
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param txMsg     Pointer to data structure containing the payload data.
 *                  note: Max ack payload size is 8 byte!
 *                  @see TWiMODLR_RadioLink_Msg for details
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_RadioLink::SetAckData(const TWiMODLR_RadioLink_Msg* txMsg,
        UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8 offset = 0;

    if (statusRsp && txMsg && (txMsg->Length <= WIMOD_RADIOLINK_ACK_DATA_LEN)
            && (txPayloadSize >= (1+3+txMsg->Length))) {
        txPayload[offset++] = txMsg->DestinationGroupAddress;
        HTON16(&txPayload[offset], txMsg->DestinationDeviceAddress);
        offset += 0x02;

        memcpy(&txPayload[offset], txMsg->Payload, txMsg->Length);
        offset += txMsg->Length;

        result = HciParser->SendHCIMessage(RADIOLINK_SAP_ID,
                                           RADIOLINK_MSG_SET_ACK_DATA_REQ,
                                           RADIOLINK_MSG_SET_ACK_DATA_RSP,
                                           txPayload, offset);

        if (result == WiMODLR_RESULT_OK) {
            *statusRsp = HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_STATUS_POS];
        }
    } else {
        result = WiMODLR_RESULT_PAYLOAD_PTR_ERROR;
        if (txMsg && txMsg->Length > WIMOD_RADIOLINK_ACK_DATA_LEN) {
            result = WiMODLR_RESULT_PAYLOAD_LENGTH_ERROR;
        }
    }
    return result;
}

//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the RX U-Data Indication - optional -
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param cb        Pointer a callback function
 *                  @see TRadioLinkUDataRxIndicationCallback for details
 *
 * @note: the callback function should call the corresponding convert in
 *        order to decode the message properly
 */
void WiMOD_SAP_RadioLink::RegisterUDataRxClient(TRadioLinkUDataRxIndicationCallback cb)
{
    UDataRxIndCallback = cb;
}
//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the TX U-Data Indication - optional -
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param cb        Pointer a callback function
 *                  @see TRadioLinkUDataTxIndicationCallback for details
 */
void WiMOD_SAP_RadioLink::RegisterUDataTxClient(TRadioLinkUDataTxIndicationCallback cb)
{
    UDataTxIndCallback = cb;
}
//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the RX Raw-Data Indication - optional -
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param cb        Pointer a callback function
 *                  @see TRadioLinkRawDataRxIndicationCallback for details
 *
 * @note: the callback function should call the corresponding convert in
 *        order to decode the message properly
 */
void WiMOD_SAP_RadioLink::RegisterRawDataRxClient(TRadioLinkRawDataRxIndicationCallback cb)
{
    RawDataRxIndCallback = cb;
}
//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the RX C-Data Indication - optional -
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param cb        Pointer a callback function
 *                  @see TRadioLinkCDataRxIndicationCallback for details
 *
 * @note: the callback function should call the corresponding convert in
 *        order to decode the message properly
 */
void WiMOD_SAP_RadioLink::RegisterCDataRxClient(TRadioLinkCDataRxIndicationCallback cb)
{
    CDataRxIndCallback = cb;
}
//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the TX C-Data Indication - optional -
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param cb        Pointer a callback function
 *                  @see TRadioLinkCDataTxIndicationCallback for details
 *
 * @note: the callback function should call the corresponding convert in
 *        order to decode the message properly
 */
void WiMOD_SAP_RadioLink::RegisterCDataTxClient(TRadioLinkCDataTxIndicationCallback cb)
{
    CDataTxIndCallback = cb;
}
//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the RX Ack (+data) Indication - optional -
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param cb        Pointer a callback function
 *                  @see TRadioLinkAckRxIndicationCallback for details
 *
 * @note: the callback function should call the corresponding convert in
 *        order to decode the message properly
 */
void WiMOD_SAP_RadioLink::RegisterAckRxClient(TRadioLinkAckRxIndicationCallback cb)
{
    AckRxDataIndCallback = cb;
}
//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the Ack Timeout Indication - optional -
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param cb        Pointer a callback function
 *                  @see TRadioLinkAckRxTimeoutIndicationCallback for details
 *
 */
void WiMOD_SAP_RadioLink::RegisterAckRxTimeoutClient(TRadioLinkAckRxTimeoutIndicationCallback cb)
{
    AckRxTimeoutIndCallback = cb;
}
//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the TX Ack Indication - optional -
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param cb        Pointer a callback function
 *                  @see TRadioLinkAckTxIndicationCallback for details
 *
 */
void WiMOD_SAP_RadioLink::RegisterAckTxCallback(TRadioLinkAckTxIndicationCallback cb)
{
    AckTxIndCallack = cb;
}


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
WiMOD_SAP_RadioLink::DispatchRadioLinkMessage(TWiMODLR_HCIMessage& rxMsg) {
    switch (rxMsg.MsgID)
    {
        case RADIOLINK_MSG_U_DATA_RX_IND:
            if (UDataRxIndCallback) {
                UDataRxIndCallback(rxMsg);
            }
            break;
        case RADIOLINK_MSG_U_DATA_TX_IND:
            if (UDataTxIndCallback) {
                UDataTxIndCallback(rxMsg);          // spec V 1.10 added paramter
            }
            break;
        case RADIOLINK_MSG_RAW_DATA_RX_IND:
            if (RawDataRxIndCallback) {
                RawDataRxIndCallback(rxMsg);
            }
            break;
        case RADIOLINK_MSG_C_DATA_RX_IND:
            if (CDataRxIndCallback) {
                CDataRxIndCallback(rxMsg);
            }
            break;
        case RADIOLINK_MSG_C_DATA_TX_IND:
            if (CDataTxIndCallback) {
                CDataTxIndCallback(rxMsg);
            }
            break;
        case RADIOLINK_MSG_ACK_RX_IND:
            if (AckRxDataIndCallback) {
                AckRxDataIndCallback(rxMsg);
            }
            break;
        case RADIOLINK_MSG_ACK_TIMEOUT_IND:
            if (AckRxTimeoutIndCallback) {
                AckRxTimeoutIndCallback();
            }
            break;
        case RADIOLINK_MSG_ACK_TX_IND:
            if (AckTxIndCallack) {
                AckTxIndCallack();
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
// Section protected functions
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//
// Section private functions
//
//------------------------------------------------------------------------------


