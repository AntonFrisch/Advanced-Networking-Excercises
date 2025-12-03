/*
 * This is a simple example file to show how to use the WiMOD Arduino
 * library to communicate with a WiMOD Module by IMST GmbH
 *
 * http://www.wireless-solutions.de
 *
 */


/*
 * Example
 *
 * This example demonstrates how to setup / configure the radio parameter set.
 *
 * For demonstration purposes this program:
 * - reads out the current parameter set
 * - writes a new parameter set into the WiMOD module.
 *
 * After that this program does nothing.
 *
 * Setup requirements:
 * -------------------
 * - 1 WiMOD module running LR-Base firmware
 *
 * Usage:
 * -------
 * - Start the program and watch the serial monitor @ 115200 baud
 *
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

/*
 * Create in instance of the interface to the WiMOD-LR-Base firmware
 */
WiMODLRBASE wimod(WIMOD_IF);  // use the Arduino Serial3 as serial interface

// structure for storing the radio parameter set
TWiMODLR_DevMgmt_RadioConfig radioConfigSet;

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
void debugMsgDec(int a)
{
	PC_IF.print(a, DEC);
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
    debugMsg(F("how read out the radio config of a WiMOD Radio Module\n"));
    debugMsg(F("runing LR-Base firmware\n"));
    debugMsg(F("==================================================\n"));
}

void showRadioConfig()
{
    bool result;

    result = wimod.GetRadioConfig(&radioConfigSet);

    if (result) {
        debugMsg(F("Current RadioParameterSet:\n"));
        debugMsg(F("---------------------------\n"));
        debugMsg(F("RadioMode-ID:      ")); debugMsgHex( (int) radioConfigSet.RadioMode);           debugMsg(F("\n"));
        debugMsg(F("GroupAddress:      ")); debugMsgHex( (int) radioConfigSet.GroupAddress);        debugMsg(F("\n"));
        debugMsg(F("TxGroupAddress:    ")); debugMsgHex( (int) radioConfigSet.TxGroupAddress);      debugMsg(F("\n"));
        debugMsg(F("DeviceAddress:     ")); debugMsgHex( (int) radioConfigSet.DeviceAddress);       debugMsg(F("\n"));
        debugMsg(F("TxDeviceAddress:   ")); debugMsgHex( (int) radioConfigSet.TxDeviceAddress);     debugMsg(F("\n"));
        debugMsg(F("Modulation-ID:     ")); debugMsgHex( (int) radioConfigSet.Modulation);          debugMsg(F("\n"));
        debugMsg(F("RfFreq_LSB:        ")); debugMsgHex( (int) radioConfigSet.RfFreq_LSB);          debugMsg(F("\n"));
        debugMsg(F("RfFreq_MID:        ")); debugMsgHex( (int) radioConfigSet.RfFreq_MID);          debugMsg(F("\n"));
        debugMsg(F("RfFreq_MSB:        ")); debugMsgHex( (int) radioConfigSet.RfFreq_MSB);          debugMsg(F("\n"));
        debugMsg(F("LoRa-BW-ID:        ")); debugMsgHex( (int) radioConfigSet.LoRaBandWidth);       debugMsg(F("\n"));
        debugMsg(F("LoRa SF-ID:        ")); debugMsgHex( (int) radioConfigSet.LoRaSpreadingFactor); debugMsg(F("\n"));
        debugMsg(F("LoRa EC-ID:        ")); debugMsgHex( (int) radioConfigSet.ErrorCoding);         debugMsg(F("\n"));
        debugMsg(F("PowerLevel-ID:     ")); debugMsgHex( (int) radioConfigSet.PowerLevel);          debugMsg(F("\n"));
        debugMsg(F("TxCtrl (bitfield): ")); debugMsgHex( (int) radioConfigSet.TxControl);           debugMsg(F("\n"));
        debugMsg(F("RxCtrl:            ")); debugMsgHex( (int) radioConfigSet.RxControl);           debugMsg(F("\n"));
        debugMsg(F("RxWindowTime:      ")); debugMsgHex( (int) radioConfigSet.RxWindowTime);        debugMsg(F("\n"));
        debugMsg(F("LedCtrl (bitfield):")); debugMsgHex( (int) radioConfigSet.LedControl);          debugMsg(F("\n"));
        debugMsg(F("MiscOp. (bitfield):")); debugMsgHex( (int) radioConfigSet.MiscOptions);         debugMsg(F("\n"));
        debugMsg(F("FskDatarate-ID:    ")); debugMsgHex( (int) radioConfigSet.FskDatarate);         debugMsg(F("\n"));
        debugMsg(F("PowerSavingMode:   ")); debugMsgHex( (int) radioConfigSet.PowerSavingMode);     debugMsg(F("\n"));
        debugMsg(F("LbtThreshold:      ")); debugMsgDec( (int) radioConfigSet.LbtThreshold);        debugMsg(F("\n"));
        debugMsg(F("---------------------------\n\n"));
    } else {
        debugMsg(F("Error getting current RadioStackParameter.\n\n"));
    }
}

