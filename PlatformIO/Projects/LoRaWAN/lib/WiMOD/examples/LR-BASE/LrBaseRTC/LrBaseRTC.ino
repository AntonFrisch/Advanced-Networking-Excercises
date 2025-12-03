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
 * This example demonstrates how to get and set the RTC of the WiMOD
 *
 * Setup requirements:
 * -------------------
 * - 1 WiMOD module running LR-Base (or LoRaWAN EndNode Modem) firmware
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

#define WIMOD_IF    Serial3         // for WiMODino use: SerialWiMOD
#define PC_IF		Serial          // for WiMODino use: SerialUSB

//-----------------------------------------------------------------------------
// special data types
//-----------------------------------------------------------------------------

typedef struct TWimodTime
{
    UINT16  year;
    UINT8   month;
    UINT8   day;
    UINT8   hour;
    UINT8   min;
    UINT8   sec;
} TWimodTime;


// function prototypes
void debugMsg(String msg);
void debugMsgInt(int a);
void showWimodTime(TWimodTime* moduleTime);
void printStartMsg();
void setRTC();
void getRTC();

//-----------------------------------------------------------------------------
// section RAM
//-----------------------------------------------------------------------------


// define a date( this is just a random example!)
static TWimodTime wimodTime = { .year  = 2018,
                                .month = 9,
                                .day   = 20,
                                .hour  = 10,
                                .min   = 30,
                                .sec   = 40};


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

void debugMsgInt(int a)
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
    debugMsg(F("how to use the RTC feature of the WiMOD Radio Module\n"));
    debugMsg(F("==================================================\n"));
}

/*****************************************************************************
 * print time
 ****************************************************************************/
void showWimodTime(TWimodTime* moduleTime)
{
    if (moduleTime) {
        debugMsgInt((int)moduleTime->year);
        debugMsg("-");
        debugMsgInt((int)moduleTime->month);
        debugMsg("-");
        debugMsgInt((int)moduleTime->day);
        debugMsg(" ");
        debugMsgInt((int)moduleTime->hour);
        debugMsg(":");
        debugMsgInt((int)moduleTime->min);
        debugMsg(":");
        debugMsgInt((int)moduleTime->sec);
        debugMsg("\r\n");
    }
}

/*****************************************************************************
 * set RTC - set the time in WiMOD
 ****************************************************************************/
void setRTC()
{
    UINT32 timestamp;

    // convert date/time to timestamp format
    timestamp = WIMOD_RTC_MAKE_DATETIME_U32(wimodTime.sec,   \
                                            wimodTime.min,   \
                                            wimodTime.hour,  \
                                            wimodTime.day,   \
                                            wimodTime.month, \
                                            wimodTime.year);

    // try to set the RTC to specified date/time
    if (wimod.SetRtc(timestamp)) {
        debugMsg("Setup of RTC: OK\r\n");

        debugMsg("WiMOD time has been  set to: ");
        showWimodTime(&wimodTime);
    } else {
        debugMsg("Setup of RTC: Failed\r\n");
    }
}

/*****************************************************************************
 * get RTC - get time from WiMOD and print it
 ****************************************************************************/

void getRTC()
{
    UINT32 timestamp;

    // try to get the RTC information out of the WiMOD
    if (wimod.GetRtc(&timestamp)) {
        wimodTime.year  = WIMOD_RTC_GET_YEARS(timestamp);
        wimodTime.month = WIMOD_RTC_GET_MONTHS(timestamp);
        wimodTime.day   = WIMOD_RTC_GET_DAYS(timestamp);
        wimodTime.hour  = WIMOD_RTC_GET_HOURS(timestamp);
        wimodTime.min   = WIMOD_RTC_GET_MINUTES(timestamp);
        wimodTime.sec   = WIMOD_RTC_GET_SECONDS(timestamp);

        debugMsg("WiMOD time is now: ");
        showWimodTime(&wimodTime);

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
    WIMOD_IF.begin(WIMOD_LR_BASE_SERIAL_BAUDRATE);
    // init the communication stack
    wimod.begin();

    // debug interface
    PC_IF.begin(115200);

    printStartMsg();

    // wait a while (for the user to open the serial monitor)
    delay(1000);

    // init the RTC time to specified date
    setRTC();
}


void loop()
{
    getRTC();

    // check for any pending data of the WiMOD
    wimod.Process();

    delay(2000);
}
