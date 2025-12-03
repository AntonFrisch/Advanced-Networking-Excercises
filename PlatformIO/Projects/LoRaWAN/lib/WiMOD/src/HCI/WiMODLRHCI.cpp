//------------------------------------------------------------------------------
//! @file WiMODLRHCI.cpp
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

//------------------------------------------------------------------------------
//
// Section Includes Files
//
//------------------------------------------------------------------------------

#include <Arduino.h>
#include "WiMODLRHCI.h"
#include "utils/CRC16.h"


//------------------------------------------------------------------------------
//
// Section public functions
//
//------------------------------------------------------------------------------

/**
 * @brief Constructor
 *
 * @param s     Reference to the serial interface the the WiMOD. The interface
 *              must be initialized before any other function of this class
 *              can be used.
 */
TWiMODLRHCI::TWiMODLRHCI(Stream& s ) :
    serial(s),
    comSlip(s)
{
    StackErrorClientCB = NULL;
    RxMessageClient    = NULL;

    wakeUp             = true;

    Rx.Active 		   = false;
    Rx.Done   		   = false;
    Rx.SapID           = 0x00;
    Rx.MsgID           = 0x00;
    Rx.Timeout = WIMODLR_RESPOMSE_TIMEOUT_MS;

    TxMessage.Length    = 0x00;
    TxMessage.SapID     = 0x00;
}

//-----------------------------------------------------------------------------
/**
 * @brief Destructor
 */
TWiMODLRHCI::~TWiMODLRHCI(void)
{

}

//------------------------------------------------------------------------------

/**
 * @brief Init function of the generic HCI message handler.
 *
 * This function must be called once before any other service can be used.
 */
void TWiMODLRHCI::begin(void) {

    // register for rx-messages
    comSlip.RegisterClient(this);

    comSlip.begin(/*WIMODLR_SERIAL_BAUDRATE*/);
    comSlip.SetRxBuffer(&Rx.Message.SapID, WIMODLR_HCI_RX_MESSAGE_SIZE);
}


//-----------------------------------------------------------------------------
/**
 * @brief shutdown function
 */
void
TWiMODLRHCI::end(void) {
    comSlip.end();
}

//-----------------------------------------------------------------------------
/**
 * @brief Generic function for transferring a HCI message to the WiMOD module
 *
 * @param   dstSapID    the SAP endpoint to address
 * @param   msgID       the command ID to address within the SAP
 * @param   rxMsgID     the expected response ID according to the msgID
 * @param   payload     pointer to the payload bytes to send
 * @param   length      the number of payload bytes to send
 *
 * @retval WiMODLR_RESULT_OK    if everything was OK
 */
TWiMODLRResultCodes
TWiMODLRHCI::SendHCIMessage(UINT8 dstSapID, UINT8 msgID, UINT8 rxMsgID, UINT8* payload, UINT16 length) {

    // send wakeup sequence to get the WiMOD out of sleep ?
    if (wakeUp) {
        SendWakeUpSequence();
    }

    // send message
    TWiMODLRResultCodes result = PostMessage(dstSapID, msgID, payload, length);

    // message sent ?
    if (result == WiMODLR_RESULT_OK)
    {
        // yes, wait for response from radio module
        if (WaitForResponse(dstSapID, rxMsgID))
        {
            return WiMODLR_RESULT_OK;
        }
        return WiMODLR_RESULT_NO_RESPONSE;
    }
    // return error
    return result;
}

//-----------------------------------------------------------------------------
/**
 * @brief Generic function for transferring a HCI message to the WiMOD module
 *
 * @param   dstSapID    the SAP endpoint to address
 * @param   msgID       the command ID to address within the SAP
 * @param   payload     pointer to the payload bytes to send
 * @param   length      the number of payload bytes to send
 *
 * @retval WiMODLR_RESULT_OK    if everything was OK
 */
TWiMODLRResultCodes
TWiMODLRHCI::SendHCIMessageWithoutRx(UINT8 dstSapID, UINT8 msgID,  UINT8* payload, UINT16 length) {

    // send message
    TWiMODLRResultCodes result = PostMessage(dstSapID, msgID, payload, length);

    // return error
    return result;
}

