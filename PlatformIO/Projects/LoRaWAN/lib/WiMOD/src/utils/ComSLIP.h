//------------------------------------------------------------------------------
//
//  File:       ComSlip.h
//
//  Abstract:   SLIP Wrapper Class Declaration
//
//  Version:    0.1
//
//  Date:       09.02.2015
//
//  Disclaimer: This example code is provided by IMST GmbH on an "AS IS" basis
//              without any warranties.
//
//------------------------------------------------------------------------------
//! @file ComSlip.h
//! @ingroup Utils
//! <!------------------------------------------------------------------------->
//! @brief SLIP layer for the HCI communication stack
//! @version 0.1
//! <!------------------------------------------------------------------------->



#ifndef COMSLIP_H
#define COMSLIP_H

//------------------------------------------------------------------------------
//
// Include Files
//
//------------------------------------------------------------------------------

#include "WMDefs.h"

#include "Arduino.h"

//------------------------------------------------------------------------------
//
// General Definitions
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//
// Class Declaration
//
//------------------------------------------------------------------------------

/**
 * @brief Class definition for enabling OO inheritance
 */
class TComSlipClient
{
    public:
                    TComSlipClient() {}
    virtual         ~TComSlipClient() {}


    //! @cond Doxygen_Suppress
    // virtual receiver function - must be implemented by real client
    virtual UINT8*  ProcessRxMessage(UINT8* /* rxBuffer */, UINT16 /* rxLength */)  { return 0;}
    //! @endcond
};

/**
 * @brief Class for handling SLIP encoding and decoding of HCI messages
 */
class TComSlip
{
    public:
                    /*explicit*/ TComSlip(Stream& s);

    void            begin(/*int baudrate*/);
    void            end(void);

    void            RegisterClient(TComSlipClient* client);

    bool            SendMessage(UINT8* msg, UINT16 msgLength);

    bool            SetRxBuffer(UINT8*  rxBuffer, UINT16 rxbufferSize);

    void            DecodeData(UINT8* rxData, UINT16 length);

    void            SendWakeUpSequence(UINT8 nbr);

    private:

    void            StoreRxByte(UINT8 rxByte);

    Stream&       serial;

    // receiver/decoder state
    int             RxState;

    // rx buffer index
    UINT16          RxIndex;

    // size of RxBuffer
    UINT16          RxBufferSize;

    // pointer to RxBuffer
    UINT8*          RxBuffer;

    // client for received messages
    TComSlipClient* RxClient;

};

#endif // COMSLIP_H

//------------------------------------------------------------------------------
// end of file
//------------------------------------------------------------------------------
