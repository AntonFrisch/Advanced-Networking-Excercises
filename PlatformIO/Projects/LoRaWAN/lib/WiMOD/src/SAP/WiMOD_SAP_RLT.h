//------------------------------------------------------------------------------
//! @file WiMOD_SAP_RLT.h
//! @ingroup WiMOD_SAP_RLT
//! <!------------------------------------------------------------------------->
//! @brief Declarations for the RadioLinkTest SericeAccessPoint
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


#ifndef ARDUINO_SAP_WIMOD_SAP_RLT_H_
#define ARDUINO_SAP_WIMOD_SAP_RLT_H_

//------------------------------------------------------------------------------
//
// Section Includes Files
//
//------------------------------------------------------------------------------

#include "WiMOD_SAP_RLT_IDs.h"
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

	/** Type definition for a 'RLT Status' indication callback  */
	typedef std::function<void (TWiMODLR_HCIMessage& rxMsg)> TRltStatusIndicationCallback;

#else
	/* pre C++11 function callback definitions */

	/** Type definition for a 'RLT Status' indication callback function */
	typedef void (*TRltStatusIndicationCallback)(TWiMODLR_HCIMessage& rxMsg);

#endif


//------------------------------------------------------------------------------
//
// Section class
//
//------------------------------------------------------------------------------


/**
 * @brief Implementation for the ServiceAccessPoint RadioLinkTest (LR-Base firmware only)
 */
class WiMOD_SAP_RLT {
public:
	WiMOD_SAP_RLT(TWiMODLRHCI* hci, UINT8* buffer, UINT16 bufferSize);
	virtual ~WiMOD_SAP_RLT(void);

	virtual TWiMODLRResultCodes StartRadioLinkTest(const TWiMODLR_RLT_Parameter* params, UINT8* statusRsp);
	virtual TWiMODLRResultCodes StopRadioLinkTest(UINT8* statusRsp);

	virtual bool convert(TWiMODLR_HCIMessage&    RxMsg,	TWiMODLR_RLT_Status* statusData);

	virtual void RegisterStatusIndClient(TRltStatusIndicationCallback cb);

	virtual void DispatchRLTMessage(TWiMODLR_HCIMessage& rxMsg);

protected:
	//! @cond Doxygen_Suppress
	UINT8*            txPayload;
	UINT16            txPayloadSize;
	//! @endcond

private:
	//! @cond Doxygen_Suppress

	TWiMODLRHCI*       HciParser;

	TRltStatusIndicationCallback				StatusIndCallback;
	//! @endcond
};






#endif /* ARDUINO_SAP_WIMOD_SAP_RLT_H_ */
