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
 * This example demonstrates how to read out information of a WiMOD
 * radio module that runs the LoRaWAN EndNode Modem firmware (V2)
 *
 * Setup requirements:
 * -------------------
 * - 1 WiMOD module running WiMOD LoRaWAN EndNode firmware
 *
 * Usage:
 * -------
 * Start the program and watch the serial monitor @ 115200 baud
 *
 */


#include <stdio.h>
#include <stdint.h>


// make sure to use only the WiMODLoRaWAN.h
// the WiMODLR_BASE.h must not be used for LoRaWAN firmware.
#include <WiMODLoRaWAN.h>

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
 * Create in instance of the interface to the WiMOD-LoRaWAN firmware
 */
WiMODLoRaWAN wimod(WIMOD_IF);  // use the Arduino Serial3 as serial interface



//-----------------------------------------------------------------------------
// section code
//-----------------------------------------------------------------------------

void 			  translate_DatarateIndex_EU868(uint8_t idx);
void 			  translate_StkOptions(uint8_t options);
TLoRaWAN_FreqBand translate_BandIndex(uint8_t idx);
bool 			  translate_NwkStatus(uint8_t nwkStatus);

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

void printPayload(const uint8_t* buf, uint8_t size) {
  for (int i = 0; i < size; i++) {
	if ((uint8_t) buf[i] < 0x10) {
		PC_IF.print(F("0"));
	}
	PC_IF.print((uint8_t) buf[i], HEX);
	PC_IF.print(F(" "));
  }
  PC_IF.print(F("\n"));
}


/******************************************************************************
 * print some basic infos about WiMOD mode
 *
 *****************************************************************************/