//-----------------------------------------------------------------------------
/**
 * @brief Handle the receiver path; process all incomming bytes from the WiMOD
 *
 * This function checks if there are any bytes from the WiMOD available and
 * tries to start decoding the received data.
 *
 * @note this function must be called at regular base from the main loop.
 */
void
TWiMODLRHCI::Process(void)
{
    // read data from comport
    int numRxBytes = this->serial.available();
    UINT8 rxByte;

    // bytes received ?
    while(numRxBytes-- > 0) {
        rxByte = this->serial.read();
        // yes, pass to SLIP Decoder
        // Complete SLIP messages will be forwarded via callback to
        // callback function "ProcessRxMessage" (see Receiver section)
        comSlip.DecodeData(&rxByte, 0x01);
    }
}

//------------------------------------------------------------------------------
/**
 * @brief: Send a sequence of dummy chars to give the WiMOD some time to wake up
 */
void
TWiMODLRHCI::SendWakeUpSequence(void)
{
    comSlip.SendWakeUpSequence(WIMODLR_NUMBER_OF_WAKEUP_CHARS);
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
void
TWiMODLRHCI::EnableWakeupSequence(bool flag) {
    wakeUp = flag;
}

//------------------------------------------------------------------------------
//
//  ProcessRxMessage
//------------------------------------------------------------------------------

/**
 * @internal
 *
 * @brief: handle received SLIP message
 *
 * @param rxBuffer  pointer to memory containing rx data bytes
 *
 * @param length    number of bytes in the buffer to process
 *
 * @return  pointer to memory that can store the next hci message for RX operation
 *
 * @endinternal
 */
UINT8*
TWiMODLRHCI::ProcessRxMessage(UINT8* rxBuffer, UINT16 length)
{
    // 1. check CRC
    if (CRC16_Check(rxBuffer, length, CRC16_INIT_VALUE))
    {
        // 2. check min length, 2 bytes for SapID + MsgID + 2 bytes CRC16
        if(length >= (WIMODLR_HCI_MSG_HEADER_SIZE + WIMODLR_HCI_MSG_FCS_SIZE))
        {
            // 3. Hack: since only one RxMessage buffer is used,
            //          rxBuffer must point to RxMessage.SapId, thus
            //          memcpy to RxMessage structure is not needed here

            // add length
            Rx.Message.Length = length - (WIMODLR_HCI_MSG_HEADER_SIZE + WIMODLR_HCI_MSG_FCS_SIZE);

            // dispatch completed RxMessage
            DispatchRxMessage(Rx.Message);
        }
    }
    else
    {
        //CRC error ??

    }

    // return same buffer again, keep receiver enabled
    return &Rx.Message.SapID;
}

//-----------------------------------------------------------------------------
/**
 * @internal
 *
 * @brief Registers a callback function for reporting an internal stack error
 *
 * @paramc  cb          pointer callback function
 *
 *@endinternal
 */
void TWiMODLRHCI::RegisterStackErrorClient(TWiMODStackErrorClient cb)
{
    StackErrorClientCB = cb;
}

//-----------------------------------------------------------------------------
/**
 * @internal
 *
 * @brief Returns the last HCI message received by the low level stack
 *
 * @return  reference to the last received HCI message
 *
 * @endinternal
 */
const TWiMODLR_HCIMessage&
TWiMODLRHCI::GetRxMessage(void)
{
#ifdef WIMOD_USE_2ND_RXBUFFER
// for non IRQ mode
    return Rx.ExpectedRsponseMsg;
#else
// for IRQ mode
    return Rx.Message;
#endif
}


//------------------------------------------------------------------------------
//
// Section protected functions
//
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 * @internal
 *
 * @brief Construct a HCI message and send it to the WiMOD
 *
 * @param   sapID   the SAP-ID to target
 * @param   msgID   the Command ID to target
 * @param   payload pointer to the payload
 * @param   length  length of the payload field to use
 *
 * @endinternal
 */
TWiMODLRResultCodes
TWiMODLRHCI::PostMessage(UINT8 sapID, UINT8 msgID, UINT8* payload, UINT16 length)
{
    // 1. check parameter
    //
    // 1.1 check length
    //
    if(length > WIMODLR_HCI_MSG_PAYLOAD_SIZE)
    {
        return WiMODLR_RESULT_PAYLOAD_LENGTH_ERROR;
    }
    // 1.2 check payload ptr
    //
    if(length && !payload)
    {
        return WiMODLR_RESULT_PAYLOAD_PTR_ERROR;
    }

    // 2.  init TxMessage
    //
    // 2.1 init SAP ID
    //
    TxMessage.SapID = sapID;

    // 2.2 init Msg ID
    //
    TxMessage.MsgID = msgID;

    // 2.3 copy payload, if present
    //
    if(payload && length)
    {
        UINT8*  dstPtr  = TxMessage.Payload;
        int     n       = (int)length;

        // copy bytes
        while(n--)
            *dstPtr++ = *payload++;
    }

    // 3. Calculate CRC16 over header and optional payload
    //
    UINT16 crc16 = CRC16_Calc(&TxMessage.SapID, length + WIMODLR_HCI_MSG_HEADER_SIZE, CRC16_INIT_VALUE);

    // 3.1 get 1's complement
    //
    crc16 = ~crc16;

    // 3.2 attach CRC16 and correct length, lobyte first
    //
    TxMessage.Payload[length++] = LOBYTE(crc16);
    TxMessage.Payload[length++] = HIBYTE(crc16);

    // 4. forward message to SLIP layer
    //    - start transmission with SAP ID
    //    - correct length by header size

    return SendPacket(&TxMessage.SapID, length + WIMODLR_HCI_MSG_HEADER_SIZE);
}


/**
 * @internal
 *
 * @brief Send a complete packet (Sap+Msg+Payload+CRC) to WiMOD.
 *
 * @param txData    pointer to COMPLETE HCI packet
 *
 * @param length    length of the HCI packet
 *
 * @endinternal
 */
TWiMODLRResultCodes
TWiMODLRHCI::SendPacket(UINT8* txData, UINT16 length)
{
    // call SLIP encoder
    // and send out data via serial interface
    comSlip.SendMessage(txData, length);

    return WiMODLR_RESULT_OK;
}



//------------------------------------------------------------------------------
/**
 * @internal
 *
 * @brief: wait for response HCI message
 *
 * @param rxSapID   the SapID of the expected response message
 *
 * @param rxMsgID   the CmdID of the expected response message
 *
 * @return true     if the expected response message has been received within timeout
 *
 * @endinternal
 */
bool
TWiMODLRHCI::WaitForResponse(UINT8 rxSapID, UINT8 rxMsgID)
{
    // init receiver struct
    Rx.Active = true;
    Rx.Done   = false;
    Rx.SapID  = rxSapID;
    Rx.MsgID  = rxMsgID;
    Rx.Timeout = WIMODLR_RESPOMSE_TIMEOUT_MS;

    // wait for response ~1000ms
    while(Rx.Timeout--)
    {
        // call receiver path
        Process();

        // response received  ?
        if(Rx.Done)
        {
            // clear flag
            Rx.Active = false;

            // ok
            return true;
        }
        delay(1);
    }
    // clear flag
    Rx.Active = false;

    // error - timeout
    return false;
}

//------------------------------------------------------------------------------
//
// Section private functions
//
//------------------------------------------------------------------------------


/**
 * @internal
 *
 * @brief Dispatch a a CRC checked HCI message for further processing
 *
 * @param rxMsg     reference to the received HCI message
 *
 * @endinternal
 */
void
TWiMODLRHCI::DispatchRxMessage(TWiMODLR_HCIMessage& rxMsg)
{
    // 1. test if a response message is expected
    if(Rx.Active)
    {
        // expected response received ?
        if ((rxMsg.SapID == Rx.SapID) && (rxMsg.MsgID == Rx.MsgID))
        {
            // yes
            Rx.Done = true;

#ifdef WIMOD_USE_2ND_RXBUFFER
            // store this message in case there are several messages at once in the uart fifo - only for non IRQ mode!
            Rx.ExpectedRsponseMsg = rxMsg;
#endif
            // no further processing here !
            return;
        }
    }

    // 2. forward async received messages to corresponding SAP
//    RxMessageClient->ProcessRxMessage(rxMsg);
    ProcessUnexpectedRxMessage(rxMsg);
    return;

}

//-----------------------------------------------------------------------------
// EOF
//-----------------------------------------------------------------------------



