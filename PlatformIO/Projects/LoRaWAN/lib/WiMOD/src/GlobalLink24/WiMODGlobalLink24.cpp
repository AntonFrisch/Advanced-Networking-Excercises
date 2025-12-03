//------------------------------------------------------------------------------
//! @file WiMODGlobalLink24.cpp
//! @ingroup WiMODGlobalLink24
//! <!------------------------------------------------------------------------->
//! @brief High-level Interface class to the WiMOD module running the WiMOD GlobalLink24 EndNode Modem firmware
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

//------------------------------------------------------------------------------
//
// Section Includes Files
//
//------------------------------------------------------------------------------

#include "WiMODGlobalLink24.h"

#include <string.h>
#include "../utils/StrToIntConverter.h"
//------------------------------------------------------------------------------
//
// Section public functions
//
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 * @brief Constructor
 *
 * This function requires a cpp reference to the serial interface object
 * that is to be used for communication.
 *
 * @code
 * #include <WiMODGlobalLink24.h>
 *
 * WiMODGlobalLink24 wimod(Serial3);  // use the Arduino Serial3 as serial interface
 *
 * void setup() {
 *	// init stack
 * 	wimod.begin();
 *	...
 * }
 * @endcode
 *
 */
WiMODGlobalLink24::WiMODGlobalLink24(Stream& s) :
GlobalLinkImp(s)
{
}

//-----------------------------------------------------------------------------
/**
 * @brief Desctructor
 */
 WiMODGlobalLink24::~WiMODGlobalLink24(void) {

}

//-----------------------------------------------------------------------------
/**
 * @brief This function must be called once before any other service can be used
 *
 * @code
 * #include <WiMODGlobalLink24.h>
 *
 * WiMODGlobalLink24 wimod(Serial3);  // use the Arduino Serial3 as serial interface
 *
 * void setup() {
 *	// init stack
 * 	wimod.begin();
 *	...
 * }
 * @endcode
 */
void WiMODGlobalLink24::begin(TLoRaWANregion region) {
    GlobalLinkImp.begin(region);
}

//! @cond Doxygen_Suppress
void WiMODGlobalLink24::beginAndAutoSetup(void) {
    GlobalLinkImp.beginAndAutoSetup();
 //   GlobalLinkImp.begin(LoRaWAN_Region_proprietary_WW2G4);
}
//! @endcond

//-----------------------------------------------------------------------------
/**
 * @brief Shut shut down function
 */
void WiMODGlobalLink24::end(void) {
    GlobalLinkImp.end();
}


//! @cond Doxygen_Suppress

/**
 * @brief print out some basic infos about the radio module on a given stream
 *
 * @param   s   reference to the output stream to use
 */
void WiMODGlobalLink24::PrintBasicDeviceInfo(Stream& s) {
    GlobalLinkImp.PrintBasicDeviceInfo(s);
}
//! @endcond



/**
 * @brief do a jump start and use OTAA to connect to a Nwk server
 *
 * @param appEUI     pointer to an array containing the AppEUI data to use
 * @param appKey     pointer to an array containing the AppKey data to use
 */

void WiMODGlobalLink24::ConnectViaOTAA(const uint8_t* appEUI, const uint8_t* appKey) {
    GlobalLinkImp.ConnectViaOTAA(appEUI, appKey);
}



/**
 * @brief Converts a string containing an App EUI string into a given byte array
 *
 * This convinience funtion offers the user the possibilty to use a string
 * containing the AppEUI as text (e.g. "0x12 34 45 78 9A BC DE f0").
 * The string will be decoded to an byte array for further usage.
 * The string can is interpreted as hex digits with or without a "0x" prefix.
 * Each digit pair must be separated by a space char.
 *
 * @param appEuiStr     String containing the hex coded appEUI
 * @param appEuiArray   array that should store the converted byte values
 * @warning There is no length check of the array. The user is responsible to reserve enough space.
 *
 */
void WiMODGlobalLink24::ConvertAppEuiStrToArray(char* appEuiStr, uint8_t* appEuiArray)
{
    GlobalLinkImp.ConvertAppEuiStrToArray(appEuiStr, appEuiArray);
}

/**
 * @brief Converts a string containing an App Key string into a given byte array
 *
 * This convinience funtion offers the user the possibilty to use a string
 * containing the AppKey as text (e.g. "0x12 34 45 78 9A BC DE f0 10 20 30 40 50 60 70 80").
 * The string will be decoded to an byte array for further usage.
 * The string can is interpreted as hex digits with or without a "0x" prefix.
 * Each digit pair must be separated by a space char.
 *
 * @param appKeyStr     String containing the hex coded appKey
 * @param appKeyArray   array that should store the converted byte values
 * @warning There is no length check of the array. The user is responsible to reserve enough space.
 *
 */
void WiMODGlobalLink24::ConvertAppKeyStrToArray(char* appKeyStr, uint8_t* appKeyArray)
{
    GlobalLinkImp.ConvertAppKeyStrToArray(appKeyStr, appKeyArray);
}

/**
 * @brief Converts a string containing an NwkSKey string into a given byte array
 *
 * This convinience funtion offers the user the possibilty to use a string
 * containing the NwkSKey as text (e.g. "0x12 34 45 78 9A BC DE f0 10 20 30 40 50 60 70 80").
 * The string will be decoded to an byte array for further usage.
 * The string can is interpreted as hex digits with or without a "0x" prefix.
 * Each digit pair must be separated by a space char.
 *
 * @param nwkSKeyStr     String containing the hex coded appKey
 * @param nwkSKeyArray   array that should store the converted byte values
 * @warning There is no length check of the array. The user is responsible to reserve enough space.
 *
 */
void WiMODGlobalLink24::ConvertNwkSKeyStrToArray(char* nwkSKeyStr, uint8_t* nwkSKeyArray)
{
    GlobalLinkImp.ConvertNwkSKeyStrToArray(nwkSKeyStr, nwkSKeyArray);
}


/**
 * @brief Converts a string containing an AppSKey string into a given byte array
 *
 * This convinience funtion offers the user the possibilty to use a string
 * containing the AppSKey as text (e.g. "0x12 34 45 78 9A BC DE f0 10 20 30 40 50 60 70 80").
 * The string will be decoded to an byte array for further usage.
 * The string can is interpreted as hex digits with or without a "0x" prefix.
 * Each digit pair must be separated by a space char.
 *
 * @param appSKeyStr     String containing the hex coded appKey
 * @param appSKeyArray   array that should store the converted byte values
 * @warning There is no length check of the array. The user is responsible to reserve enough space.
 *
 */
void WiMODGlobalLink24::ConvertAppSKeyStrToArray(char* appSKeyStr, uint8_t* appSKeyArray)
{
    GlobalLinkImp.ConvertAppSKeyStrToArray(appSKeyStr, appSKeyArray);
}



//===============================================================================
//
// DevMgmt
//
//===============================================================================
//-----------------------------------------------------------------------------
/**
 * @brief Ping Cmd - Checks the serial connection to the WiMOD module
 *
 *
 * @see HCI command specification.
 *
 * @param   hciResult       pointer for storing the the local HCI transfer result.
 *                          This is an optional parameter.
 *
 * @param  rspStatus        pointer to store the response code of the WiMOD
 *                          This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * @code
 * if (wimod.Ping()) {
 * 	// success ...
 * } else {
 * 	// error ...
 * }
 * @endcode
 *
 * OR:
 *
 * @code
 * TWiMODLRResultCodes hciResult;
 *
 * if (!wimod.Ping(&hciResult)) {
 * 	// error: check HCI result codes
 * 	if (WiMODLR_RESULT_TRANMIT_ERROR == hciResult) {...}
 * 	if (WiMODLR_RESULT_NO_RESPONSE   == hciResult) {...}
 * }
 * @endcode
 *
 * OR:
 *
 * @code
 * TWiMODLRResultCodes hciResult;
 * uint8              rspCode;
 *
 * if (!wimod.Ping(&hciResult, &rspCode)) {
 * 	// error: check HCI result codes
 * 	if (WiMODLR_RESULT_OK == hciResult) {
 * 		if (rspCode == DEVMGMT_STATUS_ERROR) {
 * 		  // operation failed
 * 		}
 * 		...
 * 	}
 * 	...
 * }
 * @endcode
 *
 */
