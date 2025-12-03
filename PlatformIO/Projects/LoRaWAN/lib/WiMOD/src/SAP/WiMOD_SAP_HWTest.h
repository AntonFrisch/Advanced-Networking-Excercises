//------------------------------------------------------------------------------
//! @file WiMOD_SAP_HWTest.h
//! @ingroup WiMOD_SAP_HWTest
//! <!------------------------------------------------------------------------->
//! @brief Declarations for the hardware test ServiceAccessPoint
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
 *
 * SEE FILE: WiMOD_LR_Base_HCI_Spec_V1_8.pdf for detailed information
 */

#if 0 /* This should not part of the normal production code */

#ifndef ARDUINO_SAP_WIMOD_SAP_HWTEST_H_
#define ARDUINO_SAP_WIMOD_SAP_HWTEST_H_

#include "WiMOD_SAP_HWTest_IDs.h"
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
/** Buffer size for DevMgmt related messages */
#define WiMOD_DEVMGMT_MSG_SIZE               (100)


//-----------------------------------------------------------------------------
//
// types for callback functions
//
//-----------------------------------------------------------------------------

// C++11 check
#ifdef WIMOD_USE_CPP11
	/* C++11 function callback definitions */

	/** Type definition for a 'device power up' indication callback  */
	typedef std::function<void (void)> TDevMgmtPowerUpCallback;

#else
	/* pre C++11 function callback definitions */

	/** Type definition for a 'device power up' indication callback function */
	typedef void (*TDevMgmtPowerUpCallback)(void);

#endif

//------------------------------------------------------------------------------
//
// Section class
//
//------------------------------------------------------------------------------

/**
 * @brief Implementation for the ServiceAccessPoint DeviceManagement
 */
class WiMOD_SAP_HWTest {
public:
    WiMOD_SAP_HWTest(TWiMODLRHCI* hci, UINT8* buffer, UINT16 bufferSize);
    ~WiMOD_SAP_HWTest(void);

    TWiMODLRResultCodes StartRadioTest(const TWiMODLR_HWTestParameter* testParams, UINT8* statusRsp);
    TWiMODLRResultCodes StopRadioTest(UINT8* statusRsp);

protected:
private:
    //! @cond Doxygen_Suppress
    UINT8*              txPayload;
    UINT16              txyPayloadSize;

    TWiMODLRHCI*       HciParser;
    //! @endcond

};

#endif

#endif /* ARDUINO_SAP_WIMOD_SAP_HWTEST_H_ */
