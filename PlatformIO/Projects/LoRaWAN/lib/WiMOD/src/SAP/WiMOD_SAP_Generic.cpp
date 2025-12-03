//------------------------------------------------------------------------------
//! @file WiMOD_SAP_Generic.cpp
//! @ingroup WIMOD_SAP_Generic
//! <!------------------------------------------------------------------------->
//! @brief Generic HCI commmand interface
//! @version 0.1
//! <!------------------------------------------------------------------------->
//!
//!
//!
//! <!--------------------------------------------------------------------------
//! Copyright (c) 2017
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

#include <string.h>
#include <SAP/WiMOD_SAP_Generic.h>

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
WiMOD_SAP_Generic::WiMOD_SAP_Generic(TWiMODLRHCI* hci, UINT8* buffer, UINT16 bufferSize) :
    HciParser(hci)
{
    txPayload        = buffer;
    txyPayloadSize  = bufferSize;
}

//-----------------------------------------------------------------------------
/**
 * @brief Destructor
 */
WiMOD_SAP_Generic::~WiMOD_SAP_Generic(void)
{

}


//-----------------------------------------------------------------------------
/**
 * @brief Reset Cmd - Do a reset / reboot of the WiMOD
 *
 *
 * @param   info        pointer to store the received information
 * @param   statusRsp   pointer to store status byte of response mesg from WiMOD
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_Generic::ExecuteGenericCmd(TWiMODLR_Generic_CmdInfo* info, UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = WiMODLR_HCI_RSP_CMD_PAYLOAD_POS;
    UINT16			   payLen;

    if (info && statusRsp) {

    	payLen = MIN(txyPayloadSize, info->CmdPayloadLength);

    	memcpy(txPayload, info->CmdPayload, payLen);

        result = HciParser->SendHCIMessage(info->SapID,
                                           info->MsgReqID,
                                           info->MsgRspID,
                                           txPayload, payLen);

        if (result == WiMODLR_RESULT_OK) {
            const TWiMODLR_HCIMessage& rx = HciParser->GetRxMessage();

            // extract data from response
            info->Status     = rx.Payload[WiMODLR_HCI_RSP_STATUS_POS];

            info->CmdPayloadLength = 0;
            if (rx.Length > WiMODLR_HCI_RSP_CMD_PAYLOAD_POS) {
				info->CmdPayloadLength = MIN(rx.Length-WiMODLR_HCI_RSP_CMD_PAYLOAD_POS, WiMOD_GENERIC_MSG_SIZE);
				memcpy(info->CmdPayload, &rx.Payload[offset], info->CmdPayloadLength);

            }

            *statusRsp = rx.Payload[WiMODLR_HCI_RSP_STATUS_POS];
        }
    }
    return result;
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


