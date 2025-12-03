//------------------------------------------------------------------------------
//! @file WiMODLR_BASE_PLUS.cpp
//! @ingroup WiMODLR_BASE_PLUS
//! <!------------------------------------------------------------------------->
//! @brief High-level Interface class to the WiMOD module running the WiMOD LR-Base Plus firmware
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

#include <WiMODLR_BASE_PLUS.h>


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
 * @param s     Reference to the serial interace to be used for communication.
 *              The serial interface must be initialized outside of this class
 *              prior calling the first command in runtime.
 *
 * @code
 * #include <WiMODLR_BASE.h>
 *
 * WiMODLRBASE wimod(Serial3);  // use the Arduino Serial3 as serial interface
 *
 * void setup() {
 *   // init stack
 *   wimod.begin();
 *   ...
 * }
 * @endcode
 */
WiMODLRBASE_PLUS::WiMODLRBASE_PLUS(Stream& s) :
    TWiMODLRHCI(s),
    SapDevMgmt(this, txBuffer, WiMOD_LR_BASE_PLUS_TX_BUFFER_SIZE),
    SapRadioLink(this, txBuffer, WiMOD_LR_BASE_PLUS_TX_BUFFER_SIZE),
    SapGeneric(this, txBuffer, WiMOD_LR_BASE_PLUS_TX_BUFFER_SIZE),
    SapRLT(this, txBuffer, WiMOD_LR_BASE_PLUS_TX_BUFFER_SIZE),
    SapSensor(this, txBuffer, WiMOD_LR_BASE_PLUS_TX_BUFFER_SIZE),
    SapRemoteCtrl(this, txBuffer, WiMOD_LR_BASE_PLUS_TX_BUFFER_SIZE)
    //SapHwTest(this, txBuffer, WiMOD_LR_BASE_PLUS_TX_BUFFER_SIZE)
{
    cmdResult       = false;
    localStatusRsp  = 0;
    localHciRes     = WiMODLR_RESULT_TRANMIT_ERROR;
    lastHciRes      = WiMODLR_RESULT_TRANMIT_ERROR;
    lastStatusRsp   = 0;
    memset(txBuffer, 0x00, WiMOD_LR_BASE_PLUS_TX_BUFFER_SIZE);
}

//-----------------------------------------------------------------------------
/**
 * @brief Destructor
 */
WiMODLRBASE_PLUS::~WiMODLRBASE_PLUS(void) {

}

//-----------------------------------------------------------------------------
/**
 * @brief Setup internal things
 *
 * This function must be executed at startup before any other service can
 * be used.
 *
 * @code
 * #include <WiMODLR_BASE_PLUS.h>
 *
 * WiMODLRBASE_PLUS wimod(Serial3);  // use the Arduino Serial3 as serial interface
 *
 * void setup() {
 *  // init stack
 *  wimod.begin();
 *  ...
 * }
 * @endcode
 */
void WiMODLRBASE_PLUS::begin(void) {
    TWiMODLRHCI::begin();
}

//-----------------------------------------------------------------------------
/**
 * @brief De-Setup internal things
 *
 */
void WiMODLRBASE_PLUS::end(void) {
    TWiMODLRHCI::end();
}

//===============================================================================
//
// DevMgmt
//
//===============================================================================

//-----------------------------------------------------------------------------
/**
 * @brief Ping Cmd - Checks serial connetion to WiMOD module
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * @code
 * if (wimod.Ping()) {
 *      // success ...
 * } else {
 *      // error ...
 * }
 * @endcode
 *
 * OR:
 *
 * @code
 * TWiMODLRResultCodes hciResult;
 *
 * if (!wimod.Ping(&hciResult)) {
 *      // error: check HCI result codes
 *      if (WiMODLR_RESULT_TRANMIT_ERROR == hciResult) {...}
 *      if (WiMODLR_RESULT_NO_RESPONSE   == hciResult) {...}
 * }
 * @endcode
 *
 * OR:
 *
 * @code
 * TWiMODLRResultCodes hciResult;
 * uint8               rspCode;
 *
 * if (!wimod.Ping(&hciResult, &rspCode)) {
 *      // error: check HCI result codes
 *      if (WiMODLR_RESULT_OK == hciResult) {
 *          if (rspCode == DEVMGMT_STATUS_ERROR) {
 *              // operation failed
 *          }
 *          ...
 *      }
 *      ...
 * }
 * @endcode
 */
bool WiMODLRBASE_PLUS::Ping(TWiMODLRResultCodes*         hciResult,
                            UINT8*                       rspStatus)
{
    localHciRes = SapDevMgmt.Ping(&localStatusRsp);
    return copyResultInfos(hciResult, rspStatus, (UINT8) DEVMGMT_STATUS_OK);
}

//-----------------------------------------------------------------------------
/**
 * @brief Reset Cmd - Do a local reboot of the WiMOD module
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * @code
 * // do a software reboot of the WiMOD device
 * if (wimod.Reset() {
 *      // ok; reset procedure has been started
 *      // wait a while for the reboot to be finished
 * }
 * ...
 * @endcode
 *
 */
bool WiMODLRBASE_PLUS::Reset(TWiMODLRResultCodes*        hciResult,
                             UINT8*                      rspStatus)
{
    localHciRes = SapDevMgmt.Reset(&localStatusRsp);
    return copyResultInfos(hciResult, rspStatus, DEVMGMT_STATUS_OK);
}

//-----------------------------------------------------------------------------
/**
 * @brief Gets the basic DeviceInfo from WiMOD module
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param info      Pointer where to store the retrieved information
 *                  @see TWiMODLR_DevMgmt_DevInfo for details
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * @code
 * //local variable
 * TWiMODLR_DevMgmt_DevInfo devInfo;
 *
 * // get information of WiMOD
 * if (wimod.GetDeviceInfo(&devInfo)) {
 *      // ok; process received information
 *      ...
 * }
 * ...
 * @endcode
 *
 */
bool WiMODLRBASE_PLUS::GetDeviceInfo(TWiMODLR_DevMgmt_DevInfo*    info,
                                     TWiMODLRResultCodes*         hciResult,
                                     UINT8*                       rspStatus)
{
    localHciRes = SapDevMgmt.GetDeviceInfo(info, &localStatusRsp);
    return copyResultInfos(hciResult, rspStatus, DEVMGMT_STATUS_OK);
}

//-----------------------------------------------------------------------------
/**
 * @brief Gets the basic FirmwareInfo from WiMOD module
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param info      Pointer where to store the retrieved information
 *                  @see TWiMODLR_DevMgmt_FwInfo for details
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * @code
 * //local variable
 * TWiMODLR_DevMgmt_FwInfo fwInfo;
 *
 * // get information of WiMOD
 * if (wimod.GetFirmwareInfo(&fwInfo)) {
 *      // ok; process received information
 *      if (fwInfo.FirmwareMayorVersion = 0x01) {...}
 * }
 * ...
 * @endcode
 */
bool WiMODLRBASE_PLUS::GetFirmwareInfo(TWiMODLR_DevMgmt_FwInfo*    info,
                                       TWiMODLRResultCodes*        hciResult,
                                       UINT8*                      rspStatus)
{
    localHciRes = SapDevMgmt.GetFirmwareInfo(info, &localStatusRsp);
    return copyResultInfos(hciResult, rspStatus, DEVMGMT_STATUS_OK);
}

//-----------------------------------------------------------------------------
/**
 * @brief Gets the bas SystemStatus info from WiMOD module
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param info      Pointer where to store the retrieved information
 *                  @see TWiMODLR_DevMgmt_SystemStatus for details
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * @code
 * //local variable
 * TWiMODLR_DevMgmt_SystemStatusPlus sysStatus;
 *
 * // get information of WiMOD
 * if (wimod.GetSystemStatus(&sysStatus)) {
 *      // ok; process received information
 *      if (sysStatus.BatteryStatus < 2500) { //low batt. voltage detected }
 * }
 * ...
 * @endcode
 */
bool WiMODLRBASE_PLUS::GetSystemStatus(TWiMODLR_DevMgmt_SystemStatusPlus* info,
                                       TWiMODLRResultCodes*               hciResult,
                                       UINT8*                             rspStatus)
{
    localHciRes = SapDevMgmt.GetSystemStatus(info, &localStatusRsp);
    if (info) {
        localStatusRsp = info->Status;
    }
    return copyResultInfos(hciResult, rspStatus, DEVMGMT_STATUS_OK);
}

//-----------------------------------------------------------------------------
/**
 * @brief Gets the current RTC data info from WiMOD module
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param rtcTime   Pointer where to store the RTC information
 *                  @see WIMOD_RTC_GET_* Macros for conversation of this value
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * @code
 * // create a local variable
 * uint32_t timestamp;
 *
 * // get information of WiMOD
 * wimod.GetRtc(&timestamp);
 *
 * //access fields of structure
 * printf("Year : %d:", WIMOD_RTC_GET_YEARS(timestamp));
 * printf("Month: %d:", WIMOD_RTC_GET_MONTHS(timestamp));
 * printf("Day  : %d:", WIMOD_RTC_GET_DAYS(timestamp));
 * printf("Hour : %d:", WIMOD_RTC_GET_HOURS(timestamp));
 * printf("Min  : %d:", WIMOD_RTC_GET_MINUTES(timestamp));
 * printf("Sec  : %d:", WIMOD_RTC_GET_SECONDS(timestamp));
 * ...
 * @endcode
 */