void printModuleInfo(void)
{
	TWiMODLR_DevMgmt_DevInfo devInfo;
	TWiMODLR_DevMgmt_FwInfo  fwInfo;
	TLoRaWAN_FreqBand		 band;
	uint8_t devEUI[8];


	if (wimod.GetDeviceInfo(&devInfo)) {
    	debugMsg(F("\r\n"));
		debugMsg(F("General Device Info:\r\n"));
		debugMsg(F("--------------------\r\n"));
		debugMsg(F("ModuleType:      "));
		if (devInfo.ModuleType == (uint8_t) ModuleType_iM880B) {
			debugMsg(F("iM880B"));
		} else {
			debugMsg(F("0x"));
			debugMsgHex(devInfo.ModuleType);
		}
		debugMsg(F("\r\n"));
//		debugMsg(F("DeviceAdr:       0x"));
//		debugMsgHex(devInfo.DevAdr);
//		debugMsg(F("\r\n"));
		debugMsg(F("DeviceSerial:    0x"));
		debugMsgHex(devInfo.DevID);
		debugMsg(F("\r\n"));
//		debugMsg(F("GroupAdr:        0x"));
//		debugMsgHex(devInfo.GroupAdr);
//		debugMsg(F("\r\n"));
	}

    if (wimod.GetFirmwareInfo(&fwInfo)) {
    	debugMsg(F("\r\n"));
    	debugMsg(F("General Firmware Info:\r\n"));
		debugMsg(F("----------------------\r\n"));
		debugMsg(F("FW-Version:      "));
    	debugMsg(fwInfo.FirmwareMayorVersion);
    	debugMsg(F("."));
    	debugMsg(fwInfo.FirmwareMinorVersion);
    	debugMsg(F(" BC: "));
    	debugMsg(fwInfo.BuildCount);
    	debugMsg(F("\r\n"));
    	debugMsg(F("FW-Name:         "));
    	debugMsg((char*)fwInfo.FirmwareName);
    	debugMsg(F("\r\n"));
    }


    TWiMOD_OperationMode opMode;
    if (wimod.GetOperationMode(&opMode)) {
    	debugMsg(F("OperationMode:   "));
    	if (opMode == OperationMode_Application) {
    		debugMsg(F("Application"));
    	} else {
    		debugMsg(F("non App-Mode"));
    	}
        debugMsg(F("\r\n"));
    }

    if (wimod.GetDeviceEUI(devEUI)) {
    	debugMsg(F("\r\n"));
    	debugMsg(F("LoRaWAN Info:\r\n"));
		debugMsg(F("-------------\r\n"));
    	debugMsg(F("DeviceEUI:       "));
    	printPayload(devEUI, 0x08);
    }

    TWiMODLORAWAN_RadioStackConfig	stackCfg;
    if (wimod.GetRadioStackConfig(&stackCfg)) {
    	debugMsg(F("\r\n"));
    	debugMsg(F("RadioStack Info:\r\n"));
		debugMsg(F("-------------\r\n"));
    	debugMsg(F("DataRate:        "));
    	translate_DatarateIndex_EU868(stackCfg.DataRateIndex);
    	debugMsg(F("init PwrLevel:   "));
		debugMsg((int) stackCfg.TXPowerLevel);
        debugMsg(F("dBm \r\n"));
    	debugMsg(F("Stack Options:   "));
        translate_StkOptions(stackCfg.Options);
    	debugMsg(F("auto PwrSaving:  "));
		debugMsg((int) stackCfg.PowerSavingMode);
        debugMsg(F("\r\n"));
    	debugMsg(F("Retransmissions: "));
		debugMsg((int) stackCfg.Retransmissions);
        debugMsg(F("\r\n"));
    	debugMsg(F("Freq.BandIndex:  "));
        band = translate_BandIndex(stackCfg.BandIndex);

        /*
         * ONLY valid for US firmware -> wimod.begin(LoRaWAN_Region_US915)
         */
//        if (band == LORAWAN_BAND_US_915) {
//        	debugMsg(F("Sub-BandMask-1:  "));
//        	printPayload(&stackCfg.SubBandMask1, 0x01);
//        	debugMsg(F("Sub-BandMask-2:  "));
//        	printPayload(&stackCfg.SubBandMask2, 0x01);
//            debugMsg(F("\r\n"));
//        }

    }

    TWiMODLORAWAN_NwkStatus_Data nwkStatus;
    if (wimod.GetNwkStatus(&nwkStatus)) {
    	debugMsg(F("\r\n"));
    	debugMsg(F("LoRaWAN Nwk Status Info:\r\n"));
		debugMsg(F("------------------------\r\n"));
    	debugMsg(F("LoRaWAN NwkStatus:  "));
    	if (translate_NwkStatus(nwkStatus.NetworkStatus)) {
			debugMsg(F("LoRaWAN DevAdr:     0x"));
			debugMsgHex(nwkStatus.DeviceAddress);
			debugMsg(F("\r\n"));
			debugMsg(F("Uplink DataRate:    "));
			translate_DatarateIndex_EU868(nwkStatus.DataRateIndex);
			debugMsg(F("PowerLevel:         "));
			debugMsg((int) nwkStatus.PowerLevel);
			debugMsg(F(" dBm\r\n"));
			debugMsg(F("max. Payl. Size:    "));
			debugMsg((int) nwkStatus.MaxPayloadSize);
			debugMsg(F(" byte\r\n"));
    	}
    }

    TWiMODLR_DevMgmt_RtcAlarm rtcAlarm;
    if (wimod.GetRtcAlarm(&rtcAlarm)) {
    	debugMsg(F("\r\n"));
    	debugMsg(F("RTC Alarm Info:\r\n"));
		debugMsg(F("---------------\r\n"));
    	debugMsg(F("RTC alarm stat.: "));
    	if (RTC_Alarm_No_Alarm_Set == rtcAlarm.AlarmStatus) {
    		debugMsg(F("inactive"));
    	} else {
    		debugMsg(F("active"));

			debugMsg(F("\r\n"));
			debugMsg(F("RTC alarm Mode:   "));
			if (RTC_Alarm_Single == rtcAlarm.Options) {
				debugMsg(F("single alarm mode"));
			} else {
				debugMsg(F("daily alarm mode"));

			}
			debugMsg(F("\r\n"));
			debugMsg(F("RTC alarm at:    "));
			debugMsg((int) rtcAlarm.Hour);
			debugMsg(F(":"));
			debugMsg((int) rtcAlarm.Minutes);
			debugMsg(F(":"));
			debugMsg((int) rtcAlarm.Seconds);
			debugMsg(F("\r\n"));
    	}
    }

}

/******************************************************************************
 * translate internal defines / names to user friendly strings
 * - for DataRateIndex field
 *****************************************************************************/

void translate_DatarateIndex_EU868(uint8_t idx) {
	switch (idx) {
		case LoRaWAN_DataRate_EU868_LoRa_SF12_125kHz:
			debugMsg(F("SF12 - 125kHz"));
			break;
		case LoRaWAN_DataRate_EU868_LoRa_SF11_125kHz:
			debugMsg(F("SF11 - 125kHz"));
			break;
		case LoRaWAN_DataRate_EU868_LoRa_SF10_125kHz:
			debugMsg(F("SF10 - 125kHz"));
			break;
		case LoRaWAN_DataRate_EU868_LoRa_SF9_125kHz:
			debugMsg(F("SF9 - 125kHz"));
			break;
		case LoRaWAN_DataRate_EU868_LoRa_SF8_125kHz:
			debugMsg(F("SF8 - 125kHz"));
			break;
		case LoRaWAN_DataRate_EU868_LoRa_SF7_125kHz:
			debugMsg(F("SF7 - 125kHz"));
			break;
		case LoRaWAN_DataRate_EU868_LoRa_SF7_250kHz:
			debugMsg(F("SF7 - 250kHz"));
			break;
		case LoRaWAN_DataRate_EU868_LoRa_FSK:
			debugMsg(F("FSK"));
			break;
		default:
			debugMsg(F("unknown"));
			break;
	}
	debugMsg(F("\r\n"));
}

