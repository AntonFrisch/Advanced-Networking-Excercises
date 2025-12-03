//------------------------------------------------------------------------------
//! @file WiMODLoRaWAN.h
//! @ingroup WiMODLoRaWAN
//! <!------------------------------------------------------------------------->
//! @brief Declarations for the High-level Interface for WiMOD LoRaWAN EndNode Modem firmware
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


#ifndef ARDUINO_WIMODLORAWAN_H_
#define ARDUINO_WIMODLORAWAN_H_



/**
 * THIS IS AN EXAMPLE IMPLEMENTATION ACCORDING THE THE HCI SPEC: V1.26
 * FOR FIRMWARE: LoRaWAN
 */


//------------------------------------------------------------------------------
//
// Section Includes Files
//
//------------------------------------------------------------------------------

#include "Arduino.h"
//#include <cstddef>
#include <string.h>

#include "SAP/WiMOD_SAP_LORAWAN.h"
#include "SAP/WiMOD_SAP_DEVMGMT.h"
#include "SAP/WiMOD_SAP_Generic.h"
#include "utils/ComSLIP.h"
#include "HCI/WiMODLRHCI.h"

//-----------------------------------------------------------------------------
// common defines
//-----------------------------------------------------------------------------
//! @cond Doxygen_Suppress
#define WIMOD_LORAWAN_SERIAL_BAUDRATE               115200

#define WiMOD_LORAWAN_TX_BUFFER_SIZE                256
//! @endcond
//-----------------------------------------------------------------------------
// types for callback functions
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// API class declaration for the WiMOD LR BASE Stack
//
// note: this is NOT compliant with the WiMOD LoRaWAN Stack
//-----------------------------------------------------------------------------
/**
 * @brief Main class representing the interface to the WiMOD running the firmware LoRaWAN EndNode Modem
 *
 * This class is the only API class a user should use for interacting with
 * a WiMOD module that runs the IMST LoRaWAN EndNode Modem firmware.
 *
 */
class WiMODLoRaWAN : public TWiMODLRHCI {
public:
    /*explicit*/ WiMODLoRaWAN(Stream& s);
    ~WiMODLoRaWAN(void);

    void begin(TLoRaWANregion region = LoRaWAN_Region_EU868);
    void end(void);

    //! @cond Doxygen_Suppress
    void beginAndAutoSetup(void);
    void autoSetupSupportedRegion(void);
    //! @endcond

