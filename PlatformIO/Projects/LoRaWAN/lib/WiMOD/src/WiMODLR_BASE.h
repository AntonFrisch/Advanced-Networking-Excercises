//------------------------------------------------------------------------------
//! @file WiMODLR_BASE.h
//! @ingroup WiMODLR_BASE
//! <!------------------------------------------------------------------------->
//! @brief Declarations for the High-level Interface for WiMOD LR-Base firmware
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


/**
 * THIS IS AN EXAMPLE IMPLEMENTATION ACCORDING THE THE HCI SPEC: V1.10
 * FOR FIRMWARE: LR-BASE
 */

/*
 * THIS FILE IMPLEMENTS THE HIGH LEVEL API FOR THE WiMOD LR BASE STACK
 */


#ifndef WIMODLR_BASE_H_
#define WIMODLR_BASE_H_

//-----------------------------------------------------------------------------
// Include files needed by this module
//-----------------------------------------------------------------------------
#include "Arduino.h"
#include <string.h>

#include "utils/WMDefs.h"
#include "utils/ComSLIP.h"
#include "HCI/WiMODLRHCI.h"
#include "utils/FreqCalc_SX127x.h"

#include "SAP/WiMOD_SAP_DEVMGMT.h"
#include "SAP/WiMOD_SAP_RadioLink.h"
#include "SAP/WiMOD_SAP_Generic.h"
#include "SAP/WiMOD_SAP_HWTest.h"


/*
 * C++11 supports a better way for function pointers / function objects
 * But C++11 mode is not supported by all platforms.
 */
#ifdef WIMOD_USE_CPP11
#include <functional>
#endif
//-----------------------------------------------------------------------------
// common defines
//-----------------------------------------------------------------------------
//! @cond Doxygen_Suppress
#define WIMOD_LR_BASE_SERIAL_BAUDRATE               115200

#define WiMOD_LR_BASE_TX_BUFFER_SIZE                100
//! @endcond
//-----------------------------------------------------------------------------
// types for callback functions
//-----------------------------------------------------------------------------

// C++11 check
#ifdef WIMOD_USE_CPP11
	/* C++11 function callback definitions */

	/** Type definition for callback that handles a 'RX U-Data event' for the DataLink service */
	typedef std::function<void (UINT8* data, UINT16 length)> TDataLinkRxUDataIndClient;

	/** Type definition for callback  that handles a 'TX U-Data event' for the DataLink service */
	typedef std::function<void (UINT8 status)> TDataLinkSendUdataIndClient;

#else
	/* pre C++11 function callback definitions */

	/** Type definition for callback function that handles a 'RX U-Data event' for the DataLink service */
	typedef void (*TDataLinkRxUDataIndClient)(UINT8* data, UINT16 length);

	/** Type definition for callback function that handles a 'TX U-Data event' for the DataLink service */
	typedef void (*TDataLinkSendUdataIndClient)(UINT8 status);

#endif

//-----------------------------------------------------------------------------
// API class declaration for the WiMOD LR BASE Stack
//
// note: this is NOT compliant with the WiMOD LoRaWAN Stack
//-----------------------------------------------------------------------------

/**
 * @brief Main class representing the interface to the WiMOD running the firmware WiMODLR-BASE
 *
 * This class is the only API class a user should use for interacting with
 * a WiMOD module that runs the IMST WiMODLR-Base firmware.
 *
 */
class WiMODLRBASE : public TWiMODLRHCI {
public:
    /*explicit*/ WiMODLRBASE(Stream& s);
    ~WiMODLRBASE(void);

    void begin(void);
    void end(void);

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

