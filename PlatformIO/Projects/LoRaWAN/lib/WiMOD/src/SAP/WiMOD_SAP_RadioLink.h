//------------------------------------------------------------------------------
//! @file WiMOD_SAP_RadioLink.h
//! @ingroup WiMOD_SAP_RadioLink
//! <!------------------------------------------------------------------------->
//! @brief Declarations for the RadioLink SericeAccessPoint
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
 * THIS IS AN EXAMPLE IMPLEMENTATION ACCORDING THE THE HCI SPEC: V1.10
 * FOR FIRMWARE: LR-BASE
 *
 * SEE FILE: WiMOD_LR_Base_HCI_Spec_V1_10.pdf for detailed information
 */



#ifndef ARDUINO_SAP_WIMOD_SAP_RADIOLINK_H_
#define ARDUINO_SAP_WIMOD_SAP_RADIOLINK_H_

//------------------------------------------------------------------------------
//
// Section Includes Files
//
//------------------------------------------------------------------------------

#include "WiMOD_SAP_RadioLink_IDs.h"
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
#define WiMOD_RADIOLINK_MSG_SIZE               (WiMODLRBASE_APP_PAYLOAD_LEN)
//! @endcond



//-----------------------------------------------------------------------------
//
// types for callback functions
//
//-----------------------------------------------------------------------------


// C++11 check
#ifdef WIMOD_USE_CPP11
	/* C++11 function callback definitions */

	/** Type definition for a 'RX U-Data' indication callback  */
	typedef std::function<void (TWiMODLR_HCIMessage& rxMsg)> TRadioLinkUDataRxIndicationCallback;

	/** Type definition for a 'TX U-Data' indication callback  */
	typedef std::function<void (void)> TRadioLinkUDataTxIndicationCallback;

	/** Type definition for a 'RX raw data' indication callback  */
	typedef std::function<void (TWiMODLR_HCIMessage& rxMsg)> TRadioLinkRawDataRxIndicationCallback;

	/** Type definition for a 'RX C-Data' indication callback  */
	typedef std::function<void (TWiMODLR_HCIMessage& rxMsg)> TRadioLinkCDataRxIndicationCallback;

	/** Type definition for a 'TX C-Data' indication callback  */
	typedef std::function<void (TWiMODLR_HCIMessage& rxMsg)> TRadioLinkCDataTxIndicationCallback;

	/** Type definition for a 'RX Ack' indication callback  */
	typedef std::function<void (TWiMODLR_HCIMessage& rxMsg)> TRadioLinkAckRxIndicationCallback;

	/** Type definition for a 'Ack timeout (RX)' indication callback  */
	typedef std::function<void (void)> TRadioLinkAckRxTimeoutIndicationCallback;

	/** Type definition for a 'TX Ack' indication callback  */
	typedef std::function<void (void)> TRadioLinkAckTxIndicationCallback;

#else
	/* pre C++11 function callback definitions */



	/** Type definition for a 'RX U-Data' indication callback function */
	typedef void (*TRadioLinkUDataRxIndicationCallback)(TWiMODLR_HCIMessage& rxMsg);

	/** Type definition for a 'TX U-Data' indication callback function */
	typedef void (*TRadioLinkUDataTxIndicationCallback)(TWiMODLR_HCIMessage& rxMsg); // V1.10 : added parameter

	/** Type definition for a 'RX raw data' indication callback function */
	typedef void (*TRadioLinkRawDataRxIndicationCallback)(TWiMODLR_HCIMessage& rxMsg);

	/** Type definition for a 'RX C-Data' indication callback function */
	typedef void (*TRadioLinkCDataRxIndicationCallback)(TWiMODLR_HCIMessage& rxMsg);

	/** Type definition for a 'TX C-Data' indication callback function */
	typedef void (*TRadioLinkCDataTxIndicationCallback)(TWiMODLR_HCIMessage& rxMsg);

	/** Type definition for a 'RX Ack' indication callback function */
	typedef void (*TRadioLinkAckRxIndicationCallback)(TWiMODLR_HCIMessage& rxMsg);

	/** Type definition for a 'Ack timeout (RX)' indication callback function */
	typedef void (*TRadioLinkAckRxTimeoutIndicationCallback)(void);

	/** Type definition for a 'TX Ack' indication callback function */
	typedef void (*TRadioLinkAckTxIndicationCallback)(void);

#endif


//------------------------------------------------------------------------------
//
// Section class
//
//------------------------------------------------------------------------------


/**
 * @brief Implementation for the ServiceAccessPoint RadioLink (LR-Base firmware only)
 */
class WiMOD_SAP_RadioLink {
public:
    WiMOD_SAP_RadioLink(TWiMODLRHCI* hci, UINT8* buffer, UINT16 bufferSize);
    ~WiMOD_SAP_RadioLink(void);

    TWiMODLRResultCodes SendUData(const TWiMODLR_RadioLink_Msg* txMsg, UINT8* statusRsp);
    bool convert(TWiMODLR_HCIMessage& RxMsg, TWiMODLR_RadioLink_Msg* radioLinkMsg);
    TWiMODLRResultCodes SendCData(const TWiMODLR_RadioLink_Msg* txMsg, UINT8* statusRsp);
    bool convert(TWiMODLR_HCIMessage& RxMsg, TWiMODLR_RadioLink_CdataInd* cDataTxInfo);
    bool convert(TWiMODLR_HCIMessage& RxMsg, TWiMODLR_RadioLink_UdataInd* uDataTxInfo);
    bool convert(TWiMODLR_HCIMessage& RxMsg, TWiMODLR_AckTxInd_Msg* txAckIndInfo);
    TWiMODLRResultCodes SetAckData(const TWiMODLR_RadioLink_Msg* txMsg, UINT8* statusRsp);

    void RegisterUDataRxClient(TRadioLinkUDataRxIndicationCallback cb);
    void RegisterUDataTxClient(TRadioLinkUDataTxIndicationCallback cb);
    void RegisterRawDataRxClient(TRadioLinkRawDataRxIndicationCallback cb);
    void RegisterCDataRxClient(TRadioLinkCDataRxIndicationCallback cb);
    void RegisterCDataTxClient(TRadioLinkCDataTxIndicationCallback cb);
    void RegisterAckRxClient(TRadioLinkAckRxIndicationCallback cb);
    void RegisterAckRxTimeoutClient(TRadioLinkAckRxTimeoutIndicationCallback cb);
    void RegisterAckTxCallback(TRadioLinkAckTxIndicationCallback cb);

    void DispatchRadioLinkMessage(TWiMODLR_HCIMessage& rxMsg);

protected:

private:
    //! @cond Doxygen_Suppress
    UINT8*            txPayload;
    UINT16            txPayloadSize;

    TWiMODLRHCI*       HciParser;


    TRadioLinkUDataRxIndicationCallback         UDataRxIndCallback;
    TRadioLinkUDataTxIndicationCallback         UDataTxIndCallback;
    TRadioLinkRawDataRxIndicationCallback       RawDataRxIndCallback;
    TRadioLinkCDataRxIndicationCallback         CDataRxIndCallback;
    TRadioLinkCDataTxIndicationCallback         CDataTxIndCallback;
    TRadioLinkAckRxIndicationCallback           AckRxDataIndCallback;
    TRadioLinkAckRxTimeoutIndicationCallback    AckRxTimeoutIndCallback;
    TRadioLinkAckTxIndicationCallback           AckTxIndCallack;
    //! @endcond
};



#endif /* ARDUINO_SAP_WIMOD_SAP_RADIOLINK_H_ */