bool WiMODGlobalLink24::Ping(TWiMODLRResultCodes*         hciResult,
                        UINT8*                      rspStatus)
{
    return GlobalLinkImp.Ping(hciResult, rspStatus);
}

//-----------------------------------------------------------------------------
/**
 * @brief Reset Cmd - Reboots the WiMOD module
 *
 *
 * @see HCI command specification.
 *
 * @param   hciResult       pointer for storing the the local HCI transfer result.
 *                          This is an optional parameter.
 *
 * @param  rspStatus        pointer to store the response code of the WiMOD
 *                          This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 */
bool WiMODGlobalLink24::Reset(TWiMODLRResultCodes*         hciResult,
                         UINT8*                      rspStatus)
{
    return GlobalLinkImp.Reset(hciResult, rspStatus);
}

//-----------------------------------------------------------------------------
/**
 * @brief GetDeviceInfo Cmd - Gets the basic device information of the WiMOD
 *
 *
 * @see HCI command specification.
 *
 * @param   info            pointer for storing the requested information (structure)
 *
 * @param   hciResult       pointer for storing the the local HCI transfer result.
 *                          This is an optional parameter.
 *
 * @param  rspStatus        pointer to store the response code of the WiMOD
 *                          This is an optional parameter.
 *
 * @retval true             if everything was successful
 *
 * @code
 * // create a local variable
 * TWiMODLR_DevMgmt_DevInfo devInfo;
 *
 * // get information of WiMOD
 * wimod.GetDeviceInfo(&devInfo);
 *
 * //access fields of structure
 * printf("ModulType-ID:  %d:", devInfo.ModuleType);
 * printf("DeviceAddress: %x:", devInfo.DevAdr);
 * ...
 * @endcode
 */
bool WiMODGlobalLink24::GetDeviceInfo(TWiMODLR_DevMgmt_DevInfo*   info,
                                 TWiMODLRResultCodes*         hciResult,
                                 UINT8*                      rspStatus)
{
    return GlobalLinkImp.GetDeviceInfo(info, hciResult, rspStatus);
}

//-----------------------------------------------------------------------------
/**
 * @brief GetFirmwareInfo Cmd - Gets the basic information about the firmware of the WiMOD
 *
 *
 * @see HCI command specification.
 *
 * @param   info            pointer for storing the requested information (structure)
 *
 * @param   hciResult       pointer for storing the the local HCI transfer result.
 *                          This is an optional parameter.
 *
 * @param  rspStatus        pointer to store the response code of the WiMOD
 *                          This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * @code
 * // create a local variable
 * TWiMODLR_DevMgmt_FwInfo fwInfo;
 *
 * // get information of WiMOD
 * wimod.GetFirmwareInfo(&fwInfo);
 *
 * //access fields of structure
 * printf("FW-Version:  %d.%d:", fwInfo.FirmwareMayorVersion, fwInfo.FirmwareMinorVersion );
 * printf("FW-Name:     %s:",    fwInfo.FirmwareName);
 * ...
 * @endcode
 */
 bool WiMODGlobalLink24::GetFirmwareInfo(TWiMODLR_DevMgmt_FwInfo*  info,
                                 TWiMODLRResultCodes*         hciResult,
                                 UINT8*                      rspStatus)
{
     return GlobalLinkImp.GetFirmwareInfo(info, hciResult, rspStatus);
}


//-----------------------------------------------------------------------------
/**
 * @brief GetSystemStatus Cmd - Gets the basic information about the system status of the WiMOD
 *
 *
 * @see HCI command specification.
 *
 * @param   info            pointer for storing the requested information (structure)
 *
 * @param   hciResult       pointer for storing the the local HCI transfer result.
 *                          This is an optional parameter.
 *
 * @param  rspStatus        pointer to store the response code of the WiMOD
 *                          This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 *
 * @code
 * // create a local variable
 * TWiMODLR_DevMgmt_SystemStatus sysStatus;
 *
 * // get information of WiMOD
 * wimod.GetSystemStatus(&sysStatus);
 *
 * //access fields of structure
 * printf("Sys-SysTicks:      %d:", sysStatus.SysTickCounter);
 * printf("Sys-Voltage (mv):  %d:", sysStatus.BatteryStatus);
 * ...
 * @endcode
 */
