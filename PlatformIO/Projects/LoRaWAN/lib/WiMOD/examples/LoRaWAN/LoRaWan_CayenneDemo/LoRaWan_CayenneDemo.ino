/*
 * This is a simple example file to show how to use the WiMOD Arduino
 * library in combination with the myDevices.com Cayenne LPP interface class
 * to send structured data to the myDevices Cayenne platform via LoRaWAN.
 *
 * (Note: you have to setup an LoRaWAN server to forward the transmitted data
 * to the cayenne server. Please follow the instructions given on the
 * https://mdswp-staging.mydevices.com/cayenne/docs/#lora web site.)
 *
 *
 *
 * http://www.wireless-solutions.de
 *
 */


/*
 * Example:
 *
 * This example demonstrates how to send data to the the myDevices Cayenne
 * platform.
 *
 * After a successfull join to the network this node simulates a
 * 	- temperature sensor
 * 	- a ditial Output
 * 	- a digital Input
 * 	and convers the data into the cayenne LPP format before transmitting the
 * 	simulated values to the LoRaWAN server.
 *
 * Setup requirements:
 * -------------------
 * - One WiMOD shield for Arduino running WiMOD_LoRaWAN_EndNode_Modem firmware
 * - An Arduino DUE-like board
 * - A registered account at a LoRaWAN server
 * - A working data redirection on the LoraWAN server to the myDevices Cayenne
 *   server
 * - An account on the myDevices Cayenne server.
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

#include <Cayenne/CayenneLPP.h>
#include <math.h>


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
// constant values
//-----------------------------------------------------------------------------

/*
 * ABP Parameters
 */

const uint32_t  DEV_ADR = 0x00; // ToDO: add your device address here

// network session key (128bit)
// TODO: add your network session key here
const unsigned char NWKSKEY[] = {  0x00, 0x01, .... };


// application session key (128bit)
// TODO add your application session key here
const unsigned char APPSKEY[] = { 0x00, 0x01, .... };


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
 * Create in instance of the interface to the WiMOD-LoRaWAN EndNode firmware
 */
WiMODLoRaWAN wimod(WIMOD_IF);  // use the Arduino Serial3 as serial interface


TRuntimeInfo RIB = {  };


static uint32_t loopCnt = 0;
static TWiMODLORAWAN_TX_Data txData;


// Cayenne related buffer and objects
#define BUF_SIZE_CAYENNE 	20
static uint8_t bufCayenne[BUF_SIZE_CAYENNE];

// cayenne translation object
CayenneLPP cayenne(bufCayenne, BUF_SIZE_CAYENNE);


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

#include <stdio.h>
#include <stdint.h>

#define TEXT_SIZE	80
uint8_t text[TEXT_SIZE];

void printPayload(uint8_t* buf, uint8_t size) {
  for (int i = 0; i < size; i++) {
	  PC_IF.print((uint8_t) buf[i], HEX);
	  PC_IF.print(F(" "));
  }
  PC_IF.print(F("\n"));
}

/*
void printPayload(uint8_t* buf, uint8_t size) {
	int len = 0;
	int i;

	for (i = 0; i < size; i++) {
		if (len < TEXT_SIZE) {
			len += snprintf((char*) &text[len], TEXT_SIZE - len, "%02x ", buf[i]);
		} else {

		}
	}

	len += snprintf((char*) &text[len], TEXT_SIZE - len, "\n");

	for (int e = 0; e < len; e++) {
		printf("%c", text[e]);
	}
//	Serial.print("\n");
	printf(" \n");


	return;
}
*/

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
    debugMsg(F("send simulated sensor data to the mydevices cayenne server.\n"));
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

    delay(500);

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
		debugMsg(F("Starting join ABP procedure...\n"));

		TWiMODLORAWAN_RadioStackConfig cfg;

		if ( wimod.GetRadioStackConfig(&cfg)) {
		   cfg.Options = 0;
		   cfg.Options = LORAWAN_STK_OPTION_ADR | LORAWAN_STK_OPTION_DEV_CLASS_C;
			wimod.SetRadioStackConfig(&cfg);
		}



		TWiMODLORAWAN_ActivateDeviceData activationData;

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

float temperature;
int cnt = 0;
int x;

void loop()
{
	// check of ABP procedure has finished
    if (RIB.ModemState == ModemState_Connected) {

        // send out a hello world every 30 sec ( =6* 50*100 ms)
    	// (due to duty cycle restrictions 30 sec is recommended
        if ((loopCnt > 0) && (loopCnt % (6*50)) == 0) {

            // prepare TX data structure

            temperature = sin( (float)cnt / 4.0f )*25.0f + 25.0f;
            cnt++;
            debugMsg(F("Temp: "));
            debugMsg(temperature);
            debugMsg(F("\n"));

            // reset old data
            cayenne.reset();
            // add the simulated temperature value
            cayenne.addTemperature(1, temperature);
            // add a simulated digital input value
            if (cnt % 2) {
                   cayenne.addDigitalInput(2, 1);
            } else {
                   cayenne.addDigitalInput(2, 0);
            }
            // add a simulated digital output value
            cayenne.addDigitalOutput(3, cnt);

            // setup plain data to send via LoRaWAN link
            txData.Port = 0x02;
            txData.Length = cayenne.getSize();
            cayenne.copy(txData.Payload);

            debugMsg("Raw-Payload-bytes: ");
            printPayload(txData.Payload, txData.Length);

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