bool WiMODLRBASE_PLUS::GetRtc(UINT32*                     rtcTime,
                              TWiMODLRResultCodes*        hciResult,
                              UINT8*                      rspStatus)
{
    localHciRes = SapDevMgmt.GetRtc(rtcTime, &localStatusRsp);
    return copyResultInfos(hciResult, rspStatus, DEVMGMT_STATUS_OK);
}

//-----------------------------------------------------------------------------
/**
 * @brief Sets the current RTC values to WiMOD module
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param rtcTime   RTC information to setup
 *                  @see WIMOD_RTC_MAKE_DATETIME_U32 Macro for conversation of this value
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * @code
 * // create a local variable
 * uint32_t timestamp;
 *
 * // convert date/time to timestamp format ( 1st Oct 2016 16:12:55 )
 * timestamp = WIMOD_RTC_MAKE_DATETIME_U32(55, 12, 16, 1, 10, 2016);
 *
 * // set information to WiMOD
 * wimod.SetRtc(timestamp);
 * ...
 * @endcode
 */
bool WiMODLRBASE_PLUS::SetRtc(const UINT32                 rtcTime,
                              TWiMODLRResultCodes*         hciResult,
                              UINT8*                       rspStatus)
{
    localHciRes = SapDevMgmt.SetRtc(rtcTime, &localStatusRsp);
    return copyResultInfos(hciResult, rspStatus, DEVMGMT_STATUS_OK);
}


//-----------------------------------------------------------------------------
/**
 * @brief Gets the current configuration of the radio of the WiMOD module
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param radioCfg  Pointer where to store the information
 *                  @see TWiMODLR_DevMgmt_RadioConfig for details
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * @code
 * // create a local variable
 * TWiMODLR_DevMgmt_RadioConfigPlus radioCfg;
 *
 * // get information from WiMOD
 * if (wimod.GetRadioConfig(&radioCfg)) {
 *      // access the fields
 *      if (radioCfg.RadioMode != RadioMode_Standard) {
 *          // error unsupported mode
 *      }
 * if (radioCfg.Modulation == LRBASE_PLUS_Modulation_FLRC) {...}
 *      uint32_t freq = FreqCalc_calcRegisterToFreq(radioCfg.RfFreq_MSB,
 *                              radioCfg.RfFreq_MID,
 *                              radioCfg.RfFreq_LSB);
 *      ...
 * }
 * ...
 * @endcode
 */
bool WiMODLRBASE_PLUS::GetRadioConfig(TWiMODLR_DevMgmt_RadioConfigPlus* radioCfg,
                                      TWiMODLRResultCodes*              hciResult,
                                      UINT8*                            rspStatus)
{
    localHciRes = SapDevMgmt.GetRadioConfig(radioCfg, &localStatusRsp);
    return copyResultInfos(hciResult, rspStatus, DEVMGMT_STATUS_OK);
}

//-----------------------------------------------------------------------------
/**
 * @brief Sets the current configuration for the radio of the WiMOD module
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param radioCfg  Pointer where to read the information
 *                  @see TWiMODLR_DevMgmt_RadioConfig for details
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * @code
 * // create a local variable
 * TWiMODLR_DevMgmt_RadioConfigPlus radioCfg;
 *
 * // setup new radio config
 * radioCfg.RadioMode    = RadioMode_Standard;
 * radioCfg.StoreNwmFlag = 0x01; // store new config permanently
 * radioCfg.Modulation   = LRBASE_PLUS_Modulation_LoRa;
 * radioCfg.PowerLevel   = LRBASE_PLUS_TxPowerLevel_p10_dBm;
 * ...
 * FreqCalc_calcFreqToRegister(2458300000,
 *                             &radioCfg.RfFreq_MSB,
 *                             &radioCfg.RfFreq_MID,
 *                             &radioCfg.RfFreq_LSB);
 * ...
 *
 * // set information from WiMOD
 * if (wimod.SetRadioConfig(&radioCfg)) {
 * //ok new config has been setup
 * }
 * ...
 * @endcode
 */
bool WiMODLRBASE_PLUS::SetRadioConfig(const TWiMODLR_DevMgmt_RadioConfigPlus* radioCfg,
                                      TWiMODLRResultCodes*                    hciResult,
                                      UINT8*                                  rspStatus)
{
    uint8_t wrongParamBitMask[4] = {0x00};
    localHciRes = SapDevMgmt.SetRadioConfig(radioCfg, &localStatusRsp, wrongParamBitMask);

    (void) wrongParamBitMask;

    wrongParamBitMask[1] = wrongParamBitMask[0];
    return copyResultInfos(hciResult, rspStatus, DEVMGMT_STATUS_OK);
}

//-----------------------------------------------------------------------------
/**
 * @brief Resets the current configuration to defaults
 *
 * @see HCI Documentation for details of default settings
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * @code
 * ...
 * // reset radio config from WiMOD to defaults
 * if (wimod.ResetRadioConfig()) {
 *      //ok config has been setup to defaults
 * }
 * ...
 * @endcode
 */
bool WiMODLRBASE_PLUS::ResetRadioConfig(TWiMODLRResultCodes*         hciResult,
                                         UINT8*                      rspStatus)
{
    localHciRes = SapDevMgmt.ResetRadioConfig(&localStatusRsp);
    return copyResultInfos(hciResult, rspStatus, DEVMGMT_STATUS_OK);
}


//-----------------------------------------------------------------------------
/**
 * @brief Gets the current operation mode of the WiMOD module
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param opMode   Pointer where to store the information
 *                  @see TWiMOD_OperationMode for details
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * @code
 * // local variable
 * TWiMOD_OperationMode opMode;
 *
 * // get information from WiMOD
 * if (wimod.GetOperationMode(&opMode)) {
 *      //ok
 *      if (opMode != OperationMode_Application) {
 *          // normal operation is disabled; only test functionality is enabled
 *      }
 * }
 * ...
 * @endcode
 */
bool WiMODLRBASE_PLUS::GetOperationMode(TWiMOD_OperationMode*       opMode,
                                        TWiMODLRResultCodes*        hciResult,
                                        UINT8*                      rspStatus)
{
    localHciRes = SapDevMgmt.GetOperationMode(opMode, &localStatusRsp);
    return copyResultInfos(hciResult, rspStatus, DEVMGMT_STATUS_OK);
}

//-----------------------------------------------------------------------------
/**
 * @brief Sets the current operation mode of the WiMOD module
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param opMode    The new operation mode to set
 *                  @see TWiMOD_OperationMode for details
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * @code
 * // local variable
 * TWiMOD_OperationMode opMode;
 *
 * // setup new opMode
 * opMode = OperationMode_Application;
 *
 * // write information to WiMOD
 * if (wimod.SetOperationMode(opMode)) {
 *      //ok; wait a little bit to setup everthing
 * }
 * ...
 * @endcode
 */
bool WiMODLRBASE_PLUS::SetOperationMode(const TWiMOD_OperationMode  opMode,
                                        TWiMODLRResultCodes*        hciResult,
                                        UINT8*                      rspStatus)
{
    localHciRes = SapDevMgmt.SetOperationMode(opMode, &localStatusRsp);
    return copyResultInfos(hciResult, rspStatus, DEVMGMT_STATUS_OK);
}


//-----------------------------------------------------------------------------
/**
 * @brief SetRadioMode - Set the current radio mode of the WiMOD
 *
 *
 * @param   radioMode    the new radio mode to set
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @warning: This use this command with care!
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 */

bool WiMODLRBASE_PLUS::SetRadioMode(const TRadioCfg_RadioMode   radioMode,
                                    TWiMODLRResultCodes*        hciResult,
                                    UINT8*                      rspStatus)
{
    localHciRes = SapDevMgmt.SetRadioMode(radioMode, &localStatusRsp);
    return copyResultInfos(hciResult, rspStatus, DEVMGMT_STATUS_OK);
}


//-----------------------------------------------------------------------------
/**
 * @brief Sets the security 128bit AES key to use for RF communication
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param key       Pointer to the 128bit AES key to by used
 *
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * @code
 * const uint8_t AesKey[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0C, 0x0D, 0x0E, 0x0F};
 *
 * // write new AES key to WiMOD (LR-Base (PLUS) mode ONLY!)
 * if (wimod.SetAesKey(AesKey)) {
 *      //ok;
 * }
 * ...
 * @endcode
 */
bool WiMODLRBASE_PLUS::SetAesKey(const UINT8*                key,
                                 TWiMODLRResultCodes*        hciResult,
                                 UINT8*                      rspStatus)
{
    localHciRes = SapDevMgmt.SetAesKey(key, &localStatusRsp);
    return copyResultInfos(hciResult, rspStatus, DEVMGMT_STATUS_OK);

}

//-----------------------------------------------------------------------------
/**
 * @brief Gets the used security 128bit AES key
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param key       Pointer to the 128bit AES key that is used
 *
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * @code
 * const uint8_t AesKey[16];
 *
 * // read AES key from WiMOD (LR-Base (PLUS) mode ONLY!)
 * if (wimod.GetAesKey(AesKey)) {
 *      //ok;
 * }
 * ...
 * @endcode
 *
 */
bool WiMODLRBASE_PLUS::GetAesKey(UINT8*                      key,
                                 TWiMODLRResultCodes*        hciResult,
                                 UINT8*                      rspStatus)
{
    localHciRes = SapDevMgmt.GetAesKey(key, &localStatusRsp);
    return copyResultInfos(hciResult, rspStatus, DEVMGMT_STATUS_OK);

}


