/*
 * This is a simple example file to show how to use the WiMOD Arduino
 * library in combination with the myDevices.com Cayenne LPP interface class
 * to send structured data to the myDevices Cayenne platform via LoRaWAN.
 *
 * (Note: you have to setup an LoRaWAN server to forward the transmitted data
 * to the cayenne server. Please follow the instructions given on the
 * https://mdswp-staging.mydevices.com/cayenne/docs/#lora web site.)
 *
 * This exmaple requires an external Bosch BME280 Sensor
 * (temperature, humidity , preasure). The following Arduino librarys
 * need to be installed:
 *  - Adafruit_Sensor
 *  - Adafruit_BME280
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
 * After a successfull join to the network this node use a
 *  - temperature sensor
 *  - humidity sensor
 *  - barometric sensor
 *
 *  and convers the data into the cayenne LPP format before transmitting
 *  the values to the LoRaWAN server.
 *
 *  If a BME280 Sensor is connected by I2C interface, real data will be used.
 *
 * Setup requirements:
 * -------------------
 * - One WSA01  with a iM880B/im881A and LoRaWAN firmware
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

// include the API to the WiMOD firmware
#include <WiMODLoRaWAN.h>
// include the CayenneLPP converter
#include <Cayenne/CayenneLPP.h>
#include <math.h>

// for BME280 sensor
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>


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
#define PC_IF       Serial      // for WiMODino use: SerialUSB
#define WIMOD_IF    Serial3     // for WiMODino use: SerialWiMOD

//-----------------------------------------------------------------------------
// constant values
//-----------------------------------------------------------------------------

/*
 * BME280 Hardware Sensor
 */
#define           BME280_I2C_ADR                (0x76)


/*****************************************************************************
 * LoRaWAN related parameters ( provided by the Network operator )
 ****************************************************************************/

/*
 * ABP Parameters
 */
const uint32_t  DEV_ADR = 0x00; // ToDO: add your device address here

// network session key (128bit)

// TODO change this key according to your one provided by your NWK server
const char NWKSKEY[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// application session key (128bit)
// TODO change this key according to your one provided by your NWK server
const char APPSKEY[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };


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
 * Create in instance of the interface to the WiMOD-LoRaWAN Modem firmware
 */
WiMODLoRaWAN wimod(WIMOD_IF);  // use the Arduino Serial3 as serial interface


// runtime info for the WiMOD device
TRuntimeInfo RIB = {  };

// BME280 related stuff
bool              BME280status = false;
Adafruit_BME280   bme;

static uint32_t loopCnt = 0;

// buffer for the preparation of the LoRa txData
static TWiMODLORAWAN_TX_Data txData;


// Cayenne related buffer and objects
#define BUF_SIZE_CAYENNE    30
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

void debugMsg(float a)
{
    PC_IF.print(a, 1);
}

#include <stdio.h>
#include <stdint.h>

#define TEXT_SIZE   80
uint8_t text[TEXT_SIZE];

void printPayload(uint8_t* buf, uint8_t size) {
  for (int i = 0; i < size; i++) {
      PC_IF.print((uint8_t) buf[i], HEX);
      PC_IF.print(F(" "));
  }
  PC_IF.print(F("\n"));
}


void printStartMsg()
{
    debugMsg(F("==================================================\n"));
    debugMsg(F("This is FileName: "));
    debugMsg(F(__FILE__));
    debugMsg(F("\r\n"));
    debugMsg(F("Starting...\n"));
    debugMsg(F("This simple demo will try to "));
    debugMsg(F("do the ABP procedure and "));
    debugMsg(F("send BME280 sensor data to the mydevices cayenne server.\n"));
    //debugMsg(F("With no BME280 sensor connected simulated data will be used.\n"));
    debugMsg(F("==================================================\n"));
}

void printSensorValues(float temperature, float RelHumidity, float BarPressure)
{
    debugMsg(F("Temp: "));
    debugMsg(temperature);
    debugMsg(F("\n"));
    debugMsg(F("Hum : "));
    debugMsg(RelHumidity);
    debugMsg(F("\n"));
    debugMsg(F("Pres: "));
    debugMsg(BarPressure);
    debugMsg(F("\n"));

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


        // try to register the device at network server via ABP procedure
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

    // try to find a BME280 connected to I2C
    BME280status = bme.begin(BME280_I2C_ADR);
    if (!BME280status) {
        debugMsg(F("Could not find a valid BME280 sensor, check wiring! Will use demo Sensor-Data!!!\n"));
    } else {
        debugMsg(F("Found a BME280 sensor! Will use real Sensor-Data!!!\n"));
    }

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

/*****************************************************************************
 * "gernate" sensor data
 ****************************************************************************/
int cnt = 0;


void aquireSensorData(float* temp, float* humidity, float* pressure)
{
    // is there a real sensor connected?
    if (BME280status == false) {
        // no -> "simulate" values
        *temp     = sin((float) cnt / 4.0f) * 25.0f + 25.0f;
        *humidity = sin((float) cnt / 4.0f) * 30.0f + 40.0f;
        *pressure = sin((float) cnt / 4.0f) * 100.0f + 1020.0f;
    } else {
        // yes -> read out the sensor readings
        *temp     = (float) bme.readTemperature();       // in °C
        *pressure = (float) bme.readPressure() / 100.0F; // in hPa
        *humidity = (float) bme.readHumidity();          // in %
    }
}

/*****************************************************************************
 * LED TX indicator
 ****************************************************************************/

void ledUpdate(uint32_t counter, bool txIndication) {
    static uint32_t lastCnt_txInd = 0;
    uint32_t        diff;

    if (txIndication) {
        lastCnt_txInd = counter;
    }

    diff = counter - lastCnt_txInd;

    if ((diff < 10)) {
        digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN)^1);
    } else {
        digitalWrite(LED_BUILTIN, LOW);
    }
}

/*****************************************************************************
 * Arduino loop function
 ****************************************************************************/

float temperature;
float RelHumidity;
float BarPressure;

void loop()
{
    // check of ABP procedure has finished
    if (RIB.ModemState == ModemState_Connected) {

        // send out a hello world every 30 sec ( =6* 50*100 ms)
          // (due to duty cycle restrictions 30 sec is recommended
        if ((loopCnt > 0) && (loopCnt % (6*50)) == 0) {

            // get new sensor data
            aquireSensorData(&temperature, &RelHumidity, &BarPressure);

            // print the sensor values on the debug console
            printSensorValues(temperature, RelHumidity, BarPressure);

            cnt++;

            // reset old data
            cayenne.reset();
            // add the simulated temperature value
            cayenne.addTemperature(1, temperature );
            cayenne.addRelativeHumidity(2, RelHumidity );
            cayenne.addBarometricPressure(3, BarPressure );

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

            // led update
            ledUpdate(loopCnt, true);
        }
    }

    loopCnt++;

    // led update
    ledUpdate(loopCnt, false);

    // check for any pending data of the WiMOD
    wimod.Process();

    delay(100);

}
