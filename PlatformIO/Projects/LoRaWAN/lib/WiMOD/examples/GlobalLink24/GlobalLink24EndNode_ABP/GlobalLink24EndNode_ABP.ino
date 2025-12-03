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
 * This example demonstrates how to start a GlobalLink24 ABP procedure to "register"
 * the WiMOD to a server that emulates LoRaWAN on 2.4 GHz
 *
 * Setup requirements:
 * -------------------
 * - 1 WiMOD module running wiMOD_GlobalLink24_EndNode_Modemfirmware
 *
 * Usage:
 * -------
 * - Change the keys according to your LoRaWAN emulating server before starting
 * - Start the program and watch the serial monitor @ 115200 baud
 */


// make sure to use only the WiMODGlobalLink24.h
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

//-----------------------------------------------------------------------------
// constant values
//-----------------------------------------------------------------------------

/*
 * ABP Parameters; must be adapted by user before usage
 */

const uint32_t  DEV_ADR = 0x22; // CHANGE ME!

// network session key (128bit)
const unsigned char NWKSKEY[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                         0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0f, 0x10 }; // CHANGE ME!

// application session key (128bit)
const unsigned char APPSKEY[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                         0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0f, 0x10 }; // CHANGE ME!




//-----------------------------------------------------------------------------
// user defined types
//-----------------------------------------------------------------------------

typedef enum TModemState
{
    ModemState_Disconnected = 0,
    ModemState_ConnectRequestSent,
    ModemState_Connected,
    ModemState_FailedToConnect,
} TModemState;


typedef struct TRuntimeInfo
{
    TModemState ModemState;
} TRuntimeInfo;


//-----------------------------------------------------------------------------
// section RAM
//-----------------------------------------------------------------------------

/*
 * Create in instance of the interface to the WiMOD-GlobalLink24 firmware
 */
WiMODGlobalLink24 wimod(WIMOD_IF);  // use the Arduino Serial3 as serial interface

TRuntimeInfo RIB = {  };

static uint32_t loopCnt = 0;
static TWiMODGlobalLink24_TX_Data txData;



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

void debugMsg(int a)
{
	PC_IF.print(a, DEC);
}

void debugMsgChar(char c)
{
	PC_IF.print(c);
}

void debugMsgHex(int a)
{
    if (a < 0x10) {
        PC_IF.print(F("0"));
    }
    PC_IF.print(a, HEX);
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
    debugMsg(F("This simple demo will try to "));
    debugMsg(F("do the ABP procedure and "));
    debugMsg(F("send a demo message each 30 sec.\n"));
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

    // do a software reset of the WiMOD
    delay(100);
    wimod.Reset();
    delay(100);
    // deactivate device in order to get a clean start for this demo
    wimod.DeactivateDevice();

    // do a simple ping to check the local serial connection
    debugMsg(F("Ping WiMOD: "));
    if (wimod.Ping() != true) {
        debugMsg(F("FAILED\n"));
    } else {
        debugMsg(F("OK\n"));

		// try to register the device at network server via ABP procedure
		debugMsg(F("Starting join ABP procedure...\n"));
        TWiMODGlobalLink24_ActivateDeviceData activationData;

		//setup ABP data
		activationData.DeviceAddress = DEV_ADR;
		memcpy(activationData.NwkSKey, NWKSKEY, 16);
		memcpy(activationData.AppSKey, APPSKEY, 16);


		// activate device
		if (wimod.ActivateDevice(activationData)) {
			debugMsg(F("ABP procedure done\n"));
			debugMsg(F("(An 'alive' message has been sent to server)\n"));
			RIB.ModemState = ModemState_Connected;
		} else {
		  debugMsg("Error executing ABP join request: ");
		  debugMsg((int) wimod.GetLastResponseStatus());
		  RIB.ModemState = ModemState_FailedToConnect;
		}
    }

}


/*****************************************************************************
 * Arduino loop function
 ****************************************************************************/

void loop()
{
	// check of ABP procedure has finished
    if (RIB.ModemState == ModemState_Connected) {

        // send out a hello world every 30 sec ( =6* 50*100 ms)
    	// (due to duty cycle restrictions 30 sec is recommended
        if ((loopCnt > 0) && (loopCnt % (6*50)) == 0) {
        	// send out a simple HelloWorld messsage
            debugMsg(F("Sending a 'Hello World!' message...\n"));

            // prepare TX data structure
            txData.Port = 0x01;
            txData.Length = strlen_P(PSTR("Hello World!"));
            strcpy_P((char*) txData.Payload, PSTR("Hello World!"));

            // try to send a message
            if (false == wimod.SendUData(&txData)) {
                // an error occurred

                 // check if we have got a duty cycle problem
                 if (GLOBALLINK24_STATUS_CHANNEL_BLOCKED == wimod.GetLastResponseStatus()) {
                     // yes; it is a duty cycle violation
                     // -> try again later
                     debugMsg(F("TX failed: Blocked due to DutyCycle...\n"));
                 }
            }
        }
    }

    // check for any pending data of the WiMOD
    wimod.Process();

    delay(100);
    loopCnt++;
}
