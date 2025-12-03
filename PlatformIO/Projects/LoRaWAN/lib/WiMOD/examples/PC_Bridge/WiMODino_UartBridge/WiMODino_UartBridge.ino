/*****************************************************************************
 * WiMODino UartBridge
 *
 * This programm receives the usart data from one serial interface and
 * sends the data to the other serial interface. And vice versa.
 *
 * This allows the user to use the PC-Tools to communicate with the WiMOD
 * module mounted on the boards.
 *
 * Note: This sketch is for the IMST LoRWiMODino device with has got two
 *       separate interfaces.   One for the PC and one for the WiMOD.
 *
 *
 * Warning: Do not use the Firmware-update feature of the WiMOD PC-Tools
 *          in combination with this program!!!
 ******************************************************************************/

//-----------------------------------------------------------------------------
// platform defines
//-----------------------------------------------------------------------------

/*
 * Note: This sketch is for an WiMODino device with two separate serial interfaces.
 * One interface is connected to a PC and one is used for WiMOD.
 *
 * Single serial interface boards (e.g. UNO) are NOT supported by this
 * file.
 */

#define WIMOD_IF    SerialWiMOD
#define PC_IF     	SerialUSB

//-----------------------------------------------------------------------------
// section RAM
//-----------------------------------------------------------------------------

char buff[500];
int len;

//-----------------------------------------------------------------------------
// section code
//-----------------------------------------------------------------------------

void setup() {
	// put your setup code here, to run once:
	WIMOD_IF.begin(115200);
	PC_IF.begin(115200);

}

void loop() {

	// put your main code here, to run repeatedly:
	len = WIMOD_IF.available();
	if (len > 0) {
		WIMOD_IF.readBytes(buff, len);
		PC_IF.write(buff, len);
	}

	len = PC_IF.available();
	if (len > 0) {
		PC_IF.readBytes(buff, len);
		WIMOD_IF.write(buff, len);
	}

	// do not use a delay() function here.
	// the arduino plaform uses a 64 byte low level buffer that is likely to
	// overflow, if a delay is used here
}
