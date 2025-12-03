//------------------------------------------------------------------------------
//! @file WiMOD_SAP_HWTest_IDs.h
//! @ingroup WiMOD_SAP_HSTest
//! <!------------------------------------------------------------------------->
//! @brief Supporting IDs and definitions for the hardware test ServiceAccessPoint
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

#ifndef ARDUINO_SAP_WIMOD_SAP_HWTEST_IDS_H_
#define ARDUINO_SAP_WIMOD_SAP_HWTEST_IDS_H_

#include "WiMOD_SAP_DEVMGMT_IDs.h"

//------------------------------------------------------------------------------
//
// Service Access Point Identifier
//
//------------------------------------------------------------------------------

/** SAP ID for the DevMmgt service */
#define HWTEST_SAP_ID                      0xA1



//------------------------------------------------------------------------------
//
//  Hardware Test Message Identifier
//
//------------------------------------------------------------------------------

#define HWTEST_MSG_RADIO_TEST_REQ        0x01
#define HWTEST_MSG_RADIO_TEST_RSP        0x02



//------------------------------------------------------------------------------
//
// enums
//
//------------------------------------------------------------------------------

/**
 * @brief	Radio Test mode for hardware radio tests
 */
typedef enum TWiMODLRRadioTestMode
{
	RadioTestMode_Off = 0x00,													/*!< Test mode off, deactivate all tests */
	RadioTestMode_CW,															/*!< Continuous Wave test */

} TWiMODLRRadioTestMode;


//------------------------------------------------------------------------------
//
// structures
//
//------------------------------------------------------------------------------

/**
 * @brief Radio Test Parameter Set
 */
typedef struct TWiMODLR_HWTestParameter
{
	TWiMODLRRadioTestMode			TestMode;									/*!< test mode to use */
	TRadioCfg_PowerLevel			PowerLevel;                                 /*!< tx power settings to use */
	TRadioCfg_Modulation			Modulation;                                 /*!< modulation to use */
    UINT8                           RfFreq_LSB;                                 /*!< lower part of the 24bit frequency register of the transceiver*/
    UINT8                           RfFreq_MID;                                 /*!< mid part of the 24bit frequency register of the transceiver*/
    UINT8                           RfFreq_MSB;                                 /*!< high part of the 24bit frequency register of the transceiver*/
	TRadioCfg_LoRaBandwidth			Bandwitdh;                                  /*!< bandwidth settings (for lora modulation) */
	TRadioCfg_LoRaSpreadingFactor	SpreadingFactor;                            /*!< lora spreading factor to use */
	TRadioCfg_ErrorCoding			ErrorCoding;                                /*!< lora error coding to use*/
} TWiMODLR_HWTestParameter;

#endif

#endif /* ARDUINO_SAP_WIMOD_SAP_HWTEST_IDS_H_ */
