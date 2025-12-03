/*****************************************************************************
 * (Arduino UNO) UartBridge for Bootloading of the WiMOD module ONLY !!!
 *
 * This sketch can be used if the interface "Serial0" is used to communicate
 * with the WiMOD module.
 *
 * WARNNG: Using the interface called "Serial" is not recommended because the
 *         communication is not very stable (depending on the conctrete board)
 *
 * If the interface called "Serial3" is to be used please use the other sketch
 * called "DUE_BtlBridge.ino"
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
 * - make sure to set the UART jumpers to "UART0"
 * - make sure to set the UART switch to position "C"
 * - Program this sketch to the Arduino board
 * - close all Terminal-tools that may access the serial port
 * - press the RESET-Button(B2) of the WSA01 and keep it pressed
 * - start the WiMOD LR Studio or the WiMOD LoRaWan EndNode Studio
 * - select: File/Firmware Update
 * - manually select the COM port of the board and click "continue"
 * - select the right WiMOD module type and click "continue"
 * - press the Bootloader(B1)-button on the WSA01
 * - click "continue"
 * - 2 sec after the click release the RESET(B2) button; The timing is important here!
 * - wait one second and release the BOOTLOADER(B1)-button
 * - if the bootloader has been reached:
 * 		- select the firmware image to flash
 * 		- continue the process
 * 	-if there was an error: close the Firmware Update dialog and try again
 *
 * - after the data transfer is finished close the Firmware Update dialog
 *
 * - programm any other Arduino sketch to your Arduino and continue using it
 */


void setup() {

	pinMode(1, INPUT);
	pinMode(2, INPUT);

}

void loop() {
	/*
	 * DO NOTHING IN ORDER NOT TO DISTURB THE COMMUNIKATION BETWEEN
	 * THE PC AND THE WiMOD MODULE
	 */

}
