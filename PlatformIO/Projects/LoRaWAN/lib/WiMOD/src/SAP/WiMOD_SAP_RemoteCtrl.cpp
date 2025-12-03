//------------------------------------------------------------------------------
//! @file WiMOD_SAP_RemoteCtrl.cpp
//! @ingroup WiMOD_SAP_RemoteCtrl
//! <!------------------------------------------------------------------------->
//! @brief Implementation of the commands of the RemoteControl SericeAccessPoint
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

#include "WiMOD_SAP_RemoteCtrl.h"
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
WiMOD_SAP_RemoteCtrl::WiMOD_SAP_RemoteCtrl(TWiMODLRHCI* hci, UINT8* buffer, UINT16 bufferSize):
    HciParser(hci)
{
	BtnPressedIndCallback  = NULL;

    txPayload = buffer;
    txPayloadSize = bufferSize;
}

WiMOD_SAP_RemoteCtrl::~WiMOD_SAP_RemoteCtrl()
{

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
 * @param   btnInfo     Pointer to the buffer where to store the received data
 *
 * @retval true     if the conversion was successful
 */
bool WiMOD_SAP_RemoteCtrl::convert(TWiMODLR_HCIMessage& RxMsg,
								   TWiMODLR_RemoteCtrl_BtnPressed* btnInfo)
{
    UINT8 offset = 0;



    if (btnInfo && RxMsg.Length >= 7) {

        btnInfo->DestinationGroupAddress = (UINT8) RxMsg.Payload[offset++];
        btnInfo->DestinationDeviceAddress = NTOH16(&RxMsg.Payload[offset]);
        offset += 2;
        btnInfo->SourceGroupAddress =  (UINT8) RxMsg.Payload[offset++];
        btnInfo->SourceDeviceAddress = NTOH16(&RxMsg.Payload[offset]);
        offset += 2;
        btnInfo->ButtonBitmap = (UINT8) RxMsg.Payload[offset++];

        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the event "Button Pressed Indication"
 *
 * This registered callback is called when the specified event is called by
 * the WiMOD stack.
 *
 * @param   cb          pointer to a callback function that should be called
 *                      if the event occurs.
 */

void WiMOD_SAP_RemoteCtrl::RegisterBtnPressedClient(TRemoteCtrlBtnPressedIndicationCallback cb) {
	BtnPressedIndCallback = cb;
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
WiMOD_SAP_RemoteCtrl::DispatcRemoteCtlMessage(TWiMODLR_HCIMessage& rxMsg) {
    switch (rxMsg.MsgID)
    {
        case REMOTE_CTRL_MSG_BUTTON_PRESSED_ID:
            if (BtnPressedIndCallback) {
            	BtnPressedIndCallback(rxMsg);
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