bool WiMODGlobalLink24::GetSystemStatus(TWiMODLR_DevMgmt_SystemStatus* info,
                                     TWiMODLRResultCodes*         hciResult,
                                     UINT8*                      rspStatus)
{
    return GlobalLinkImp.GetSystemStatus(info, hciResult, rspStatus);
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
bool WiMODGlobalLink24::GetRtc(UINT32*                    rtcTime,
                         TWiMODLRResultCodes*         hciResult,
                         UINT8*                      rspStatus)
{
    return GlobalLinkImp.GetRtc(rtcTime, hciResult, rspStatus);
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
bool WiMODGlobalLink24::SetRtc(const UINT32 rtcTime,
                         TWiMODLRResultCodes*         hciResult,
                         UINT8*                      rspStatus)
{
    return GlobalLinkImp.SetRtc(rtcTime, hciResult, rspStatus);
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
 * // create a local variable
 * TWiMOD_OperationMode opMode;
 *
 * // get information of WiMOD
 * wimod.GetOperationMode(&opMode);
 *
 * // access value
 * if (opMode == OperationMode_Application) {
 * 	// do normal operations
 * 	...
 * } else {
 * 	// special operation mode; normal operations are disabled
 * 	...
 * }
 * ...
 * @endcode
 */
bool WiMODGlobalLink24::GetOperationMode(TWiMOD_OperationMode* opMode,
                         TWiMODLRResultCodes*         hciResult,
                         UINT8*                      rspStatus)
{
    return GlobalLinkImp.GetOperationMode(opMode, hciResult, rspStatus);
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
 * // create a local variable
 * TWiMOD_OperationMode opMode;
 *
 * // set new operation mode
 * opMode = OperationMode_Application;
 *
 * // set information of WiMOD
 * wimod.SetOperationMode(opMode);
 *
 * ...
 * @endcode
 */
bool WiMODGlobalLink24::SetOperationMode(const TWiMOD_OperationMode opMode,
                         TWiMODLRResultCodes*         hciResult,
                         UINT8*                      rspStatus)
{
    return GlobalLinkImp.SetOperationMode(opMode, hciResult, rspStatus);
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
bool WiMODGlobalLink24::SetRtcAlarm(const TWiMODLR_DevMgmt_RtcAlarm* rtcAlarm,
							  TWiMODLRResultCodes* 			   hciResult,
							  UINT8* 						   rspStatus)
{
    return GlobalLinkImp.SetRtcAlarm(rtcAlarm, hciResult, rspStatus);
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
 * 	printf("Alarm is active");
 * 	...
 * }
 * ...
 * @endcode
 */
bool WiMODGlobalLink24::GetRtcAlarm(TWiMODLR_DevMgmt_RtcAlarm* 		rtcAlarm,
							  TWiMODLRResultCodes* 				hciResult,
							  UINT8* 							rspStatus)
{
    return GlobalLinkImp.GetRtcAlarm(rtcAlarm, hciResult, rspStatus);
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
bool WiMODGlobalLink24::ClearRtcAlarm(TWiMODLRResultCodes* 			hciResult,
								UINT8* 							rspStatus)
{
    return GlobalLinkImp.ClearRtcAlarm(hciResult, rspStatus);
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
 * \msc
 * UserApp,WiMOD;
 * UserApp=>WiMOD  [label="RegisterPowerUpIndicationClient(clientCB)"];
 * ...;
 * UserApp<<=WiMOD [label="PowerUpCallback()"];
 * \endmsc
 *
 * @code
 * // user defined callback function
 * void myPowerUpCallback(void) {
 * 	 // WiMOD just powered up
 * 	...
 * }
 *
 * void setup() {
 * 	...
 * 	// "connect" the user defined to callback to the WiMOD callback feature
 * 	wimod.RegisterPowerUpIndicationClient(myPowerUpCallback);
 * 	...
 * }
 * @endcode
 */
void WiMODGlobalLink24::RegisterPowerUpIndicationClient(TDevMgmtPowerUpCallback cb)
{
    return GlobalLinkImp.RegisterPowerUpIndicationClient(cb);
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
 * 	// RTC Alarm has just been triggered
 * 	...
 * }
 *
 * void setup() {
 * 	...
 * 	// "connect" the user defined to callback to the WiMOD callback feature
 * 	wimod.RegisterRtcAlarmIndicationClient(myRtcAlarmCallback);
 * 	...
 * }
 * @endcode
 *
 */
void WiMODGlobalLink24::RegisterRtcAlarmIndicationClient(TDevMgmtRtcAlarmCallback cb)
{
    return GlobalLinkImp.RegisterRtcAlarmIndicationClient(cb);
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
 *  ...
 * }
 * ...
 * @endcode
 */
bool WiMODGlobalLink24::GetHciConfig(TWiMODLR_DevMgmt_HciConfig*     hciConfig,
                                TWiMODLRResultCodes*            hciResult,
                                UINT8*                          rspStatus)
{
    return GlobalLinkImp.GetHciConfig(hciConfig, hciResult, rspStatus);
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
bool WiMODGlobalLink24::SetHciConfig(const TWiMODLR_DevMgmt_HciConfig* hciConfig,
                                TWiMODLRResultCodes*              hciResult,
                                UINT8*                            rspStatus)
{
    return GlobalLinkImp.SetHciConfig(hciConfig, hciResult, rspStatus);
}



//===============================================================================
//
// LoRaWAN
//
//===============================================================================

//-----------------------------------------------------------------------------
/**
 * @brief Activates the device via the ABP procedure
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param activationData   structure holding the necessary parameters
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
 * UserApp,WiMOD;
 * UserApp=>WiMOD  [label="ActivateDevice(activationData)"];
 * UserApp<<WiMOD  [label="Activation response"];
 * ...;
 * --- [label = "time to transmit data"];
 * UserApp=>WiMOD [label="SendUData(data)"];
 * \endmsc
 *
 * @code
 * // ABP data
 * const uint32_t  DEV_ADR = 0x22;
 * const char NWKSKEY[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0f, 0x10 };
 * const char APPSKEY[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0f, 0x10 };
 *
 * // create a local variable
 * TWiMODGlobalLink24_ActivateDeviceData activationData;
 *
 * void setup() {
 *	//setup data
 *	activationData.DeviceAddress = DEV_ADR;
 *	memcpy(activationData.NwkSKey, NWKSKEY, 16);
 *	memcpy(activationData.AppSKey, APPSKEY, 16);
 *
 *	// activate device
 *	if (wimod.ActivateDevice(activationData)) {
 *		// ABP procedure done
 *		...
 *	} else {
 *		// Error executing ABP join request
 *		...
 *	}
 * ...
 * @endcode
 */
bool WiMODGlobalLink24::ActivateDevice(TWiMODGlobalLink24_ActivateDeviceData& activationData,
                                        TWiMODLRResultCodes*         hciResult,
                                        UINT8*                      rspStatus)
{
    return GlobalLinkImp.ActivateDevice(activationData, hciResult, rspStatus);
}


//-----------------------------------------------------------------------------
/**
 * @brief Re-Activates the device via the ABP procedure
 *
 * In case the device has been previously activated (e.g. during manufacturing)
 * the user does not know the security keys.
 * In this case a re-activation procedure must be used in order to use the
 * stored values. (For ABP ONLY!)
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param devAdr   pointer where the store the "received" device address
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
 * UserApp,WiMOD;
 * |||;
 * UserApp note WiMOD [label="Device was (pre) activated in the past; ABP ONLY"];
 * ...;
 * UserApp=>WiMOD  [label="ReactivateDevice(devAdr)"];
 * UserApp<<WiMOD  [label="Reactivation response"];
 * ...;
 * --- [label = "time to transmit data"];
 * UserApp=>WiMOD [label="SendUData(data)"];
 * \endmsc
 *
 * @code
 * // ABP data
 * const uint32_t  DEV_ADR = 0x22;
 *
 * // security keys are not known the user
 *
 *
 * void setup() {
 *	// local variable
 *	uint32_t devAddr = DEV_ADR;
 *
 *	// activate device
 *	if (wimod.ReactivateDevice(activationData)) {
 *		// ABP procedure done
 *		...
 *	} else {
 *		// Error executing ABP join request
 *		...
 *	}
 * ...
 * @endcode
 */
bool WiMODGlobalLink24::ReactivateDevice(UINT32* devAdr,
        TWiMODLRResultCodes*         hciResult,
        UINT8*                      rspStatus)
{
    return GlobalLinkImp.ReactivateDevice(devAdr, hciResult, rspStatus);
}

//-----------------------------------------------------------------------------
/**
 * @brief Sets the parameters used for the OTAA activation procedure
 *
 *
 * @see TWiMODLRResultCodes for interpretation of the values
 *
 * @param joinParams   structure holding the necessary parameters
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
 * // OTAA data
 * const char APPEUI[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
 * const char APPKEY[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0f, 0x10 };
 *
 * void setup() {
 *	// local variable
 *	TWiMODGlobalLink24_JoinParams joinParams;
 *
 *	// setup parameters
 *	memcpy(joinParams.AppEUI, APPEUI, 8);
 *	memcpy(joinParams.AppKey, APPKEY, 16);
 *
 *	// transfer parameters to WiMOD
 *	wimod.SetJoinParameter(joinParams);
 *	...
 * @endcode
 */
bool WiMODGlobalLink24::SetJoinParameter(TWiMODGlobalLink24_JoinParams& joinParams,
                                        TWiMODLRResultCodes*         hciResult,
                                        UINT8*                      rspStatus)
{
    return GlobalLinkImp.SetJoinParameter(joinParams, hciResult, rspStatus);
}

//-----------------------------------------------------------------------------
/**
 * @brief Start joining the network via the OTAA procedure. Asynchronous process
 *
 * This functions starts an asynchronous process of joining to a network.
 * It will take a time until a (final) result is ready. Therefore the
 * callback interface (e.g. RegisterJoinedNwkIndicationClient) should be used.
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
 * UserApp,WiMOD,Server;
 * |||;
 * UserApp=>WiMOD  [label="RegisterJoinedNwkIndicationClient(myJoinCallback)"];
 * UserApp=>WiMOD  [label="SetJoinParameter(joinParams)"];
 * UserApp=>WiMOD  [label="JoinNetwork()"];
 * WiMOD->Server   [label="JoinNwk Req."];
 * UserApp<<WiMOD  [label="JoinTxInd [optional]"];
 * ...;
 * WiMOD<-Server   [label="JoinNwk Rsp"];
 * UserApp<<=WiMOD [label="JoinedNwkIndication()"];
 * ...;
 * --- [label = "time to transmit data"];
 * UserApp=>WiMOD [label="SendUData(data)"];
 * \endmsc
 *
 * @code
 * void myNwkJoinedCallback(TWiMODLR_HCIMessage& rxMsg) {
 * 	// handle and check the given rsp data of the server
 * 	...
 * }
 *
 * void setup() {
 *  ...
 *	// setup OTAA / join related parameters and transfer it to WiMOD
 *	wimod.SetJoinParameter(joinParams);
 *
 *	// register joined nwk callback
 *	wimod.RegisterJoinedNwkIndicationClient(myNwkJoinedCallback);
 *
 *	// start OTAA procedure by sending a join request
 *	if (wimod.JoinNetwork()) {
 *		// OK procedure has been started
 *		// wait for callback indicating status of procedure
 *	} else {
 *		// error
 *		...
 *	}
 * ...
 * @endcode
 */
bool WiMODGlobalLink24::JoinNetwork(
        TWiMODLRResultCodes*         hciResult,
        UINT8*                      rspStatus)
{
    return GlobalLinkImp.JoinNetwork(hciResult, rspStatus);
}

//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the event "TX Join Indication"
 *
 * This registered callback is called when the specified event is called by
 * the WiMOD stack.
 *
 * @param   cb          pointer to a callback function that should be called
 *                      if the event occurs.
 *
 *
 * @code
 * void myJoinReqTxCallback(TWiMODLR_HCIMessage& rxMsg) {
 * 	// handle and check the given rsp data of the server
 *	TWiMODGlobalLink24_TxIndData txData;
 *	wimod.convert(rxMsg, &txData);
 *	printf("joining attempt: %d", txData.NumTxPackets);
 *	...
 * }
 *
 * void setup() {
 * 	...
 *	// register joined nwk callback
 *	wimod.RegisterJoinTxIndicationClient(myJoinReqTxCallback);
 * 	...
 * }
 * @endcode
 */
void WiMODGlobalLink24::RegisterJoinTxIndicationClient(TJoinTxIndicationCallback cb)
{
    return GlobalLinkImp.RegisterJoinTxIndicationClient(cb);
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
 * @param   globalLink24RxData Pointer to the buffer where to store the received data
 *
 * @retval true     if the conversion was successful
 *
 * \msc
 * UserApp,WiMOD,Server;
 * |||;
 * UserApp->WiMOD  [label="RegisterRxUDataIndicationClient(myNewDataCallback)"];
 * ...;
 * WiMOD<-Server   [label="new (U)Data"];
 * UserApp<<=WiMOD [label="myNewDataCallback(LowLevelData)"];
 * UserApp->WiMOD  [label="convert(LowLevelData, RXmsg)"];
 * UserApp<<WiMOD  ;
 * UserApp->UserApp [label="further processing"];
 * ...;
 * \endmsc
 *
 * @code
 * void onRxData(TWiMODLR_HCIMessage& rxMsg) {
 * 	TWiMODGlobalLink24_RX_Data radioRxMsg;
 *
 * 	// convert/copy the raw message to RX radio buffer
 * 	if (wimod.convert(rxMsg, &radioRxMsg)) {
 * 		// OK, process RX data
 * 		...
 * 	}
 * }
 * ...
 * void setup() {
 * 	...
 * 	// setup user callbacks for RX data events
 * 	wimod.RegisterRxCDataIndicationClient(onRxData);
 * 	wimod.RegisterRxUDataIndicationClient(onRxData);
 * }
 * ...
 *
 * @endcode
 */
bool WiMODGlobalLink24::convert(TWiMODLR_HCIMessage& RxMsg,
        TWiMODGlobalLink24_RX_Data* globalLink24RxData)
{
    return GlobalLinkImp.convert(RxMsg, globalLink24RxData);
}

//-----------------------------------------------------------------------------
/**
 * @brief Convert a received low level HCI-Msg to a high-level tx ind structure
 *
 * This function should be used by the Tx Data Indication Callback functions
 * prior processing the received data message.
 *
 * @param   RxMsg       Reference to low-level HCI message.
 *                      @warning DO NOT MANIPULATE THESE VALUES !!!
 *
 * @param   sendIndData Pointer to the buffer where to store the received data
 *
 * @retval true     if the conversion was successful
 *
 * \msc
 * UserApp,WiMOD,Server;
 * |||;
 * UserApp->WiMOD  [label="RegisterTxUDataIndicationClient(myTxInd)"];
 * UserApp->WiMOD  [label="RegisterTxCDataIndicationClient(myTxInd)"];
 * ...;
 * UserApp->WiMOD  [label="SendUData()"];
 * WiMOD->Server   [label="transfer data"];
 * UserApp<<=WiMOD [label="myTxInd(LowLevelData)"];
 * UserApp->WiMOD  [label="convert(LowLevelData, txData)"];
 * ...;
 * UserApp<<WiMOD  ;
 * UserApp->UserApp [label="further processing"];
 * \endmsc
 *
 * @code
 * void myTxInd(TWiMODLR_HCIMessage& rxMsg) {
 * 	TWiMODGlobalLink24_TxIndData txData;
 *
 * 	// convert/copy the raw message to tx indication data
 * 	if (wimod.convert(rxMsg, &txData)) {
 * 		// OK, process data
 * 		...
 * 	}
 * }
 * ...
 * void setup() {
 * 	...
 * 	// setup user callbacks for TX indication events
 * 	wimod.RegisterTxUDataIndicationClient(myTxInd);
 * 	wimod.RegisterTxCDataIndicationClient(myTxInd);
 * }
 * ...
 *
 * @endcode
 */
bool WiMODGlobalLink24::convert(TWiMODLR_HCIMessage& RxMsg,
        TWiMODGlobalLink24_TxIndData* sendIndData)
{
    return GlobalLinkImp.convert(RxMsg, sendIndData);
}

//-----------------------------------------------------------------------------
/**
 * @brief Convert a received low level HCI-Msg to a high-level MAC-Cmd structure
 *
 * This function should be used by the Rx Mac Cmd Indication Callback function
 * prior processing the received data message.
 *
 * @param   RxMsg       Reference to low-level HCI message.
 *                      @warning DO NOT MANIPULATE THESE VALUES !!!
 *
 * @param   globalLink24MacCmdData Pointer to the buffer where to store the received data
 *
 * @retval true     if the conversion was successful
 *
 * \msc
 * UserApp,WiMOD,Server;
 * |||;
 * UserApp->WiMOD  [label="RegisterRxMacCmdIndicationClient(myMacCmdInd)"];
 * ...;
 * WiMOD<-Server   [label="MAC Cmd + data"];
 * UserApp<<=WiMOD [label="myMacCmdInd(LowLevelData)"];
 * UserApp->WiMOD  [label="convert(LowLevelData, macData)"];
 * ...;
 * UserApp<<WiMOD  ;
 * UserApp->UserApp [label="further processing"];
 * \endmsc
 *
 * @code
 * void myMacCmdInd(TWiMODLR_HCIMessage& rxMsg) {
 * 	TWiMODGlobalLink24_RX_MacCmdData macData;
 *
 * 	// convert/copy the raw message to MAC cmd indication data
 * 	if (wimod.convert(rxMsg, &macData)) {
 * 		// OK, process data
 * 		...
 * 	}
 * }
 * ...
 * void setup() {
 * 	...
 * 	// setup user callback for MAC cmd indication events
 * 	wimod.RegisterRxMacCmdIndicationClient(myMacCmdInd);
 * }
 * ...
 *
 * @endcode
 */
bool WiMODGlobalLink24::convert(TWiMODLR_HCIMessage& RxMsg,
        TWiMODGlobalLink24_RX_MacCmdData* globalLink24MacCmdData)
{
    return GlobalLinkImp.convert(RxMsg, globalLink24MacCmdData);
}

//-----------------------------------------------------------------------------
/**
 * @brief Convert a received low level HCI-Msg to a high-level NwkJoined structure
 *
 * This function should be used by the JoinedNwk Indication Callback function
 * prior processing the received data message.
 *
 * @param   RxMsg       Reference to low-level HCI message.
 *                      @warning DO NOT MANIPULATE THESE VALUES !!!
 *
 * @param   joinedNwkData Pointer to the buffer where to store the received data
 *
 * @retval true     if the conversion was successful
 *
 * \msc
 * UserApp,WiMOD,Server;
 * |||;
 * UserApp->WiMOD  [label="RegisterJoinedNwkIndicationClient(myJoinedNwkInd)"];
 * ...;
 * UserApp=>WiMOD  [label="SetJoinParameter(params)"];
 * UserApp=>WiMOD  [label="JoinNetwork()"];
 * WiMOD->Server   [label="Start OTAA procedure"];
 * ...;
 * ...;
 * WiMOD<-Server   [label="JoinNwkIndication"];
 * UserApp<<=WiMOD [label="myJoinedNwkInd(LowLevelData)"];
 * UserApp->WiMOD  [label="convert(LowLevelData, joinData)"];
 * ...;
 * UserApp<<WiMOD  ;
 * UserApp->UserApp [label="further processing"];
 * \endmsc
 *
 * @code
 * void myJoinedNwkInd(TWiMODLR_HCIMessage& rxMsg) {
 * 	TWiMODGlobalLink24_RX_JoinedNwkData joinData;
 *
 * 	// convert/copy the raw message to a joined nwk indication data structure
 * 	if (wimod.convert(rxMsg, &joinData)) {
 * 		// OK, process data
 * 		...
 * 	}
 * }
 * ...
 * void setup() {
 * 	...
 * 	// setup user callback for joined network indication (for OTAA procedure)
 * 	wimod.RegisterJoinedNwkIndicationClient(myJoinedNwkInd);
 * }
 * ...
 *
 * @endcode
 */
bool WiMODGlobalLink24::convert(TWiMODLR_HCIMessage& RxMsg,
        TWiMODGlobalLink24_RX_JoinedNwkData* joinedNwkData)
{
    return GlobalLinkImp.convert(RxMsg, joinedNwkData);
}

//-----------------------------------------------------------------------------
/**
 * @brief Convert a received low level HCI-Msg to a high-level AckData structure
 *
 * This function should be used by the Rx Ack Indication Callback function
 * prior processing the received data message.
 *
 * @param   RxMsg       Reference to low-level HCI message.
 *                      @warning DO NOT MANIPULATE THESE VALUES !!!
 *
 * @param   ackData     Pointer to the buffer where to store the received data
 *
 * @retval true     if the conversion was successful
 *
 * \msc
 * UserApp,WiMOD,Server;
 * |||;
 * UserApp->WiMOD  [label="RegisterRxAckIndicationClient(myRxAckInd)"];
 * ...;
 * UserApp->WiMOD  [label="SendCData(data)"];
 * WiMOD->Server   [label="transmit C data"];
 * WiMOD<-Server   [label="ACK"];
 * UserApp<<=WiMOD [label="myRxAckInd(LowLevelData)"];
 * UserApp->WiMOD  [label="convert(LowLevelData, rxAckData)"];
 * ...;
 * UserApp<<WiMOD  ;
 * UserApp->UserApp [label="further processing"];
 * \endmsc
 *
 * @code
 * void myRxAckInd(TWiMODLR_HCIMessage& rxMsg) {
 * 	TWiMODGlobalLink24_RX_ACK_Data rxAckData;
 *
 * 	// convert/copy the raw message to a RX ACK data structure
 * 	if (wimod.convert(rxMsg, &rxAckData)) {
 * 		// OK, process data
 * 		...
 * 	}
 * }
 * ...
 * void setup() {
 * 	...
 * 	// setup user callback for receiving RX ACK indications
 * 	wimod.RegisterRxAckIndicationClient(myRxAckInd);
 * }
 * ...
 *
 * @endcode
 */
bool WiMODGlobalLink24::convert(TWiMODLR_HCIMessage& RxMsg,
        TWiMODGlobalLink24_RX_ACK_Data* ackData)
{
    return GlobalLinkImp.convert(RxMsg, ackData);
}


//-----------------------------------------------------------------------------
/**
 * @brief Convert a received low level HCI-Msg to a high-level NoData structure
 *
 * This function should be used by the NoData Indication Callback function
 * prior processing the received data message.
 *
 * @param   RxMsg       Reference to low-level HCI message.
 *                      @warning DO NOT MANIPULATE THESE VALUES !!!
 *
 * @param   info      Pointer to the buffer where to store the received information
 *
 * @retval true     if the conversion was successful
 *
 *
 * \msc
 * UserApp,WiMOD,Server;
 * |||;
 * UserApp->WiMOD  [label="RegisterNoDataIndicationClient(myNoDataInd)"];
 * ...;
 * UserApp->WiMOD  [label="SendUData(data)"];
 * WiMOD->Server   [label="transmit U data"];
 * |||;
 * ...;
 * |||;
 * UserApp<<=WiMOD [label="myNoDataInd(LowLevelData)"];
 * UserApp->WiMOD  [label="convert(LowLevelData, info)"];
 * ...;
 * UserApp<<WiMOD  ;
 * UserApp->UserApp [label="further processing"];
 * \endmsc
 *
 */

bool WiMODGlobalLink24::convert(TWiMODLR_HCIMessage& RxMsg,
		TWiMODGlobalLink24_NoData_Data* info)
{
    return GlobalLinkImp.convert(RxMsg, info);
}


//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the event "TX Join Indication"
 *
 * This registered callback is called when the specified event is called by
 * the WiMOD stack.
 *
 * @param   cb          pointer to a callback function that should be called
 *                      if the event occurs.
 *
 * \msc
 * UserApp,WiMOD,Server;
 * |||;
 * UserApp->WiMOD  [label="RegisterNoDataIndicationClient(myNoDataInd)"];
 * ...;
 * --- [label="timeout example for TX C-Data"];
 * UserApp->WiMOD  [label="SendCData(data)"];
 * WiMOD->Server   [label="transmit C data"];
 * WiMOD x- Server [label="ACK"];
 * UserApp<<=WiMOD [label="myNoDataInd()"];
 * UserApp->UserApp [label="further processing"];
 * ...;
 * \endmsc
 *
 * @code
 * void myNoDataInd() {
 * 	// no data has been received; timeout occurred.?
 * 	...
 * }
 * ...
 * void setup() {
 * 	...
 * 	// setup user callback for "no data rx" indications
 * 	wimod.RegisterNoDataIndicationClient(myNoDataInd);
 * }
 * ...
 * @endcode
 */
void WiMODGlobalLink24::RegisterNoDataIndicationClient(TNoDataIndicationCallback cb)
{
    return GlobalLinkImp.RegisterNoDataIndicationClient(cb);
}

//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the event "TX C-Data Indication"
 *
 * This registered callback is called when the specified event is called by
 * the WiMOD stack.
 *
 * @param   cb          pointer to a callback function that should be called
 *                      if the event occurs.
 * \msc
 * UserApp,WiMOD,Server;
 * |||;
 * UserApp=>WiMOD  [label="RegisterTxCDataIndicationClient(myTxInd)"];
 * ...;
 * UserApp->WiMOD  [label="SendCData(data)"];
 * WiMOD->Server   [label="transfer data"];
 * UserApp<<=WiMOD [label="myTxInd(LowLevelData)"];
 * UserApp->WiMOD  [label="convert(LowLevelData, txIndData)"];
 * UserApp<<WiMOD  ;
 * UserApp->UserApp [label="further processing"];
 * ...;
 * \endmsc
 *
 * @code
 * void myTxInd(TWiMODLR_HCIMessage& rxMsg) {
 * 	TWiMODGlobalLink24_TxIndData txData;
 *
 * 	// convert/copy the raw message to tx indication data
 * 	if (wimod.convert(rxMsg, &txData)) {
 * 		// OK, process data
 * 		...
 * 	}
 * }
 * ...
 * void setup() {
 * 	...
 * 	// setup user callback(s) for TX indication events
 * 	wimod.RegisterTxCDataIndicationClient(myTxInd);
 * }
 * ...
 * @endcode
 */
void WiMODGlobalLink24::RegisterTxCDataIndicationClient(TTxCDataIndicationCallback cb)
{
    return GlobalLinkImp.RegisterTxCDataIndicationClient(cb);
}

//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the event "TX U Data Indication"
 *
 * This registered callback is called when the specified event is called by
 * the WiMOD stack.
 *
 * @param   cb          pointer to a callback function that should be called
 *                      if the event occurs.
 *
 * \msc
 * UserApp,WiMOD,Server;
 * |||;
 * UserApp=>WiMOD  [label="RegisterTxUDataIndicationClient(myTxInd)"];
 * ...;
 * UserApp->WiMOD  [label="SendUData(data)"];
 * WiMOD->Server   [label="transfer data"];
 * UserApp<<=WiMOD [label="myTxInd(LowLevelData)"];
 * UserApp->WiMOD  [label="convert(LowLevelData, txIndData)"];
 * UserApp<<WiMOD  ;
 * UserApp->UserApp [label="further processing"];
 * ...;
 * \endmsc
 *
 * @code
 * void myTxInd(TWiMODLR_HCIMessage& rxMsg) {
 * 	TWiMODGlobalLink24_TxIndData txData;
 *
 * 	// convert/copy the raw message to tx indication data
 * 	if (wimod.convert(rxMsg, &txData)) {
 * 		// OK, process data
 * 		...
 * 	}
 * }
 * ...
 * void setup() {
 * 	...
 * 	// setup user callback(s) for TX indication events
 * 	wimod.RegisterTxUDataIndicationClient(myTxInd);
 * }
 * ...
 * @endcode
 */
void WiMODGlobalLink24::RegisterTxUDataIndicationClient(TTxUDataIndicationCallback cb)
{
    return GlobalLinkImp.RegisterTxUDataIndicationClient(cb);
}

//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the event "RX U-Data Indication"
 *
 * This registered callback is called when the specified event is called by
 * the WiMOD stack.
 *
 * @param   cb          pointer to a callback function that should be called
 *                      if the event occurs.
 * \msc
 * UserApp,WiMOD;
 * UserApp=>WiMOD  [label="RegisterRxUDataIndicationClient(clientCB)"];
 * ...;
 * UserApp<<=WiMOD [label="RxUDataIndication()"];
 * \endmsc
 */
void WiMODGlobalLink24::RegisterRxUDataIndicationClient(TRxUDataIndicationCallback cb)
{
    return GlobalLinkImp.RegisterRxUDataIndicationClient(cb);
}

//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the event "RX C-Data Indication"
 *
 * This registered callback is called when the specified event is called by
 * the WiMOD stack.
 *
 * @param   cb          pointer to a callback function that should be called
 *                      if the event occurs.
 * \msc
 * UserApp,WiMOD;
 * UserApp=>WiMOD  [label="RegisterRxCDataIndicationClient(clientCB)"];
 * ...;
 * UserApp<<=WiMOD [label="RxCDataIndication()"];
 * \endmsc
 */
void WiMODGlobalLink24::RegisterRxCDataIndicationClient(TRxCDataIndicationCallback cb)
{
    return GlobalLinkImp.RegisterRxCDataIndicationClient(cb);
}

//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the event "RX MAC Cmd Indication"
 *
 * This registered callback is called when the specified event is called by
 * the WiMOD stack.
 *
 * @param   cb          pointer to a callback function that should be called
 *                      if the event occurs.
 * \msc
 * UserApp,WiMOD;
 * UserApp=>WiMOD  [label="RegisterRxMacCmdIndicationClient(clientCB)"];
 * ...;
 * UserApp<<=WiMOD [label="RxMacCmdIndication()"];
 * \endmsc
 */
void WiMODGlobalLink24::RegisterRxMacCmdIndicationClient(TRxMacCmdIndicationCallback cb)
{
    return GlobalLinkImp.RegisterRxMacCmdIndicationClient(cb);
}

//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the event "Joined Nwk Indication"
 *
 * This registered callback is called when the specified event is called by
 * the WiMOD stack.
 *
 * @param   cb          pointer to a callback function that should be called
 *                      if the event occurs.
 * \msc
 * UserApp,WiMOD;
 * UserApp=>WiMOD  [label="RegisterJoinedNwkIndicationClient(clientCB)"];
 * ...;
 * UserApp<<=WiMOD [label="JoinedNwkIndication()"];
 * \endmsc
 */
void WiMODGlobalLink24::RegisterJoinedNwkIndicationClient(TJoinedNwkIndicationCallback cb)
{
    return GlobalLinkImp.RegisterJoinedNwkIndicationClient(cb);
}

//-----------------------------------------------------------------------------
/**
 * @brief Register a callback function for the event "RX ACK (data) Indication"
 *
 * This registered callback is called when the specified event is called by
 * the WiMOD stack.
 *
 * @param   cb          pointer to a callback function that should be called
 *                      if the event occurs.
 * \msc
 * UserApp,WiMOD;
 * UserApp=>WiMOD  [label="RegisterRxAckIndicationClient(clientCB)"];
 * ...;
 * UserApp<<=WiMOD [label="RxAckIndication()"];
 * \endmsc
 */
void WiMODGlobalLink24::RegisterRxAckIndicationClient(TRxAckIndicationCallback cb)
{
    return GlobalLinkImp.RegisterRxAckIndicationClient(cb);
}


//-----------------------------------------------------------------------------
/**
 * @brief Tries to send transmit U-Data to network server via RF link
 *
 *
 * @param data       pointer to data structure containing the TX-data and options.
 *                   @see TWiMODGlobalLink24_TX_Data for details
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
 * UserApp,WiMOD,Server;
 * ... ;
 * UserApp=>WiMOD  [label="SendUData(data)"];
 * WiMOD->Server   [label="transfer data"];
 * UserApp<<WiMOD  [label="true/false"];
 * ...;
 * \endmsc
 *
 * @code
 * ...
 * // local variable
 * TWiMODGlobalLink24_TX_Data txData;
 *
 * // setup tx packet
 * txData.Port   = 1;
 * txData.Length =  strlen("Hello World!");
 * strncpy(txData.Payload, "Hello World!", WiMODLORAWAN_APP_PAYLOAD_LEN);
 *
 * // send data
 * wimod.SendUData(&txData);
 * ...
 * @endcode
 */
bool WiMODGlobalLink24::SendUData(const TWiMODGlobalLink24_TX_Data* data,
                             TWiMODLRResultCodes*         hciResult,
                             UINT8*                      rspStatus)
{
    return GlobalLinkImp.SendUData(data, hciResult, rspStatus);
}

//-----------------------------------------------------------------------------
/**
 * @brief Tries to send transmit C-Data to network server via RF link
 *
 *
 * @param data       pointer to data structure containing the TX-data and options.
 *                   @see TWiMODGlobalLink24_TX_Data for details
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
 * UserApp,WiMOD,Server;
 * ... ;
 * UserApp=>WiMOD  [label="SendCData(data)"];
 * WiMOD->Server   [label="transfer data"];
 * UserApp<<WiMOD  [label="true/false"];
 * ...;
 * WiMOD << Server [label= "ACK"];
 * --- [label="if ack callback is registered"];
 * UserApp << WiMOD [label="myAckCallback"];
 * ...;
 *
 * \endmsc
 *
 * @code
 * ...
 * // local variable
 * TWiMODGlobalLink24_TX_Data txData;
 *
 * // setup tx packet
 * txData.Port   = 1;
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
 */
bool WiMODGlobalLink24::SendCData(const TWiMODGlobalLink24_TX_Data* data,
                             TWiMODLRResultCodes*         hciResult,
                             UINT8*                      rspStatus)
{
    return GlobalLinkImp.SendCData(data, hciResult, rspStatus);
}

//-----------------------------------------------------------------------------
/**
 * @brief Sets a new radio config parameter set of the WiMOD
 *
 *
 * @param data       pointer to data structure containing the new parameters
 *                   @see TWiMODGlobalLink24_TX_Data for details
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
 * // local variable
 * TWiMODGlobalLink24_RadioStackConfig radioCfg;
 *
 * // setup new config
 * radioCfg.DataRateIndex   = (uint8_t) LoRaWAN_DataRate_WW2G4_LoRa_SF9_812kHz;
 * radioCfg.TXPowerLevel    = LORAWAN_TX_POWER_LEVEL_MIN;
 * radioCfg.Options         = LORAWAN_STK_OPTION_ADR |
 *                            LORAWAN_STK_OPTION_DEV_CLASS_C |
 *                            LORAWAN_STK_OPTION_EXT_PKT_FORMAT;
 * radioCfg.PowerSavingMode = LORAWAN_POWER_SAVING_MODE_OFF;
 * radioCfg.Retransmissions = 7;
 * radioCfg.BandIndex       = LORAWAN_BAND_EU_868_RX2_SF9;
 *
 * // set new radio config
 * wimod.SetRadioStackConfig(&radioCfg);
 *
 * // wait a little bit for config activation
 * @endcode
 *
 */
bool WiMODGlobalLink24::SetRadioStackConfig(TWiMODGlobalLink24_RadioStackConfig* data,
                                       TWiMODLRResultCodes*         hciResult,
                                       UINT8*                      rspStatus)
{
    return GlobalLinkImp.SetRadioStackConfig(data, hciResult, rspStatus);

}

//-----------------------------------------------------------------------------
/**
 * @brief Gets the current radio config parameter set of the WiMOD
 *
 *
 * @param data       pointer to data structure for storing the requested information
 *                   @see TWiMODGlobalLink24_TX_Data for details
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
 * // local variable
 * TWiMODGlobalLink24_RadioStackConfig radioCfg;
 *
 * // get current radio config
 * if (wimod.GetRadioStackConfig(&radioCfg)) {
 *	// check / process
 * 	...
 * }
 * @endcode
 *
 */
bool WiMODGlobalLink24::GetRadioStackConfig(TWiMODGlobalLink24_RadioStackConfig* data,
                                       TWiMODLRResultCodes*         hciResult,
                                       UINT8*                      rspStatus)
{
    return GlobalLinkImp.GetRadioStackConfig(data, hciResult, rspStatus);
}

//-----------------------------------------------------------------------------
/**
 * @brief Deactivate the device (logical disconnect from lora network)
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
 * // "disconnect" this endnode from the network
 * if (wimod.DeactivateDevice()) {
 *		// node is disconnected and can not transfer data to server
 *		// unless it will be re-activated
 * }
 * ...
 * @endcode
 */
bool WiMODGlobalLink24::DeactivateDevice(TWiMODLRResultCodes*         hciResult,
                                    UINT8*                      rspStatus)
{
    return GlobalLinkImp.DeactivateDevice(hciResult, rspStatus);
}

//-----------------------------------------------------------------------------
/**
 * @brief Reset all internal settings to default values (incl. DevEUI !!!)
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
 * // do a complete factory reset of the WiMOD module
 * if (wimod.FactoryReset()) {
 *		// WiMOD module will reset all internal values and will
 *		// do a reboot;
 *
 *		// wait a while for reboot to be completed
 * }
 * ...
 * @endcode
 */
bool WiMODGlobalLink24::FactoryReset(TWiMODLRResultCodes*         hciResult,
                                UINT8*                      rspStatus)
{
    return GlobalLinkImp.FactoryReset(hciResult, rspStatus);
}

//-----------------------------------------------------------------------------
/**
 * @brief Sets a new DeviceEUI (aka. IEEE-Address) to the WiMOD
 *
 * Note: Setting a new DeviceEUI is requires to switch to customer operation
 *       mode. In "normal" application mode, this command is locked.
 *
 * @param deviceEUI  pointer to data structure containing the new parameters
 *                   (Must be an pointer of a 64bit address)
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
 * // (local) variable for the device EUI
 * uint8_t devEUI[8];
 *
 * // setup deviceEUI
 * ...
 *
 * // send the devEUI to the WiMOD module
 * if (wimod.SetDeviceEUI(devEUI)) {
 *		// WiMOD module will use the given devEUI...
 * }
 * ...
 * @endcode
 */
bool WiMODGlobalLink24::SetDeviceEUI(const UINT8*                deviceEUI,
                                TWiMODLRResultCodes*         hciResult,
                                UINT8*                      rspStatus)
{
    return GlobalLinkImp.SetDeviceEUI(deviceEUI, hciResult, rspStatus);

}

//-----------------------------------------------------------------------------
/**
 * @brief Gets the  DeviceEUI (aka. IEEE-Address) of the WiMOD
 *
 *
 * @param deviceEUI  pointer for storing the received 64bit address
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
 * // (local) variable for the device EUI
 * uint8_t devEUI[8];
 *
 * // get the devEUI
 * if (wimod.GetDeviceEUI(devEUI)) {
 *		...
 * }
 * ...
 * @endcode
 */
bool WiMODGlobalLink24::GetDeviceEUI(UINT8*                      deviceEUI,
                                TWiMODLRResultCodes*         hciResult,
                                UINT8*                      rspStatus)
{
    return GlobalLinkImp.GetDeviceEUI(deviceEUI, hciResult, rspStatus);

}

//-----------------------------------------------------------------------------
/**
 * @brief Gets the  current status of the network "connection"
 *
 *
 * @param nwkStatus  pointer for storing the requested information
 *                   @see LORAWAN_NWK_STATUS defines; e.g.:
 *                     - LORAWAN_NWK_STATUS_INACTIVE,
 *                     - LORAWAN_NWK_STATUS_ACTIVE_ABP,
 *                     - LORAWAN_NWK_STATUS_ACTIVE_OTAA,
 *                     - LORAWAN_NWK_STATUS_JOINING_OTAA
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
 * // (local) variable
 * TWiMODGlobalLink24_NwkStatus_Data nwkStatus;
 *
 * // get the nwk status
 * if (wimod.GetNwkStatus(&nwkStatus)) {
 *		switch (nwkStatus.NetworkStatus):
 *		case LORAWAN_NWK_STATUS_INACTIVE:    // device is not "connected"
 *		case LORAWAN_NWK_STATUS_ACTIVE_ABP:  // device has been activated by ABP
 *		case LORAWAN_NWK_STATUS_ACTIVE_OTAA: // device has been activated by OTAA
 *		case LORAWAN_NWK_STATUS_JOINING_OTAA:// an OTAA activation procedure is currently pending
 *		...
 * }
 * ...
 * @endcode
 */
bool WiMODGlobalLink24::GetNwkStatus(TWiMODGlobalLink24_NwkStatus_Data*	nwkStatus,
                                TWiMODLRResultCodes*         	hciResult,
                                UINT8*                      	rspStatus)
{
    return GlobalLinkImp.GetNwkStatus(nwkStatus, hciResult, rspStatus);
}

//-----------------------------------------------------------------------------
/**
 * @brief Send a MAC command to the server; expert level only
 *
 * @param cmd      pointer containing the MAC command and parameters
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
 * // (local) variable
 * TWiMODGlobalLink24_MacCmd macCmd;
 *
 * // setup
 * macCmd.DataServiceType = LORAWAN_MAC_DATA_SERVICE_TYPE_U_DATA;
 * macCmd.MacCmdID = // see LoRa spec. for MAC command IDs
 * macCmd.Length = ...;
 * macCmd.Payload = ...;
 *
 * // send mac command
 * if (wimod.SendMacCmd(macCmd)) {
 *		...
 * }
 * ...
 * @endcode
 */
bool WiMODGlobalLink24::SendMacCmd(const TWiMODGlobalLink24_MacCmd* cmd,
                              TWiMODLRResultCodes*         hciResult,
                              UINT8*                      rspStatus)
{
    return GlobalLinkImp.SendMacCmd(cmd, hciResult, rspStatus);
}

//-----------------------------------------------------------------------------
/**
 * @brief Setup a custom config for tx power settings; expert level only
 *
 * @param rfGain     new rfGain value for tx power settings; see HCI Spec.
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 */
bool WiMODGlobalLink24::SetCustomConfig(const INT8                  rfGain,
                                   TWiMODLRResultCodes*         hciResult,
                                   UINT8*                      rspStatus)
{
    return GlobalLinkImp.SetCustomConfig(rfGain, hciResult, rspStatus);
}

//-----------------------------------------------------------------------------
/**
 * @brief Get the current offet for tx power level; expert level only
 *
 * @param rfGain     pointer to store the offset value for tx power settings
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 */
bool WiMODGlobalLink24::GetCustomConfig(INT8*                rfGain,
                                   TWiMODLRResultCodes*  hciResult,
                                   UINT8*               rspStatus)
{
    return GlobalLinkImp.GetCustomConfig(rfGain, hciResult, rspStatus);
}


//-----------------------------------------------------------------------------
/**
 * @brief Get the supported bands of this firmware
 *
 * @param supportedBands  pointer  to store area for result
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 */

bool WiMODGlobalLink24::GetSupportedBands(TWiMODGlobalLink24_SupportedBands* supportedBands,
                                   TWiMODLRResultCodes*  hciResult,
                                   UINT8*               rspStatus)
{
    return GlobalLinkImp.GetSupportedBands(supportedBands, hciResult, rspStatus);
}

//-----------------------------------------------------------------------------
/**
 * @brief Get the current LinkAdrRequest configuration
 *
 * @param linkAdrReqCfg  pointer where to store the current configuration
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 */

bool WiMODGlobalLink24::GetLinkAdrReqConfig(TWiMODGlobalLink24_LinkAdrReqConfig* linkAdrReqCfg,
                                   TWiMODLRResultCodes*  hciResult,
                                   UINT8*               rspStatus)
{
    return GlobalLinkImp.GetLinkAdrReqConfig(linkAdrReqCfg, hciResult, rspStatus);
}

//-----------------------------------------------------------------------------
/**
 * @brief Set the LinkAdrRequest configuration
 *
 * @param linkAdrReqCfg  reference to the configuration to set
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 */

bool WiMODGlobalLink24::SetLinkAdrReqConfig(TWiMODGlobalLink24_LinkAdrReqConfig& linkAdrReqCfg,
								   TWiMODLRResultCodes*  hciResult,
								   UINT8*               rspStatus)
{
    return GlobalLinkImp.SetLinkAdrReqConfig(linkAdrReqCfg, hciResult, rspStatus);
}


//-----------------------------------------------------------------------------
/**
 * @brief Set the Battry Level Status
 *
 * @param battStatus Status value; 0 : mains power, 1-254 : batt level; 255: undefiend
 *
 * @param hciResult Result of the local command transmission to module
 *                  This is an optional parameter.
 *
 * @param rspStatus Status byte contained in the local response of the module
 *                  This is an optional parameter.
 *
 * @retval true     if everything is ok
 * @retval false    if something went wrong; see hciResult & rspStatus for details
 */
bool WiMODGlobalLink24::SetBatteryLevelStatus(UINT8 battStatus,
                                    TWiMODLRResultCodes*  hciResult,
                                    UINT8*                rspStatus)
{
    return GlobalLinkImp.SetBatteryLevelStatus(battStatus, hciResult, rspStatus);
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
bool WiMODGlobalLink24::ExecuteGenericCmd(TWiMODLR_Generic_CmdInfo*   info,
	       	   	   	   	   	   	   	TWiMODLRResultCodes*         hciResult,
	                                UINT8*                      rspStatus)
{
    return GlobalLinkImp.ExecuteGenericCmd(info, hciResult, rspStatus);
}


//-----------------------------------------------------------------------------
/**
 * @brief Gets the last HCI result code from the last executed command.
 *
 * If you missed setting the HciResult parameter while executing the last
 * command, this method can recall the last value of that parameter.
 *
 * @see TWiMODLRResultCodes for possible values.
 *
 * @return the result value of the last executed command
 *
 * \msc
 * UserApp,WiMOD;
 * UserApp=>WiMOD	[label="Ping()"];
 * UserApp<<WiMOD   [label="(false)"];
 * UserApp=>WiMOD	[label="GetGetLastHciResult()"];
 * UserApp<<WiMOD	[label="(result of last HCI TX operation)"];
 * ...;
 * \endmsc
 *
 * @code
 * ...
 * if (wimod.Ping() == false) {
 * 	TWiMODLRResultCodes hciResult;
 *
 * 	// get (local) result of HCI command transmission
 * 	hciResult = wimod.GetLastHciResult();
 *
 * 	// check result code
 * 	switch (hciResult):
 * 	case WiMODLR_RESULT_OK:                   // OK, no error
 * 	case WiMODLR_RESULT_PAYLOAD_LENGTH_ERROR: // given payload is too big
 * 	case WiMODLR_RESULT_PAYLOAD_PTR_ERROR:    // wrong pointer to payload (NULL?)
 * 	case WiMODLR_RESULT_TRANMIT_ERROR,        // Error sending data to WiMOD via serial interface
 * 	case WiMODLR_RESULT_SLIP_ENCODER_ERROR,   // Error during SLIP encoding
 * 	case WiMODLR_RESULT_NO_RESPONSE           // The WiMOD did not respond to a request command
 *  ...
 * }
 *
 * ...
 * @endcode
 *
 */
TWiMODLRResultCodes WiMODGlobalLink24::GetLastHciResult(void)
{
    return GlobalLinkImp.GetLastHciResult();
}

//-----------------------------------------------------------------------------
/**
 * @brief Gets the last response code of the WiMOD of the last executed command.
 *
 * If you missed setting the rspStatus parameter while executing the last
 * command, this method can recall the last value of that parameter.
 *
 * @see LORAWAN_STATUS_* defines for possible values.
 *
 * @return the result value of the last executed command
 *
 *  * @code
 * ...
 * if (wimod.Ping() == false) {
 * 	uint8_t rsp;
 *
 * 	// get (remote) response code
 * 	rsp = wimod.GetLastResponseStatus();
 *
 * 	// check result code
 * 	switch (rsp):
 * 	case GLOBALLINK24_Status_Ok:                   // OK; No error;
 * 	        // --> check "remote" ResponseStatus
 * 	case GLOBALLINK24_Status_Error:                // general (un-specific) error
 * 	case GLOBALLINK24_Status_Cmd_Not_Supported:    // Command is not supported (or unknown)
 * 	case GLOBALLINK24_Status_Wrong_parameter:      // Wrong parameter applied
 * 	case GLOBALLINK24_Status_Wrong_DeviceMode:     // Device is in wong device mode for specific command
 * 	case GLOBALLINK24_Status_Device_Not_Activated: // Device has NOT been activated; command has not been executed
 * 	case GLOBALLINK24_Status_Device_Busy:          // There is another operation pending; and or duty cycle restriction is active
 * 	case GLOBALLINK24_Status_Queue_Full:           // internal command queue is full
 * 	case GLOBALLINK24_Status_Length_Error:         // some error related to the given length information
 * 	case GLOBALLINK24_Status_No_Factory_Settings:  // Factory settings are not available
 * 	case GLOBALLINK24_Status_Channel_Blocked:      // Selected channel is blocked (duty cycle?)
 * 	case GLOBALLINK24_Status_Channel_Not_Available: // Selected channel is not available
 * 	...
 * }
 *
 * ...
 * @endcode
 *
 */
UINT8 WiMODGlobalLink24::GetLastResponseStatus(void)
{
    return GlobalLinkImp.GetLastResponseStatus();
}


void WiMODGlobalLink24::Process(void)
{
    return GlobalLinkImp.Process();
}

//------------------------------------------------------------------------------
/**
 * @internal
 *
 * @brief: Enable / Disable transmitting a wakeup sequence before every command request
 *
 * @param flag  flag for enabling / disabling the wakeup sequence (true = enable)
 *
 * @endinternal
 */
void WiMODGlobalLink24::EnableWakeupSequence(bool flag) {
    return GlobalLinkImp.EnableWakeupSequence(flag);
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


//------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------

