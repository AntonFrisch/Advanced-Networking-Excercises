//------------------------------------------------------------------------------
//! @file WiMOD_SAP_RemoteCtrl.h
//! @ingroup WiMOD_SAP_RemoteCtrl
//! <!------------------------------------------------------------------------->
//! @brief Declarations for the RemoteControl SericeAccessPoint
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


#ifndef ARDUINO_SAP_WIMOD_SAP_REMOTECTRL_H_
#define ARDUINO_SAP_WIMOD_SAP_REMOTECTRL_H_

//------------------------------------------------------------------------------
//
// Section Includes Files
//
//------------------------------------------------------------------------------


#include "WiMOD_SAP_RemoteCtrl_IDs.h"
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

//-----------------------------------------------------------------------------
//
// types for callback functions
//
//-----------------------------------------------------------------------------


// C++11 check
#ifdef WIMOD_USE_CPP11
	/* C++11 function callback definitions */

	/** Type definition for a 'RX U-Data' indication callback  */
	typedef std::function<void (TWiMODLR_HCIMessage& rxMsg)> TRemoteCtrlBtnPressedIndicationCallback;


#else
	/* pre C++11 function callback definitions */

	/** Type definition for a 'RX U-Data' indication callback function */
	typedef void (*TRemoteCtrlBtnPressedIndicationCallback)(TWiMODLR_HCIMessage& rxMsg);

#endif


//------------------------------------------------------------------------------
//
// Section class
//
//------------------------------------------------------------------------------


/**
 * @brief Implementation for the ServiceAccessPoint RadioLink (LR-Base firmware only)
 */
class WiMOD_SAP_RemoteCtrl {
public:
	WiMOD_SAP_RemoteCtrl(TWiMODLRHCI* hci, UINT8* buffer, UINT16 bufferSize);
	virtual ~WiMOD_SAP_RemoteCtrl();

    virtual bool convert(TWiMODLR_HCIMessage& RxMsg, TWiMODLR_RemoteCtrl_BtnPressed* btnInfo);

    virtual void RegisterBtnPressedClient(TRemoteCtrlBtnPressedIndicationCallback cb);

    virtual void DispatcRemoteCtlMessage(TWiMODLR_HCIMessage& rxMsg);

protected:
	//! @cond Doxygen_Suppress
	UINT8*            txPayload;
	UINT16            txPayloadSize;
	//! @endcond

private:
    //! @cond Doxygen_Suppress
    TWiMODLRHCI*       HciParser;


    TRemoteCtrlBtnPressedIndicationCallback         BtnPressedIndCallback;
    //! @endcond
};

#endif /* ARDUINO_SAP_WIMOD_SAP_REMOTECTRL_H_ */
