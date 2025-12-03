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
 * This example demonstrates how to get and set the encryption key (AES)
 * for securing LR-BASE radio messages. Note that the AES encryption is
 * NOT activated within this example. For activation and usage of the
 * encryption features the radio stack configuration has to be setup.
 * (This is not part of this example.)
 *
 * Setup requirements:
 * -------------------
 * - 1 WiMOD module running LR-Base firmware
 *
 * Usage:
 * -------
 * Start the program and watch the serial monitor @ 115200 baud
 *
 */




// make sure to use only the WiMODLR_BASE.h
// the WiMODLoRaWAN.h must not be used for LR-BASE firmware.
#include <WiMODLR_BASE.h>
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
 * For single serial interface boards (e.g. UNO) it is recommended to disbale
 * the PC / Debug messages
 */

#define WIMOD_IF    Serial3     // for WiMODino use: SerialWiMOD
#define PC_IF		Serial      // for WiMODino use: SerialUSB

//-----------------------------------------------------------------------------
// section RAM
//-----------------------------------------------------------------------------

// global variable for storing the AES key
static UINT8 aesKey[DEVMGMT_AES_KEY_LEN];

static UINT8 demoKey[DEVMGMT_AES_KEY_LEN] =
        { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
          0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00 };

/*
 * Create in instance of the interface to the WiMOD-LR-Base firmware
 */
WiMODLRBASE wimod(WIMOD_IF);  // use the Arduino Serial3 as serial interface


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
    debugMsg(F("This simple demo will show \n"));
    debugMsg(F("how to use the LR-BASE AES encryption\n"));
    debugMsg(F("features of the a LR-Base Firmware.\n"));
    debugMsg(F("==================================================\n"));
}

/*****************************************************************************
 * print aes key
 ****************************************************************************/
void showAesKey(UINT8* key)
{
    UINT8 i;
    if (key) {
        for(i = 0; i < DEVMGMT_AES_KEY_LEN; i++) {
            debugMsgHex((int) key[i]);
            debugMsg(F(" "));
        }
    }
    debugMsg(F("\r\n"));
}

/*****************************************************************************
 * set AES - write a new key into the WiMOD
 *
 * note: in order to use the key, the user must active the encryption
 *       via SetRadioConfig() (see encryption bit in field MiscOptions)
 *       see also at example LrBaseSetRadioConfig.ino
 *
 ****************************************************************************/
void setAes()
{
    // try to set the AES
    if (wimod.SetAesKey(demoKey)) {
        debugMsg("Setup of new AES-key: OK\r\n");

        debugMsg("New AES is is now: ");
        showAesKey(demoKey);
    } else {
        debugMsg("Setup new AES-key: Failed\r\n");
    }
}

/*****************************************************************************
 * get AES - get current AES key stored in WiMOD
 ****************************************************************************/

void getAes()
{

    // try to get the RTC information out of the WiMOD
    if (wimod.GetAesKey(aesKey)) {
        debugMsg("pre-set AES-key is: ");
        showAesKey(aesKey);
    } else {
        // error
        debugMsg("Error getting current AES key from WiMOD\r\n");
    }
}

/*****************************************************************************
 * Arduino setup function
 ****************************************************************************/
void setup()
{
    // wait for the PC interface to be ready (max 10 sec); usefull for USB
    while (!PC_IF && millis() < 10000 ){}

    // init / setup the serial interface connected to WiMOD
    WIMOD_IF.begin(WIMOD_LR_BASE_SERIAL_BAUDRATE);
    // init the communication stack
    wimod.begin();

    // debug interface
    PC_IF.begin(115200);

    // wait a while (for the user to open the serial monitor)
    delay(1000);

    // set a new demo key to WiMOD
    setAes();
}

static int loopCnt = 0;

void loop()
{
    getAes();

    // in order not to stress and damage EEPROM/NVM memory of the
    // WiMOD, a new AES key is NOT set in every loop.
    if ((loopCnt > 0) && ((loopCnt % 10) == 0)) {
		// just increase the last byte of the key
    	demoKey[DEVMGMT_AES_KEY_LEN-1]++;
		setAes();
    }

    // check for any pending data of the WiMOD
    wimod.Process();

    delay(1000);
}
