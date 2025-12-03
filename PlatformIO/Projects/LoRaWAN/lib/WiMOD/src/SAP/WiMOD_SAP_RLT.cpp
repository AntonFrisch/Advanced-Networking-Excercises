//------------------------------------------------------------------------------
//! @file WiMOD_SAP_RLT.cpp
//! @ingroup WiMOD_SAP_RLT
//! <!------------------------------------------------------------------------->
//! @brief Implementation of the commands of the RadioLinkTest SericeAccessPoint
//! @version 0.1
//! <!------------------------------------------------------------------------->
//!
//!
//!
//! <!--------------------------------------------------------------------------
//! Copyright (c) 2018
//! IMST GmbH
//! Carl-Friedrich Gauss Str. 2
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


//------------------------------------------------------------------------------
//
// Section Includes Files
//
//------------------------------------------------------------------------------

#include "WiMOD_SAP_RLT.h"
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
WiMOD_SAP_RLT::WiMOD_SAP_RLT(TWiMODLRHCI* hci, UINT8* buffer, UINT16 bufferSize):
    HciParser(hci)
{
	StatusIndCallback = NULL;

    txPayload = buffer;
    txPayloadSize = bufferSize;

}

//-----------------------------------------------------------------------------
/**
 * @brief Destructor
 */

WiMOD_SAP_RLT::~WiMOD_SAP_RLT(void) {

}


//-----------------------------------------------------------------------------
/**
 * @brief Starts the integrated Radio Link Test mode/feature
 *
 * This functions starts the integrated Radio Link Test mode / feature.
 * The user can determnine the radio coverage with the helf of this mode.
 *
 * @warning: Thes mode may interrupt normal operation. Use this mode only in defined
 *           test szenarios.
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param params    poniter to parameter structure for this mode
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_RLT::StartRadioLinkTest(const TWiMODLR_RLT_Parameter* params,
													 UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = 0;

    if (params && statusRsp && (txPayloadSize >= (7)))
    {
        txPayload[offset++] = (UINT8) params->DestGroupAddress;
        HTON16(&txPayload[offset], params->DestDevAddress);
        offset += 2;
        txPayload[offset++] = (UINT8) params->PacketSize;
        HTON16(&txPayload[offset], params->NumPackets);
        offset += 2;
        txPayload[offset++] = (UINT8) params->TestMode;

        result = HciParser->SendHCIMessage(RLT_SAP_ID,
                                           RLT_MSG_START_REQ,
										   RLT_MSG_START_RSP,
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
 * @brief Stops the integrated Radio Link Test mode/feature
 *
 * This functions stops the integrated Radio Link Test mode / feature.
 * The user can determnine the radio coverage with the helf of this mode.
 *
 * @warning: Thes mode may interrupt normal operation. Use this mode only in defined
 *           test szenarios.
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_RLT::StopRadioLinkTest(UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = 0;

    if (statusRsp )
    {
        result = HciParser->SendHCIMessage(RLT_SAP_ID,
                                           RLT_MSG_STOP_REQ,
										   RLT_MSG_STOP_RSP,
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
 * @brief Register a callback function for the event "RLT Status Indication"
 *
 * This registered callback is called when the specified event is called by
 * the WiMOD stack.
 *
 * @param   cb          pointer to a callback function that should be called
 *                      if the event occurs.
 */
void WiMOD_SAP_RLT::RegisterStatusIndClient(TRltStatusIndicationCallback cb) {
	StatusIndCallback = cb;
}


//-----------------------------------------------------------------------------
/**
 * @brief Convert a received low level HCI-Msg to a high-level SensorAckData structure
 *
 * This function should be used by the RLT Status Indication Callback function
 * prior processing the received data message.
 *
 * @param   RxMsg       Reference to low-level HCI message.
 *                      @warning DO NOT MANIPULATE THESE VALUES !!!
 *
 * @param   statusData Pointer to the buffer where to store the received data
 *
 * @retval true     if the conversion was successful
 */
bool WiMOD_SAP_RLT::convert(TWiMODLR_HCIMessage&    RxMsg,
							TWiMODLR_RLT_Status*    statusData)
{
    UINT8 offset = 0;

    if (statusData) {
    	statusData->TestStatus = RLT_STATUS_ERROR;
    }



    if (statusData && RxMsg.Length >= 15) {

        statusData->TestStatus       = (UINT8) RxMsg.Payload[offset++];
        statusData->LocalTxCounter   = NTOH16(&RxMsg.Payload[offset]);
        offset += 2;
        statusData->LocalRxCounter   = NTOH16(&RxMsg.Payload[offset]);
        offset += 2;
        statusData->PeerTxCounter    = NTOH16(&RxMsg.Payload[offset]);
        offset += 2;
        statusData->PeerRxCounter    = NTOH16(&RxMsg.Payload[offset]);
        offset += 2;
        statusData->LocalRSSI        = NTOH16(&RxMsg.Payload[offset]);
        offset += 2;
        statusData->PeerRSSI         = NTOH16(&RxMsg.Payload[offset]);
        offset += 2;
        statusData->LocalSNR    	 = (UINT8) RxMsg.Payload[offset++];
        statusData->PeerSNR      	 = (UINT8) RxMsg.Payload[offset++];

        return true;
    }
    return false;
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
void WiMOD_SAP_RLT::DispatchRLTMessage(TWiMODLR_HCIMessage& rxMsg)
{
    switch (rxMsg.MsgID)
    {
        case RLT_MSG_STATUS_IND:
            if (StatusIndCallback) {
            	StatusIndCallback(rxMsg);
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


