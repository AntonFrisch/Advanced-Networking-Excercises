/*
 * This is a minimalistic example file to show how to use the WiMOD Arduino
 * library to communicate with a WiMOD Module by IMST GmbH
 *
 * http://www.wireless-solutions.de
 *
 */

/*
 * Example:
 *
 * This example demonstrates how to send a 'Hello World' message via
 * the WiMOD module to a LoRaWAN server
 *
 * Setup requirements:
 * -------------------
 * - 1 WiMOD module running WiMOD_LoRaWAN_EndNode_Modemfirmware
 *
 * Usage:
 * -------
 * - Change the keys according to your LoRaWAN server before starting
 * - Start the program and watch the serial monitor @ 115200 baud
 */


// make sure to use only the WiMODLoRaWAN.h
// the WiMODLR_BASE.h must not be used for LoRaWAN firmware.
#include <WiMODLoRaWAN.h>

//-----------------------------------------------------------------------------
// platform defines
//-----------------------------------------------------------------------------
/*
 * Note: This sketch is for Arduino devices with two separate serial interfaces
 * (e.g. DUE). One interface is connected to a PC and one is used for WiMOD.
 *
 * For single serial interface boards (e.g. UNO) it is recommended to disbale
 * the PC / Debug messages
 */

#define WIMOD_IF    Serial3         // for WiMODino use: SerialWiMOD
#define PC_IF		Serial          // for WiMODino use: SerialUSB

//-----------------------------------------------------------------------------
// constant values
//-----------------------------------------------------------------------------

/*
 * OTAA Parameters; must be changed by user
 */

// application  key (64bit)
const unsigned char APPEUI[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 }; // CHANGE ME!!!

// application  key (128bit)
const unsigned char APPKEY[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                         0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0f, 0x10 };      // CHANGE ME !!!




//-----------------------------------------------------------------------------
// user defined types
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// section RAM
//-----------------------------------------------------------------------------

/*
 * Create in instance of the interface to the WiMOD-LoRaWAN EndNode firmware
 */
WiMODLoRaWAN wimod(WIMOD_IF);  // use the Arduino Serialx as serial interface


static uint32_t loopCnt = 0;
static TWiMODLORAWAN_TX_Data txData;



//-----------------------------------------------------------------------------
// section code
//-----------------------------------------------------------------------------


/*****************************************************************************
 * print out a welcome message
 ****************************************************************************/
void printStartMsg()
{
    PC_IF.print(F("==================================================\n"));
    PC_IF.print(F("This is FileName: "));
    PC_IF.print(F(__FILE__));
    PC_IF.print(F("\r\n"));
    PC_IF.print(F("Starting...\n"));
    PC_IF.print(F("This minimalistic demo will try to "));
    PC_IF.print(F("do the OTAA procedure and "));
    PC_IF.print(F("send a demo message each 30 sec.\n"));
    PC_IF.print(F("==================================================\n"));
}

/*****************************************************************************
 * Arduino setup function
 ****************************************************************************/
void setup()
{
    // wait for the PC interface to be ready (max 10 sec); usefull for USB
    while (!PC_IF && millis() < 10000 ){}
    // debug interface
    PC_IF.begin(115200);

    // init / setup the serial interface connected to WiMOD
    WIMOD_IF.begin(WIMOD_LORAWAN_SERIAL_BAUDRATE);

    // init the communication stack
    wimod.begin();

    printStartMsg();

    // print out some basic info about the module
    wimod.PrintBasicDeviceInfo(PC_IF);

    //wimod.DeactivateDevice(); // for multiple starts

    // connect to a LoRaWAN server via an OTAA join
    wimod.ConnectViaOTAA(APPEUI, APPKEY);
}


/*****************************************************************************
 * Arduino loop function
 ****************************************************************************/

void loop()
{

    // send out a hello world every 30 sec ( =6* 50*100 ms)
    // (due to duty cycle restrictions 30 sec is recommended
    if ((loopCnt > 1) && (loopCnt % (6*50)) == 0) {
        // send out a simple HelloWorld messsage
        PC_IF.print(F("Sending HelloWorld message...\n"));

        // prepare TX data structure
        txData.Port = 0x42;
        txData.Length = strlen_P(PSTR("Hello World!"));
        strcpy_P((char*) txData.Payload, PSTR("Hello World!"));

        // try to send a message
        if (false == wimod.SendUData(&txData)) {
            // an error occurred

            switch (wimod.GetLastResponseStatus()) {
                case LORAWAN_STATUS_CHANNEL_BLOCKED:
                    // duty cycle violation
                    // -> try again later
                    PC_IF.print(F("-> TX failed: Blocked due to DutyCycle...\r\n"));
                    break;

                case LORAWAN_STATUS_DEVICE_NOT_ACTIVATED:
                    // activation problem
                    // -> try again later
                    PC_IF.print(F("-> TX failed: Device is still connecting to server...\r\n"));
                    break;
                default:
                    PC_IF.print(F("-> TX failed\r\n"));
                    break;
            }

             // check if we have got a duty cycle problem

        } else {
            PC_IF.print(F("-> TX Done\r\n"));
        }
    }

    // check for any pending data of the WiMOD
    wimod.Process();

    delay(100);
    loopCnt++;
}
