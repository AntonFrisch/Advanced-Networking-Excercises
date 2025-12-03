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
 * This example demonstrates how to start a LoRaWAN OTAA procedure to "register"
 * the WiMOD to a LoRaWAN server
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

#define WIMOD_IF    Serial3         // for WiMODino use: SerialWiMOD
#define PC_IF		Serial          // for WiMODino use: SerialUSB

//-----------------------------------------------------------------------------
// constant values
//-----------------------------------------------------------------------------

/*
 * OTAA Parameters
 */

// application  key (64bit)
const unsigned char APPEUI[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };

// application  key (128bit)
const unsigned char APPKEY[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                         0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0f, 0x10 };




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
 * Create in instance of the interface to the WiMOD-LR-Base firmware
 */
WiMODLoRaWAN wimod(WIMOD_IF);  // use the Arduino Serial3 as serial interface

TRuntimeInfo RIB = {  };

static uint32_t loopCnt = 0;
static TWiMODLORAWAN_TX_Data txData;



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
 * join tx indication callback
 ****************************************************************************/

void onJoinTx(TWiMODLR_HCIMessage& rxMsg) {
    TWiMODLORAWAN_TxIndData txData = {};
    wimod.convert(rxMsg, &txData);

    debugMsg(F("joining attempt: "));
    if (txData.FieldAvailability == LORAWAN_OPT_TX_IND_INFOS_INCL_PKT_CNT) {
        debugMsg((int) txData.NumTxPackets);
    }
    debugMsg(F("\n"));
}

/*****************************************************************************
 * joined network indication
 ****************************************************************************/

void onJoinedNwk(TWiMODLR_HCIMessage& rxMsg) {
    TWiMODLORAWAN_RX_JoinedNwkData joinedData;

    debugMsg(F("Join-Indication received.\n"));

    if (wimod.convert(rxMsg, &joinedData)) {
        if ((LORAWAN_JOIN_NWK_IND_FORMAT_STATUS_JOIN_OK == joinedData.StatusFormat)
                || (LORAWAN_JOIN_NWK_IND_FORMAT_STATUS_JOIN_OK_CH_INFO == joinedData.StatusFormat)){
            //Ok device is now joined to nwk (server)
            RIB.ModemState = ModemState_Connected;

            debugMsg(F("Device has joined a network.\n"));
            debugMsg(F("New Device address is: "));
            debugMsg((int) joinedData.DeviceAddress);
            debugMsg(F("\n"));
        } else {
            // error joining procedure did not succeed
            RIB.ModemState = ModemState_FailedToConnect;

            debugMsg(F("Failed to join a network.\n"));
        }
    }
}

/*****************************************************************************
 * rx data callback
 ****************************************************************************/
void onRxData(TWiMODLR_HCIMessage& rxMsg) {
    TWiMODLORAWAN_RX_Data radioRxMsg;
    int i;

    debugMsg("Rx-Data Indication received.\n");

    // convert/copy the raw message to RX radio buffer
    if (wimod.convert(rxMsg, &radioRxMsg)) {

  	if (radioRxMsg.StatusFormat & LORAWAN_FORMAT_ACK_RECEIVED) {
  		// yes, this is an ack
  		debugMsg(F("Ack-Packet received."));
  	}
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
        } else {
            // no payload included
//            debugMsg(F("Rx-Message with no Payload received; Status: "));
//	    debugMsg((int) radioRxMsg.StatusFormat);
          debugMsg(F("\n"));
        }
    }
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
    debugMsg(F("do the OTAA procedure and "));
    debugMsg(F("send a demo message each 30 sec.\n"));
    debugMsg(F("==================================================\n"));
}

/*****************************************************************************
 * Arduino setup function
 ****************************************************************************/
void setup()
{
    // wait for the PC interface to be ready (max 10 sec); usefull for USB
    while (!PC_IF && millis() < 10000 ){}

    // init / setup the serial interface connected to WiMOD
    WIMOD_IF.begin(WIMOD_LORAWAN_SERIAL_BAUDRATE);
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


		// try to register the device at network server via OTAA procedure
		debugMsg(F("Starting join OTAA procedure...\n"));

		TWiMODLORAWAN_JoinParams joinParams;

		//setup OTAA parameters
		memcpy(joinParams.AppEUI, APPEUI, 8);
		memcpy(joinParams.AppKey, APPKEY, 16);

		// transfer parameters to WiMOD
		wimod.SetJoinParameter(joinParams);

		// Register callbacks for join related events
		wimod.RegisterJoinedNwkIndicationClient(onJoinedNwk);
		wimod.RegisterJoinTxIndicationClient(onJoinTx);

		// send join request
		if (wimod.JoinNetwork()) {
			RIB.ModemState = ModemState_ConnectRequestSent;
			debugMsg(F("...waiting for nwk response...\n"));
		} else {
		  debugMsg("Error sending join request: ");
		  debugMsg((int) wimod.GetLastResponseStatus());
		  debugMsg(F("\n"));
		}
    }

}


/*****************************************************************************
 * Arduino loop function
 ****************************************************************************/

void loop()
{
	// check of OTAA procedure has finished
    if (RIB.ModemState == ModemState_Connected) {

        // send out a hello world every 30 sec ( =6* 50*100 ms)
    	// (due to duty cycle restrictions 30 sec is recommended
        if ((loopCnt > 1) && (loopCnt % (6*50)) == 0) {
        	// send out a simple HelloWorld messsage
            debugMsg(F("Sending HelloWorld message...\n"));

            // prepare TX data structure
            txData.Port = 0x01;
            txData.Length = strlen_P(PSTR("Hello World!"));
            strcpy_P((char*) txData.Payload, PSTR("Hello World!"));

            // try to send a message
            if (false == wimod.SendUData(&txData)) {
                // an error occurred

                 // check if we have got a duty cycle problem
                 if (LORAWAN_STATUS_CHANNEL_BLOCKED == wimod.GetLastResponseStatus()) {
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