/******************************************************************************
 * translate internal defines / names to user friendly strings
 * - for Radio Stack Options field
 *****************************************************************************/

void translate_StkOptions(uint8_t options) {

	if (options & LORAWAN_STK_OPTION_ADR) {
		debugMsg(F("ADR "));
	}
	if (options & LORAWAN_STK_OPTION_DUTY_CYCLE_CTRL) {
		debugMsg(F("DUTY_CYCLE "));

	}
	if (options & LORAWAN_STK_OPTION_DEV_CLASS_C) {
		debugMsg(F("CLASS_C "));
	} else {
		debugMsg(F("CLASS_A "));
	}
	if (options & LORAWAN_STK_OPTION_EXT_PKT_FORMAT) {
		debugMsg(F("EXT_HCI "));

	}
	if (options & LORAWAN_STK_OPTION_MAC_CMD) {
		debugMsg(F("MAC_CMD"));
	}
	debugMsg(F("\r\n"));
}

/******************************************************************************
 * translate internal defines / names to user friendly strings
 * - for BandIndex field
 *****************************************************************************/




TLoRaWAN_FreqBand translate_BandIndex(uint8_t idx) {
	TLoRaWAN_FreqBand result;
	switch (idx) {
	case LoRaWAN_FreqBand_EU_868:
		debugMsg(F("EU"));
		result = LoRaWAN_FreqBand_EU_868;
		break;
	case LoRaWAN_FreqBand_US_915:
		debugMsg(F("US"));
		result = LoRaWAN_FreqBand_US_915;
		break;
	case LoRaWAN_FreqBand_India_865:
		debugMsg(F("India"));
		result = LoRaWAN_FreqBand_India_865;
		break;

	case LoRaWAN_FreqBand_AS_923_Brunei:
		debugMsg(F("AS923"));
		result = LoRaWAN_FreqBand_AS_923_Brunei;
		break;
	case LoRaWAN_FreqBand_AS_923_Cambodia:
		debugMsg(F("AS923"));
		result = LoRaWAN_FreqBand_AS_923_Cambodia;
		break;
	case LoRaWAN_FreqBand_AS_923_Indonesia:
		debugMsg(F("AS923"));
		result = LoRaWAN_FreqBand_AS_923_Indonesia;
		break;
	case LoRaWAN_FreqBand_AS_923_Japan:
		debugMsg(F("AS923"));
		result = LoRaWAN_FreqBand_AS_923_Japan;
		break;
	case LoRaWAN_FreqBand_AS_923_Laos:
		debugMsg(F("AS923"));
		result = LoRaWAN_FreqBand_AS_923_Laos;
		break;
	case LoRaWAN_FreqBand_AS_923_NewZealand:
		debugMsg(F("AS923"));
		result = LoRaWAN_FreqBand_AS_923_NewZealand;
		break;
	case LoRaWAN_FreqBand_AS_923_Singapore:
		debugMsg(F("AS923"));
		result = LoRaWAN_FreqBand_AS_923_Singapore;
		break;
	case LoRaWAN_FreqBand_AS_923_Taiwan:
		debugMsg(F("AS923"));
		result = LoRaWAN_FreqBand_AS_923_Taiwan;
		break;
	case LoRaWAN_FreqBand_AS_923_Thailand:
		debugMsg(F("AS923"));
		result = LoRaWAN_FreqBand_AS_923_Thailand;
		break;
	case LoRaWAN_FreqBand_AS_923_Vietnam:
		debugMsg(F("AS923"));
		result = LoRaWAN_FreqBand_AS_923_Vietnam;
		break;
	case LoRaWAN_FreqBand_EU_868_RX2_SF9:
		debugMsg(F("EU- RX2"));
		result = LoRaWAN_FreqBand_EU_868_RX2_SF9;
		break;
	case LoRaWAN_FreqBand_IN_865_RX2_SF8:
		debugMsg(F("India- RX2"));
		result = LoRaWAN_FreqBand_IN_865_RX2_SF8;
		break;
	case LoRaWAN_FreqBand_RU_868_V1:
		debugMsg(F("Russia"));
		result = LoRaWAN_FreqBand_RU_868_V1;
		break;
	case LoRaWAN_FreqBand_RU_868_V2:
		debugMsg(F("Russia"));
		result = LoRaWAN_FreqBand_RU_868_V2;
		break;
	case LoRaWAN_FreqBand_RU_868_V3:
		debugMsg(F("Russia"));
		result = LoRaWAN_FreqBand_RU_868_V3;
		break;
	case LoRaWAN_FreqBand_RU_868_V4:
		debugMsg(F("Russia"));
		result = LoRaWAN_FreqBand_RU_868_V4;
		break;
	case LoRaWAN_FreqBand_RU_868_V5:
		debugMsg(F("Russia"));
		result = LoRaWAN_FreqBand_RU_868_V5;
		break;
	case LoRaWAN_FreqBand_RU_868_V6:
		debugMsg(F("Russia"));
		result = LoRaWAN_FreqBand_RU_868_V6;
		break;
	case LoRaWAN_FreqBand_RU_868_V7:
		debugMsg(F("Russia"));
		result = LoRaWAN_FreqBand_RU_868_V7;
		break;
	default:
		debugMsg(F("unknown"));
		result = LoRaWAN_FreqBand_EU_868;
		break;
	}
	debugMsg(F("\r\n"));
	return result;
}

