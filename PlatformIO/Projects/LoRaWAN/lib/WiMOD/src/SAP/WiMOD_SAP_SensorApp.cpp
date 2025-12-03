//------------------------------------------------------------------------------
//! @file WiMOD_SAP_SensorApp.cpp
//! @ingroup WiMOD_SAP_SensorApp
//! <!------------------------------------------------------------------------->
//! @brief Implementation of the commands of the SensorApplication SericeAccessPoint
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

#include "WiMOD_SAP_SensorApp.h"
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
WiMOD_SAP_SensorApp::WiMOD_SAP_SensorApp(TWiMODLRHCI* hci, UINT8* buffer, UINT16 bufferSize):
    HciParser(hci)
{
	SensorAckIndCallback  = NULL;
	SensorDataIndCallback = NULL;

    txPayload = buffer;
    txPayloadSize = bufferSize;
}

WiMOD_SAP_SensorApp::~WiMOD_SAP_SensorApp()
{

}


//-----------------------------------------------------------------------------
/**
 * @brief Sets a new SensorApp configuration to the WiMOD
 *
 * @param config     pointer to data structure containing the new parameters
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_SensorApp::SetSensorAppConfig(const TWiMODLR_SensorApp_Config* config,
														   UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    uint8_t offset = 0;

    if ( config && statusRsp && txPayloadSize > 10) {

    	txPayload[offset++] = (UINT8) config->Mode;
    	txPayload[offset++] = (UINT8) config->Options;
    	HTON32(&txPayload[offset], config->SendingPeriod);
    	offset += 4;
    	HTON32(&txPayload[offset], config->LinkTimeout);
    	offset += 4;
        result = HciParser->SendHCIMessage(SENSORAPP_SAP_ID,
        								   SENSORAPP_MSG_SET_CONFIG_REQ,
										   SENSORAPP_MSG_SET_CONFIG_RSP,
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
 * @brief Gets the current SensorApp configuration from the WiMOD
 *
 * @param config     pointer to data structure where to store the data
 *
 * @param statusRsp Status byte contained in the local response of the module
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_SensorApp::GetSensorAppConfig(TWiMODLR_SensorApp_Config* config,
														   UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = WiMODLR_HCI_RSP_STATUS_POS + 1;


    if ( config && statusRsp) {
        result = HciParser->SendHCIMessage(SENSORAPP_SAP_ID,
        								   SENSORAPP_MSG_GET_CONFIG_REQ,
										   SENSORAPP_MSG_GET_CONFIG_RSP,
                                           txPayload, 0x00);

        if (result == WiMODLR_RESULT_OK) {
            const TWiMODLR_HCIMessage& rx = HciParser->GetRxMessage();

            // copy response status
            *statusRsp = rx.Payload[WiMODLR_HCI_RSP_STATUS_POS];

            config->Status = (UINT8) *statusRsp;
            // extract content
            config->Mode = (TSensorApp_Mode) rx.Payload[offset++];
        	config->Options = (UINT8)        rx.Payload[offset++];
        	config->SendingPeriod =  NTOH32(&rx.Payload[offset]);
        	offset += 4;
        	config->LinkTimeout = NTOH32(&rx.Payload[offset]);
        	offset += 4;
       }
    } else {
        result = WiMODLR_RESULT_PAYLOAD_PTR_ERROR;
    }
    return result;
}

//-----------------------------------------------------------------------------
/**
 * @brief Convert a received low level HCI-Msg to a high-level SensorData structure
 *
 * This function should be used by the SensorData Indication Callback function
 * prior processing the received data message.
 *
 * @param   RxMsg       Reference to low-level HCI message.
 *                      @warning DO NOT MANIPULATE THESE VALUES !!!
 *
 * @param   sensorData Pointer to the buffer where to store the received data
 *
 * @retval true     if the conversion was successful
 */
