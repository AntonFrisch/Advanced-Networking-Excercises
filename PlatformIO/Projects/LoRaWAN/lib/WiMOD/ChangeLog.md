# ChangeLog

This file gives a short overview about the changes of the library.

## V1.0.0 (2017-02-20)
* Pre-release Version for beta testing

## V1.1.0 (2017-03-08)
* First release version
* added support for non AVR-based boards 
 
## V1.2.0 (2017-05-25)
* BUG-FIX: CRC generation for some non AVR-based boards was wrong
* added possibility to use Cpp11 function objects for callback mechanism. (Since not all platforms support Cpp11 (by now) this feauture has to be manually activated by the user! see "#define WIMOD_USE_CPP11")

## V1.3.0 (2017-06-05)
* adoption to LoRaWAN EndNode Modem Firmware V 2.0

## V1.3.1 (2017-06-26)
* BUG-FIX: fixed with of DevAddr in the example files
* Added new example showing how to read out some LoRaWAN related infos of a radio module
* minor documentation update

## V1.3.2 (2017-08-21)
* Added the MIC-field within RadioLinkMessage
* fixed a typo in an encryption related define
* added the airtime parameter of spec V1.10
* modified the TRadioLinkUDataTxIndicationCallback  (according to spec V1.10)

## V1.4.0 (2018-07-16)
* gernal LoRaWAN upate according to HCI spev V1.22 + Regionalsettings 
* added bootloader demo files
* renamed some stack constants: 
**  LORAWAN_FORMAT_STATUS_JOIN_OK         -> LORAWAN_JOIN_NWK_IND_FORMAT_STATUS_JOIN_OK
**  LORAWAN_FORMAT_STATUS_JOIN_OK_EXT_HCI -> LORAWAN_JOIN_NWK_IND_FORMAT_STATUS_JOIN_OK_CH_INFO

## V1.4.1 (2018-09-19)
* fixed some documentation bugs
* added an additional buffer for message processing

## V1.5.0 (2019-04-26)
* added support for LR-BASE-PLUS for WiMOD iM282A module
* minor documentation updates

## V1.5.1 (2020-01-20)
* added exmaple for SHT35 sensor
* added Bootloader sketch for WiMODino
* added comments in the examples with Serial port to use for the WiMODino

## V1.6.0 (2020-12-09)
* added support for the GlobalLink24 stack
* changed the defualt Serial Ports to match the WiMODino configuration
* fixed some documentation bugs

