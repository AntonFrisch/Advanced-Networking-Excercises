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
 * This example demonstrates how to read out some basic information about
 * the WiMOD firmware
 *
 * Setup requirements:
 * -------------------
 * - 1 WiMOD module running LR-Base firmware firmware
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

/*
 * Create in instance of the interface to the WiMOD-LR-Base firmware
 */
WiMODLRBASE wimod(WIMOD_IF);  // use the Arduino Serial3 as serial interface


//-----------------------------------------------------------------------------
// section code
//-----------------------------------------------------------------------------

void translate_Frequency(uint8_t msb, uint8_t mid, uint8_t lsb);
void translate_Modulation(TRadioCfg_Modulation mod);
void translate_LoRaBandwith(TRadioCfg_LoRaBandwidth bw);
void translate_LoRaErrorCoding(TRadioCfg_ErrorCoding ec);
void translate_LoRaSF(TRadioCfg_LoRaSpreadingFactor sf);
void translate_PowerLevel(TRadioCfg_PowerLevel pwrLevel);
void translate_RxControlMode(TRadioCfg_RxControl rxc);
void translate_PwrSavingMode(TRadioCfg_PowerSavingMode pwr);

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

/******************************************************************************
 * translate internal defines / names to user friendly strings
 * - for DataRateIndex field
 *****************************************************************************/

void translate_Frequency(uint8_t msb, uint8_t mid, uint8_t lsb) {
    uint32_t freq;
    char text[30];

    freq = wimod.calcRegisterToFreq(msb, mid, lsb);
    snprintf(text, 30, "%lu Hz", freq);

    debugMsg(String(text));
}

void translate_Modulation(TRadioCfg_Modulation mod) {
    switch (mod) {
    case Modulation_LoRa:
        debugMsg(F(" -> (LoRa)"));
        break;
    case Modulation_FSK:
        debugMsg(F(" -> (FSK)"));
        break;
    default:
        break;
    }
}

void translate_LoRaBandwith(TRadioCfg_LoRaBandwidth bw) {
    switch (bw) {
    case LoRaBandwith_125kHz:
        debugMsg(F(" -> (125 kHz)"));
        break;
    case LoRaBandwith_250kHz:
        debugMsg(F(" -> (250 kHz)"));
        break;
    case LoRaBandwith_500kHz:
        debugMsg(F(" -> (500 kHz)"));
        break;
    default:
        break;
    }
}

void translate_LoRaErrorCoding(TRadioCfg_ErrorCoding ec) {
    switch (ec) {
    case ErrorCoding0_4_5:
        debugMsg(F(" -> (4/5)"));
        break;
    case ErrorCoding1_4_5:
        debugMsg(F(" -> (4/5)"));
        break;
    case ErrorCoding2_4_6:
        debugMsg(F(" -> (4/6)"));
        break;
    case ErrorCoding3_4_7:
        debugMsg(F(" -> (4/7)"));
        break;
    case ErrorCoding4_4_8:
        debugMsg(F(" -> (4/8)"));
        break;
    default:
        break;
    }
}

void translate_LoRaSF(TRadioCfg_LoRaSpreadingFactor sf) {
    switch (sf) {
    case LoRa0_SF7:
    case LoRa1_SF7:
    case LoRa2_SF7:
    case LoRa3_SF7:
    case LoRa4_SF7:
    case LoRa5_SF7:
    case LoRa6_SF7:
    case LoRa7_SF7:
        debugMsg(F(" -> (SF7)"));
        break;
    case LoRa8_SF8:
        debugMsg(F(" -> (SF8)"));
        break;
    case LoRa9_SF9:
        debugMsg(F(" -> (SF9)"));
        break;
    case LoRa10_SF10:
        debugMsg(F(" -> (SF10)"));
        break;
    case LoRa11_SF11:
        debugMsg(F(" -> (SF11)"));
        break;
    case LoRa12_SF12:
        debugMsg(F(" -> (SF12)"));
        break;
    default:
        break;
    }
}

