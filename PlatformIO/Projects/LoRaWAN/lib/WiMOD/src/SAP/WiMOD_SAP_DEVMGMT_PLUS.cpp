//------------------------------------------------------------------------------
//! @file WiMOD_SAP_DEVMGMT_PLUS.cpp
//! @ingroup WiMOD_SAP_DEVMGMT_PLUS
//! <!------------------------------------------------------------------------->
//! @brief Implementation of the commands of the DeviceManagement SericeAccessPoint for LR BASE PLUS firmware
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


#include "WiMOD_SAP_DEVMGMT_PLUS.h"

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
WiMOD_SAP_DevMgmt_Plus::WiMOD_SAP_DevMgmt_Plus(TWiMODLRHCI* hci, UINT8* buffer, UINT16 bufferSize) :
	WiMOD_SAP_DevMgmt(hci, buffer, bufferSize)
{

}

//-----------------------------------------------------------------------------
/**
 * @brief Destructor
 */
WiMOD_SAP_DevMgmt_Plus::~WiMOD_SAP_DevMgmt_Plus(void)
{

}




//------------------------------------------------------------------------------

TWiMODLRResultCodes WiMOD_SAP_DevMgmt_Plus::GetRadioConfig(TWiMODLR_DevMgmt_RadioConfig* radioCfg, UINT8* statusRsp) {
    (void) radioCfg;
    // not supported in this firmware
	return unsupportedCmd(statusRsp);
}

