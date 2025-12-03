//------------------------------------------------------------------------------
//! @file WiMOD_SAP_RLT_IDs.h
//! @ingroup WiMOD_SAP_RLT
//! <!------------------------------------------------------------------------->
//! @brief Supporting IDs and definitions for the RadioLinkTest ServiceAccessPoint
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


#ifndef ARDUINO_SAP_WIMOD_SAP_RLT_IDS_H_
#define ARDUINO_SAP_WIMOD_SAP_RLT_IDS_H_

//------------------------------------------------------------------------------
//
// Section Includes Files
//
//------------------------------------------------------------------------------

#include "utils/WMDefs.h"

//------------------------------------------------------------------------------
//
// Service Access Point Identifier
//
//------------------------------------------------------------------------------

/** SAP ID for the RadioLink service */
#define RLT_SAP_ID                      0x02



//------------------------------------------------------------------------------
//
//  Radio Link Services Identifier
//
//------------------------------------------------------------------------------
//! @cond Doxygen_Suppress
#define RLT_MSG_START_REQ		                  		0x01
#define RLT_MSG_START_RSP		                  		0x02
#define RLT_MSG_STOP_REQ		                  		0x03
#define RLT_MSG_STOP_RSP		                  		0x04
#define RLT_MSG_STATUS_IND		                  		0x06
//! @endcond

//------------------------------------------------------------------------------
//
//  RLT Status Codes
//
//------------------------------------------------------------------------------
//! @cond Doxygen_Suppress

#define RLT_STATUS_OK    		                        0x00
#define RLT_STATUS_ERROR                                0x01
#define RLT_STATUS_CMD_NOT_SUPPORTED              		0x02
#define RLT_STATUS_WRONG_PARAMETER                		0x03
#define RLT_STATUS_WRONG_RADIO_MODE               		0x04
#define RLT_STATUS_WRONG_DEVICECONFIG                   0x05
//! @endcond


//------------------------------------------------------------------------------
//
// structures
//
//------------------------------------------------------------------------------

/**
 * @brief RTL TestModes
 */
typedef enum TRLT_TestMode {
	RLT_TestMode_Single   = 0,													/*!< Single test run */
	RLT_TestMode_Repeated = 1,													/*!< repeated test runs; must be stpped by host! */
} TRLT_TestMode;

/**
 * @brief Structure containing a general RadioLink HCI message
 */
typedef struct TWiMODLR_RLT_Parameter
{
	UINT8			DestGroupAddress;											/*!< destionation group of peer device */
	UINT16			DestDevAddress;												/*!< address of peer device */
	UINT8			PacketSize;													/*!< number of dummy payload bytes to use */
	UINT16			NumPackets;													/*!< number of tests / test transmisions */
	TRLT_TestMode	TestMode;													/*!< test mode to use; @see TRLT_TestMode */
} TWiMODLR_RLT_Parameter;

/**
 * @brief Structure containing a general RadioLink HCI message
 */
typedef struct TWiMODLR_RLT_Status
{
    UINT8       TestStatus;														/*!< Status of Test; 0 = OK, 1 = start of new test run*/
    UINT16		LocalTxCounter;													/*!< Number of transmitted packets from local device to peer device */
    UINT16		LocalRxCounter;													/*!< Number of received packets on local device*/
    UINT16		PeerTxCounter;													/*!< Number of transmitted packets from peer device to local device*/
    UINT16		PeerRxCounter;													/*!< Number of packets received on peer device*/
    UINT16		LocalRSSI;														/*!< RSSI value [dBm] of last received packet on local device*/
    UINT16		PeerRSSI;														/*!< RSSI value [dBm] of last received packet on peer device */
    UINT8		LocalSNR;														/*!< SNR value [dB] of last received packet on local device  */
    UINT8		PeerSNR;														/*!< SNR value [dB] of last received value on peer device    */
} TWiMODLR_RLT_Status;




#endif /* ARDUINO_SAP_WIMOD_SAP_RLT_IDS_H_ */
