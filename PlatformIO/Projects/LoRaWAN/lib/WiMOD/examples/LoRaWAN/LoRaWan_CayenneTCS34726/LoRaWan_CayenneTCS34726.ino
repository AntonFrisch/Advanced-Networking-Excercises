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
 * platform. As a physical sensor element a TCS3725 will be used
 *
 * After a successfull join to the network this node will send the
 * recorded light values to the LoRaWAN + myDevices server
 *
 * Setup requirements:
 * -------------------
 * - One WiMOD shield for Arduino running WiMOD_LoRaWAN_EndNode_Modem firmware
 * - An Arduino DUE-like board
 * - A registered account at a LoRaWAN server
 * - A working data redirection on the LoraWAN server to the myDevices Cayenne
 *   server
 * - An account on the myDevices Cayenne server.
 * - A TCS34724 sensor (e.g. https://www.adafruit.com/product/1334)
 * - The adafruit driver lib for the TCS34724
 *
 * Usage:
 * -------
 * - Change the keys according to your LoRaWAN server before starting
 * - Start the program and watch the serial monitor @ 115200 baud
 *
 * Hardware setup:
 * --------------
 *
 * Connect an Adafruit TCS3725 sensor baord to your Arduino board using the
 * I2C communication lines SDA and SCL.
 * - Connect the GND and 3.3V power supply lines
 * - Connect the LED line to I/O 5 of the Arduino
 * - Connect the IRQ/INT line to I/O 2 of the Arduino
 *
 */


// make sure to use only the WiMODLoRaWAN.h
// the WiMODLR_BASE.h must not be used for LoRaWAN firmware.
#include <WiMODLoRaWAN.h>
#if defined(ARDUINO_ARCH_AVR)
#include <avr/pgmspace.h>
#endif

#include <Cayenne/CayenneLPP.h>
#include <math.h>
#include <Wire.h>
#include <Adafruit_TCS34725.h>


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


#define TCS_HELP_LED_PIN 5

#define LPP_CH_LUM            1
#define LPP_CH_COLOR_RED      2
#define LPP_CH_COLOR_GREEN    3
#define LPP_CH_COLOR_BLUE     4
#define LPP_CH_COLOR_CLEAR    5
#define LPP_CH_HELPER_LED     6



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

typedef struct TTcsData
{
    uint16_t r;
    uint16_t g;
    uint16_t b;
    uint16_t c;
    uint16_t colorTemp;
    uint16_t lux;
} TTcsData;

//-----------------------------------------------------------------------------
// section RAM
//-----------------------------------------------------------------------------

/*
 * Create in instance of the interface to the WiMOD-LoRaWAN EndNode firmware
 */
WiMODLoRaWAN wimod(WIMOD_IF);  // use the Arduino Serial3 as serial interface

// Initialise the Adafruit_TCS34725
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

uint8_t tcsHelperLEDState = 0;

TRuntimeInfo RIB = {  };

TTcsData   tcsData = { };

static uint32_t loopCnt = 0;
static TWiMODLORAWAN_TX_Data txData;


// Cayenne related buffer and objects
#define BUF_SIZE_CAYENNE 	30
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

/*****************************************************************************
 * TCS helper functions
 ****************************************************************************/

/* tcs.getRawData() does a delay(Integration_Time) after the sensor readout.
We don't need to wait for the next integration cycle because we receive an interrupt when the integration cycle is complete*/
void TCS_getRawData_noDelay(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c)
{
  *c = tcs.read16(TCS34725_CDATAL);
  *r = tcs.read16(TCS34725_RDATAL);
  *g = tcs.read16(TCS34725_GDATAL);
  *b = tcs.read16(TCS34725_BDATAL);
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

            // check if the downlink message addresses the digital outpu channel
            if (radioRxMsg.Payload[0] == LPP_CH_HELPER_LED) {
                // yes the digital output channel should be set
                // set the new state
                digitalWrite(TCS_HELP_LED_PIN, radioRxMsg.Payload[2]);
            }

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
        }
    }
}


void printTcsValues(void) {
    PC_IF.print(F("Color Temp: "));
    PC_IF.print(tcsData.colorTemp, DEC);
    PC_IF.print(F(" K - "));
    PC_IF.print("Lux: ");
    PC_IF.print(tcsData.lux, DEC);
    PC_IF.print(F(" - "));
    PC_IF.print(F("R: "));
    PC_IF.print(tcsData.r, DEC);
    PC_IF.print(F(" "));
    PC_IF.print(F("G: "));
    PC_IF.print(tcsData.g, DEC);
    PC_IF.print(F(" "));
    PC_IF.print(F("B: "));
    PC_IF.print(tcsData.b, DEC);
    PC_IF.print(F(" "));
    PC_IF.print(F("C: "));
    PC_IF.print(tcsData.c, DEC);
    PC_IF.println(F(" "));
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
    debugMsg(F("send data of TCS34724 sensor to the mydevices cayenne server.\n"));
    debugMsg(F("==================================================\n"));
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

    printStartMsg();

    // start the TCS sensor
    if (tcs.begin()) {
        PC_IF.println(F("Found TCS sensor"));
    } else {
        PC_IF.println(F("No TCS34725 found ... check your connections"));
      while (1);
    }
    // TCS sensor: Set persistence filter
    tcs.write8(TCS34725_PERS, TCS34725_PERS_NONE);

    pinMode(TCS_HELP_LED_PIN, OUTPUT);

    // init / setup the serial interface connected to WiMOD
    WIMOD_IF.begin(WIMOD_LORAWAN_SERIAL_BAUDRATE);

    delay(500);

    // init the communication stack
    wimod.begin();


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

        // register a client callbacks
        wimod.RegisterRxCDataIndicationClient(onRxData);
        wimod.RegisterRxUDataIndicationClient(onRxData);

    }
}


/*****************************************************************************
 * Arduino loop function
 ****************************************************************************/

int cnt = 0;


void loop()
{
	// check of ABP procedure has finished
    if (RIB.ModemState == ModemState_Connected) {

        // send out a message every 60 sec ( =2*6* 50*100 ms)
    	// (due to duty cycle restrictions 60 sec is recommended
        if ((loopCnt > 0) && (loopCnt % (2*6*50)) == 0) {

            // prepare TX data structure


            TCS_getRawData_noDelay(&tcsData.r, &tcsData.g, &tcsData.b, &tcsData.c);
            tcsData.colorTemp = tcs.calculateColorTemperature(tcsData.r, tcsData.g, tcsData.b);
            tcsData.lux = tcs.calculateLux(tcsData.r, tcsData.g, tcsData.b);

            printTcsValues();

            cnt++;

            // reset old data
            cayenne.reset();

            // add measurement values to cayenne structure / data stream

            cayenne.addLuminosity(LPP_CH_LUM          , tcsData.lux);
            // note: Cayenne LPP suports analoge input values up to a value of 327.67
            //       higher values are being misinterpretet !!!
            cayenne.addAnalogInput(LPP_CH_COLOR_RED   , (float) tcsData.r);
            cayenne.addAnalogInput(LPP_CH_COLOR_GREEN , (float) tcsData.g);
            cayenne.addAnalogInput(LPP_CH_COLOR_BLUE  , (float) tcsData.b);
            cayenne.addAnalogInput(LPP_CH_COLOR_CLEAR , (float) tcsData.c);
            cayenne.addDigitalOutput(LPP_CH_HELPER_LED, tcsHelperLEDState);


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
