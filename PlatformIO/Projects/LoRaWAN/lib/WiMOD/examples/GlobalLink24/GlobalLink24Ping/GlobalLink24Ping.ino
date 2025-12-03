/*
 * This is a simple example file to show how to use the WiMOD Arduino
 * library to communicate with a WiMOD Module by IMST GmbH
 *
 * http://www.wireless-solutions.de
 *
 */


/*
 * Example:
 *
 * This example demonstrates how to issue a (local) ping command to the WiMOD.
 * There is no radio communication involved in this example.
 *
 * Setup requirements:
 * -------------------
 * - 1 WiMOD module running GlobalLink24 firmware
 *
 * Usage:
 * -------
 * Start the program and watch the serial monitor @ 115200 baud
 *
 */




#include <WiMODGlobalLink24.h>
#if defined(ARDUINO_ARCH_AVR)
#include <avr/pgmspace.h>
#endif

//-----------------------------------------------------------------------------
// platform defines
//-----------------------------------------------------------------------------
/*
 * Note: This sketch is for Arduino devices with two separate serial interfaces
 * (e.g. DUE). One interface is connected to a PC and one is used for WiMOD.
 *
 * For single serial interface boards (e.g. UNO) it is recommended to disable
 * the PC / Debug messages
 */

#define WIMOD_IF    SerialWiMOD        // for Mega / Due use: Serial3
#define PC_IF       SerialUSB          // for Mega / Due use: Serial


/*
 * Create in instance of the interface to the WiMOD GlobalLink24 firmware
 */
WiMODGlobalLink24 wimod(WIMOD_IF);  // use the Arduino Serial3 as serial interface


//-----------------------------------------------------------------------------
// section code
//-----------------------------------------------------------------------------

/*****************************************************************************
 * Function for printing out some debug infos via serial interface
 ****************************************************************************/
void debugMsg(String msg)
{
	PC_IF.print(msg);  // use default Arduino serial interface
}

/*****************************************************************************
 * print out a welcome message
 ****************************************************************************/
void printStartMsg()
{
    debugMsg(F("==================================================\n"));
    debugMsg(F("This is FileName: "));
    debugMsg(F(__FILE__));
    debugMsg(F("\r\n"));
    debugMsg(F("Starting...\n"));
    debugMsg(F("This simple demo will show \n"));
    debugMsg(F("how to ping a WiMOD radio module\n"));
    debugMsg(F("running the LR-Base(+) or LoRaWAN or GlobalLink24 Firmware.\n"));
    debugMsg(F("==================================================\n"));
}




/*****************************************************************************
 * Arduino setup function
 ****************************************************************************/
void setup()
{
    // wait for the PC interface to be ready (max 10 sec); useful for USB
    while (!PC_IF && millis() < 10000 ){}

    // init / setup the serial interface connected to WiMOD
    WIMOD_IF.begin(WIMOD_GLOBALLINK24_SERIAL_BAUDRATE);
    // init the communication stack
    wimod.begin();

    // debug interface
    PC_IF.begin(115200);

    printStartMsg();

    delay(100);
}


void loop()
{
    debugMsg(F("Ping WiMOD: "));
    if (wimod.Ping() == true) {
        debugMsg(F("OK\n"));
    } else {
        debugMsg(F("FAILED\n"));
    }

    // check for any pending data of the WiMOD
    wimod.Process();

    delay(2000);
}
