//------------------------------------------------------------------------------
//! @file WiMOD_SAP_HWTest.cpp
//! @ingroup WiMOD_SAP_HWTest
//! <!------------------------------------------------------------------------->
//! @brief Implementation of the commands of the hardware test SericeAccessPoint
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

#if 0 /* This should not part of the normal production code */

//------------------------------------------------------------------------------
//
// Section Includes Files
//
//------------------------------------------------------------------------------

#include "WiMOD_SAP_HWTest.h"
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
WiMOD_SAP_HWTest::WiMOD_SAP_HWTest(TWiMODLRHCI* hci, UINT8* buffer, UINT16 bufferSize) :
    HciParser(hci)
{
    txPayload = buffer;
    txyPayloadSize = bufferSize;
}


//-----------------------------------------------------------------------------
/**
 * @brief Destructor
 */
WiMOD_SAP_HWTest::~WiMOD_SAP_HWTest(void)
{

}

//-----------------------------------------------------------------------------
/**
 * @brief StartRadioTest Cmd - Try to start the specificed radio test mode
 *
 * @note: The WiMOD must be in Test-Mode in order to execute this command!
 *
 * @param   testParams  pointer to structure holding the parameters for the test mode
 * @param   statusRsp   pointer to store status byte of response mesg from WiMOD
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */

TWiMODLRResultCodes
WiMOD_SAP_HWTest::StartRadioTest(const TWiMODLR_HWTestParameter* testParams, UINT8* statusRsp) {
	TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
	UINT8 			   offset = 0;

	if (testParams && statusRsp && txPayload && (txyPayloadSize >= 9)) {

		// prepare command parameters
		txPayload[offset++] = (UINT8) testParams->TestMode;
		txPayload[offset++] = (UINT8) testParams->PowerLevel;
		txPayload[offset++] = (UINT8) testParams->Modulation;
		txPayload[offset++] = (UINT8) testParams->RfFreq_LSB;
		txPayload[offset++] = (UINT8) testParams->RfFreq_MID;
		txPayload[offset++] = (UINT8) testParams->RfFreq_MSB;
		txPayload[offset++] = (UINT8) testParams->Bandwitdh;
		txPayload[offset++] = (UINT8) testParams->SpreadingFactor;
		txPayload[offset++] = (UINT8) testParams->ErrorCoding;

        result = HciParser->SendHCIMessage(HWTEST_SAP_ID,
        								   HWTEST_MSG_RADIO_TEST_REQ,
										   HWTEST_MSG_RADIO_TEST_RSP,
                                           txPayload, offset);
        // copy response status
        if (WiMODLR_RESULT_OK == result) {
            *statusRsp = HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_STATUS_POS];
        }

	}
	return result;
}

//-----------------------------------------------------------------------------
/**
 * @brief StopRadioTest Cmd - Try to stop all test modes
 *
 * @note: The WiMOD must be in Test-Mode in order to execute this command!
 *
 * @param   statusRsp   pointer to store status byte of response mesg from WiMOD
 *
 * @retval WiMODLR_RESULT_OK     if command transmit to WiMOD was ok
 */
TWiMODLRResultCodes
WiMOD_SAP_HWTest::StopRadioTest(UINT8* statusRsp) {
	TWiMODLRResultCodes result = WiMODLR_RESULT_TRANMIT_ERROR;
	UINT8 			   offset = 0;

	if (statusRsp && txPayload && (txyPayloadSize >= 9)) {

		// prepare command parameters
		txPayload[offset++] = (UINT8) RadioTestMode_Off;
		txPayload[offset++] = (UINT8) 0x00; //PowerLevel;
		txPayload[offset++] = (UINT8) 0x00; //Modulation;
		txPayload[offset++] = (UINT8) 0x00; //RfFreq_LSB;
		txPayload[offset++] = (UINT8) 0x00; //RfFreq_MID;
		txPayload[offset++] = (UINT8) 0x00; //RfFreq_MSB;
		txPayload[offset++] = (UINT8) 0x00; //Bandwidth;
		txPayload[offset++] = (UINT8) 0x00; //SpreadingFactor;
		txPayload[offset++] = (UINT8) 0x00; //ErrorCodeing;


        result = HciParser->SendHCIMessage(HWTEST_SAP_ID,
        								   HWTEST_MSG_RADIO_TEST_REQ,
										   HWTEST_MSG_RADIO_TEST_RSP,
                                           txPayload, offset);
        // copy response status
        if (WiMODLR_RESULT_OK == result) {
            *statusRsp = HciParser->GetRxMessage().Payload[WiMODLR_HCI_RSP_STATUS_POS];
        }

	}
	return result;
}

#endif
