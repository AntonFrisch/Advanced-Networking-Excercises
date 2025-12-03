//------------------------------------------------------------------------------
//! @file WiMOD_SAP_DEVMGMT.cpp
//! @ingroup WiMOD_SAP_DEVMGMT
//! <!------------------------------------------------------------------------->
//! @brief Implementation of the commands of the DeviceManagement SericeAccessPoint
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
 * THIS IS AN EXAMPLE IMPLEMENTATION ACCORDING THE THE HCI SPEC: V1.8
 * FOR FIRMWARE: LR-BASE
 */

//------------------------------------------------------------------------------
//
// Section Includes Files
//
//------------------------------------------------------------------------------

#include "WiMOD_SAP_DEVMGMT.h"
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
WiMOD_SAP_DevMgmt::WiMOD_SAP_DevMgmt(TWiMODLRHCI* hci, UINT8* buffer, UINT16 bufferSize) :
    HciParser(hci)
{
    PowerUpCallack   = NULL;
    RtcAlarmCallback = NULL;
    txPayload        = buffer;
    txyPayloadSize  = bufferSize;
}

//-----------------------------------------------------------------------------
/**
 * @brief Destructor
 */
WiMOD_SAP_DevMgmt::~WiMOD_SAP_DevMgmt(void)
{

}