//-----------------------------------------------------------------------------
/**
 * @brief Sets and enables the RTC alarm feature of the WiMOD
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param rtcAlarm   Pointer to a structure containing the RTC alarm related
 *                   information
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * \msc
 * UserApp,WiMOD;
 * UserApp=>WiMOD  [label="SetRtcAlarm(rtcAlarmData)"];
 * ...;
 * --- [label="RTC triggers alarm"];
 * UserApp<<=WiMOD [label="myRtcAlarmCallback()"];
 * UserApp->UserApp [label="start triggered processing"];
 * \endmsc
 *
 *
 * @code
 * // create a local variable
 * TWiMODLR_DevMgmt_RtcAlarm rtcAlarm;
 *
 * // setup RTC Alarm
 * rtcAlarm.Options = RTC_Alarm_Single;  // single, non repeated alarm
 * rtcAlarm.Hour    = 13;                // alarm at 13:45:20
 * rtcAlarm.Minutes = 32;
 * rtcAlarm.Seconds = 20;
 *
 * // set information of WiMOD
 * wimod.SetRtcAlarm(&rtcAlarm);
 * ...
 * // wait for alarm to occur
 * @endcode
 */
bool WiMODLRBASE_PLUS::SetRtcAlarm(const TWiMODLR_DevMgmt_RtcAlarm* rtcAlarm,
                                   TWiMODLRResultCodes*             hciResult,
                                   UINT8*                           rspStatus)
{
localHciRes = SapDevMgmt.SetRtcAlarm(rtcAlarm, &localStatusRsp);
    return copyResultInfos(hciResult, rspStatus, DEVMGMT_STATUS_OK);
}

//-----------------------------------------------------------------------------
/**
 * @brief Gets information about the RTC alarm feature of the WiMOD
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param rtcAlarm   Pointer to a structure where to store the RTC alarm related
 *                   information
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * @code
 * // create a local variable
 * TWiMODLR_DevMgmt_RtcAlarm rtcAlarm;
 *
 * // get information of WiMOD
 * wimod.GetRtcAlarm(&rtcAlarm);
 * ...
 * // access values
 * if (rtcAlarm.AlarmStatus == RTC_Alarm_Alarm_Set) {
 *      printf("Alarm is active");
 * ...
 * }
 * ...
 * @endcode
 */
bool WiMODLRBASE_PLUS::GetRtcAlarm(TWiMODLR_DevMgmt_RtcAlarm* rtcAlarm,
                                   TWiMODLRResultCodes*       hciResult,
                                   UINT8*                     rspStatus)
{
localHciRes = SapDevMgmt.GetRtcAlarm(rtcAlarm, &localStatusRsp);
    return copyResultInfos(hciResult, rspStatus, DEVMGMT_STATUS_OK);
}

//-----------------------------------------------------------------------------
/**
 * @brief Clears pending RTC Alarms of the WiMOD
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * @code
 * // clear a pending RTC alarm of WiMOD
 * wimod.ClearRtcAlarm();
 * ...
 * @endcode
 */
bool WiMODLRBASE_PLUS::ClearRtcAlarm(TWiMODLRResultCodes* hciResult,
                                     UINT8*               rspStatus)
{
localHciRes = SapDevMgmt.ClearRtcAlarm(&localStatusRsp);
    return copyResultInfos(hciResult, rspStatus, DEVMGMT_STATUS_OK);
}

//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the PowerUp Indication - optional -
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param cb        Pointer a callback function
 *                  @see TDevMgmtPowerUpCallback for details
 *
 * \msc
 * UserApp,WiMOD;
 * UserApp=>WiMOD  [label="RegisterPowerUpIndicationClient(clientCB)"];
 * ...;
 * ---             [label="if WiMOD reboots"];
 * UserApp<<=WiMOD [label="PowerUpCallback()"];
 * \endmsc
 *
 * @code
 * // user defined callback function
 * void myPowerUpCallback(void) {
 *  // WiMOD just powered up
 * ...
 * }
 *
 * void setup() {
 *      ...
 *      // "connect" the user defined to callback to the WiMOD callback feature
 *      wimod.RegisterPowerUpIndicationClient(myPowerUpCallback);
 *      ...
 * }
 * @endcode
 */
void WiMODLRBASE_PLUS::RegisterPowerUpIndicationClient(TDevMgmtPowerUpCallback cb)
{
    SapDevMgmt.RegisterPowerUpIndicationClient(cb);
}

//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the RTC Alarm Indication - optional -
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param cb        Pointer a callback function
 *                  @see TDevMgmtRtcAlarmCallback for details
 * \msc
 * UserApp,WiMOD;
 * UserApp=>WiMOD  [label="RegisterRtcAlarmIndicationClient(clientCB)"];
 * ...;
 * UserApp<<=WiMOD [label="RtcAlarmCallback()"];
 * \endmsc
 *
 * @code
 * // user defined callback function
 * void myRtcAlarmCallback(void) {
 *      // RTC Alarm has just been triggered
 * ...
 * }
 *
 * void setup() {
 *      ...
 *      // "connect" the user defined to callback to the WiMOD callback feature
 *      wimod.RegisterRtcAlarmIndicationClient(myRtcAlarmCallback);
 *      ...
 * }
 * @endcode
 */
void WiMODLRBASE_PLUS::RegisterRtcAlarmIndicationClient(TDevMgmtRtcAlarmCallback cb)
{
SapDevMgmt.RegisterRtcAlarmIndicationClient(cb);
}


//-----------------------------------------------------------------------------
/**
 * @brief Gets the current HCI configuration of the WiMOD
 *
 *
 * @see HCI documentation and feature specification for details
 *
 * @param hciConfig   Pointer to a structure where to store the HCI config related
 *                   information
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * @code
 * // create a local variable
 * TWiMODLR_DevMgmt_HciConfig hciConfig;
 *
 * // get information of WiMOD
 * wimod.GetHciConfig(&hciConfig);
 * ...
 * // access values
 * if (hciConfig.NumWakeUpChars == 40) {
 *      ...
 * }
 * ...
 * @endcode
 */
bool WiMODLRBASE_PLUS::GetHciConfig(TWiMODLR_DevMgmt_HciConfig*     hciConfig,
                                    TWiMODLRResultCodes*            hciResult,
                                    UINT8*                          rspStatus)
{
    localHciRes = SapDevMgmt.GetHCIConfig(hciConfig, &localStatusRsp);
    return copyResultInfos(hciResult, rspStatus, DEVMGMT_STATUS_OK);
}

//-----------------------------------------------------------------------------
/**
 * @brief Sets a new HCI configuration of the WiMOD
 *
 *
 * @see HCI documentation and feature specification for details
 *
 * @warning Changing the Baudrate applies instandly afer the command is executed!
 *
 * @param hciConfig   Pointer to a structure where to store the HCI config related
 *                   information
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * @code
 * // create a local variable
 * TWiMODLR_DevMgmt_HciConfig hciConfig;
 *
 * // create new config values
 * hciConfig.BaudrateID = WimodHciBaudrate_57600;
 * ...
 *
 * // set information of WiMOD
 * wimod.GetHciConfig(&hciConfig);
 * ...
 * @endcode
 */
bool WiMODLRBASE_PLUS::SetHciConfig(const TWiMODLR_DevMgmt_HciConfig* hciConfig,
                                    TWiMODLRResultCodes*              hciResult,
                                    UINT8*                            rspStatus)
{
    localHciRes = SapDevMgmt.SetHCIConfig(hciConfig, &localStatusRsp);
    return copyResultInfos(hciResult, rspStatus, DEVMGMT_STATUS_OK);
}

//===============================================================================
//
// Radio Link Service
//
//===============================================================================

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
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * \msc
 * UserApp,WiMOD,RemotePeer;
 * ... ;
 * UserApp=>WiMOD       [label="SendUData(data)"];
 * WiMOD->RemotePeer    [label="transfer data"];
 * UserApp<<WiMOD       [label="true/false"];
 * ...;
 * \endmsc
 *
 * @code
 * ...
 * // local variable
 * TWiMODLR_RadioLink_Msg txData;
 *
 * // setup tx packet
 * txData.DestinationGroupAddress   = 0x10;
 * txData.DestinationDeviceAddress  = 0x1234;
 * txData.Length =  strlen("Hello World!");
 * strncpy(txData.Payload, "Hello World!", WiMODLORAWAN_APP_PAYLOAD_LEN);
 *
 * // send data
 * wimod.SendUData(&txData);
 * ...
 * @endcode
 */
