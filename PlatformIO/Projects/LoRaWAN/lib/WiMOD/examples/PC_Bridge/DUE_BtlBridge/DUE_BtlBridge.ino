/*****************************************************************************
 * (Arduino DUE/Mega/..) UartBridge for Bootloading of the WiMOD module ONLY !!!
 *
 * This sketch can be used if the interface "Serial3" is used to communicate
 * with the WiMOD module.
 *
 * If the interface called "Serial" is to be used please use the other sketch
 * called "UNO_BtlBridge.ino"
 *
 * This program is a special version of a UART Bridge that is designed for
 * bootloading the WiMOD module (mounted on the WSA01), ONLY!
 *
 * A "normal" HCI communication is not possible with this programm.
 * (For "normal" HCI communication, please use the *_UartBridge.ino demo.)
 *
 *
 * This allows the user to update the firmware of the mounted WiMOD module
 * via the integrated bootloader and the WiMOD* Studio PC-tools.
 *
 ******************************************************************************/

/*
 * Usage:
 *
 * - Disconnect all shields from the board, expect the WSA01
 * - make sure to set the UART jumpers to "UART3"
 * - make sure to set the UART switch to position "A"
 * - Program this sketch to the Arduino board
 * - close all Terminal-tools that may access the serial port
 * - press the RESET-Button(B2) of the WSA01 and keep it pressed
 * - start the WiMOD LR Studio or the WiMOD LoRaWan EndNode Studio
 * - select: File/Firmware Update
 * - manually select the COM port of the board and click "continue"
 * - select the right WiMOD module type and click "continue"
 * - press the Bootloader(B1)-button on the WSA01 and release the RESET-button
 * - wait one second and release the BOOTLOADER(B1)-button
 * - click "continue"
 * - select the firmware image to flash
 * - continue the process
 * - after the data transfer is finished close the Firmware Update dialog
 *
 * - programm any other Arduino sketch to your Arduino and continue using it
 */



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

char buff[500];
int len;

//-----------------------------------------------------------------------------
// section code
//-----------------------------------------------------------------------------

void setup() {
	// init serial interfaces
	WIMOD_IF.begin(57600, SERIAL_8E1);
	PC_IF.begin(57600, SERIAL_8E1);

}

void loop() {

	/*
	 * just forward the bytes on the serial interfaces
	 */
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

	// keep this delay command under all contitions.
	delay(10);
}
