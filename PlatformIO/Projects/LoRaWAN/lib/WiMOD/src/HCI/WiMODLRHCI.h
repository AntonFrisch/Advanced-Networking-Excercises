//------------------------------------------------------------------------------
//! @file WiMODLRHCI.h
//! @ingroup WiMODLRHCI
//! <!------------------------------------------------------------------------->
//! @brief Common low level HCI message processing base
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


#ifndef ARDUINO_WIMODLRHCI_H_
#define ARDUINO_WIMODLRHCI_H_

//------------------------------------------------------------------------------
//
// Section Includes Files
//
//------------------------------------------------------------------------------


#include "utils/WMDefs.h"

#include <string.h>

#include "utils/ComSLIP.h"

#include "Arduino.h"

/*
 * C++11 supports a better way for function pointers / function objects
 * But C++11 mode is not supported by all platforms.
 */
#ifdef WIMOD_USE_CPP11
#include <functional>
#endif

//------------------------------------------------------------------------------
//
// Serial Baudrate
//
//------------------------------------------------------------------------------
/** default serial baudrate to communicate with a WiMOD module */
#define WIMODLR_SERIAL_BAUDRATE             115200

/** default timeout in ms for waiting for a response msg from WiMOD */
#define WIMODLR_RESPOMSE_TIMEOUT_MS         1000;

//------------------------------------------------------------------------------
//
// HCI Message Declaration
//
//------------------------------------------------------------------------------
//! @cond Doxygen_Suppress

// message header size: 2 bytes for SapID + MsgID
#define WIMODLR_HCI_MSG_HEADER_SIZE     2

// message payload size
#define WIMODLR_HCI_MSG_PAYLOAD_SIZE    280

// frame check sequence field size: 2 bytes for CRC16
#define WIMODLR_HCI_MSG_FCS_SIZE        2

// visible max. buffer size for lower SLIP layer
#define WIMODLR_HCI_RX_MESSAGE_SIZE     (WIMODLR_HCI_MSG_HEADER_SIZE\
                                         + WIMODLR_HCI_MSG_PAYLOAD_SIZE\
                                         + WIMODLR_HCI_MSG_FCS_SIZE)


#define WiMODLR_HCI_RSP_STATUS_POS      0x00

#define WiMODLR_HCI_RSP_CMD_PAYLOAD_POS 0x01

//! @endcond


//------------------------------------------------------------------------------
//
// Wake up sequence
//
//------------------------------------------------------------------------------
//! @cond Doxygen_Suppress

#define WIMODLR_NUMBER_OF_WAKEUP_CHARS      40

//! @endcond

//------------------------------------------------------------------------------
//
// HCI Message
//
//------------------------------------------------------------------------------

/**
 * @brief basic low level HCI message structure used for all serial messages to/from WiMOD
 */
typedef struct TWiMODLR_HCIMessage
{
    // Payload Length Information, not transmitted over UART interface !
    UINT16  Length;                                                             /*!< length indicator; this field is not being transfered via serial link */

    // Service Access Point Identifier
    UINT8   SapID;                                                              /*!< Service Access Point ID for this message */

    // Message Identifier
    UINT8   MsgID;                                                              /*!< Command OpCode ID for this message */

    // Payload Field
    UINT8   Payload[WIMODLR_HCI_MSG_PAYLOAD_SIZE];                              /*!< Payload field; content is command specific */

    // Frame Check Sequence Field
    UINT8   CRC16[WIMODLR_HCI_MSG_FCS_SIZE];                                    /*!< Frame Check Sum field; a 16-bit CRC algorithm is used */

} TWiMODLR_HCIMessage;


//------------------------------------------------------------------------------
//
// Definition of Result/Error Codes
//
//------------------------------------------------------------------------------

/**
 * @brief   Result codes for the local serial communication itself
 */
typedef enum TWiMODLRResultCodes
{
    WiMODLR_RESULT_OK = 0,                                                      /*!< OK, no error  */
    WiMODLR_RESULT_PAYLOAD_LENGTH_ERROR,                                        /*!< given payload is too big */
    WiMODLR_RESULT_PAYLOAD_PTR_ERROR,                                           /*!< wrong pointer to payload (NULL?) */
    WiMODLR_RESULT_TRANMIT_ERROR,                                               /*!< Error sending data to WiMOD via serial interface*/
    WiMODLR_RESULT_SLIP_ENCODER_ERROR,                                          /*!< Error during SLIP encoding */
    WiMODLR_RESULT_NO_RESPONSE                                                  /*!< The WiMOD did not respond to a request command*/
}TWiMODLRResultCodes;



//------------------------------------------------------------------------------
//
// Error indicator callback
//
//------------------------------------------------------------------------------

/**
 * @brief Internal error reasons; not to be used in user code
 */