void translate_PowerLevel(TRadioCfg_PowerLevel pwrLevel) {
    switch (pwrLevel) {
        case TxPowerLevel_5_dBm:
            debugMsg(F(" -> (5 dBm)"));
            break;
        case TxPowerLevel_6_dBm:
            debugMsg(F(" -> (6 dBm)"));
            break;
        case TxPowerLevel_7_dBm:
            debugMsg(F(" -> (7 dBm)"));
            break;
        case TxPowerLevel_8_dBm:
            debugMsg(F(" -> (8 dBm)="));
            break;
        case TxPowerLevel_9_dBm:
            debugMsg(F(" -> (9 dBm)"));
            break;
        case TxPowerLevel_10_dBm:
            debugMsg(F(" -> (10 dBm)"));
            break;
        case TxPowerLevel_11_dBm:
            debugMsg(F(" -> (11 dBm)"));
            break;
        case TxPowerLevel_12_dBm:
            debugMsg(F(" -> (12 dBm)"));
            break;
        case TxPowerLevel_13_dBm:
            debugMsg(F(" -> (13 dBm)"));
            break;
        case TxPowerLevel_14_dBm:
            debugMsg(F(" -> (14 dBm)"));
            break;
        case TxPowerLevel_15_dBm:
            debugMsg(F(" -> (15 dBm)"));
            break;
        case TxPowerLevel_16_dBm:
            debugMsg(F(" -> (16 dBm)"));
            break;
        case TxPowerLevel_17_dBm:
            debugMsg(F(" -> (17 dBm)"));
            break;
        case TxPowerLevel_18_dBm:
            debugMsg(F(" -> (18 dBm)"));
            break;
        case TxPowerLevel_19_dBm:
            debugMsg(F(" -> (19 dBm)"));
            break;
        case TxPowerLevel_20_dBm:
            debugMsg(F(" -> (20 dBm)"));
            break;
        default:
            break;
    }
}

void translate_RxControlMode(TRadioCfg_RxControl rxc) {
    switch (rxc) {
        case RxCtrl_Receiver_Off:
            debugMsg(F(" -> (Receiver Off)"));
            break;
        case RxCtrl_Receiver_AlwaysOn:
            debugMsg(F(" -> (Receiver Always On)"));
            break;
        case RxCtrl_Receiver_RxWindowed:
            debugMsg(F(" -> (Receiver windowed RX)"));
            break;
        default:
            break;
    }
}

void translate_PwrSavingMode(TRadioCfg_PowerSavingMode pwr) {
    switch (pwr) {
        case PowerSaving_Off:
            debugMsg(F(" -> (PwrSaving is Off)"));
            break;
        case PowerSaving_On:
            debugMsg(F(" -> (PwrSaving is On)"));
            break;
        default:
            break;
    }
}


/*****************************************************************************
 * print out a welcome message
 ****************************************************************************/


void printStartMsg()
{
    debugMsg(F("==================================================\n"));
    debugMsg(F("Starting...\n"));
    debugMsg(F("==================================================\n"));
}


/*****************************************************************************
 * Arduino setup function
 ****************************************************************************/

void setup()
{
    // wait for the PC interface to be ready (max 10 sec); usefull for USB
    while (!PC_IF && millis() < 10000 ){}

    // wait for the PC interface to be ready (max 10 sec); usefull for USB
    while (!PC_IF && millis() < 10000 ){}

    // init / setup the serial interface connected to WiMOD
    WIMOD_IF.begin(WIMOD_LR_BASE_SERIAL_BAUDRATE);
    // init the communication stack
    wimod.begin();

    // debug interface
    PC_IF.begin(115200);
    printStartMsg();
}


/*****************************************************************************
 * read and print out the configuration of the WiMOD module
 ****************************************************************************/