bool WiMODLRBASE_PLUS::SendUData(const TWiMODLR_RadioLink_Msg* txMsg,
                                 TWiMODLRResultCodes*          hciResult,
                                 UINT8*                        rspStatus)
{
    localHciRes = SapRadioLink.SendUData(txMsg, &localStatusRsp);
    return copyResultInfos(hciResult, rspStatus, RADIOLINK_STATUS_OK);
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
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * \msc
 * UserApp,WiMOD,RemotePeer;
 * ... ;
 * UserApp=>WiMOD     [label="SendUData(data)"];
 * WiMOD->RemotePeer  [label="transfer data"];
 * UserApp<<WiMOD     [label="true/false"];
 * ...;
 * WiMOD << RemotePeer [label= "ACK"];
 * --- [label="if ack callback is registered"];
 * UserApp << WiMOD [label="myAckCallback"];
 * ...;
 *
 * \endmsc
 *
 * @code
 * ...
 * // local variable
 * TWiMODLR_RadioLink_Msg txData;
 *
 * // setup tx packet
 * txData.DestinationGroupAddress   = 0x10;
 * txData.DestinationDeviceAddress  = 0x1234;
 * txData.Length =  strlen("Hello World!");
 * strncpy(txData.Payload, "Hello World!", WiMODLORAWAN_APP_PAYLOAD_LEN);
 *
 * // send data
 * wimod.SendCData(&txData);
 * ...
 * // if ACK callback has been registered: wait for callback
 * // to be called and check the received indication data
 * // in order to get the status of the transmission
 * ...
 * @endcode
 *
 */
bool WiMODLRBASE_PLUS::SendCData(const TWiMODLR_RadioLink_Msg* txMsg,
                                 TWiMODLRResultCodes*          hciResult,
                                 UINT8*                        rspStatus)
{
    localHciRes = SapRadioLink.SendCData(txMsg, &localStatusRsp);
    return copyResultInfos(hciResult, rspStatus, RADIOLINK_STATUS_OK);
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
 *
 *
 * \msc
 * UserApp,WiMOD,RemotePeer;
 * ... ;
 * UserApp=>WiMOD     [label="RegisterUDataRxClient(myRxDataInd)"];
 * ...;
 * --- [label="Peer device want to transfer data"];
 * WiMOD<-RemotePeer  [label="transfer data"];
 * --- [label="if ack callback is registered"];
 * UserApp << WiMOD [label="myRxDataInd()"];
 * UserApp->UserApp [label="further processing"];
 * ...;
 *
 * \endmsc
 *
 * @code
 * // global buffer for incomming messages
 * TWiMODLR_RadioLink_Msg radioRxMsg;
 *
 * void myRxDataInd(TWiMODLR_HCIMessage& rxMsg) {
 *    // convert/copy the raw message to RX radio buffer
 *    if (wimod.convert(rxMsg, &radioRxMsg)) {
 *        // access the radioRxMsg fields
 *
 *        // radioRxMsg.Length
 *        // radioRxMsg.Payload
 *    ...
 *    }
 * }
 *...
 *
 * void setup() {
 *    ...
 *    // init the communication stack
 *    wimod.begin();
 *
 *     // register callbacks for incommig RF messages
 *    wimod.RegisterUDataRxClient(myRxDataInd);
 *    wimod.RegisterCDataRxClient(myRxDataInd);
 *    ...
 * }
 * @endcode
 */
bool WiMODLRBASE_PLUS::convert(TWiMODLR_HCIMessage&    RxMsg,
                               TWiMODLR_RadioLink_Msg* radioLinkMsg) {
    return SapRadioLink.convert(RxMsg, radioLinkMsg);
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
 *
 * \msc
 * UserApp,WiMOD,RemotePeer;
 * ... ;
 * UserApp=>WiMOD     [label="RegisterCDataTxClient(myTxCDataInd)"];
 * ...;
 * UserApp->WiMOD     [label="SendCData(data)"];
 * WiMOD->RemotePeer  [label="transfer data"];
 * --- [label="if callback is registered"];
 * UserApp << WiMOD   [label="myTxCDataInd()"];
 * UserApp->UserApp [label="further processing"];
 * ...;
 *
 * \endmsc
 *
 * @code
 * // global buffer for indication that CData has been send
 * TWiMODLR_RadioLink_CdataInd txDataMsg;
 *
 * void myTxCDataInd(TWiMODLR_HCIMessage& rxMsg) {
 *    // convert/copy the raw message to RX radio buffer
 *    if (wimod.convert(rxMsg, &txDataMsg)) {
 *        // access the fields folding information about the transmission
 *
 *        // txDataMsg.TxEventCounter
 *    ...
 *    }
 * }
 *...
 *
 * void setup() {
 *    ...
 *    // init the communication stack
 *    wimod.begin();
 *
 *     // register callback
 *    wimod.RegisterCDataTxClient(myTxCDataInd);
 *    ...
 * }
 * @endcode
 *
 */
bool WiMODLRBASE_PLUS::convert(TWiMODLR_HCIMessage&         RxMsg,
                               TWiMODLR_RadioLink_CdataInd* cDataTxInfo) {
    return SapRadioLink.convert(RxMsg, cDataTxInfo);
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
 *
 * \msc
 * UserApp,WiMOD,RemotePeer;
 * ... ;
 * UserApp=>WiMOD     [label="RegisterUDataTxClient(myTxUDataInd)"];
 * ...;
 * UserApp->WiMOD     [label="SendUData(data)"];
 * WiMOD->RemotePeer  [label="transfer data"];
 * --- [label="if callback is registered"];
 * UserApp << WiMOD   [label="myTxUDataInd()"];
 * UserApp->UserApp [label="further processing"];
 * ...;
 *
 * \endmsc
 *
 * @code
 * // global buffer for indication that UData has been send
 * TWiMODLR_RadioLink_UdataInd txDataMsg;
 *
 * void myTxCDataInd(TWiMODLR_HCIMessage& rxMsg) {
 *    // convert/copy the raw message to RX radio buffer
 *    if (wimod.convert(rxMsg, &txDataMsg)) {
 *        // access the fields folding information about the transmission
 *
 *        // txDataMsg.TxEventCounter
 *    ...
 *    }
 * }
 *...
 *
 * void setup() {
 *    ...
 *    // init the communication stack
 *    wimod.begin();
 *
 *     // register callback
 *    wimod.RegisterCDataTxClient(myTxCDataInd);
 *    ...
 * }
 * @endcode
 *
 */
bool WiMODLRBASE_PLUS::convert(TWiMODLR_HCIMessage&         RxMsg,
                               TWiMODLR_RadioLink_UdataInd* uDataTxInfo) {
    return SapRadioLink.convert(RxMsg, uDataTxInfo);
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
 *
 * \msc
 * UserApp,WiMOD,RemotePeer;
 * ... ;
 * UserApp=>WiMOD     [label="RegisterAckTxCallback(myTxAckInd)"];
 * ...;
 * WiMOD << RemotePeer [label="transfer C Data"];
 * WiMOD -> RemotePeer [label= "C data ack"];
 * UserApp << WiMOD   [label="myTxAckInd()"];
 * UserApp->UserApp [label="further processing"];
 * ...;
 * \endmsc
 *
 * @code
 * // global buffer
 * TWiMODLR_AckTxInd_Msg txAckData;
 *
 * void myTxAckInd(TWiMODLR_HCIMessage& rxMsg) {
 *    // convert/copy the raw message
 *    if (wimod.convert(rxMsg, &txAckData)) {
 *        // access the fields folding information about the ACK transmission
 *
 *        // txDataMsg.Status
 *    ...
 *    }
 * }
 *...
 *
 * void setup() {
 *    ...
 *    // init the communication stack
 *    wimod.begin();
 *
 *     // register callback
 *    wimod.RegisterAckTxCallback(myTxAckInd);
 *    ...
 * }
 * @endcode
 */
bool WiMODLRBASE_PLUS::convert(TWiMODLR_HCIMessage&   RxMsg,
                               TWiMODLR_AckTxInd_Msg* txAckIndInfo) {
    return SapRadioLink.convert(RxMsg, txAckIndInfo);
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
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * \msc
 * UserApp,WiMOD,RemotePeer;
 * ... ;
 * UserApp -> WiMOD     [label="SetAckData(payload)"];
 * ...;
 * WiMOD << RemotePeer [label="transfer C Data"];
 * WiMOD -> RemotePeer [label= "C data ack + payload"];
 * ...;
 * \endmsc
 *
 * @code
 * // global buffer
 * TWiMODLR_RadioLink_Msg ackPayloadData;
 * ...
 * // if you want that the next ack that will be send should contain
 * // additional payload: setup the data structure
 * ackPayloadData.DestinationGroupAddress = ...;
 * ackPayloadData.DestinationDeviceAddress = ...;
 * ackPayloadData.Length = ...;
 * ackPayloadData.Payload = ...;
 *
 * // prepare the next ack packet
 * wimod.SetAckData(ackPayloadData);
 * ...
 * @endcode
 *
 */
bool WiMODLRBASE_PLUS::SetAckData(const TWiMODLR_RadioLink_Msg* txMsg,
                                  TWiMODLRResultCodes*          hciResult,
                                  UINT8*                        rspStatus)
{
    localHciRes = SapRadioLink.SetAckData(txMsg, &localStatusRsp);
    return copyResultInfos(hciResult, rspStatus, RADIOLINK_STATUS_OK);
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
 * @note The callback function should call the corresponding convert in
 *        order to decode the message properly
 *
 * \msc
 * UserApp,WiMOD,RemotePeer;
 * ... ;
 * UserApp -> WiMOD    [label="RegisterUDataTxClient(rxUDataInd)"];
 * ...;
 * WiMOD << RemotePeer [label="transfer U Data"];
 * --- [label= "if callback is registered"];
 * UserApp << WiMOD    [label="rxUDataInd(U Data)"];
 * ...;
 * \endmsc
 *
 * @code
 * // callback for U data Rx indication
 * void myUDataRxInd(TWiMODLR_HCIMessage& rxMsg) {
 * // use the convert function before accessing the data
 * ...
 * }
 *
 * void setup() {
 * ...
 * // setup the callback
 * wimod.RegisterUDataRxClient(myUDataRxInd);
 * ...
 * }
 *
 * ...
 * @endcode
 */
void WiMODLRBASE_PLUS::RegisterUDataRxClient(TRadioLinkUDataRxIndicationCallback cb)
{
    SapRadioLink.RegisterUDataRxClient(cb);
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
 *
 * \msc
 * UserApp,WiMOD,RemotePeer;
 * ... ;
 * UserApp -> WiMOD    [label="RegisterUDataTxClient(txUDataInd)"];
 * ...;
 * UserApp -> WiMOD    [label="SendUData(data)"];
 * WiMOD -> RemotePeer [label="transfer data"];
 * --- [label= "if callback is registered"];
 * UserApp << WiMOD    [label="txUDataInd()"];
 * ...;
 * \endmsc
 *
 * @code
 * // callback for U data Tx indication
 * void myUDataTxInd() {
 * ...
 * }
 *
 * void setup() {
 * ...
 * // setup the callback
 * wimod.RegisterUDataTxClient(myUDataTxInd);
 * ...
 * }
 *
 * ...
 * @endcode
 */
void WiMODLRBASE_PLUS::RegisterUDataTxClient(TRadioLinkUDataTxIndicationCallback cb)
{
    SapRadioLink.RegisterUDataTxClient(cb);
}

//-----------------------------------------------------------------------------

/**
 * @internal
 * @brief Register a callback function for the RX Raw-Data Indication - optional -
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param cb        Pointer a callback function
 *                  @see TRadioLinkRawDataRxIndicationCallback for details
 *
 * @note The callback function should call the corresponding convert in
 *        order to decode the message properly
 *
 * @endinternal
 */
void WiMODLRBASE_PLUS::RegisterRawDataRxClient(TRadioLinkRawDataRxIndicationCallback cb)
{
    SapRadioLink.RegisterRawDataRxClient(cb);
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
 * @note The callback function should call the corresponding convert in
 *        order to decode the message properly
 *
 * \msc
 * UserApp,WiMOD,RemotePeer;
 * ... ;
 * UserApp -> WiMOD    [label="RegisterCDataTxClient(rxCDataInd)"];
 * ...;
 * WiMOD << RemotePeer [label="transfer C Data"];
 * --- [label= "if callback is registered"];
 * UserApp << WiMOD    [label="rxCDataInd(C Data)"];
 * ...;
 * \endmsc
 *
 * @code
 * // callback for C data Rx indication
 * void myCDataRxInd(TWiMODLR_HCIMessage& rxMsg) {
 *      // use the convert function before accessing the data
 *      ...
 * }
 *
 * void setup() {
 *      ...
 *      // setup the callback
 *      wimod.RegisterCDataRxClient(myCDataRxInd);
 *      ...
 * }
 *
 * ...
 * @endcode
 */
void WiMODLRBASE_PLUS::RegisterCDataRxClient(TRadioLinkCDataRxIndicationCallback cb)
{
    SapRadioLink.RegisterCDataRxClient(cb);
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
 * @note The callback function should call the corresponding convert in
 *        order to decode the message properly
 *
 * \msc
 * UserApp,WiMOD,RemotePeer;
 * ... ;
 * UserApp -> WiMOD    [label="RegisterCDataTxClient(txCDataInd)"];
 * ...;
 * UserApp -> WiMOD    [label="SendCData(data)"];
 * WiMOD -> RemotePeer [label="transfer data"];
 * --- [label= "if callback is registered"];
 * UserApp << WiMOD    [label="txCDataInd()"];
 * ...;
 * \endmsc
 *
 * @code
 * // callback for U data Tx indication
 * void myCDataTxInd(TWiMODLR_HCIMessage& rxMsg) {
 *      // use the convert function before accesing the data
 *      ...
 * }
 *
 * void setup() {
 *      ...
 *      // setup the callback
 *      wimod.RegisterCDataTxClient(myCDataTxInd);
 *      ...
 * }
 *
 * ...
 * @endcode
 */
void WiMODLRBASE_PLUS::RegisterCDataTxClient(TRadioLinkCDataTxIndicationCallback cb)
{
    SapRadioLink.RegisterCDataTxClient(cb);
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
 * @note The callback function should call the corresponding convert in
 *        order to decode the message properly
 *
 * \msc
 * UserApp,WiMOD,RemotePeer;
 * ... ;
 * UserApp -> WiMOD    [label="RegisterAckRxClient(ackRxInd)"];
 * ...;
 * UserApp -> WiMOD    [label="SendCData(data)"];
 * WiMOD -> RemotePeer [label="transfer data"];
 * WiMOD <- RemotePeer [label="Ack"];
 * --- [label= "if callback is registered"];
 * UserApp << WiMOD    [label="ackRxInd()"];
 * ...;
 * \endmsc
 *
 * @code
 * // callback for U data Tx indication
 * void myAckRxInd(TWiMODLR_HCIMessage& rxMsg) {
 *      // use the convert function before accesing the data
 *      ...
 * }
 *
 * void setup() {
 *      ...
 *      // setup the callback
 *      wimod.RegisterAckRxClient(myAckRxInd);
 *      ...
 * }
 *
 * ...
 * @endcode
 */
void WiMODLRBASE_PLUS::RegisterAckRxClient(TRadioLinkAckRxIndicationCallback cb)
{
    SapRadioLink.RegisterAckRxClient(cb);
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
 * \msc
 * UserApp,WiMOD,RemotePeer;
 * ... ;
 * UserApp -> WiMOD    [label="RegisterAckRxTimeoutClient(ackTimeoutInd)"];
 * ...;
 * UserApp -> WiMOD    [label="SendCData(data)"];
 * WiMOD -> RemotePeer [label="transfer data"];
 * WiMOD x- RemotePeer [label="Ack"];
 * --- [label= "if callback is registered"];
 * ...;
 * UserApp << WiMOD    [label="ackTimeoutInd()"];
 * ...;
 * \endmsc
 *
 * @code
 * // callback for a missing ACK packet
 * void myAckRxTimemoutInd() {
 *  // this is called if an expected ACK has NOT been received
 *  // within expected time; it it up to the application to
 *  // react accordingly! (e.g. retransmission the data)
 *   ...
 * }
 *
 * void setup() {
 *      ...
 *      // setup the callback
 *      wimod.RegisterAckRxTimeoutClient(myAckRxTimemoutInd);
 *      ...
 * }
 *
 * ...
 * @endcode
 */
void WiMODLRBASE_PLUS::RegisterAckRxTimeoutClient(TRadioLinkAckRxTimeoutIndicationCallback cb)
{
    SapRadioLink.RegisterAckRxTimeoutClient(cb);
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
 * \msc
 * UserApp,WiMOD,RemotePeer;
 * ... ;
 * UserApp -> WiMOD    [label="RegisterAckTxCallback(ackTxInd)"];
 * ...;
 * WiMOD <- RemotePeer [label="transfer C data"];
 * WiMOD -> RemotePeer [label= "Ack"];
 * --- [label= "if callback is registered"];
 * UserApp << WiMOD    [label="ackTxInd()"];
 * ...;
 * \endmsc
 *
 * @code
 * // callback for C data Ack Tx indication
 * void myAckTxInd() {
 *      // this is called if an ACK has been send
 *      ...
 * }
 *
 * void setup() {
 *      ...
 *      // setup the callback
 *      wimod.RegisterAckTxCallback(myAckTxInd);
 *      ...
 * }
 *
 * ...
 * @endcode
 *
 */
void WiMODLRBASE_PLUS::RegisterAckTxCallback(TRadioLinkAckTxIndicationCallback cb)
{
    SapRadioLink.RegisterAckTxCallback(cb);
}

/**
 * @brief Convert a frequency in Hz to the corresponding low level register values
 *
 *
 * @param freq      Target frequencey in Hz
 *
 * @param msb       Pointer to register value for MSB-Part
 * @param mid       Pointer to register value for MID-Part
 * @param lsb       Pointer to register value for LSB-Part
 */
void WiMODLRBASE_PLUS::calcFreqToRegister(uint32_t freq, uint8_t* msb, uint8_t* mid, uint8_t* lsb)
{
    if (msb && mid && lsb) {
        FreqCalcSX1280_calcFreqToRegister(freq, msb, mid, lsb);
    }
}

/**
 * @brief Convert a frequency registers to frequency in Hz
 *
 * @param msb       register value for MSB-Part
 * @param mid       register value for MID-Part
 * @param lsb       register value for LSB-Part
 *
 * return Frequency in Hz
 */
uint32_t WiMODLRBASE_PLUS::calcRegisterToFreq(uint8_t msb, uint8_t mid, uint8_t lsb)
{
    return FreqCalcSX1280_calcRegisterToFreq(msb, mid, lsb);
}



// ============================================================================

//-----------------------------------------------------------------------------
/**
 * @brief Gets the value of the last local HCI TX status
 *
 * If the optional parameters have been left out (for the normal commands),
 * the user can get the last value back by using this function
 *
 * @retval the last value of the last command issued
 *
 * \msc
 * UserApp,WiMOD;
 * UserApp=>WiMOD   [label="Ping()"];
 * UserApp<<WiMOD   [label="(false)"];
 * UserApp=>WiMOD   [label="GetGetLastHciResult()"];
 * UserApp<<WiMOD   [label="(result of last HCI TX operation)"];
 * ...;
 * \endmsc
 *
 * @code
 * ...
 * if (wimod.Ping() == false) {
 * TWiMODLRResultCodes hciResult;
 *
 * // get (local) result of HCI command transmission
 * hciResult = wimod.GetLastHciResult();
 *
 * // check result code
 * switch (hciResult):
 * case WiMODLR_RESULT_OK:                   // OK, no error
 * case WiMODLR_RESULT_PAYLOAD_LENGTH_ERROR: // given payload is too big
 * case WiMODLR_RESULT_PAYLOAD_PTR_ERROR:    // wrong pointer to payload (NULL?)
 * case WiMODLR_RESULT_TRANMIT_ERROR,        // Error sending data to WiMOD via serial interface
 * case WiMODLR_RESULT_SLIP_ENCODER_ERROR,   // Error during SLIP encoding
 * case WiMODLR_RESULT_NO_RESPONSE           // The WiMOD did not respond to a request command
 *  ...
 * }
 *
 * ...
 * @endcode
 */
TWiMODLRResultCodes WiMODLRBASE_PLUS::GetLastHciResult(void)
{
    return lastHciRes;
}

//-----------------------------------------------------------------------------
/**
 * @brief Gets the value of the last status response value of the WiMOD
 *
 * If the optional parameters have been left out (for the normal commands),
 * the user can get the last value back by using this function
 *
 * @retval the last value of the last command issued
 *
 *
 *  * @code
 * ...
 * if (wimod.Ping() == false) {
 * uint8_t rsp;
 *
 * // get (remote) response code
 * rsp = wimod.GetLastResponseStatus();
 *
 * // check result code
 * switch (rsp):
 * case LoRaWAN_Status_Ok:                    // OK; No error;
 *         // --> check "remote" ResponseStatus
 * case LoRaWAN_Status_Error:                 // general (un-specific) error
 * case LoRaWAN_Status_Cmd_Not_Supported:     // Command is not supported (or unknown)
 * case LoRaWAN_Status_Wrong_parameter:       // Wrong parameter applied
 * case LoRaWAN_Status_Wrong_DeviceMode:      // Device is in wong device mode for specific command
 * case LoRaWAN_Status_Device_Not_Activated:  // Device has NOT been activated; command has not been executed
 * case LoRaWAN_Status_Device_Busy:           // There is another operation pending; and or duty cycle restriction is active
 * case LoRaWAN_Status_Queue_Full:            // internal command queue is full
 * case LoRaWAN_Status_Length_Error:          // some error related to the given length information
 * case LoRaWAN_Status_No_Factory_Settings:   // Factory settings are not available
 * case LoRaWAN_Status_Channel_Blocked:       // Selected channel is blocked (duty cycle?)
 * case LoRaWAN_Status_Channel_Not_Available: // Selected channel is not available
 * ...
 * }
 *
 * ...
 * @endcode
 *
 */
UINT8 WiMODLRBASE_PLUS::GetLastResponseStatus(void)
{
    return lastStatusRsp;
}



//===============================================================================
//
// Generic
//
//===============================================================================

/**
 * @brief Generic Execution Function for HCI commands that are currently not implemented
 *
 * This funtion enables the user to execute various HCI commands that are
 * currently not implemented as commands within this API class.
 *
 * The user has to know the HCI Command IDs and has to decode the response data
 * on his own.
 *
 * @param info      pointer to a structure containing the HCI commands to execute
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * @code
 * ...
 * // prepare command to send to WiMOD
 * TWiMODLR_Generic_CmdInfo cmdInfo;
 * cmdInfo.SapID    = 0x01; // DevMgmt
 * cmdInfo.MsgReqID = 0x01; // Ping Req
 * cmdInfo.MsgRspID = 0x02; // expected response
 *
 * // prepare payload for tx command
 * cmdInfo.CmdPayloadLength = 0
 * cmdInfo.CmdPayload[0] = 0xab;
 * ...
 *
 * // execute command
 * if (wimod.ExecuteGenericCmd(&cmdInfo)) {
 *      // extract cmd reponse data
 *      if (cmdInfo.CmdPayloadLength > 0) {
 *          // repsonse data available
 *          rspData =cmdInfo.CmdPayload[0]
 *          ...
 *          // process data
 *          ...
 *      }
 * }
 *
 * @endcode
 */
bool WiMODLRBASE_PLUS::ExecuteGenericCmd(TWiMODLR_Generic_CmdInfo*   info,
                                         TWiMODLRResultCodes*        hciResult,
                                         UINT8*                      rspStatus)
{
    localHciRes = SapGeneric.ExecuteGenericCmd(info, &localStatusRsp);
    return copyResultInfos(hciResult, rspStatus, DEVMGMT_STATUS_OK);
}

//===============================================================================
//
// Radio Link Test
//
//===============================================================================

//-----------------------------------------------------------------------------
/**
 * @brief Tries to start the integrated Radio Link Test feature of the WiMOD
 *
 * The firmware has got a feature called "Radio Link Test". This feauture can
 * be used to test the radio link between two devices.
 *
 * Please note that the Radio Link Test may interrupt the normal operation of
 * the WiMOD. So it is recommended to use this feature only in isolated
 * test szenarios.
 *
 * Two devices are required: One that akts as "Master" that starts the
 * transmission of test packets and a 2nd one that replies the test packets.
 *
 * For detailed infos please refer to the feature specificaton document(s).
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param params      Data structure containing the setup data for the radio
 *                   link test..
 *                   @see TWiMODLR_RLT_Parameter for details
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * \msc
 * UserApp,WiMOD,RemotePeer;
 * ... ;
 * UserApp=>WiMOD           [label="StartRadioLinkTest(data)"];
 * WiMOD=>WiMOD             [label="Prepare test"];
 * ---                      [label="start test for x times"];
 * WiMOD->RemotePeer        [label="transfer RLT packet"];
 * RemotePeer->WiMOD        [label="RLT packet reply"];
 * UserApp<-WiMOD           [label="Status Indication"];
 * UserApp->UserApp         [label="convert & update statistics"];
 * WiMOD->RemotePeer        [label="transfer RLT packet"];
 * RemotePeer->WiMOD        [label="RLT packet reply"];
 * UserApp<-WiMOD           [label="Status Indication"];
 * UserApp->UserApp         [label="convert & update statistics"];
 * ---                      [label="end of test or"];
 * UserApp=>WiMOD           [label="StopRadioLinkTest()"];
 * ...;
 * \endmsc
 *
 * @code
 * ...
 * // local variable
 * TWiMODLR_RadioLink_Msg params;
 *
 * // setup RLT
 * params.DestinationGroupAddress   = 0x10;
 * params.DestDevAddress            = 0x1234;
 * params.PacketSize                = 10;
 * params.NumPackets                = 20;
 * params.TestMode                  = RLT_TestMode_Single;
 *
 * // register status update client in order to get statistic data
 * wimod.RegisterRltStatusClient(rltClient);
 *
 * // start the test
 * wimod.StartRadioLinkTest(&params);
 * ...
 * // update the RLT results in the client callback and do analysis with it
 * ...
 * @endcode
 *
 */
bool WiMODLRBASE_PLUS::StartRadioLinkTest(const TWiMODLR_RLT_Parameter* params,
                                          TWiMODLRResultCodes*          hciResult,
                                          UINT8*                        rspStatus )
{
    localHciRes = SapRLT.StartRadioLinkTest(params, &localStatusRsp);
    return copyResultInfos(hciResult, rspStatus, RLT_STATUS_OK);
}

//-----------------------------------------------------------------------------
/**
 * @brief Tries to stop a running  Radio Link Test
 *
 * The firmware has got a feature called "Radio Link Test". This feauture can
 * be used to test the radio link between two devices.
 *
 * Please note that the Radio Link Test may interrupt the normal operation of
 * the WiMOD. So it is recommended to use this feature only in isolated
 * test szenarios.
 *
 * Two devices are required: One that akts as "Master" that starts the
 * transmission of test packets and a 2nd one that replies the test packets.
 *
 * For detailed infos please refer to the feature specificaton document(s).
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * \msc
 * UserApp,WiMOD,RemotePeer;
 * ... ;
 * UserApp=>WiMOD           [label="StartRadioLinkTest(data)"];
 * WiMOD=>WiMOD             [label="Prepare test"];
 * ---                      [label="start test for x times"];
 * WiMOD->RemotePeer        [label="transfer RLT packet"];
 * RemotePeer->WiMOD        [label="RLT packet reply"];
 * UserApp<-WiMOD           [label="Status Indication"];
 * UserApp->UserApp         [label="convert & update statistics"];
 * WiMOD->RemotePeer        [label="transfer RLT packet"];
 * RemotePeer->WiMOD        [label="RLT packet reply"];
 * UserApp<-WiMOD           [label="Status Indication"];
 * UserApp->UserApp         [label="convert & update statistics"];
 * ---                      [label="end of test or"];
 * UserApp=>WiMOD           [label="StopRadioLinkTest()"];
 * ...;
 * \endmsc
 *
 * @code
 * ...
 * // stop a running  test
 * wimod.StopRadioLinkTest();
 * ...
 * @endcode
 *
 */
bool WiMODLRBASE_PLUS::StopRadioLinkTest(TWiMODLRResultCodes* hciResult, UINT8* rspStatus)
{
    localHciRes = SapRLT.StopRadioLinkTest(&localStatusRsp);
    return copyResultInfos(hciResult, rspStatus, RLT_STATUS_OK);

}

//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for RLT updates
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param cb        Pointer a callback function
 *                  @see TRltStatusIndicationCallback for details
 *
 * @note The callback function should call the corresponding convert in
 *        order to decode the message properly
 *
 * \msc
 * UserApp,WiMOD,RemotePeer;
 * ... ;
 * UserApp=>WiMOD           [label="StartRadioLinkTest(data)"];
 * WiMOD=>WiMOD             [label="Prepare test"];
 * ---                      [label="start test for x times"];
 * WiMOD->RemotePeer        [label="transfer RLT packet"];
 * RemotePeer->WiMOD        [label="RLT packet reply"];
 * UserApp<-WiMOD           [label="Status Indication"];
 * UserApp->UserApp         [label="convert & update statistics"];
 * WiMOD->RemotePeer        [label="transfer RLT packet"];
 * RemotePeer->WiMOD        [label="RLT packet reply"];
 * UserApp<-WiMOD           [label="Status Indication"];
 * UserApp->UserApp         [label="convert & update statistics"];
 * ---                      [label="end of test or"];
 * UserApp=>WiMOD           [label="StopRadioLinkTest()"];
 * ...;
 * \endmsc
 *
 * @code
 * // callback for RLT Status Update indication
 * void myRltUpdateInd(TWiMODLR_HCIMessage& rxMsg) {
 * // use the convert function before accessing the data
 * ...
 * }
 *
 * void setup() {
 * ...
 * // setup the callback
 * wimod.RegisterRltStatusClient(myRltUpdateInd);
 * // start the RLT
 * ...
 * }
 *
 * ...
 * @endcode
 */
void WiMODLRBASE_PLUS::RegisterRltStatusClient(TRltStatusIndicationCallback cb)
{
    SapRLT.RegisterStatusIndClient(cb);
}

//-----------------------------------------------------------------------------
/**
 * @brief Convert a received low level HCI-Msg to a high-level RLT Status Info
 *
 * This function should be used by the RLT Status Indication Callback functions
 * prior processing the received data message.
 *
 * @param   RxMsg       Reference to low-level HCI message.
 *                      @warning DO NOT MANIPULATE THESE VALUES !!!
 *
 * @param   statusData  Pointer to the buffer where to store the received data
 *
 * @retval true     if the conversion was successful
 *
 * \msc
 * UserApp,WiMOD,RemotePeer;
 * ... ;
 * UserApp=>WiMOD     [label="RegisterRltStatusClient(myRLTStatusInd)"];
 * ...;
 * UserApp->WiMOD     [label="StartRadioLinkTest(...)"];
 * ...;
 * WiMOD<-RemotePeer  [label="RTL reply"];
 * --- [label="if callback is registered"];
 * UserApp << WiMOD   [label="myRLTStatusInd()"];
 * UserApp->UserApp [label="further processing"];
 * ...;
 *
 * \endmsc
 *
 * @code
 * // global buffer for indication that RLT Status data has been send
 * TWiMODLR_RLT_Status statusData;
 *
 * void myRLTStatusInd(TWiMODLR_HCIMessage& rxMsg) {
 *    // convert/copy the raw message to RX radio buffer
 *    if (wimod.convert(rxMsg, &statusData)) {
 *        // access the fields folding information about the transmission
 *
 *        // statusData.LocalTxCounter ...
 *        // statusData.PeerRxCounter ...
 *    ...
 *    }
 * }
 *...
 *
 * void setup() {
 *    ...
 *    // init the communication stack
 *    wimod.begin();
 *
 *     // register callback
 *    wimod.RegisterRltStatusClient(myRLTStatusInd);
 *    ...
 *
 *    // start Radio Link Test
 *    wimod.StartRadioLinkTest(...)
 * }
 * @endcode
 *
 */
bool WiMODLRBASE_PLUS::convert(TWiMODLR_HCIMessage& RxMsg,
                               TWiMODLR_RLT_Status* statusData) {
    return SapRLT.convert(RxMsg, statusData);
}


//===============================================================================
//
// SensorApp
//
//===============================================================================
//-----------------------------------------------------------------------------
/**
 * @brief Tries to configure the integrated Sensor App feature of the WiMOD
 *
 * The firmware has got a feature called "SensorApp". This feauture can
 * be used to transfer several "sensor" values from the DemoBoard to remote
 * peer WiMOD device. The transferable sensor values are:
 *  \li states of the push buttons (of the DemoBoard)
 *  \li state of the DIP switch (of the DemoBoard)
 *  \li current voltage reading of the supply voltage
 *  \li current ADC value / position of the potentio meter (of the DemoBoard)
 *  \li current temperature reading of the WiMOD
 *
 *
 * Please note that the SensorApp may interrupt the normal operation of
 * the WiMOD. So it is recommended to use this feature only in isolated
 * szenarios.
 *
 * Two devices are required: One that akts as "Transmitter" while the other one
 * acts a "Receiver".
 *
 * For detailed infos please refer to the feature specificaton document(s).
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param config      Data structure containing the setup data for the SensorApp
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * \msc
 * User,UserApp,WiMOD,RemoteWiMOD,RemoteUserApp;
 * ... ;
 * UserApp=>WiMOD               [label="SetSensorAppConfig(config)"];
 * ... ;
 * RemoteUserApp=>RemoteWiMOD   [label="SetSensorAppConfig(config)"];
 * ... ;
 * User->WiMOD                  [label="Press a push button"];
 * WiMOD->RemoteWiMOD           [label="transfer SensorEvent"];
 * ...;
 * \endmsc
 *
 * @code
 * ...
 * // local variable
 * TWiMODLR_SensorApp_Config config;
 *
 * // setup SensorApp
 * config.Mode               = SensorApp_Mode_SensorDataTransmitter;
 * config.Options            = SENSORAPP_OPTION_PUSH_BUTTON_ONE;
 * config.SendingPeriod      = 1000;
 * config.LinkTimeout        = 2000;
 *
 * // start the sensorApp
 * wimod.SetSensorAppConfig(&config);
 * ...
 * @endcode
 *
 */
bool WiMODLRBASE_PLUS::SetSensorAppConfig(const TWiMODLR_SensorApp_Config* config,
                                          TWiMODLRResultCodes*             hciResult,
                                          UINT8*                           rspStatus)
{
    localHciRes = SapSensor.SetSensorAppConfig(config, &localStatusRsp);
    return copyResultInfos(hciResult, rspStatus, RLT_STATUS_OK);

}
//-----------------------------------------------------------------------------
/**
 * @brief Tries to get the current config of the integrated Sensor App feature of the WiMOD
 *
 * The firmware has got a feature called "SensorApp". This feauture can
 * be used to transfer several "sensor" values from the DemoBoard to remote
 * peer WiMOD device. The transferable sensor values are:
 *  \li states of the push buttons (of the DemoBoard)
 *  \li state of the DIP switch (of the DemoBoard)
 *  \li current voltage reading of the supply voltage
 *  \li current ADC value / position of the potentio meter (of the DemoBoard)
 *  \li current temperature reading of the WiMOD
 *
 *
 * Please note that the SensorApp may interrupt the normal operation of
 * the WiMOD. So it is recommended to use this feature only in isolated
 * szenarios.
 *
 * Two devices are required: One that akts as "Transmitter" while the other one
 * acts a "Receiver".
 *
 * For detailed infos please refer to the feature specificaton document(s).
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param config      Data structure containing the setup data for the SensorApp
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 *
 * @code
 * ...
 * // local variable
 * TWiMODLR_SensorApp_Config config;
 *
 * // get the current config
 * if wimod.GetSensorAppConfig(&config)) {
 *      // process the result
 *      if (config.Mode == SensorApp_Mode_SensorDataTransmitter) {...}
 *      ...
 * }
 * ...
 * @endcode
 *
 */
bool WiMODLRBASE_PLUS::GetSensorAppConfig(TWiMODLR_SensorApp_Config* config,
                                          TWiMODLRResultCodes*       hciResult,
                                          UINT8*                     rspStatus)
{
    localHciRes = SapSensor.GetSensorAppConfig(config, &localStatusRsp);
    return copyResultInfos(hciResult, rspStatus, RLT_STATUS_OK);

}

//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the SensorAppAckData Indication - optional -
 *
 * Please see the feature specification document for details.
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param cb        Pointer a callback function
 *
 * @note The callback function should call the corresponding convert in
 *        order to decode the message properly
 *
 * \msc
 * UserApp,WiMOD,RemoteWiMOD;
 * ... ;
 * WiMOD -> RemoteWiMOD    [label="transmit sensor data"];
 * WiMOD <- RemoteWiMOD    [label="transmit ack(button state)"];
 * UserApp <- WiMOD        [label="mySensorAppAckInd()"];
  * ...;
 * \endmsc
 *
 * @code
 * // callback for SensorApp ACK callaback
 * void mySensorAppAckInd(TWiMODLR_HCIMessage& rxMsg) {
 *   // use the convert function before accesing the data
 *   ...
 * }
 *
 * void setup() {
 *  ...
 *  // setup the callback
 *  wimod.RegisterSensorAppAckIndClient(mySensorAppAckInd);
 *  ...
 * }
 *
 * ...
 * @endcode
 */
void WiMODLRBASE_PLUS::RegisterSensorAppAckIndClient(TSensorAppAckIndicationCallback cb)
{
    SapSensor.RegisterSensorAckIndClient(cb);
}

//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the SensorAppData Indication
 *
 * Please see the feature specification document for details.
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param cb        Pointer a callback function
 *
 * @note The callback function should call the corresponding convert in
 *        order to decode the message properly
 *
 * \msc
 * WiMOD,RemoteWiMOD,RemoteApp;
 * ... ;
 * WiMOD -> RemoteWiMOD    [label="transmit sensor data"];
 * WiMOD <- RemoteWiMOD    [label="transmit ack(button state)"];
 * RemoteWiMOD -> RemoteApp   [label="mySensorAppInd()"];
  * ...;
 * \endmsc
 *
 * @code
 * // callback for mySensorApp data indication callaback
 * void mySensorAppInd(TWiMODLR_HCIMessage& rxMsg) {
 *   // use the convert function before accesing the data
 *   ...
 * }
 *
 * void setup() {
 *  ...
 *  // setup the callback
 *  wimod.RegisterSensorAppDataIndClient(mySensorAppInd);
 *  ...
 * }
 *
 * ...
 * @endcode
 */
void WiMODLRBASE_PLUS::RegisterSensorAppDataIndClient(TSensorAppSensorDataIndicationCallback cb)
{
    SapSensor.RegisterSensorDataIndClient(cb);
}

//-----------------------------------------------------------------------------
/**
 * @brief Convert a received low level HCI-Msg to a high-level SensorData Info
 *
 * This function should be used by the SensorApp Data Indication Callback functions
 * prior processing the received data message.
 *
 * @param   RxMsg       Reference to low-level HCI message.
 *                      @warning DO NOT MANIPULATE THESE VALUES !!!
 *
 * @param   sensorData  Pointer to the buffer where to store the received data
 *
 * @retval true     if the conversion was successful
 *
 * \msc
 * UserApp,WiMOD,RemotePeer;
 * ... ;
 * UserApp=>WiMOD     [label="RegisterSensorAppDataIndClient(mySensorDataInd)"];
 * ...;
 * --- [label="if callback is registered"];
 * UserApp << WiMOD   [label="mySensorDataInd()"];
 * UserApp->UserApp [label="further processing"];
 * ...;
 *
 * \endmsc
 *
 * @code
 * // global buffer for indication that RLT Status data has been send
 * TWiMODLR_SensorApp_SensorData sensorData;
 *
 * void mySensorDataInd(TWiMODLR_HCIMessage& rxMsg) {
 *    // convert/copy the raw message to RX radio buffer
 *    if (wimod.convert(rxMsg, &sensorData)) {
 *        // access the fields folding information about the transmission
 *
 *        // sensorData.Voltage ...
 *        ...
 *        if (statusData.DigitalInputs & SENSORAPP_DIGITAL_INPUT_BOTTON1_SET) {...}
 *    ...
 *    }
 * }
 *...
 *
 * void setup() {
 *    ...
 *    // init the communication stack
 *    wimod.begin();
 *
 *     // register callback
 *    wimod.RegisterSensorAppDataIndClient(mySensorDataInd);
 *    ...
 * }
 * @endcode
 *
 */
bool WiMODLRBASE_PLUS::convert(TWiMODLR_HCIMessage&           RxMsg,
                               TWiMODLR_SensorApp_SensorData* sensorData) {
    return SapSensor.convert(RxMsg, sensorData);
}

//-----------------------------------------------------------------------------
/**
 * @brief Convert a received low level HCI-Msg to a high-level SensorData Info
 *
 * This function should be used by the SensorApp Data Indication Callback functions
 * prior processing the received data message.
 *
 * @param   RxMsg       Reference to low-level HCI message.
 *                      @warning DO NOT MANIPULATE THESE VALUES !!!
 *
 * @param   sensorAckData  Pointer to the buffer where to store the received data
 *
 * @retval true     if the conversion was successful
 *
 *
 * @code
 * // global buffer for indication that RLT Status data has been send
 * TWiMODLR_SensorApp_AckSensorData sensorAckData;
 *
 * void mySensorAckDataInd(TWiMODLR_HCIMessage& rxMsg) {
 *    // convert/copy the raw message to RX radio buffer
 *    if (wimod.convert(rxMsg, &sensorAckData)) {
 *        // access the fields folding information about the transmission
 *        ...
 *        if (statusData.DigitalInputs & SENSORAPP_DIGITAL_INPUT_BOTTON1_SET) {...}
 *    ...
 *    }
 * }
 *...
 *
 * void setup() {
 *    ...
 *    // init the communication stack
 *    wimod.begin();
 *
 *     // register callback
 *    wimod.RegisterSensorAppAckIndClient(mySensorAckDataInd);
 *    ...
 * }
 * @endcode
 *
 */
bool WiMODLRBASE_PLUS::convert(TWiMODLR_HCIMessage&              RxMsg,
                               TWiMODLR_SensorApp_AckSensorData* sensorAckData) {
    return SapSensor.convert(RxMsg, sensorAckData);
}


//===============================================================================
//
// RemoteCtrl App
//
//===============================================================================


//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the RemoteCtrl Btn Pressed Indication
 *
 * Please see the feature specification document for details.
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param cb        Pointer a callback function
 *
 * @note The callback function should call the corresponding convert in
 *        order to decode the message properly
 *
 * \msc
 * UserApp,WiMOD,RemoteWiMOD;
 * ... ;
 * RemoteWiMOD->WiMOD      [label="transfer RemoteCtrlButtonPress event"];
 * WiMOD -> UserApp        [label="myBtnPressedInd()"];
 * UserApp <- UserApp      [label="process indication"];
  * ...;
 * \endmsc
 *
 * @code
 * // callback for my button pressed  indication callaback
 * void myBtnPressedInd(TWiMODLR_HCIMessage& rxMsg) {
 *   // use the convert function before accesing the data
 *   ...
 * }
 *
 * void setup() {
 *  ...
 *  // setup the callback
 *  wimod.RegisterBtnPressedClient(myBtnPressedInd);
 *  ...
 * }
 *
 * ...
 * @endcode
 */
void WiMODLRBASE_PLUS::RegisterBtnPressedClient(TRemoteCtrlBtnPressedIndicationCallback cb)
{
    SapRemoteCtrl.RegisterBtnPressedClient(cb);
}

//-----------------------------------------------------------------------------
/**
 * @brief Convert a received low level HCI-Msg to a high-level RemoteCtrl Btn Info
 *
 * This function should be used by the RemoteCtrl Button Pressed Indication Callback functions
 * prior processing the received data message.
 *
 * @param   RxMsg       Reference to low-level HCI message.
 *                      @warning DO NOT MANIPULATE THESE VALUES !!!
 *
 * @param   btnInfo    Pointer to the buffer where to store the received data
 *
 * @retval true     if the conversion was successful
 *
 *
 * @code
 * // global buffer for indication that RemoteCtrl Button Pressed indication has been send
 * TWiMODLR_RemoteCtrl_BtnPressed btnInfo;
 *
 * void myBtnPressedInd(TWiMODLR_HCIMessage& rxMsg) {
 *    // convert/copy the raw message to RX radio buffer
 *    if (wimod.convert(rxMsg, &btnInfo)) {
 *        // access the fields folding information about the transmission
 *        ...
 *        if (btnInfo.ButtonBitmap & SENSORAPP_DIGITAL_INPUT_BOTTON1_SET) {...}
 *    ...
 *    }
 * }
 *...
 *
 * void setup() {
 *    ...
 *    // init the communication stack
 *    wimod.begin();
 *
 *     // register callback
 *    wimod.RegisterBtnPressedClient(myBtnPressedInd);
 *    ...
 * }
 * @endcode
 *
 */
bool WiMODLRBASE_PLUS::convert(TWiMODLR_HCIMessage& RxMsg, TWiMODLR_RemoteCtrl_BtnPressed* btnInfo)
{
    return SapRemoteCtrl.convert(RxMsg, btnInfo);
}


//------------------------------------------------------------------------------
//
// Section protected functions
//
//------------------------------------------------------------------------------
/**
 * @internal
 *
 * @brief dispatches an received indication message from the module
 *
 * @param rxMsg     reference to the received messasge
 *
 * @endinternal
 */
void WiMODLRBASE_PLUS::ProcessUnexpectedRxMessage(TWiMODLR_HCIMessage& rxMsg) {
    switch(rxMsg.SapID)
    {
        case    DEVMGMT_SAP_ID:
                SapDevMgmt.DispatchDeviceMgmtMessage(rxMsg);
                break;

        case RADIOLINK_SAP_ID:
                SapRadioLink.DispatchRadioLinkMessage(rxMsg);
                break;

        case RLT_SAP_ID:
                SapRLT.DispatchRLTMessage(rxMsg);
                break;

        case SENSORAPP_SAP_ID:
                SapSensor.DispatchSensorAppMessage(rxMsg);
                break;
        case REMOTECTRL_SAP_ID:
                SapRemoteCtrl.DispatcRemoteCtlMessage(rxMsg);
                break;

        default:
                if (StackErrorClientCB) {
                    StackErrorClientCB(WIMOD_STACK_ERR_UNKNOWN_RX_SAP_ID);
                }
                break;
    }
    return;
}


//------------------------------------------------------------------------------
//
// Section private functions
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------

/**
 * @internal
 *
 * @brief stores the status and response code of the last executed (user) command
 *
 * @param   hciResult   pointer to the result of the last HCI communication
 *
 * @param   rspStatus   pointer to the status code of the last HCI response
 *
 * @param   successValue expected value if last command was OK
 *
 * @return  true if actual results are equal to the expected value
 *
 * @endinternal
 *
 */
bool WiMODLRBASE_PLUS::copyResultInfos(TWiMODLRResultCodes* hciResult,
                                  UINT8*              rspStatus,
                                  UINT8               successValue)
{
    cmdResult = false;

    //check if the transmisstion of the HCI frame was successful
    if ((WiMODLR_RESULT_OK == localHciRes)
            && (localStatusRsp == successValue)) {
        // -> yes
        cmdResult = true;
    }
    // if pointers are valid copy the values for external processing
    if (hciResult) {
        *hciResult = localHciRes;
    }
    if (rspStatus) {
        *rspStatus = localStatusRsp;
    }

    // backup last status data
    lastHciRes = localHciRes;
    lastStatusRsp = localStatusRsp;

    // return true if HCI TX operation and command execution status are OK
    return cmdResult;
}



//-----------------------------------------------------------------------------
// EOF
//-----------------------------------------------------------------------------