typedef enum TWiMODStackError
{
    WIMOD_STACK_ERR_UNKNOWN_RX_MESSAGE,                                         /*!< rx message is not processed */
    WIMOD_STACK_ERR_UNKNOWN_RX_SAP_ID,                                          /*!< unknown SAP-ID  */
    WIMOD_STACK_ERR_UNKNOWN_RX_CMD_ID,                                          /*!< unknown CMD-ID */

} TWiMODStackError;


// C++11 check
#ifdef WIMOD_USE_CPP11
	/* C++11 function callback definitions */


	/**
	 * @brief Type definition for indicator callback for stack (internal) error
	 */
	typedef std::function<void (TWiMODStackError)> TWiMODStackErrorClient;

#else
	/* pre C++11 function callback definitions */

	/**
	 * @brief Type definition for indicator callback for stack (internal) error
	 */
	typedef void (*TWiMODStackErrorClient)(TWiMODStackError);

#endif

//------------------------------------------------------------------------------
//
// TWiMODLRHCIClient Class Declaration
//
//------------------------------------------------------------------------------

/**
 * @brief Internal helper class for processing HCI frames
 */
class TWiMODLRHCIClient
{
    public:
                        TWiMODLRHCIClient() {}
    virtual             ~TWiMODLRHCIClient() {}


    // define handler for received indication messasges
    /**
     * @internal
     * @brief abstract function for doing the processing of received HCI messages.
     * @param TWiMODLR_HCIMessage reference to HCI message
     * @endinternal
     */
    virtual void        ProcessRxMessage(const TWiMODLR_HCIMessage& /* rxMsg */) = 0;
};


//------------------------------------------------------------------------------
//
// TWiMODLRHCI Class Declaration
//
//------------------------------------------------------------------------------

/**
 * @brief Internal helper class for processing SLIP frames
 */
class TWiMODLRHCI : public TComSlipClient
{
    public:
	/*explicit*/ TWiMODLRHCI(Stream& s);
    ~TWiMODLRHCI(void);

    virtual void begin(void);
    virtual void end(void);


    TWiMODLRResultCodes  SendHCIMessage(UINT8 dstSapID, UINT8 msgID, UINT8 rxMsgID, UINT8* payload, UINT16 length);
    TWiMODLRResultCodes  SendHCIMessageWithoutRx(UINT8 dstSapID, UINT8 msgID,  UINT8* payload, UINT16 length);
    void                Process(void);
    void                SendWakeUpSequence(void);

    void                RegisterStackErrorClient(TWiMODStackErrorClient cb);
//    void                RegisterRxMessageClient(TWiMODLRHCIClient* cb);

    const TWiMODLR_HCIMessage& GetRxMessage(void);

    // enable / disable wakeup sequence
    void EnableWakeupSequence(bool flag);


    protected:
    TWiMODLRResultCodes  PostMessage(UINT8 sapID, UINT8 msgID, UINT8* payload, UINT16 length);
    TWiMODLRResultCodes  SendPacket(UINT8* txData, UINT16 length);
    bool                WaitForResponse(UINT8 rxSapID, UINT8 rxMsgID);
    UINT8*              ProcessRxMessage(UINT8* rxBuffer, UINT16 length);
    //! @cond Doxygen_Suppress
    virtual void        ProcessUnexpectedRxMessage(TWiMODLR_HCIMessage& rxMsg) = 0;
    // @end_cond

    // receiver struct
    /**
     * @brief Structure for storing serial RX related variables
     */
    typedef struct TReceiver
    {

        bool        Active;                                                     /*!< flag indicating that a response is expected */
        bool        Done;                                                       /*!< flag indicating response successfully received */
        UINT8       SapID;                                                      /*!< SAP ID of expected response */
        UINT8       MsgID;                                                      /*!< Msg ID  of expected response */
        TWiMODLR_HCIMessage Message;                                            /*!< reserve one Rx-Message-Buffer */

#ifdef WIMOD_USE_2ND_RXBUFFER
        // only neccessary for non IRQ mode!
        TWiMODLR_HCIMessage ExpectedRsponseMsg;                                 /*!< Rx-Message-Buffer hat holds the expected rsponse message */
#endif
        // Timeout (~1000ms)
        int         Timeout;                                                    /*!< timout in ms for waiting for response message */
    }TReceiver;

    //! @cond Doxygen_Suppress

    // receiver instance
    TReceiver           Rx;


    // Stack error indicator callback
    TWiMODStackErrorClient   StackErrorClientCB;


    //! @endcond

    private:
        virtual void            DispatchRxMessage(TWiMODLR_HCIMessage& rxMsg);

        //! @cond Doxygen_Suppress
        TWiMODLRHCIClient*      RxMessageClient;

        Stream&             serial;
        TComSlip            comSlip;

        TWiMODLR_HCIMessage TxMessage;

        bool                wakeUp;

        //! @endcond
};

#endif /* ARDUINO_WIMODLRHCI_H_ */

//------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------