void printWimdRadioConfig()
{
    TWiMODLR_DevMgmt_RadioConfig radioCfg;

    debugMsg(F("\n\n RadioConfig:\n"));


    // read the current configuration of the WiMOD module
    if (wimod.GetRadioConfig(&radioCfg)) {
        // print out some parts of the config
        debugMsg(F("GroupAdr:       "));
        debugMsg(radioCfg.GroupAddress);
        debugMsg(F(" (0x"));
        debugMsgHex(radioCfg.GroupAddress);
        debugMsg(F(")\n"));

        debugMsg(F("TxGroupAdr:     "));
        debugMsg(radioCfg.TxGroupAddress);
        debugMsg(F(" (0x"));
        debugMsgHex(radioCfg.TxGroupAddress);
        debugMsg(F(")\n"));

        debugMsg(F("DeviceAdr:      "));
        debugMsg(radioCfg.DeviceAddress);
        debugMsg(F(" (0x"));
        debugMsgHex((radioCfg.DeviceAddress & 0xFF00) >> 8);
        debugMsgHex((radioCfg.DeviceAddress & 0xFF));
        debugMsg(F(")\n"));

        debugMsg(F("TxDeviceAdr:    "));
        debugMsg(radioCfg.TxDeviceAddress);
        debugMsg(F(" (0x"));
        debugMsgHex((radioCfg.TxDeviceAddress & 0xFF00) >> 8);
        debugMsgHex((radioCfg.TxDeviceAddress & 0xFF));
        debugMsg(F(")\n"));

        debugMsg(F("Frequency:      "));
        debugMsg(F("("));
        debugMsgHex(radioCfg.RfFreq_MSB);
        debugMsg(F("|"));
        debugMsgHex(radioCfg.RfFreq_MID);
        debugMsg(F("|"));
        debugMsgHex(radioCfg.RfFreq_LSB);
        debugMsg(F(")"));
        debugMsg(F(" -> "));
        translate_Frequency(radioCfg.RfFreq_MSB, radioCfg.RfFreq_MID,
                radioCfg.RfFreq_LSB);
        debugMsg(F("\n"));


        debugMsg(F("Modulation:     "));
        debugMsg((int) radioCfg.Modulation);    // see TRadioCfg_Modulation for symbolic meaning
        translate_Modulation(radioCfg.Modulation);
        debugMsg(F("\n"));

        debugMsg(F("LoRa-BW ID:     "));
        debugMsg((int) radioCfg.LoRaBandWidth); // see TRadioCfg_LoRaBandwidth for symbolic meaning
        translate_LoRaBandwith(radioCfg.LoRaBandWidth);
        debugMsg(F("\n"));

        debugMsg(F("LoRa-EC ID:     "));
        debugMsg((int) radioCfg.ErrorCoding);   // see TRadioCfg_ErrorCoding for symbolic meaning
        translate_LoRaErrorCoding(radioCfg.ErrorCoding);
        debugMsg(F("\n"));

        debugMsg(F("LoRa-SF ID:     "));
        debugMsg((int) radioCfg.LoRaSpreadingFactor); // see TRadioCfg_LoRaSpreadingFactor for symbolic meaning
        translate_LoRaSF(radioCfg.LoRaSpreadingFactor);
        debugMsg(F("\n"));

        debugMsg(F("TxPwrLevel ID:  "));  // see TRadioCfg_PowerLevelPlus for symbolic meaning
        debugMsg(radioCfg.PowerLevel);
        translate_PowerLevel(radioCfg.PowerLevel);
        debugMsg(F("\n"));

        debugMsg(F("TxControl:      0x"));  // see TRadioCfg_TxControlPlus for symbolic meaning
        debugMsgHex(radioCfg.TxControl);
        debugMsg(F("\n"));

        debugMsg(F("RxControl:      0x"));  // see TRadioCfg_RxControlPlus for symbolic meaning
        debugMsgHex(radioCfg.RxControl);
        translate_RxControlMode(radioCfg.RxControl);
        debugMsg(F("\n"));

        debugMsg(F("RxWindow:       "));
        debugMsg((int) radioCfg.RxWindowTime);
        debugMsg(F(" ms \n"));

        debugMsg(F("LED Control:    0x"));
        debugMsgHex((int) radioCfg.LedControl);
        debugMsg(F("  \n"));

        debugMsg(F("Misc-Options:   0x"));
        debugMsgHex((int) radioCfg.MiscOptions);
        debugMsg(F("  \n"));

        debugMsg(F("PwrSave Mode:   "));
        debugMsg((int) radioCfg.PowerSavingMode); // see TRadioCfg_PowerSavingModePlus for symbolic meaning
        translate_PwrSavingMode(radioCfg.PowerSavingMode);
        debugMsg(F("  \n"));

        debugMsg(F("LBT Threshold:  "));
        debugMsg((int) radioCfg.LbtThreshold);
        debugMsg(F("  dBm\n"));

    }

}

/*****************************************************************************
 * read and print out some infos about the firmware of the WiMOD module
 ****************************************************************************/
void printWimodFirmwareInfos()
{
    TWiMODLR_DevMgmt_FwInfo fwInfo;

    debugMsg(F("\n\n FirmwareInfos:\n"));

    // readout the Firmware infomation
    if (wimod.GetFirmwareInfo(&fwInfo)) {
        debugMsg(F("FW-Name:  "));
        debugMsg((const char*) fwInfo.FirmwareName);
        debugMsg(F("\n"));

        debugMsg(F("FW-Date:  "));
        debugMsg((const char*) fwInfo.BuildDateStr);
        debugMsg(F("\n"));

        debugMsg(F("FW-Ver:   "));
        debugMsg(fwInfo.FirmwareMayorVersion);
        debugMsg(F("."));
        debugMsg(fwInfo.FirmwareMinorVersion);
        debugMsg(F("\n"));
        debugMsg(F("BuildCnt: "));
        debugMsg((int) fwInfo.BuildCount);
        debugMsg(F("\n"));
    }
}



void loop()
{
    bool result;

    printWimdRadioConfig();

    printWimodFirmwareInfos();

    // check for any pending data of the WiMOD
    wimod.Process();

    delay(2000);
}

