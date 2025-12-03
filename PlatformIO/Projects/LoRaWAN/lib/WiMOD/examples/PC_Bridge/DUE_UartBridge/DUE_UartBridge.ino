/*****************************************************************************
* Arduino DUE UartBridge
*
* This programm receives the usart data from one serial interface and
* sends the data to the other serial interface. And vice versa.
*
* This allows the user to use the PC-Tools to communicate with the WiMOD
* module mounted on the boards.
*
* Note: This sketch is for Arduino devices with two separate interfaces.
*       One for the PC and one for the WiMOD.
*
*       The uart select switch must be on position "A". (+ jumper on "UART3").
*
* Warning: Do not use the Firmware-update feature of the WiMOD PC-Tools
*          in combination with this program!!!
******************************************************************************/


//-----------------------------------------------------------------------------
// platform defines
//-----------------------------------------------------------------------------

/*
 * Note: This sketch is for Arduino devices with two separate serial interfaces
 * (e.g. DUE). One interface is connected to a PC and one is used for WiMOD.
 *
 * Single serial interface boards (e.g. UNO) are NOT supported in this
 * file.
 */

#define WIMOD_IF    Serial3
#define PC_IF       Serial


//-----------------------------------------------------------------------------
// section RAM
//-----------------------------------------------------------------------------
byte buff[500];
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
  len = PC_IF.available();
  if (len > 0) {
	  PC_IF.readBytes(buff, len);
	  WIMOD_IF.write(buff, len);
  }

  len = WIMOD_IF.available();
  if (len > 0) {
	  WIMOD_IF.readBytes(buff, len);
      PC_IF.write(buff, len);
  }

  // keep this delay command under all contitions.
  delay(10);
}