    bool GetRadioConfig(TWiMODLR_DevMgmt_RadioConfig* radioCfg, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool SetRadioConfig(const TWiMODLR_DevMgmt_RadioConfig* radioCfg, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool ResetRadioConfig(TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);

    bool GetOperationMode(TWiMOD_OperationMode* opMode, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool SetOperationMode(const TWiMOD_OperationMode opMode, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);

    bool SetRadioMode(const TRadioCfg_RadioMode radioMode, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);

    bool SetAesKey(const UINT8* key, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool GetAesKey(UINT8* key, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);

    bool SetRtcAlarm(const TWiMODLR_DevMgmt_RtcAlarm* rtcAlarm, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool GetRtcAlarm(TWiMODLR_DevMgmt_RtcAlarm* rtcAlarm, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool ClearRtcAlarm(TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);

    void RegisterPowerUpIndicationClient(TDevMgmtPowerUpCallback cb);
    void RegisterRtcAlarmIndicationClient(TDevMgmtRtcAlarmCallback cb);

    /*
     * Radio Link SAP
     */
    bool SendUData(const TWiMODLR_RadioLink_Msg* txMsg, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
    bool SendCData(const TWiMODLR_RadioLink_Msg* txMsg, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);

    bool SetAckData(const TWiMODLR_RadioLink_Msg* txMsg, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);

    bool convert(TWiMODLR_HCIMessage& RxMsg, TWiMODLR_RadioLink_Msg* radioLinkMsg);
    bool convert(TWiMODLR_HCIMessage& RxMsg, TWiMODLR_RadioLink_CdataInd* cDataTxInfo);
    bool convert(TWiMODLR_HCIMessage& RxMsg, TWiMODLR_RadioLink_UdataInd* cDataTxInfo);
    bool convert(TWiMODLR_HCIMessage& RxMsg, TWiMODLR_AckTxInd_Msg* txAckIndInfo);

    void RegisterUDataRxClient(TRadioLinkUDataRxIndicationCallback cb);
    void RegisterUDataTxClient(TRadioLinkUDataTxIndicationCallback cb);
    void RegisterRawDataRxClient(TRadioLinkRawDataRxIndicationCallback cb);
    void RegisterCDataRxClient(TRadioLinkCDataRxIndicationCallback cb);
    void RegisterCDataTxClient(TRadioLinkCDataTxIndicationCallback cb);
    void RegisterAckRxClient(TRadioLinkAckRxIndicationCallback cb);
    void RegisterAckRxTimeoutClient(TRadioLinkAckRxTimeoutIndicationCallback cb);
    void RegisterAckTxCallback(TRadioLinkAckTxIndicationCallback cb);


    /*
     * Generic Cmd
     */
    bool ExecuteGenericCmd(TWiMODLR_Generic_CmdInfo* info, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);

    /*
     * Hardware Test SAP
     */
//    bool StartRadioTest(const TWiMODLR_HWTestParameter* testParams, TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);
//    bool StopRadioTest(TWiMODLRResultCodes* hciResult = NULL, UINT8* rspStatus = NULL);


    void calcFreqToRegister(uint32_t freq, uint8_t* msb, uint8_t* mid, uint8_t* lsb);
    uint32_t calcRegisterToFreq(uint8_t msb, uint8_t mid, uint8_t lsb);


    TWiMODLRResultCodes  GetLastHciResult(void);
    UINT8               GetLastResponseStatus(void);

protected:

    bool               copyResultInfos(TWiMODLRResultCodes* hciResult, UINT8* rspStatus, UINT8 successValue);


    virtual void       ProcessUnexpectedRxMessage(TWiMODLR_HCIMessage& rxMsg);

    WiMOD_SAP_DevMgmt   SapDevMgmt;                                             /*!< Service Access Point for 'DeviceManagement' */
    WiMOD_SAP_RadioLink SapRadioLink;                                           /*!< Service Access Point for 'RadioLink' */
    WiMOD_SAP_Generic   SapGeneric;												/*!< dumy SAP for generic HCI command */
//    WiMOD_SAP_HWTest	SapHwTest;												/*!< Service Access Point for 'HW Test' */
private:
    //! @cond Doxygen_Suppress
    UINT8               txBuffer[WiMOD_LR_BASE_TX_BUFFER_SIZE];

    UINT8               localStatusRsp;
    bool                cmdResult;

    TWiMODLRResultCodes  localHciRes;

    TWiMODLRResultCodes  lastHciRes;
    UINT8               lastStatusRsp;
    //! @endcond
};



#endif /* WIMODLR_BASE_H_ */
