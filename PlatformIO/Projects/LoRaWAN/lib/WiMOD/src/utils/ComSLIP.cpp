//------------------------------------------------------------------------------
//
//  File:       ComSlip.cpp
//
//  Abstract:   SLIP Wrapper Class Implementation
//
//  Version:    0.1
//
//  Date:       09.02.2015
//
//  Disclaimer: This example code is provided by IMST GmbH on an "AS IS" basis
//              without any warranties.
//
//------------------------------------------------------------------------------
//! @file ComSlip.cpp
//! @ingroup Utils
//! <!------------------------------------------------------------------------->
//! @brief SLIP layer for the HCI communication stack
//! @version 0.1
//! <!------------------------------------------------------------------------->

//------------------------------------------------------------------------------
//
//  Include Files
//
//------------------------------------------------------------------------------

#include "ComSLIP.h"

//------------------------------------------------------------------------------
//
//  Protocol Definitions
//
//------------------------------------------------------------------------------

// SLIP Protocol Characters
#define SLIP_END                    0xC0
#define SLIP_ESC                    0xDB
#define SLIP_ESC_END                0xDC
#define SLIP_ESC_ESC                0xDD

// SLIP Receiver/Decoder States
#define SLIPDEC_IDLE_STATE          0
#define SLIPDEC_START_STATE         1
#define SLIPDEC_IN_FRAME_STATE      2
#define SLIPDEC_ESC_STATE           3

//------------------------------------------------------------------------------
//
//  Class Constructor
//
//------------------------------------------------------------------------------

/**
 * @brief Constructor
 *
 * @param s     Reference to the serial interface to use for communication
 */
TComSlip::TComSlip(Stream& s) :
    serial(s)
{

    // init to idle state, no rx-buffer available
    RxState         =   SLIPDEC_IDLE_STATE;
    RxIndex         =   0;
    RxBuffer        =   0;
    RxBufferSize    =   0;
    RxClient        =   0;


}

/**
 * @brief Init function that must be called once prior to any other function of this object
 */
void
TComSlip::begin(/*int baudrate*/)
{
    //this->serial.begin(baudrate);

}


/**
 * @brief De-Init function
 */
void
TComSlip::end(void) {

}

/**
 * @brief register a client for processing
 *
 * @param client  pointer to client that should handle a received SLIP frame
 */
void
TComSlip::RegisterClient(TComSlipClient* client)
{
    RxClient = client;
}


//------------------------------------------------------------------------------
//
//  SendMessage
//
//  @brief: send a message as SLIP frame
//
//------------------------------------------------------------------------------

/**
 * @brief Generic function to transfer a message from the host to the module.
 *
 * This is a generic function to send out a stream of bytes. The bytes are
 * encoded into the SLIP format before transmission. Therefore a complete
 * message has to be given.
 *
 * @param msg       pointer to the bytes to encoded and send via serial interface
 * @param msgLength number of bytes
 *
 */
bool
TComSlip::SendMessage(UINT8* msg, UINT16 msgLength)
{
    // send start of SLIP message
    this->serial.write(SLIP_END);

    // iterate over all message bytes
    while(msgLength--)
    {
        switch (*msg)
        {
            case SLIP_END:
                this->serial.write(SLIP_ESC);
                this->serial.write(SLIP_ESC_END);
                break;

            case SLIP_ESC:
                this->serial.write(SLIP_ESC);
                this->serial.write(SLIP_ESC_ESC);
                break;

            default:
                this->serial.write(*msg);
                break;
        }
        // next byte
        msg++;
    }

    // send end of SLIP message
    this->serial.write(SLIP_END);

    // always ok
    return true;
}


/**
 * @brief configure a rx-buffer and enable receiver/decoder
 *
 * @param rxBuffer  pointer to plain buffer
 *
 * @param rxBufferSize  size of buffer in bytes
 */
bool
TComSlip::SetRxBuffer(UINT8* rxBuffer, UINT16  rxBufferSize)
{
    // receiver in IDLE state and client already registered ?
    if ((RxState == SLIPDEC_IDLE_STATE) && RxClient)
    {
        // same buffer params
        RxBuffer        = rxBuffer;
        RxBufferSize    = rxBufferSize;

        // enable decoder
        RxState = SLIPDEC_START_STATE;

        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
 * @brief process received byte stream
 *
 * @param rxData    pointer to received bytes
 * @param length    number of bytes received
 */
void
TComSlip::DecodeData(UINT8* rxData, UINT16 length)
{
    // iterate over all received bytes
    while(length--)
    {
        // get rxByte
        UINT8 rxByte = *rxData++;

        // decode according to current state
        switch(RxState)
        {
            case    SLIPDEC_START_STATE:
                    // start of SLIP frame ?
                    if(rxByte == SLIP_END)
                    {
                        // init read index
                        RxIndex = 0;

                        // next state
                        RxState = SLIPDEC_IN_FRAME_STATE;
                    }
                    break;

            case    SLIPDEC_IN_FRAME_STATE:
                    switch(rxByte)
                    {
                        case    SLIP_END:
                                // data received ?
                                if(RxIndex > 0)
                                {
                                    // yes, return received decoded length
                                    if (RxClient)
                                    {
                                        RxBuffer = RxClient->ProcessRxMessage(RxBuffer, RxIndex);
                                        if (!RxBuffer)
                                        {
                                            RxState = SLIPDEC_IDLE_STATE;
                                        }
                                        else
                                        {
                                            RxState = SLIPDEC_START_STATE;
                                        }
                                    }
                                    else
                                    {
                                        // disable decoder, temp. no buffer available
                                        RxState = SLIPDEC_IDLE_STATE;
                                    }
                                }
                                // init read index
                                RxIndex = 0;
                                break;

                        case  SLIP_ESC:
                                // enter escape sequence state
                                RxState = SLIPDEC_ESC_STATE;
                                break;

                        default:
                                // store byte
                                StoreRxByte(rxByte);
                                break;
                    }
                    break;

            case    SLIPDEC_ESC_STATE:
                    switch(rxByte)
                    {
                        case    SLIP_ESC_END:
                                StoreRxByte(SLIP_END);
                                // quit escape sequence state
                                RxState = SLIPDEC_IN_FRAME_STATE;
                                break;

                        case    SLIP_ESC_ESC:
                                StoreRxByte(SLIP_ESC);
                                // quit escape sequence state
                                RxState = SLIPDEC_IN_FRAME_STATE;
                                break;

                        default:
                                // abort frame reception
                                RxState = SLIPDEC_START_STATE;
                                break;
                    }
                    break;

            default:
                    break;
        }
    }
}

//------------------------------------------------------------------------------
/**
 * @brief: store SLIP decoded rxByte
 *
 * @param rxByte    byte to store
 */
void
TComSlip::StoreRxByte(UINT8 rxByte)
{
    if ((RxIndex < RxBufferSize) && RxBuffer)
        RxBuffer[RxIndex++] = rxByte;
}



//------------------------------------------------------------------------------
/**
 * @brief: Send a sequence of dummy chars to give the WiMOD some time to wake up
 *
 * @param nbr    number of dummy bytes to send
 */
void
TComSlip::SendWakeUpSequence(UINT8 nbr)
{
    while (nbr--) {
        this->serial.write(SLIP_END);
    }
}


//------------------------------------------------------------------------------
// end of file
//------------------------------------------------------------------------------