//-----------------------------------------------------------------------------
/**
 * @brief Ping Cmd - Checks if the serial connection of to the WiMOD module is OK
 *
 *
 * @param   statusRsp   pointer to store status byte of response mesg from WiMOD
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_DevMgmt::Ping(UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    if (statusRsp) {
        result = HciParser->SendHCIMessage(DEVMGMT_SAP_ID,
                                            DEVMGMT_MSG_PING_REQ,
                                            DEVMGMT_MSG_PING_RSP,
                                            NULL, 0);
        // copy response status
        if (WiMODLR_RESULT_OK == result) {
            *statusRsp = HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_STATUS_POS];
        }
    }
    return result;
}



//-----------------------------------------------------------------------------
/**
 * @brief Reset Cmd - Do a reset / reboot of the WiMOD
 *
 *
 * @param   statusRsp   pointer to store status byte of response mesg from WiMOD
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_DevMgmt::Reset(UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    if (statusRsp) {
        result = HciParser->SendHCIMessage(DEVMGMT_SAP_ID,
                                           DEVMGMT_MSG_RESET_REQ,
                                           DEVMGMT_MSG_RESET_RSP,
                                           NULL, 0);
        // copy response status
        if (WiMODLR_RESULT_OK == result) {
            *statusRsp = HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_STATUS_POS];
        }
    }
    return result;
}

//-----------------------------------------------------------------------------
/**
 * @brief Get Device Info - Basic Information about the WiMOD module
 *
 *
 * @param   info        pointer to store the received information
 * @param   statusRsp   pointer to store status byte of response mesg from WiMOD
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_DevMgmt::GetDeviceInfo(TWiMODLR_DevMgmt_DevInfo* info, UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = WiMODLR_HCI_RSP_CMD_PAYLOAD_POS;

    if (info && statusRsp) {
        result = HciParser->SendHCIMessage(DEVMGMT_SAP_ID,
                                           DEVMGMT_MSG_GET_DEVICEINFO_REQ,
                                           DEVMGMT_MSG_GET_DEVICEINFO_RSP,
                                           NULL, 0);

        if (result == WiMODLR_RESULT_OK) {
            const TWiMODLR_HCIMessage& rx = HciParser->GetRxMessage();

            // extract data from response
            info->Status     = rx.Payload[WiMODLR_HCI_RSP_STATUS_POS];
            info->ModuleType = rx.Payload[offset++];
            info->DevAdr     = NTOH16(&rx.Payload[offset]);
            offset += 0x02;
            info->GroupAdr   = rx.Payload[offset++];
            // reserved field
            offset++;
            info->DevID      = NTOH32(&rx.Payload[offset]);
            offset += 0x04;

            *statusRsp = rx.Payload[WiMODLR_HCI_RSP_STATUS_POS];
        }
    }
    return result;
}

//-----------------------------------------------------------------------------
/**
 * @brief GetFirmwareInfo Cmd - Get basic info about the running firmware
 *
 *
 * @param   info        pointer to store the received information
 * @param   statusRsp   pointer to store status byte of response mesg from WiMOD
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_DevMgmt::GetFirmwareInfo(TWiMODLR_DevMgmt_FwInfo* info, UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = WiMODLR_HCI_RSP_STATUS_POS;

    if (info && statusRsp) {
        result = HciParser->SendHCIMessage(DEVMGMT_SAP_ID,
                                           DEVMGMT_MSG_GET_FW_VERSION_REQ,
                                           DEVMGMT_MSG_GET_FW_VERSION_RSP,
                                           NULL, 0);

        if (result == WiMODLR_RESULT_OK) {
            const TWiMODLR_HCIMessage& rx = HciParser->GetRxMessage();

            // extract info from response message
            info->Status              = rx.Payload[offset++];
            info->FirmwareMinorVersion= rx.Payload[offset++];
            info->FirmwareMayorVersion= rx.Payload[offset++];
            info->BuildCount          = NTOH16(&rx.Payload[offset]);
            offset+= 0x02;
            // ensure null-termination
            memset(info->BuildDateStr, 0x00, WIMOD_DEVMGMT_MAX_BUILDDATE_LEN);
            strncpy((char*)info->BuildDateStr, (const char*) &rx.Payload[offset], WIMOD_DEVMGMT_BUILDDATE_LEN);
            offset += WIMOD_DEVMGMT_BUILDDATE_LEN;
            // ensure null-termination
            memset(info->FirmwareName, 0x00, WIMOD_DEVMGMT_MAX_FIRMWARE_NAME_LEN);
            strncpy((char*)info->FirmwareName,(const char*) &rx.Payload[offset], rx.Length-offset);

            *statusRsp = HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_STATUS_POS];

        }
    }
    return result;
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
TWiMODLRResultCodes WiMOD_SAP_DevMgmt::GetSystemStatus(TWiMODLR_DevMgmt_SystemStatus* info, UINT8* statusRsp)
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
            info->TxMediaBusyEvents   = NTOH32(&rx.Payload[offset]);
            offset += 0x04;

            *statusRsp = rx.Payload[WiMODLR_HCI_RSP_STATUS_POS];
        }
    }
    return result;
}

//-----------------------------------------------------------------------------
/**
 * @brief GetRtc Cmd - Get the current RTC time/date from WiMOD
 *
 *
 * @param   rtcTime     pointer to store the received information
 * @param   statusRsp   pointer to store status byte of response mesg from WiMOD
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_DevMgmt::GetRtc(UINT32* rtcTime, UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = WiMODLR_HCI_RSP_STATUS_POS;

    if (rtcTime && statusRsp) {
        result = HciParser->SendHCIMessage(DEVMGMT_SAP_ID,
                                           DEVMGMT_MSG_GET_RTC_REQ,
                                           DEVMGMT_MSG_GET_RTC_RSP,
                                           NULL, 0);

        if (result == WiMODLR_RESULT_OK) {
            const TWiMODLR_HCIMessage& rx = HciParser->GetRxMessage();

            *statusRsp = rx.Payload[offset++];
            // status check
            if (*statusRsp == DEVMGMT_STATUS_OK) {
                *rtcTime = NTOH32(&rx.Payload[offset]);
                offset += 0x04;
            } else {
                *rtcTime = 0;
            }
        }
    }
    return result;
}

//-----------------------------------------------------------------------------
/**
 * @brief SetRtc Cmd - Set the current RTC time/date of WiMOD
 *
 *
 * @param   rtcTime     32bit data containing the new RTC timestamp
 * @param   statusRsp   pointer to store status byte of response mesg from WiMOD
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_DevMgmt::SetRtc(const UINT32 rtcTime, UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;


    if (statusRsp && (txyPayloadSize >= 4)) {
        HTON32(txPayload, rtcTime);

        result = HciParser->SendHCIMessage(DEVMGMT_SAP_ID,
                                           DEVMGMT_MSG_SET_RTC_REQ,
                                           DEVMGMT_MSG_SET_RTC_RSP,
                                           txPayload, 0x04);

        if (result == WiMODLR_RESULT_OK) {
            *statusRsp = HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_STATUS_POS];
        }
    }
    return result;
}

/**
 * @brief GetRadioConfig Cmd - Get the radio settings of the WiMOD
 *
 *
 * @param   radioCfg    pointer to store the received information
 * @param   statusRsp   pointer to store status byte of response mesg from WiMOD
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_DevMgmt::GetRadioConfig(TWiMODLR_DevMgmt_RadioConfig* radioCfg, UINT8* statusRsp)
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
                    radioCfg->RadioMode = (TRadioCfg_RadioMode) rx.Payload[offset++];
                    radioCfg->GroupAddress = rx.Payload[offset++];
                    radioCfg->TxGroupAddress = rx.Payload[offset++];
                    radioCfg->DeviceAddress = NTOH16(&rx.Payload[offset]);
                    offset += 0x02;
                    radioCfg->TxDeviceAddress = NTOH16(&rx.Payload[offset]);
                    offset += 0x02;
                    radioCfg->Modulation = (TRadioCfg_Modulation) rx.Payload[offset++];
                    radioCfg->RfFreq_LSB = rx.Payload[offset++];
                    radioCfg->RfFreq_MID = rx.Payload[offset++];
                    radioCfg->RfFreq_MSB = rx.Payload[offset++];
                    radioCfg->LoRaBandWidth = (TRadioCfg_LoRaBandwidth) rx.Payload[offset++];
                    radioCfg->LoRaSpreadingFactor = (TRadioCfg_LoRaSpreadingFactor) rx.Payload[offset++];;
                    radioCfg->ErrorCoding = (TRadioCfg_ErrorCoding) rx.Payload[offset++];;
                    radioCfg->PowerLevel =  (TRadioCfg_PowerLevel)  rx.Payload[offset++];
                    radioCfg->TxControl = rx.Payload[offset++];
                    radioCfg->RxControl = (TRadioCfg_RxControl) rx.Payload[offset++];
                    radioCfg->RxWindowTime = NTOH16(&rx.Payload[offset]);
                    offset += 0x02;
                    radioCfg->LedControl = rx.Payload[offset++];
                    radioCfg->MiscOptions = rx.Payload[offset++];
                    radioCfg->FskDatarate = (TRadioCfg_FskDatarate) rx.Payload[offset++];
                    radioCfg->PowerSavingMode = (TRadioCfg_PowerSavingMode) rx.Payload[offset++];
                    radioCfg->LbtThreshold = (INT16) NTOH16(&rx.Payload[offset]);
                    offset += 0x02;
            }
        }
    }
    return result;
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
TWiMODLRResultCodes WiMOD_SAP_DevMgmt::SetRadioConfig(const TWiMODLR_DevMgmt_RadioConfig* radioCfg, UINT8* statusRsp) {
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = 0;

    if (radioCfg && statusRsp && (txyPayloadSize >= 0x1A)) {
        txPayload[offset++] = (UINT8) radioCfg->StoreNwmFlag;
        txPayload[offset++] = (UINT8) radioCfg->RadioMode;
        txPayload[offset++] = (UINT8) radioCfg->GroupAddress;
        txPayload[offset++] = (UINT8) radioCfg->TxGroupAddress;
        HTON16(&txPayload[offset], radioCfg->DeviceAddress);
        offset += 0x02;
        HTON16(&txPayload[offset], radioCfg->TxDeviceAddress);
        offset += 0x02;
        txPayload[offset++] = (UINT8) radioCfg->Modulation;
        txPayload[offset++] = (UINT8) radioCfg->RfFreq_LSB;
        txPayload[offset++] = (UINT8) radioCfg->RfFreq_MID;
        txPayload[offset++] = (UINT8) radioCfg->RfFreq_MSB;
        txPayload[offset++] = (UINT8) radioCfg->LoRaBandWidth;
        txPayload[offset++] = (UINT8) radioCfg->LoRaSpreadingFactor;
        txPayload[offset++] = (UINT8) radioCfg->ErrorCoding;
        txPayload[offset++] = (UINT8) radioCfg->PowerLevel;
        txPayload[offset++] = (UINT8) radioCfg->TxControl;
        txPayload[offset++] = (UINT8) radioCfg->RxControl;
        HTON16(&txPayload[offset], radioCfg->RxWindowTime);
        offset += 0x02;
        txPayload[offset++] = (UINT8) radioCfg->LedControl;
        txPayload[offset++] = (UINT8) radioCfg->MiscOptions;
        txPayload[offset++] = (UINT8) radioCfg->FskDatarate;
        txPayload[offset++] = (UINT8) radioCfg->PowerSavingMode;
        HTON16(&txPayload[offset], (UINT16) radioCfg->LbtThreshold);
        offset += 0x02;

        result = HciParser->SendHCIMessage(DEVMGMT_SAP_ID,
                DEVMGMT_MSG_SET_RADIO_CONFIG_REQ,
                DEVMGMT_MSG_SET_RADIO_CONFIG_RSP,
                txPayload, offset);

        if (result == WiMODLR_RESULT_OK) {
            const TWiMODLR_HCIMessage& rx = HciParser->GetRxMessage();
            offset = WiMODLR_HCI_RSP_STATUS_POS;
            *statusRsp = rx.Payload[offset++];

            // status check
            if (*statusRsp == DEVMGMT_STATUS_OK) {


            } else {
            }
        }
    }
    return result;
}

//-----------------------------------------------------------------------------
/**
 * @brief ResetRadioConfig Cmd - Reset the radio config to factory defaults.
 *
 *
 * @param   statusRsp   pointer to store status byte of response mesg from WiMOD
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_DevMgmt::ResetRadioConfig(UINT8* statusRsp) {
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    if (statusRsp) {
        result = HciParser->SendHCIMessage(DEVMGMT_SAP_ID,
                DEVMGMT_MSG_RESET_RADIO_CONFIG_REQ,
                DEVMGMT_MSG_RESET_RADIO_CONFIG_RSP,
                NULL, 0);
        // copy response status
        if (WiMODLR_RESULT_OK == result) {
            *statusRsp = HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_STATUS_POS];
        }
    }
    return result;
}


//-----------------------------------------------------------------------------
/**
 * @brief GetOperationMode Cmd - Get the current operation mode of the WiMOD
 *
 *
 * @param   opMode      pointer to store the received information
 * @param   statusRsp   pointer to store status byte of response mesg from WiMOD
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_DevMgmt::GetOperationMode(TWiMOD_OperationMode* opMode, UINT8* statusRsp) {
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    if (statusRsp && opMode) {
        result = HciParser->SendHCIMessage(DEVMGMT_SAP_ID,
                DEVMGMT_MSG_GET_OPMODE_REQ,
                DEVMGMT_MSG_GET_OPMODE_RSP,
                NULL, 0);
        // copy response status
        if (WiMODLR_RESULT_OK == result) {
            *statusRsp = HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_STATUS_POS];
            *opMode = (TWiMOD_OperationMode) HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_CMD_PAYLOAD_POS];
        }
    }
    return result;
}

//-----------------------------------------------------------------------------
/**
 * @brief SetOperationMode Cmd - Set the current operation mode of the WiMOD
 *
 *
 * @param   opMode      the new operation mode to set
 * @param   statusRsp   pointer to store status byte of response mesg from WiMOD
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_DevMgmt::SetOperationMode(const TWiMOD_OperationMode opMode, UINT8* statusRsp) {
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    if (statusRsp && (txyPayloadSize >= 1)) {
        txPayload[0] = (UINT8) opMode;
        result = HciParser->SendHCIMessage(DEVMGMT_SAP_ID,
                DEVMGMT_MSG_SET_OPMODE_REQ,
                DEVMGMT_MSG_SET_OPMODE_RSP,
                txPayload, 1);
        // copy response status
        if (WiMODLR_RESULT_OK == result) {
            *statusRsp = HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_STATUS_POS];
        }
    }
    return result;
}

//-----------------------------------------------------------------------------
/**
 * @brief SetRadioMode - Set the current radio mode of the WiMOD
 *
 *
 * @param   radioMode    the new radio mode to set
 * @param   statusRsp   pointer to store status byte of response mesg from WiMOD
 *
 * @warning: This use this command with care!
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_DevMgmt::SetRadioMode(const TRadioCfg_RadioMode radioMode, UINT8* statusRsp) {
     TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
     if (statusRsp && (txyPayloadSize >= 1)) {
         txPayload[0] = (UINT8) radioMode;
         result = HciParser->SendHCIMessage(DEVMGMT_SAP_ID,
                 DEVMGMT_MSG_SET_RADIO_MODE_REQ,
                 DEVMGMT_MSG_SET_RADIO_MODE_RSP,
                 txPayload, 1);
         // copy response status
         if (WiMODLR_RESULT_OK == result) {
             *statusRsp = HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_STATUS_POS];
         }
     }
     return result;
 }


//-----------------------------------------------------------------------------
/**
 * @brief SetAesKey Cmd - Set the 128bit AES that is to be used for encryption
 *
 *
 * @param   key         pointer to the 128bit AES key (pointer to array of 16 UINT8 entries)
 * @param   statusRsp   pointer to store status byte of response mesg from WiMOD
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_DevMgmt::SetAesKey(const UINT8* key, UINT8* statusRsp) {
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    if (statusRsp && key && (txyPayloadSize >= DEVMGMT_AES_KEY_LEN)) {

        memcpy(txPayload, key, DEVMGMT_AES_KEY_LEN);
        result = HciParser->SendHCIMessage(DEVMGMT_SAP_ID,
                DEVMGMT_MSG_SET_AES_KEY_REQ,
                DEVMGMT_MSG_SET_AES_KEY_RSP,
                txPayload, DEVMGMT_AES_KEY_LEN);
        // copy response status
        if (WiMODLR_RESULT_OK == result) {
            *statusRsp = HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_STATUS_POS];
        }
    } else {
        result = WiMODLR_RESULT_PAYLOAD_PTR_ERROR;
    }
    return result;
}

//-----------------------------------------------------------------------------
/**
 * @brief GetAesKey Cmd - Get the 128bit AES that that is used for encryption
 *
 *
 * @param   key         pointer where to store the 128bit AES key (pointer to array of 16 UINT8 entries)
 * @param   statusRsp   pointer to store status byte of response mesg from WiMOD
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_DevMgmt::GetAesKey(UINT8* key, UINT8* statusRsp) {
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    if (statusRsp && key) {

        result = HciParser->SendHCIMessage(DEVMGMT_SAP_ID,
                DEVMGMT_MSG_GET_AES_KEY_REQ,
                DEVMGMT_MSG_GET_AES_KEY_RSP,
                NULL, 0);

        // copy response status
        if (WiMODLR_RESULT_OK == result) {
            *statusRsp = HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_STATUS_POS];
            memcpy(key, &HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_CMD_PAYLOAD_POS],  DEVMGMT_AES_KEY_LEN);
        }
    } else {
        result = WiMODLR_RESULT_PAYLOAD_PTR_ERROR;
    }
    return result;
}

//-----------------------------------------------------------------------------
/**
 * @brief SetRtcAlarm Cmd - Set a new RTC alarm config
 *
 *
 * @param   rtcAlarm   pointer to write the RTC alarm relevant parameter set to
 * @param   statusRsp   pointer to store status byte of response mesg from WiMOD
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_DevMgmt::SetRtcAlarm(const TWiMODLR_DevMgmt_RtcAlarm* rtcAlarm, UINT8* statusRsp) {
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8 			   offset = 0;

    if (statusRsp && rtcAlarm && (txyPayloadSize >= 0x04)) {

    	txPayload[offset++] = (UINT8) rtcAlarm->Options;
    	txPayload[offset++] = (UINT8) rtcAlarm->Hour;
    	txPayload[offset++] = (UINT8) rtcAlarm->Minutes;
    	txPayload[offset++] = (UINT8) rtcAlarm->Seconds;

        result = HciParser->SendHCIMessage(DEVMGMT_SAP_ID,
        		DEVMGMT_MSG_SET_RTC_ALARM_REQ,
				DEVMGMT_MSG_SET_RTC_ALARM_RSP,
                txPayload, offset);

        // copy response status
        if (WiMODLR_RESULT_OK == result) {
            *statusRsp = HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_STATUS_POS];
        }
    } else {
        result = WiMODLR_RESULT_PAYLOAD_PTR_ERROR;
    }
    return result;
}

//-----------------------------------------------------------------------------
/**
 * @brief GetRtcAlarm Cmd - Get information about RTC alarm feature
 *
 *
 * @param   rtcAlarm    pointer where to store the RTC alarm parameter set
 * @param   statusRsp   pointer to store status byte of response mesg from WiMOD
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_DevMgmt::GetRtcAlarm(TWiMODLR_DevMgmt_RtcAlarm* rtcAlarm, UINT8* statusRsp) {
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8			   offset = WiMODLR_HCI_RSP_STATUS_POS;

    result = HciParser->SendHCIMessage(DEVMGMT_SAP_ID,
            DEVMGMT_MSG_GET_RTC_ALARM_REQ,
			DEVMGMT_MSG_GET_RTC_ALARM_RSP,
            NULL, 0);

    // copy response status
    if (WiMODLR_RESULT_OK == result) {
        const TWiMODLR_HCIMessage& rx = HciParser->GetRxMessage();
        *statusRsp = rx.Payload[offset++];

        // status check
        if (*statusRsp == DEVMGMT_STATUS_OK && rx.Length >= 5) {
        	rtcAlarm->AlarmStatus = (TWiMOD_RtcAlarmStatus) rx.Payload[offset++];
        	rtcAlarm->Options     = (TWiMOD_RtcAlarmType)   rx.Payload[offset++];
        	rtcAlarm->Hour		  = (UINT8)					rx.Payload[offset++];
        	rtcAlarm->Minutes	  = (UINT8)					rx.Payload[offset++];
        	rtcAlarm->Seconds	  = (UINT8)                 rx.Payload[offset++];
        }
    } else {
        result = WiMODLR_RESULT_PAYLOAD_PTR_ERROR;
    }
    return result;
}

//-----------------------------------------------------------------------------
/**
 * @brief ClearRtcAlarm Cmd - Clear a pending RTC alarm
 *
 *
 * @param   statusRsp   pointer to store status byte of response mesg from WiMOD
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */

TWiMODLRResultCodes WiMOD_SAP_DevMgmt::ClearRtcAlarm(UINT8* statusRsp){
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;

    if (statusRsp) {
		result = HciParser->SendHCIMessage(DEVMGMT_SAP_ID,
				DEVMGMT_MSG_CLEAR_RTC_ALARM_REQ,
				DEVMGMT_MSG_CLEAR_RTC_ALARM_RSP,
				NULL, 0);
		// copy response status
		if (WiMODLR_RESULT_OK == result) {
			*statusRsp = HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_STATUS_POS];
		}
	}
    return result;
}


//-----------------------------------------------------------------------------
/**
 * @brief SetHCIConfig Cmd - Sets the HCI config options for the LR-BASE_PLUS firmware
 *
 *
 * @param   hciConfig   pointer to the new configuration
 * @param   statusRsp   pointer to store status byte of response mesg from WiMOD
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */

TWiMODLRResultCodes WiMOD_SAP_DevMgmt::SetHCIConfig(const TWiMODLR_DevMgmt_HciConfig* hciConfig, UINT8* statusRsp)
{
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = 0;

    if (hciConfig && statusRsp && (txyPayloadSize >= 0x06)) {
        txPayload[offset++] = (UINT8) hciConfig->StoreNwmFlag;
        txPayload[offset++] = (UINT8) hciConfig->BaudrateID;
        HTON16(&txPayload[offset], hciConfig->NumWakeUpChars);
        offset += 0x02;
        txPayload[offset++] = (UINT8) hciConfig->TxHoldTime;
        txPayload[offset++] = (UINT8) hciConfig->RxHoldTime;

        result = HciParser->SendHCIMessage(DEVMGMT_SAP_ID,
                                        DEVMGMT_MSG_SET_HCI_CFG_REQ,
                                        DEVMGMT_MSG_SET_HCI_CFG_RSP,
                                        txPayload, offset);

        if (result == WiMODLR_RESULT_OK) {
            const TWiMODLR_HCIMessage& rx = HciParser->GetRxMessage();
            offset = WiMODLR_HCI_RSP_STATUS_POS;
            *statusRsp = rx.Payload[offset++];

            // status check
            if (*statusRsp == DEVMGMT_STATUS_OK) {


            } else {
            }
        }
    }
    return result;

}