/******************************************************************************
 * translate internal defines / names to user friendly strings
 * - for Network Status field
 *****************************************************************************/

bool translate_NwkStatus(uint8_t nwkStatus) {
	bool result = true;
	switch ((TLoRaWAN_NwkStatus) nwkStatus) {
		case LoRaWAN_NwkStatus_Inactive:
			debugMsg(F("Inactive"));
			// deacticate further interpretation of following fields
			result = false;
			break;
		case LoRaWAN_NwkStatus_Active_ABP:
			debugMsg(F("Active(ABP)"));
			break;
		case LoRaWAN_NwkStatus_Active_OTAA:
			debugMsg(F("Active(OTAA)"));
			break;
		case LoRaWAN_NwkStatus_Joining_OTAA:
			debugMsg(F("Joining"));
			result = false;
			break;
		default:
			debugMsg(F("unknown"));
			// deacticate further interpretation of following fields
			result = false;
			break;
	}
	debugMsg(F("\r\n"));
	return result;
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
    debugMsg(F("some basic information about the WiMOD Radio Module\n"));
    debugMsg(F("running a LoRaWAN Modem Firmware.\n"));
    debugMsg(F("==================================================\n"));
}


/*****************************************************************************
 * get RTC - get time from WiMOD and print it
 ****************************************************************************/

void getRTC()
{
    uint32_t timestamp;
	uint16_t year;
	uint8_t  month;
	uint8_t  day;
	uint8_t  hour;
	uint8_t  min;
	uint8_t  sec;


    // try to get the RTC information out of the WiMOD
    if (wimod.GetRtc(&timestamp)) {
        year  = WIMOD_RTC_GET_YEARS(timestamp);
        month = WIMOD_RTC_GET_MONTHS(timestamp);
        day   = WIMOD_RTC_GET_DAYS(timestamp);
        hour  = WIMOD_RTC_GET_HOURS(timestamp);
        min   = WIMOD_RTC_GET_MINUTES(timestamp);
        sec   = WIMOD_RTC_GET_SECONDS(timestamp);

        debugMsg("WiMOD RTC: ");

        debugMsg((int) year);
        debugMsg("-");
        debugMsg((int) month);
        debugMsg("-");
        debugMsg((int) day);
        debugMsg(" ");
        debugMsg((int) hour);
        debugMsg(":");
        debugMsg((int) min);
        debugMsg(":");
        debugMsg((int) sec);
        debugMsg("\r\n");

    } else {
        // error
        debugMsg("Error getting current timestamp of WiMOD\r\n");
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
    WIMOD_IF.begin(WIMOD_LORAWAN_SERIAL_BAUDRATE);
    // init the communication stack
    wimod.begin(/*LoRaWAN_Region_US915*/);

    // debug interface
    PC_IF.begin(115200);

    delay(100);

    printStartMsg();

    printModuleInfo();

    debugMsg(F("\r\n\r\n"));
    debugMsg(F("--------------------------------------------------\r\n"));
}


/*****************************************************************************
 * Arduino loop function
 ****************************************************************************/

uint32_t loopCnt = 0;

void loop()
{

	// print out the current RTC information every 5 sec.
	if ((loopCnt % 50) == 0) {
		getRTC();
	}

    // check for any pending data of the WiMOD
    wimod.Process();

    delay(100);
    loopCnt++;
}
