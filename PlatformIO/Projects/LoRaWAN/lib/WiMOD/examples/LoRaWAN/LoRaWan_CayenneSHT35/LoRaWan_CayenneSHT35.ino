/*
 * This is a minimalistic example file to build up a minimalistic LoRaWAN
 * demo using the IMST WiMODino hardware in combination with the IMST LoRaWAN
 * Network server.
 *
 * http://www.wireless-solutions.de
 *
 */

/*
 * Example:
 *
 * This example demonstrates how to read an SHT35 Temperature & Humidity sensor
 * by Sensirion AG send it's data via LoRaWAN link into the IoT world.
 *
 * As communication platform the IMST LoRaWAN server is used for message routing.
 *
 * Setup requirements:
 * -------------------
 * - 1 WiMODino running WiMOD_LoRaWAN_EndNode_Modem Firmware (sender)
 * - 1 SHT35 Sensor (e.g http://wiki.seeedstudio.com/Grove-I2C_High_Accuracy_Temp&Humi_Sensor-SHT35/)
 *
 * Usage:
 * -------
 * - Change the keys according to your LoRaWAN server before starting
 * - Start the program and watch the serial monitor @ 115200 baud
 */


#include <WiMODLoRaWAN.h>
#include <Seeed_SHT35.h>
#include <Cayenne/CayenneLPP.h>
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

#define WIMOD_IF    Serial3   // for WiMODino: SerialWiMOD
#define PC_IF       Serial    // for WiMODino: SerialUSB
#define SCLPIN      A5  // I2C interface


// chanel IDs the for the myDevices/CayenneLPP service
#define LPP_CH_TEMP         1
#define LPP_CH_HUMIDITY     2

//-----------------------------------------------------------------------------
// constant values
//-----------------------------------------------------------------------------

/*
 * OTAA Parameters
 */
#define APPEUI          "00 00 00 00 00 00 00 00" // CHANGE ME !
#define APPKEY          "00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00" // CHANGE ME !
// the 3rd OTAA Parameter DevEUI is automatically set by the WiMOD module itself

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

/*
 * Create an instance of the SHT35 Temperature & Humitity sensor
 */
SHT35 sensor(SCLPIN);

/*
 * some global variables
 */
static uint32_t              loopCnt          = 0;
static TWiMODLORAWAN_TX_Data txData           = {};

static uint8_t appEui[8];
static uint8_t appKey[16];

static bool modemConnected   = false;

// cayenne translation object
CayenneLPP cayenne(txData.Payload, WiMODLORAWAN_APP_PAYLOAD_LEN);

//-----------------------------------------------------------------------------
// section code
//-----------------------------------------------------------------------------


/*****************************************************************************
 * print out a welcome message
 ****************************************************************************/
void printStartMsg()
{
    PC_IF.print(F("==================================================\r\n"));
    PC_IF.print(F("This is FileName: "));
    PC_IF.print(F(__FILE__));
    PC_IF.print(F("\r\r\n"));
    PC_IF.print(F("This minimalistic demo will try to read a SHT35 sensor \r\n"));
    PC_IF.print(F("and transfer the measurement to a LoRaWAN IoT server. \r\n"));
    PC_IF.print(F("For LoRaWAN activation the OTAA method is used\r\n"));
    PC_IF.print(F("==================================================\r\n"));
    PC_IF.print(F("Starting...\r\n\n"));
}


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

void debugMsgHex(int a)
{
    PC_IF.print(a, HEX);
}

void debugMsg(float f)
{
    PC_IF.print(f);
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
            modemConnected = true;

            debugMsg(F("Device has joined a network.\n"));
            debugMsg(F("New Device address is: 0x"));
            debugMsgHex((int) joinedData.DeviceAddress);
            debugMsg(F("\n"));
        } else {
            // error joining procedure did not succeed
            debugMsg(F("Failed to join a network.\n"));
        }
    }
}


/*****************************************************************************
 * Arduino setup function
 ****************************************************************************/
void setup()
{
    // wait for the PC interface to be ready (max 5 sec); useful for USB
    while (!PC_IF && millis() < 5000 ){}
    // debug interface
    PC_IF.begin(115200);

    printStartMsg();

    // init / setup the serial interface connected to WiMOD
    WIMOD_IF.begin(WIMOD_LORAWAN_SERIAL_BAUDRATE);

    // init the communication stack
    wimod.begin();

    // print out some basic info about the module
    wimod.PrintBasicDeviceInfo(PC_IF);

    // deactivate device in order to make changes in the stack config
    wimod.DeactivateDevice();

    // optional: disable duty cycle control
    // this setting violates the regulatory rules for Sub-GHz communication
    // in the EU and is NOT allowed !!!
    TWiMODLORAWAN_RadioStackConfig  stackCfg;
    if (wimod.GetRadioStackConfig(&stackCfg)) {
        stackCfg.Options = LORAWAN_STK_OPTION_ADR
                           | LORAWAN_STK_OPTION_DEV_CLASS_C
                           | LORAWAN_STK_OPTION_DUTY_CYCLE_CTRL
                           ;
        wimod.SetRadioStackConfig(&stackCfg);
    }

    // register the joined nwk indication client
    wimod.RegisterJoinedNwkIndicationClient(onJoinedNwk);

    // connect to a LoRaWAN server via an OTAA join
    wimod.ConvertAppEuiStrToArray(APPEUI, appEui);
    wimod.ConvertAppKeyStrToArray(APPKEY, appKey);
    wimod.ConnectViaOTAA(appEui, appKey);


    // IO config
    pinMode(LED_BUILTIN, OUTPUT);

    // Init the sensor
    if(sensor.init() ) {
        debugMsg(F("Error: Failed to init the SHT35 sensor. Please check wiring...\r\n"));
    }

}

bool readSensor(float* temp, float* hum) {
    bool result = false;

    if(NO_ERROR == sensor.read_meas_data_single_shot(HIGH_REP_WITH_STRCH, temp, hum)) {

        debugMsg(F("temperature = "));
        debugMsg(*temp);
        debugMsg(F(" ℃ "));

        debugMsg(F("; humidity = "));
        debugMsg(*hum);
        debugMsg(F(" % \r\n"));

        result = true;
    } else {
        debugMsg(F("Error reading sensor data...\r\n"));
    }

    return result;
}


/*****************************************************************************
 * Arduino loop function
 ****************************************************************************/

// sensor data
float temperature, humidity;

void loop() {
    // send out a message every 30 sec ( = 30*10 *100 ms)
    if ( (modemConnected) && (loopCnt > 0) && (loopCnt % (30 * 10)) == 0) {

        // read out the sensor data
        if (readSensor(&temperature, &humidity)) {
            // prepare TX data structure
            cayenne.reset(); // reset old data
            // add measurement values to cayenne structure / data stream
            cayenne.addTemperature(LPP_CH_TEMP, temperature);
            cayenne.addRelativeHumidity(LPP_CH_HUMIDITY, humidity);
            // setup plain data to send via LoRaWAN link
            txData.Port = 0x02;
            txData.Length = cayenne.getSize();

//        debugMsg("Raw-Payload-bytes: ");
//        printPayload(txData.Payload, txData.Length);

            // try to send a message
            if (false == wimod.SendUData(&txData)) {
                // an error occurred

                // check if we have got a duty cycle problem
                if (LORAWAN_STATUS_CHANNEL_BLOCKED
                        == wimod.GetLastResponseStatus()) {
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

    // toogle board's LED every 1 sec
    if ((loopCnt % 10) == 0) {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }

}
