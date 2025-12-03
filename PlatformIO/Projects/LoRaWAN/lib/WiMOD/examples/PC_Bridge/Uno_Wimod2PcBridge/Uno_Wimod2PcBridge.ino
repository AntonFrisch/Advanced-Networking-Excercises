/*****************************************************************************
* Arduino Uno UartBridge
*
* This programm should be used if the user wants to connect the WiMOD to PC
* for using the IMST PC tools.
* Since the Arduino UNO device uses a shared UART interface for the on-board
* controller and the shields, the user must make sure that the on-board
* controller does not disturb the UTART communication between PC and WiMOD.
*
* Therefore this sketch can be used.
*
* Note: This sketch is for Arduino devices with have only one (shared) uart.

*
*       The uart select switch must be on position "C".
*
******************************************************************************/

void setup() {
  // Set the pins 0 and 1 as high-impedance inputs
  pinMode(0, INPUT);
  pinMode(1, INPUT);
}

void loop() {
  // Do nothing
}
