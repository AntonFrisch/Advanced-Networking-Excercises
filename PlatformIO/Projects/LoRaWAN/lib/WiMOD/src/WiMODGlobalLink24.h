//------------------------------------------------------------------------------
//! @file WiMODGlobalLink24.h
//! @ingroup WiMODGlobalLink24
//! <!------------------------------------------------------------------------->
//! @brief Declarations for the High-level Interface for WiMOD GlobalLink24 EndNode Modem firmware
//! @version 0.1
//! <!------------------------------------------------------------------------->
//!
//!
//!
//! <!--------------------------------------------------------------------------
//! Copyright (c) 2020
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


#ifndef ARDUINO_WIMODGLOBALLINK24_H_
#define ARDUINO_WIMODGLOBALLINK24_H_



/**
 * THIS IS AN EXAMPLE IMPLEMENTATION ACCORDING THE THE HCI SPEC: V0.01
 * FOR FIRMWARE: GlobalLink24
 */


//------------------------------------------------------------------------------
//
// Section Includes Files
//
//------------------------------------------------------------------------------

#include "Arduino.h"
//#include <cstddef>
#include <string.h>

//#include "SAP/WiMOD_SAP_LORAWAN.h"
#include "SAP/WiMOD_SAP_GlobalLink24.h"
#include "SAP/WiMOD_SAP_DEVMGMT.h"
#include "SAP/WiMOD_SAP_Generic.h"
#include "utils/ComSLIP.h"
#include "HCI/WiMODLRHCI.h"

#include "WiMODLoRaWAN.h"

//-----------------------------------------------------------------------------
// common defines
//-----------------------------------------------------------------------------
//! @cond Doxygen_Suppress
#define WIMOD_GLOBALLINK24_SERIAL_BAUDRATE               115200

#define WiMOD_GLOBALLINK24_TX_BUFFER_SIZE                256
//! @endcond
//-----------------------------------------------------------------------------
// types for callback functions
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// API class declaration for the WiMOD GlobalLink24 Stack
//
//-----------------------------------------------------------------------------
/**
 * @brief Main class representing the interface to the WiMOD running the firmware GlobalLink24 EndNode Modem
 *
 * This class is the only API class a user should use for interacting with
 * a WiMOD module that runs the IMST GlobalLink24 EndNode Modem firmware.
 *
 */
class WiMODGlobalLink24 {
public:
    /*explicit*/ WiMODGlobalLink24(Stream& s);
    ~WiMODGlobalLink24(void);

    void begin(TLoRaWANregion region = LoRaWAN_Region_proprietary_WW2G4);
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
     * GlobalLink24 (LoRaWAN emulation) SAP
     */
    bool ActivateDevice(TWiMODGlobalLink24_ActivateDeviceData& activationData, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool ReactivateDevice(UINT32* devAdr, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool SetJoinParameter(TWiMODGlobalLink24_JoinParams& joinParams, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool JoinNetwork(TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);

    void RegisterJoinTxIndicationClient(TJoinTxIndicationCallback cb);

    bool
    convert(TWiMODLR_HCIMessage& RxMsg, TWiMODGlobalLink24_RX_Data* globalLink24RxData);
    bool
    convert(TWiMODLR_HCIMessage& rxMsg, TWiMODGlobalLink24_TxIndData* sendIndData);

    bool
    convert(TWiMODLR_HCIMessage& RxMsg, TWiMODGlobalLink24_RX_MacCmdData* globalLink24MacCmdData);

    bool
    convert(TWiMODLR_HCIMessage& RxMsg,TWiMODGlobalLink24_RX_JoinedNwkData* joinedNwkData);

    bool
    convert(TWiMODLR_HCIMessage& RxMsg, TWiMODGlobalLink24_RX_ACK_Data* ackData);

    bool
    convert(TWiMODLR_HCIMessage& RxMsg, TWiMODGlobalLink24_NoData_Data* info);

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

    bool SendUData(const TWiMODGlobalLink24_TX_Data* data, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool SendCData(const TWiMODGlobalLink24_TX_Data* data,TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool SetRadioStackConfig(TWiMODGlobalLink24_RadioStackConfig* data,TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool GetRadioStackConfig(TWiMODGlobalLink24_RadioStackConfig* data, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool DeactivateDevice(TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool FactoryReset(TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool SetDeviceEUI(const UINT8* deviceEUI, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool GetDeviceEUI(UINT8* deviceEUI, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
//    bool GetNwkStatus(UINT8* nwkStatus, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL); // implementation for spec up to  V1.13
    bool GetNwkStatus(TWiMODGlobalLink24_NwkStatus_Data* nwkStatus, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL); // new implementation for spec. V1.14
    bool SendMacCmd(const TWiMODGlobalLink24_MacCmd* cmd, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool SetCustomConfig(const INT8 rfGain, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool GetCustomConfig(INT8* rfGain, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool GetSupportedBands(TWiMODGlobalLink24_SupportedBands* supportedBands, TWiMODLRResultCodes*  hciResult = NULL, UINT8* rspStatus = NULL);


    bool GetLinkAdrReqConfig(TWiMODGlobalLink24_LinkAdrReqConfig* linkAdrReqCfg, TWiMODLRResultCodes*  hciResult = NULL, UINT8* rspStatus = NULL);
    bool SetLinkAdrReqConfig(TWiMODGlobalLink24_LinkAdrReqConfig& linkAdrReqCfg, TWiMODLRResultCodes*  hciResult = NULL, UINT8* rspStatus = NULL);
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

    void  Process(void);
    void  EnableWakeupSequence(bool flag);
protected:
    WiMODLoRaWAN GlobalLinkImp;
private:
};


#endif /* ARDUINO_WIMODGLOBALLINK24_H_ */
