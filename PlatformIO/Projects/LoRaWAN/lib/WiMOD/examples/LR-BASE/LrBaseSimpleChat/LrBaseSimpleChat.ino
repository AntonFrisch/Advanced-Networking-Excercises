/*
 * This is a simple example file to show how to use the WiMOD Arduino
 * library to communicate with a WiMOD Module by IMST GmbH
 *
 * http://www.wireless-solutions.de
 *
 */

/*
 * Example usage:
 *
 * This example demonstrates the data link service of the LR-Base firmware.
 *
 * A simple chat feature is implemented to send some messages from one device
 * to a peer device.
 *
 *
 * After setting up up everything the user can type in a short message that
 * is being transfered to the peer device.
 * If a message has been received from the peer device the message payload
 * will be printed as hex values.
 *
 * Setup requirements:
 * -------------------
 * - 2 WiMOD modules running LR-Base firmware
 *   * One WiMOD module is mounted to the (local) Ardunio
 *   * The second one can be an iM88x USB-Stick or a SK-iM88x connected to
 *     the WiMOD_LR_Studio Software. (Or a 2nd Arduino :-) )
 *  - Both modules must have the same radio settings (SF/Datarate/GroupAdr/...)
 *    (hint: simple way to archive that: do a factory reset on all devices)
 *
 * Usage:
 * -------
 * - Local device:
 *      -- Start the program
 *      -- open the serial monitor @ 115200 baud
 *      -- type in some short text and press enter
 *
 * - Peer device:
 *      -- the message should appear
 *      -- type in a new message and send it
 *
 * Note: Please enable the "Carriage return" option in the Serial Port Monitor
 *       of the Arduino IDE. The "carriage return" char is used to start
 *       transmitting the data.
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

#define WIMOD_IF    Serial3             // for WiMODino use: SerialWiMOD
#define PC_IF		Serial              // for WiMODino use: SerialUSB


#define MAX_USER_INPUT_LEN                       50


//-----------------------------------------------------------------------------
// section RAM
//-----------------------------------------------------------------------------

/*
 * define two global radio messages
 *  Rx : This one is used for storing incomming RF messages
 *  Tx : This one is used for preparation of outgoing RF messages
 */
TWiMODLR_RadioLink_Msg radioRxMsg;
TWiMODLR_RadioLink_Msg radioTxMsg;

/*
 * user input related variables
 */
static bool showInputPrompt = true;
static char userInputData[MAX_USER_INPUT_LEN];
static unsigned int inputPos = 0;


/*
 * Create in instance of the interface to the WiMOD-LR-Base firmware
 */
WiMODLRBASE wimod(WIMOD_IF);  // use the Arduino Serial3 as serial interface


//-----------------------------------------------------------------------------
// section code
//-----------------------------------------------------------------------------


/*****************************************************************************
 * Functions for printing out some debug infos via serial interface
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
 * function for transmitting a single radio message via RadioLink service
 ****************************************************************************/

void sendRadioMessage(char* buffer, int length)
{
    // check if there is data to send
    if (buffer && (length > 0)) {

        // setup destination address information for the RF message
        radioTxMsg.DestinationGroupAddress = 0x10;                            // use default dst group adr
        radioTxMsg.DestinationDeviceAddress = RADIOLINK_BROADCAST_DEVICE_ADR; // broadcast to every device
                                                                              // within the group

        // setup payload of RF message
        radioTxMsg.Length = length;
        memcpy(radioTxMsg.Payload, buffer, length);

      	debugMsg(F("Starting transmission... \r\n"));

        // send the RF message out (using the non-confirmed (aka. unreliable) transmission service
        // note: the confirmed service does NOT support broadcast addresses!!!!
        if (true == wimod.SendUData(&radioTxMsg)) {
            debugMsg(F("RF-Message has been send\n"));
        } else {
        	// error
        	debugMsg(F("Error sending message! "));
        	debugMsg(F("HCI result code: "));
        	debugMsgHex((int) wimod.GetLastHciResult());
           	debugMsg(F("; RSP status code: "));
           	debugMsgHex((int)wimod.GetLastResponseStatus());
           	debugMsg(F("\r\n"));
        }

    }
}

void processSerialInputByte(const byte inByte)
{

    switch (inByte) {
        case '\n':   // end of line
        case '\r':   // carriage return
            userInputData[inputPos] = 0;  // terminating null byte

            // terminator reached; process userInputData here ...
            sendRadioMessage(userInputData, inputPos);

            // reset buffer for next time
            inputPos = 0;
            showInputPrompt = true;
            break;

        default:
            // add data to buffer
            if (inputPos < (MAX_USER_INPUT_LEN - 1))
                userInputData[inputPos++] = inByte;
            break;
    }
}

/*****************************************************************************
 * function reading a user string while not blocking the main loop
 ****************************************************************************/

void debugReadData()
{
    // show the input prompt only at "start"
    if (showInputPrompt) {
        debugMsg(F("Enter Tx-Message: "));
        showInputPrompt = false;
    }

    // check of the user as entered something on the "debug" serial interface
    if (PC_IF.available()) {
        // -> yes: process the input data
        processSerialInputByte(PC_IF.read());
    }
}

/*****************************************************************************
 * print out a welcome message
 ****************************************************************************/
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
    debugMsg(F("how to use the TX and RX features of a WiMOD module\n"));
    debugMsg(F("running a LR-Base Firmware.\n"));
    debugMsg(F("==================================================\n"));
}



/*****************************************************************************
 * Callback function for processing incomming RF messages
 ****************************************************************************/

void onRxData(TWiMODLR_HCIMessage& rxMsg) {
    int i;

    // convert/copy the raw message to RX radio buffer
    wimod.convert(rxMsg, &radioRxMsg);

    // print out the received message as hex string
    if (radioRxMsg.Length > 0) {
        // print out the length
        debugMsg(F("Rx-Message: ["));
        debugMsg(radioRxMsg.Length);
        debugMsg(F("]: "));

        // print out the payload
        for (i = 0; i < radioRxMsg.Length; i++) {
            debugMsgHex(radioRxMsg.Payload[i]);
            debugMsg(F(" "));
        }
        debugMsg(F("\n"));
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
    while(!PC_IF);
    PC_IF.begin(115200);
    printStartMsg();

    // register callbacks for incommig RF messages
    wimod.RegisterUDataRxClient(onRxData);
    wimod.RegisterCDataRxClient(onRxData);

    /*****************
     * Reset the radio configuration to factory settings in order
     * to get a "clean" setup.
     *  (disable the next line if you whish to use other settings.)
     *  note: the radio settings must be the same on both peers.
     ****************/
    wimod.ResetRadioConfig();

}

void loop()
{
    // try to read a user message (non-blocking)
    debugReadData();

    // check for any pending data of the WiMOD
    wimod.Process();

    delay(500);
}
