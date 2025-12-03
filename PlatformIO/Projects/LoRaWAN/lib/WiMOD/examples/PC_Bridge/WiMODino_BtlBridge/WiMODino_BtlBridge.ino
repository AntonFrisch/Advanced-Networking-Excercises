/*****************************************************************************
 * (WiMODino) UartBridge for Bootloading of the WiMOD module ONLY !!!
 *
 * This program is a special version of a UART Bridge that is designed for
 * bootloading the WiMOD module, ONLY!
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
 * - Disconnect all shields from the board
 * - Connect a jumper cable between: GND and GPIO 3 (see START_PIN define)
 * - Program this sketch to the Arduino board
 * - close all Terminal-tools that may access the serial port
 *
 * - the LED of the board will blink
 * - start the WiMOD LR Studio or the WiMOD LoRaWan EndNode Studio
 * - select: File/Firmware Update
 * - manually select the COM port of the board and click "continue"
 * - select the right WiMOD module type and click "continue"
 * - remove the jumper cable; the LED will stop to blink
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
 * For single serial interface boards (e.g. UNO) are NOT supported for this
 * file.
 */

#define WIMOD_IF    SerialWiMOD
#define PC_IF       SerialUSB

#define WIMOD_RESET_PIN PIN_WIMOD_NRST
#define WIMOD_BOOT_PIN  PIN_WIMOD_BOOT
#define START_PIN   3

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

	// init GPIOs
	pinMode(WIMOD_RESET_PIN, OUTPUT);
	digitalWrite(WIMOD_RESET_PIN, LOW);

	pinMode(WIMOD_BOOT_PIN, OUTPUT);
	digitalWrite(WIMOD_BOOT_PIN, HIGH);

	// init start flag
	pinMode(START_PIN, INPUT);
	// active pull up for this input
	digitalWrite(START_PIN, HIGH);

	// LED
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);
}

int startFlag = 0;

void loop() {

	// check if the jumper cable is set
	startFlag = digitalRead(START_PIN);

	if (startFlag) {
		// jumper calble is not present
		// -> PC is ready for data transfer

		// release the reset state og the WiMOD
		digitalWrite(WIMOD_RESET_PIN, HIGH);
		digitalWrite(LED_BUILTIN, HIGH);

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

	} else {
		// jumper cable is set
		// -> PC is not ready jet

		// just wait for the user to complete the inital steps

		// keep the WiMOD module in reset state
		digitalWrite(WIMOD_RESET_PIN, LOW);
		digitalWrite(WIMOD_BOOT_PIN, HIGH);

		// let the LED blink to indicate the waiting state
		for (int i = 0; i < 8; i++) {
			digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
			delay(200);
		}
		digitalWrite(LED_BUILTIN, LOW);
	}

	// keep this delay command under all contitions.
	delay(10);
}
