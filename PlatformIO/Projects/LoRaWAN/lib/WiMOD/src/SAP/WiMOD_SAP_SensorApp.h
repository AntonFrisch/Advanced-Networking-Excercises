//------------------------------------------------------------------------------
//! @file WiMOD_SAP_SensorApp.h
//! @ingroup WiMOD_SAP_SensorApp
//! <!------------------------------------------------------------------------->
//! @brief Declarations for the SensorApplication SericeAccessPoint
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
//! @author  (FB), IMST
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


#ifndef ARDUINO_SAP_WIMOD_SAP_SENSORAPP_H_
#define ARDUINO_SAP_WIMOD_SAP_SENSORAPP_H_

//------------------------------------------------------------------------------
//
// Section Includes Files
//
//------------------------------------------------------------------------------


#include "WiMOD_SAP_SensorApp_IDs.h"
#include "../HCI/WiMODLRHCI.h"

/*
 * C++11 supports a better way for function pointers / function objects
 * But C++11 mode is not supported by all platforms.
 */
#ifdef WIMOD_USE_CPP11
#include <functional>
#endif
//------------------------------------------------------------------------------
//
// Section defines
//
//------------------------------------------------------------------------------


//! @cond Doxygen_Suppress
//! @endcond



//-----------------------------------------------------------------------------
//
// types for callback functions
//
//-----------------------------------------------------------------------------
// C++11 check
#ifdef WIMOD_USE_CPP11
	/* C++11 function callback definitions */

	/** Type definition for a 'Sensor Ack Data' indication callback  */
	typedef std::function<void (TWiMODLR_HCIMessage& rxMsg)> TSensorAppAckIndicationCallback;

	/** Type definition for a 'SensorData' indication callback  */
	typedef std::function<void (TWiMODLR_HCIMessage& rxMsg)> TSensorAppSensorDataIndicationCallback;
#else
	/* pre C++11 function callback definitions */

	/** Type definition for a 'Sensor Ack Data' indication callback function */
	typedef void (*TSensorAppAckIndicationCallback)(TWiMODLR_HCIMessage& rxMsg);

	/** Type definition for a 'SensorData' indication callback function */
	typedef void (*TSensorAppSensorDataIndicationCallback)(TWiMODLR_HCIMessage& rxMsg);
#endif


//------------------------------------------------------------------------------
//
// Section class
//
//------------------------------------------------------------------------------


/**
 * @brief Implementation for the ServiceAccessPoint SensorApp (LR-Base_PLUS firmware only)
 */
class WiMOD_SAP_SensorApp {
public:
	WiMOD_SAP_SensorApp(TWiMODLRHCI* hci, UINT8* buffer, UINT16 bufferSize);
	virtual ~WiMOD_SAP_SensorApp(void);

	virtual TWiMODLRResultCodes SetSensorAppConfig(const TWiMODLR_SensorApp_Config* config, UINT8* statusRsp);
	virtual TWiMODLRResultCodes GetSensorAppConfig(TWiMODLR_SensorApp_Config* config, UINT8* statusRsp);

	virtual bool convert(TWiMODLR_HCIMessage& RxMsg, TWiMODLR_SensorApp_SensorData* sensorData);
	virtual bool convert(TWiMODLR_HCIMessage& RxMsg, TWiMODLR_SensorApp_AckSensorData* sensorAckData);

	virtual void RegisterSensorAckIndClient(TSensorAppAckIndicationCallback cb);
	virtual void RegisterSensorDataIndClient(TSensorAppSensorDataIndicationCallback cb);

	virtual void DispatchSensorAppMessage(TWiMODLR_HCIMessage& rxMsg);

protected:
	//! @cond Doxygen_Suppress
	UINT8*            txPayload;
	UINT16            txPayloadSize;
	//! @endcond

private:
	//! @cond Doxygen_Suppress

	TWiMODLRHCI*       HciParser;

	TSensorAppAckIndicationCallback				SensorAckIndCallback;
	TSensorAppSensorDataIndicationCallback		SensorDataIndCallback;
	//! @endcond
};




#endif /* ARDUINO_SAP_WIMOD_SAP_SENSORAPP_H_ */