    /*
     * DevMgmt SAP
     */
    bool Ping(TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool Reset(TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool GetDeviceInfo(TWiMODLR_DevMgmt_DevInfo* info, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool GetFirmwareInfo(TWiMODLR_DevMgmt_FwInfo* info, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool GetSystemStatus(TWiMODLR_DevMgmt_SystemStatus* info, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool GetRtc(UINT32* rtcTime,TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool SetRtc(const UINT32 rtcTime, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);

    bool GetOperationMode(TWiMOD_OperationMode* opMode, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool SetOperationMode(const TWiMOD_OperationMode opMode, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);

    bool SetRtcAlarm(const TWiMODLR_DevMgmt_RtcAlarm* rtcAlarm, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool GetRtcAlarm(TWiMODLR_DevMgmt_RtcAlarm* rtcAlarm, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool ClearRtcAlarm(TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);

    void RegisterPowerUpIndicationClient(TDevMgmtPowerUpCallback cb);
    void RegisterRtcAlarmIndicationClient(TDevMgmtRtcAlarmCallback cb);

    bool GetHciConfig(TWiMODLR_DevMgmt_HciConfig* hciConfig, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool SetHciConfig(const TWiMODLR_DevMgmt_HciConfig* hciConfig, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);


    /*
     * LoRaWAN SAP
     */
    bool ActivateDevice(TWiMODLORAWAN_ActivateDeviceData& activationData,TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool ReactivateDevice(UINT32* devAdr, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool SetJoinParameter(TWiMODLORAWAN_JoinParams& joinParams, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool JoinNetwork(TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);

    void RegisterJoinTxIndicationClient(TJoinTxIndicationCallback cb);

    bool
    convert(TWiMODLR_HCIMessage& RxMsg, TWiMODLORAWAN_RX_Data* loraWanRxData);
    bool
    convert(TWiMODLR_HCIMessage& rxMsg, TWiMODLORAWAN_TxIndData* sendIndData);

    bool
    convert(TWiMODLR_HCIMessage& RxMsg, TWiMODLORAWAN_RX_MacCmdData* loraWanMacCmdData);

    bool
    convert(TWiMODLR_HCIMessage& RxMsg,TWiMODLORAWAN_RX_JoinedNwkData* joinedNwkData);

    bool
    convert(TWiMODLR_HCIMessage& RxMsg, TWiMODLORAWAN_RX_ACK_Data* ackData);

    bool
	convert(TWiMODLR_HCIMessage& RxMsg, TWiMODLORAWAN_NoData_Data* info);

    void
    RegisterNoDataIndicationClient(TNoDataIndicationCallback cb);

    void
    RegisterTxCDataIndicationClient(TTxCDataIndicationCallback cb);

    void
    RegisterTxUDataIndicationClient(TTxUDataIndicationCallback cb);

    void
    RegisterRxUDataIndicationClient(TRxUDataIndicationCallback cb);

    void
    RegisterRxCDataIndicationClient(TRxCDataIndicationCallback cb);

    void
    RegisterRxMacCmdIndicationClient(TRxMacCmdIndicationCallback cb);

    void
    RegisterJoinedNwkIndicationClient(TJoinedNwkIndicationCallback cb);

    void
    RegisterRxAckIndicationClient(TRxAckIndicationCallback cb);

    bool SendUData(const TWiMODLORAWAN_TX_Data* data, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool SendCData(const TWiMODLORAWAN_TX_Data* data,TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool SetRadioStackConfig(TWiMODLORAWAN_RadioStackConfig* data,TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool GetRadioStackConfig(TWiMODLORAWAN_RadioStackConfig* data, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool DeactivateDevice(TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool FactoryReset(TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool SetDeviceEUI(const UINT8* deviceEUI, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool GetDeviceEUI(UINT8* deviceEUI, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
//    bool GetNwkStatus(UINT8* nwkStatus, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL); // implementation for spec up to  V1.13
    bool GetNwkStatus(TWiMODLORAWAN_NwkStatus_Data*	nwkStatus, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL); // new implementation for spec. V1.14
    bool SendMacCmd(const TWiMODLORAWAN_MacCmd* cmd, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool SetCustomConfig(const INT8 rfGain, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool GetCustomConfig(INT8* rfGain, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool GetSupportedBands(TWiMODLORAWAN_SupportedBands* supportedBands, TWiMODLRResultCodes*  hciResult = NULL, UINT8* rspStatus = NULL);
    bool GetTxPowerLimitConfig(TWiMODLORAWAN_TxPwrLimitConfig* txPwrLimitCfg, TWiMODLRResultCodes*  hciResult = NULL, UINT8* rspStatus = NULL);
    bool SetTxPowerLimitConfig(TWiMODLORAWAN_TxPwrLimitConfig& txPwrLimitCfg, TWiMODLRResultCodes*  hciResult = NULL, UINT8* rspStatus = NULL);
    bool GetLinkAdrReqConfig(TWiMODLORAWAN_LinkAdrReqConfig* linkAdrReqCfg, TWiMODLRResultCodes*  hciResult = NULL, UINT8* rspStatus = NULL);
    bool SetLinkAdrReqConfig(TWiMODLORAWAN_LinkAdrReqConfig& linkAdrReqCfg, TWiMODLRResultCodes*  hciResult = NULL, UINT8* rspStatus = NULL);
    bool SetBatteryLevelStatus(UINT8 battStatus, TWiMODLRResultCodes*  hciResult = NULL, UINT8* rspStatus = NULL);

    /*
     * Generic Cmd
     */
    bool ExecuteGenericCmd(TWiMODLR_Generic_CmdInfo* info, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);

    /*
     * Info & QuickStart Cmds
     */
    void PrintBasicDeviceInfo(Stream& s);
    void ConnectViaOTAA(const uint8_t* appEUI = NULL, const uint8_t* appKey = NULL);
    void ConvertAppEuiStrToArray(char* appEuiStr, uint8_t* appEuiArray);
    void ConvertAppKeyStrToArray(char* appKeyStr, uint8_t* appKeyArray);

    void ConvertNwkSKeyStrToArray(char* nwkSKeyStr, uint8_t* nwkSKeyArray);
    void ConvertAppSKeyStrToArray(char* appSKeyStr, uint8_t* appSKeyArray);

    TWiMODLRResultCodes  GetLastHciResult(void);
    UINT8               GetLastResponseStatus(void);

protected:
    WiMOD_SAP_DevMgmt   SapDevMgmt;                                             /*!< Service Access Point for 'DeviceManagement' */
    WiMOD_SAP_LoRaWAN   SapLoRaWan;                                             /*!< Service Access Point for 'LoRaWAN' */
    WiMOD_SAP_Generic   SapGeneric;												/*!< dumy SAP for generic HCI command */


    virtual void       ProcessUnexpectedRxMessage(TWiMODLR_HCIMessage& rxMsg);


    bool               copyLoRaWanResultInfos(TWiMODLRResultCodes* hciResult, UINT8* rspStatus);
    bool               copyDevMgmtResultInfos(TWiMODLRResultCodes* hciResult, UINT8* rspStatus);
private:
    //! @cond Doxygen_Suppress
    UINT8               txBuffer[WiMOD_LORAWAN_TX_BUFFER_SIZE];

    UINT8               localStatusRsp;
    bool                cmdResult;
    TWiMODLRResultCodes  localHciRes;

    TWiMODLRResultCodes  lastHciRes;
    UINT8               lastStatusRsp;
    //! @endcond
};


#endif /* ARDUINO_WIMODLORAWAN_H_ */