void writeRadioConfig()
{
    bool result;

    radioConfigSet.RadioMode             = RadioMode_Standard; // do not change this
    radioConfigSet.GroupAddress          = 0x20;
    radioConfigSet.TxGroupAddress        = 0x20; // should be the same as GroupAddress; keep thinks simple
    radioConfigSet.DeviceAddress         = 0x1234;
    radioConfigSet.TxDeviceAddress       = 0x1234;
    radioConfigSet.Modulation            = Modulation_LoRa;
    // specifiy the RF frequency by hand
    // or use the calcFreqToRegister() function of the WiMODLR_BASE object)
    radioConfigSet.RfFreq_LSB            = 0x91; // Frequency approx 869.524.475 Hz
    radioConfigSet.RfFreq_MID            = 0x61; // see data sheet for formula
    radioConfigSet.RfFreq_MSB            = 0xD9;
    radioConfigSet.LoRaBandWidth         = LoRaBandwith_125kHz;
    radioConfigSet.LoRaSpreadingFactor   = LoRa7_SF7;
    radioConfigSet.ErrorCoding           = ErrorCoding0_4_5;
    radioConfigSet.PowerLevel            = TxPowerLevel_5_dBm;
    radioConfigSet.TxControl             = (DEVMGMT_RADIO_CFG_TX_CTRL_TX_NARROW_FILTER_ON);
    radioConfigSet.RxControl             = RxCtrl_Receiver_AlwaysOn;
    radioConfigSet.RxWindowTime          = 0xffff;  // value is ignored due to always on setting
    radioConfigSet.LedControl            = DEVMGMT_RADIO_CFG_LED_CTRL_LED4_ALIVE_IND_ON; // the shield has got only one LED
    radioConfigSet.MiscOptions           = (DEVMGMT_RADIO_CFG_MISC_EXTENDED_HCI_OUTPUT_FORMAT  | \
                                            DEVMGMT_RADIO_CFG_MISC_RTC_ENABLED                | \
                                            DEVMGMT_RADIO_CFG_MISC_HCI_TX_IND_ENABLED         | \
                                            DEVMGMT_RADIO_CFG_MISC_HCI_PWR_UP_IND_ENABLED);
    radioConfigSet.FskDatarate           = FskDatarate_50kbps;  // value is ignored due to LoRa modulation
    radioConfigSet.PowerSavingMode       = PowerSaving_Off;
    radioConfigSet.LbtThreshold          = -90;  // value is ignored due to TxControl setting

    // in order to take care of the number of EEPROM write access cycles
    // this configuration is NOT stored in NWM / EEPROM
    // for permanent storage use a value of 1 or true
    radioConfigSet.StoreNwmFlag          = 0;

    result = wimod.SetRadioConfig(&radioConfigSet);

    if (result) {
        debugMsg(F("A new RadioParameterSet has been set.\n\n"));
    } else {
        debugMsg(F("Failed to set new RadioParameterSet.\n\n"));
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

    printStartMsg();

    debugMsg(F("waiting for user...\r\n"));

    // wait for the user to open serial terminal
    delay(5000);

    // show "old" config
    debugMsg(F("### readout old configuration\r\n"));
    showRadioConfig();

    // write "new" config
    debugMsg(F("### write new configuration\r\n"));
    writeRadioConfig();

    // show "new" config
    debugMsg(F("### readout new configuration\r\n"));
    showRadioConfig();
}




void loop()
{

    // check for any pending data of the WiMOD
    wimod.Process();

    delay(2000);
}