//-----------------------------------------------------------------------------
/**
 * @brief GetRadioConfig Cmd - Get the radio settings of the WiMOD
 *
 *
 * @param   radioCfg    pointer to store the received information
 * @param   statusRsp   pointer to store status byte of response mesg from WiMOD
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_DevMgmt_Plus::GetRadioConfig(TWiMODLR_DevMgmt_RadioConfigPlus* radioCfg, UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = WiMODLR_HCI_RSP_STATUS_POS;

    if (radioCfg && statusRsp) {

        result = HciParser->SendHCIMessage(DEVMGMT_SAP_ID,
                                           DEVMGMT_MSG_GET_RADIO_CONFIG_REQ,
                                           DEVMGMT_MSG_GET_RADIO_CONFIG_RSP,
                                           NULL, 0);

        if (result == WiMODLR_RESULT_OK) {
            const TWiMODLR_HCIMessage& rx = HciParser->GetRxMessage();
            *statusRsp = rx.Payload[offset++];

            // status check
            if (*statusRsp == DEVMGMT_STATUS_OK) {

                    radioCfg->Status = *statusRsp;
                    // 0
                    radioCfg->RadioMode = (TRadioCfg_RadioModePlus) rx.Payload[offset++];
                    // 1
                    radioCfg->GroupAddress = rx.Payload[offset++];
                    // 2
                    radioCfg->TxGroupAddress = rx.Payload[offset++];
                    // 3
                    radioCfg->DeviceAddress = NTOH16(&rx.Payload[offset]);
                    offset += 0x02;
                    // 5
                    radioCfg->TxDeviceAddress = NTOH16(&rx.Payload[offset]);
                    offset += 0x02;
                    // 7
                    radioCfg->Modulation = (TRadioCfg_ModulationPlus) rx.Payload[offset++];
                    // 8
                    radioCfg->RfFreq_LSB = rx.Payload[offset++];
                    // 9
                    radioCfg->RfFreq_MID = rx.Payload[offset++];
                    // 10
                    radioCfg->RfFreq_MSB = rx.Payload[offset++];

                    radioCfg->LoRaBandWidth = LRBASE_PLUS_LoRaBandwith_200kHz;
                    radioCfg->FLRCBandWidth = LRBASE_PLUS_FLRCBandwith_0_260MBs_0_3_MHz_DSB;
                    radioCfg->FSKBandWidth = LRBASE_PLUS_FSKBandwith_2_0MBs_2_4_MHz;

                    // 11
                    switch (radioCfg->Modulation) {
                        case LRBASE_PLUS_Modulation_LoRa:
                            radioCfg->LoRaBandWidth = (TRadioCfg_LoRaBandwidthPlus) rx.Payload[offset++];
                            break;
                        case LRBASE_PLUS_Modulation_FLRC:
                            radioCfg->FLRCBandWidth = (TRadioCfg_FLRCBandwidthPlus) rx.Payload[offset++];
                            break;
                        case LRBASE_PLUS_Modulation_FSK:
                            radioCfg->FSKBandWidth = (TRadioCfg_FSKBandwidthPlus) rx.Payload[offset++];
                            break;
                        default:
                            offset++;
                            break;
                    }
                    // 12
                    radioCfg->LoRaSpreadingFactor = (TRadioCfg_LoRaSpreadingFactorPlus) rx.Payload[offset++];
                    // 13
                    radioCfg->LoRaErrorCoding = LRBASE_PLUS_LoRa_ErrorCoding_4_5;
                    radioCfg->FLRCErrorCoding = LRBASE_PLUS_FLRC_ErrorCoding_1_2;

                    switch (radioCfg->Modulation) {
                        case LRBASE_PLUS_Modulation_LoRa:
                            radioCfg->LoRaErrorCoding = (TRadioCfg_LoRaErrorCodingPlus) rx.Payload[offset++];
                            break;
                        case LRBASE_PLUS_Modulation_FLRC:
                            radioCfg->FLRCErrorCoding = (TRadioCfg_FLRCErrorCodingPlus) rx.Payload[offset++];
                            break;
                        case LRBASE_PLUS_Modulation_FSK:
                            offset++;
                            break;
                        default:
                            offset++;
                            break;
                    }
                    // 14
                    radioCfg->PowerLevel =  (TRadioCfg_PowerLevelPlus)  rx.Payload[offset++];
                    // 15
                    radioCfg->TxControl = (TRadioCfg_TxControlPlus) rx.Payload[offset++];
                    // 16
                    radioCfg->RxControl = (TRadioCfg_RxControlPlus) rx.Payload[offset++];
                    // 17
                    radioCfg->RxWindowTime = NTOH16(&rx.Payload[offset]);
                    offset += 0x02;
                    // 19
                    radioCfg->LedControl = rx.Payload[offset++];
                    // 20
                    radioCfg->MiscOptions = rx.Payload[offset++];
                    // 21
                    // reserved
                    offset++;
                    // 22
                    radioCfg->PowerSavingMode = (TRadioCfg_PowerSavingModePlus) rx.Payload[offset++];
                    // 23
                    radioCfg->LbtThreshold = (INT16) NTOH16(&rx.Payload[offset]);
                    offset += 0x02;

            }
        }
    }
    return result;
}


//-----------------------------------------------------------------------------
TWiMODLRResultCodes WiMOD_SAP_DevMgmt_Plus::SetRadioConfig(const TWiMODLR_DevMgmt_RadioConfig* radioCfg, UINT8* statusRsp) {
	(void) radioCfg;
    if (statusRsp) {
		*statusRsp = DEVMGMT_STATUS_CMD_NOT_SUPPORTED;
	}
	return WiMODLR_RESULT_TRANMIT_ERROR;
}

//-----------------------------------------------------------------------------
/**
 * @brief SetRadioConfig Cmd - Set the radio settings of the WiMOD
 *
 *
 * @param   radioCfg    pointer to the new radio configuration
 * @param   statusRsp   pointer to store status byte of response mesg from WiMOD
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
//#include <stdio.h>
TWiMODLRResultCodes WiMOD_SAP_DevMgmt_Plus::SetRadioConfig(const TWiMODLR_DevMgmt_RadioConfigPlus* radioCfg, UINT8* statusRsp,  UINT8 wrgParamBM[]) {
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = 0;

    if (radioCfg && statusRsp && (txyPayloadSize >= 0x1A)) {
        txPayload[offset++] = (UINT8) radioCfg->StoreNwmFlag;
        // 0
        txPayload[offset++] = (UINT8) radioCfg->RadioMode;
        // 1
        txPayload[offset++] = (UINT8) radioCfg->GroupAddress;
        // 2
        txPayload[offset++] = (UINT8) radioCfg->TxGroupAddress;
        // 3
        HTON16(&txPayload[offset], radioCfg->DeviceAddress);
        offset += 0x02;
        // 5
        HTON16(&txPayload[offset], radioCfg->TxDeviceAddress);
        offset += 0x02;
        // 7
        txPayload[offset++] = (UINT8) radioCfg->Modulation;
        // 8
        txPayload[offset++] = (UINT8) radioCfg->RfFreq_LSB;
        // 9
        txPayload[offset++] = (UINT8) radioCfg->RfFreq_MID;
        // 10
        txPayload[offset++] = (UINT8) radioCfg->RfFreq_MSB;

        switch (radioCfg->Modulation) {
            case LRBASE_PLUS_Modulation_LoRa:
                // 11
                txPayload[offset++] = (UINT8) radioCfg->LoRaBandWidth;
                // 12
                txPayload[offset++] = (UINT8) radioCfg->LoRaSpreadingFactor;
                // 13
                txPayload[offset++] = (UINT8) radioCfg->LoRaErrorCoding;
                break;
            case LRBASE_PLUS_Modulation_FLRC:
                // 11
                txPayload[offset++] = (UINT8) radioCfg->FLRCBandWidth;
                // 12
                txPayload[offset++] = (UINT8) 0x00; // reserved
                // 13
                txPayload[offset++] = (UINT8) radioCfg->FLRCErrorCoding;
                break;
            case LRBASE_PLUS_Modulation_FSK:
                // 11
                txPayload[offset++] = (UINT8) radioCfg->FSKBandWidth;
                // 12
                txPayload[offset++] = (UINT8) 0x00; // reserved
                // 13
                txPayload[offset++] = (UINT8) 0x00; // reserved
                break;
            default:
                // 11
                txPayload[offset++] = (UINT8) 0x00; // reserved / unkonwn
                // 12
                txPayload[offset++] = (UINT8) 0x00; // reserved / unknown
                // 13
                txPayload[offset++] = (UINT8) 0x00; // reserved / unknown
                break;
        }
        // 14
        txPayload[offset++] = (UINT8) radioCfg->PowerLevel;
        // 15
        txPayload[offset++] = (UINT8) radioCfg->TxControl;
        // 16
        txPayload[offset++] = (UINT8) radioCfg->RxControl;
        // 17
        HTON16(&txPayload[offset], radioCfg->RxWindowTime);
        offset += 0x02;
        // 19
        txPayload[offset++] = (UINT8) radioCfg->LedControl;
        // 20
        txPayload[offset++] = (UINT8) radioCfg->MiscOptions;
        // 21
        txPayload[offset++] = (UINT8) 0x00; // reserved
        // 22
        txPayload[offset++] = (UINT8) radioCfg->PowerSavingMode;
        // 23
        HTON16(&txPayload[offset], (UINT16) radioCfg->LbtThreshold);
        offset += 0x02;
//for (int z = 0; z < offset; z++) {
////    printf("[%02d]: %02x \n", z, txPayload[z]);
//    printf("%02x ", txPayload[z]);
//}
//printf("\n");
        result = HciParser->SendHCIMessage(DEVMGMT_SAP_ID,
                DEVMGMT_MSG_SET_RADIO_CONFIG_REQ,
                DEVMGMT_MSG_SET_RADIO_CONFIG_RSP,
                txPayload, offset);

        if (result == WiMODLR_RESULT_OK) {
            const TWiMODLR_HCIMessage& rx = HciParser->GetRxMessage();
            offset = WiMODLR_HCI_RSP_STATUS_POS;
            *statusRsp = rx.Payload[offset++];

            // status check
            if (*statusRsp == DEVMGMT_STATUS_WRONG_PARAMETER) {
                const TWiMODLR_HCIMessage& rx = HciParser->GetRxMessage();
                if (wrgParamBM && (rx.Length >=4)) {
                    wrgParamBM[0] = rx.Payload[WiMODLR_HCI_RSP_CMD_PAYLOAD_POS + 0];
                    wrgParamBM[1] = rx.Payload[WiMODLR_HCI_RSP_CMD_PAYLOAD_POS + 1];
                    wrgParamBM[2] = rx.Payload[WiMODLR_HCI_RSP_CMD_PAYLOAD_POS + 2];
                    wrgParamBM[3] = rx.Payload[WiMODLR_HCI_RSP_CMD_PAYLOAD_POS + 3];
                }
            } else {
            }
        }
    }
    return result;
}


//-----------------------------------------------------------------------------
TWiMODLRResultCodes WiMOD_SAP_DevMgmt_Plus::GetSystemStatus(TWiMODLR_DevMgmt_SystemStatus* info, UINT8* statusRsp)
{
    (void) info;
	// not supported in this firmware
	return unsupportedCmd(statusRsp);

}
//-----------------------------------------------------------------------------
/**
 * @brief GetSystemStatus Cmd - Get basic info about the system status of WiMOD
 *
 *
 * @param   info        pointer to store the received information
 * @param   statusRsp   pointer to store status byte of response mesg from WiMOD
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_DevMgmt_Plus::GetSystemStatus(TWiMODLR_DevMgmt_SystemStatusPlus* info, UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = WiMODLR_HCI_RSP_STATUS_POS;

    if (info && statusRsp) {
        result = HciParser->SendHCIMessage(DEVMGMT_SAP_ID,
                                           DEVMGMT_MSG_GET_SYSTEM_STATUS_REQ,
                                           DEVMGMT_MSG_GET_SYSTEM_STATUS_RSP,
                                           NULL, 0);

        if (result == WiMODLR_RESULT_OK) {
            const TWiMODLR_HCIMessage& rx = HciParser->GetRxMessage();

            info->Status              = rx.Payload[offset++];
            info->SysTickResolution   = rx.Payload[offset++];
            info->SysTickCounter      = NTOH32(&rx.Payload[offset]);
            offset += 0x04;
            info->RtcTime             = NTOH32(&rx.Payload[offset]);
            offset += 0x04;
            info->NvmStatus           = NTOH16(&rx.Payload[offset]);
            offset += 0x02;
            info->BatteryStatus       = NTOH16(&rx.Payload[offset]);
            offset += 0x02;
            info->ExtraStatus         = NTOH16(&rx.Payload[offset]);
            offset += 0x02;
            info->RxPackets           = NTOH32(&rx.Payload[offset]);
            offset += 0x04;
            info->RxAddressMatch      = NTOH32(&rx.Payload[offset]);
            offset += 0x04;
            info->RxCRCError          = NTOH32(&rx.Payload[offset]);
            offset += 0x04;
            info->TxPackets           = NTOH32(&rx.Payload[offset]);
            offset += 0x04;
            info->TxError             = NTOH32(&rx.Payload[offset]);
            offset += 0x04;
            info->TxMediaBusyEvents    = NTOH32(&rx.Payload[offset]);
            offset += 0x04;

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
/**
 * @internal
 * @brief default cmd handler for unknown / unsupported commands
 * @params statusRsp    pointer to HCI status response code to set
 * @endinternal
 */
TWiMODLRResultCodes WiMOD_SAP_DevMgmt_Plus::unsupportedCmd(UINT8* statusRsp) {
	if (statusRsp) {
		*statusRsp = DEVMGMT_STATUS_CMD_NOT_SUPPORTED;
	}
	return WiMODLR_RESULT_TRANMIT_ERROR;
}


//------------------------------------------------------------------------------
//
// Section private functions
//
//------------------------------------------------------------------------------