//-----------------------------------------------------------------------------
/**
 * @brief SetHCIConfig Cmd - Gets the HCI config options for the LR-BASE_PLUS firmware
 *
 *
 * @param   hciConfig   pointer to store the received information
 * @param   statusRsp   pointer to store status byte of response mesg from WiMOD
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes WiMOD_SAP_DevMgmt::GetHCIConfig(TWiMODLR_DevMgmt_HciConfig* hciConfig, UINT8* statusRsp) {
    TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
    UINT8              offset = WiMODLR_HCI_RSP_STATUS_POS;

    if (hciConfig && statusRsp) {
        result = HciParser->SendHCIMessage(DEVMGMT_SAP_ID,
                                           DEVMGMT_MSG_GET_HCI_CFG_REQ,
                                           DEVMGMT_MSG_GET_HCI_CFG_RSP,
                                           NULL, 0);

        if (result == WiMODLR_RESULT_OK) {
            const TWiMODLR_HCIMessage& rx = HciParser->GetRxMessage();

            hciConfig->Status         = rx.Payload[offset++];
            hciConfig->BaudrateID     = (TWiMOD_HCI_Baudrate) rx.Payload[offset++];
            hciConfig->NumWakeUpChars = NTOH16(&rx.Payload[offset]);
            offset += 0x02;
            hciConfig->TxHoldTime     = (UINT8)rx.Payload[offset++];
            hciConfig->RxHoldTime     = (UINT8)rx.Payload[offset++];

            *statusRsp = rx.Payload[WiMODLR_HCI_RSP_STATUS_POS];
        }
    }
    return result;
}


//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for processing a PowerUp Indication message
 *
 *
 * @param   cb          pointer to callback function
 */

void WiMOD_SAP_DevMgmt::RegisterPowerUpIndicationClient(TDevMgmtPowerUpCallback cb)
{
    PowerUpCallack = cb;
}

//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for processing a RTC Alarm Indication message
 *
 *
 * @param   cb          pointer to callback function
 */
void WiMOD_SAP_DevMgmt::RegisterRtcAlarmIndicationClient(TDevMgmtRtcAlarmCallback cb)
{
    RtcAlarmCallback = cb;
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
void WiMOD_SAP_DevMgmt::DispatchDeviceMgmtMessage(TWiMODLR_HCIMessage& rxMsg)
{
    switch (rxMsg.MsgID)
    {
        case DEVMGMT_MSG_POWER_UP_IND:
            if (PowerUpCallack) {
                PowerUpCallack();
            }
            break;
        case DEVMGMT_MSG_RTC_ALARM_IND:
        	if (RtcAlarmCallback) {
        		RtcAlarmCallback();
        	}
        	break;
        default:
            break;
    }
    return;
}


//------------------------------------------------------------------------------
//
// Section private functions
//
//------------------------------------------------------------------------------