bool WiMOD_SAP_SensorApp::convert(TWiMODLR_HCIMessage&            RxMsg,
								TWiMODLR_SensorApp_SensorData* sensorData)
{
    UINT8 offset = 0;

    if (sensorData) {
    	sensorData->OptionalInfoAvaiable = false;
    }

    if (sensorData && RxMsg.Length >= 13) {

    	sensorData->StatusFormat = RxMsg.Payload[offset++];

        if (sensorData->StatusFormat & SENSORAPP_FORMAT_EXT_HCI_OUT_ACTIVE) {
            sensorData->OptionalInfoAvaiable = true;
        } else {
        	sensorData->OptionalInfoAvaiable = false;
        }

        // infos from message
        sensorData->DestGroupAddress = RxMsg.Payload[offset++];
        sensorData->DestDevAddress   = NTOH16(&RxMsg.Payload[offset]);
        offset += 2;
        sensorData->SourceGroupAddress = RxMsg.Payload[offset++];
        sensorData->SourceDevAddress   = NTOH16(&RxMsg.Payload[offset]);
        offset += 2;
        sensorData->Voltage            = NTOH16(&RxMsg.Payload[offset]);
        offset += 2;
        sensorData->AdcValue           = NTOH16(&RxMsg.Payload[offset]);
        offset += 2;
        sensorData->Temperature		   = (UINT8) RxMsg.Payload[offset++];
        sensorData->DigitalInputs      = (UINT8) RxMsg.Payload[offset++];

        // check if optional attributes are present
        if ( (uint16_t) (offset+7) <= RxMsg.Length) {
        	sensorData->RSSI = (INT16) NTOH16(&RxMsg.Payload[offset]);
        	offset += 2;
        	sensorData->SNR  = (INT8) RxMsg.Payload[offset++];
        	sensorData->RxTime = NTOH32(&RxMsg.Payload[offset]);
        	offset += 4;
        }
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
/**
 * @brief Convert a received low level HCI-Msg to a high-level SensorAckData structure
 *
 * This function should be used by the SensorAckData Indication Callback function
 * prior processing the received data message.
 *
 * @param   RxMsg       Reference to low-level HCI message.
 *                      @warning DO NOT MANIPULATE THESE VALUES !!!
 *
 * @param   sensorAckData Pointer to the buffer where to store the received data
 *
 * @retval true     if the conversion was successful
 */
bool WiMOD_SAP_SensorApp::convert(TWiMODLR_HCIMessage&            RxMsg,
								  TWiMODLR_SensorApp_AckSensorData* sensorAckData)
{
    UINT8 offset = 0;

    if (sensorAckData) {
    	sensorAckData->OptionalInfoAvaiable = false;
    }

    if (sensorAckData && RxMsg.Length >= 8) {

    	sensorAckData->StatusFormat = RxMsg.Payload[offset++];

        if (sensorAckData->StatusFormat & SENSORAPP_FORMAT_EXT_HCI_OUT_ACTIVE) {
        	sensorAckData->OptionalInfoAvaiable = true;
        } else {
        	sensorAckData->OptionalInfoAvaiable = false;
        }

        // infos from message
        sensorAckData->DestGroupAddress = RxMsg.Payload[offset++];
        sensorAckData->DestDevAddress   = NTOH16(&RxMsg.Payload[offset]);
        offset += 2;
        sensorAckData->SourceGroupAddress = RxMsg.Payload[offset++];
        sensorAckData->SourceDevAddress   = NTOH16(&RxMsg.Payload[offset]);
        offset += 2;
        sensorAckData->DigitalInputs      = (UINT8) RxMsg.Payload[offset++];

        // check if optional attributes are present
        if ((uint16_t)(offset+7) <= RxMsg.Length) {
        	sensorAckData->RSSI = (INT16) NTOH16(&RxMsg.Payload[offset]);
        	offset += 2;
        	sensorAckData->SNR  = (INT8) RxMsg.Payload[offset++];
        	sensorAckData->RxTime = NTOH32(&RxMsg.Payload[offset]);
        	offset += 4;
        }
        return true;
    }
    return false;
}



//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the event "SensorApp Ack Data Indication"
 *
 * This registered callback is called when the specified event is called by
 * the WiMOD stack.
 *
 * @param   cb          pointer to a callback function that should be called
 *                      if the event occurs.
 */
void WiMOD_SAP_SensorApp::RegisterSensorAckIndClient(TSensorAppAckIndicationCallback cb)
{
	SensorAckIndCallback = cb;
}

//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the event "SensorApp Data Indication"
 *
 * This registered callback is called when the specified event is called by
 * the WiMOD stack.
 *
 * @param   cb          pointer to a callback function that should be called
 *                      if the event occurs.
 */
void WiMOD_SAP_SensorApp::RegisterSensorDataIndClient(TSensorAppSensorDataIndicationCallback cb)
{
	SensorDataIndCallback = cb;
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
void WiMOD_SAP_SensorApp::DispatchSensorAppMessage(TWiMODLR_HCIMessage& rxMsg)
 {
    switch (rxMsg.MsgID)
    {

        case SENSORAPP_MSG_SEND_DATA_IND:
            if (SensorDataIndCallback) {
            	SensorDataIndCallback(rxMsg);
            }
            break;
        case SENSORAPP_MSG_ACK_IND:
        	if (SensorAckIndCallback) {
        		SensorAckIndCallback(rxMsg);
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


