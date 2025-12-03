/*
 * CayenneLPP_constants.h
 *
 *  Created on: Feb 10, 2017
 *      Author: FB
 */

//------------------------------------------------------------------------------
//! @file CayenneLPP_constants.h
//! @ingroup CayenneLPP
//! <!------------------------------------------------------------------------->
//! @brief Format converter for myDevices Cayenne LPP format definition
//! @version 0.1
//! <!------------------------------------------------------------------------->
//!
//! THIS FILE IS BASED ON THE PUBLIC IMPLEMENTATION FOR THE MyDevices Cayenne
//! PLATFORM.
//!
//! see https://mdswp-staging.mydevices.com/cayenne/docs/#lora
//! see https://mydevices.com/cayenne/docs/lora/#lora-cayenne-low-power-payload
//!
//!
//------------------------------------------------------------------------------


#ifndef ARDUINO_CAYENNE_CAYENNELPP_CONSTANTS_H_
#define ARDUINO_CAYENNE_CAYENNELPP_CONSTANTS_H_

//------------------------------------------------------------------------------
//
// Section Includes Files
//
//------------------------------------------------------------------------------

//! @cond Doxygen_Suppress

// source: https://mdswp-staging.mydevices.com/cayenne/docs/#lora

#define LPP_DIGITAL_INPUT       0       // 1 byte
#define LPP_DIGITAL_OUTPUT      1       // 1 byte
#define LPP_ANALOG_INPUT        2       // 2 bytes, 0.01 signed
#define LPP_ANALOG_OUTPUT       3       // 2 bytes, 0.01 signed
#define LPP_LUMINOSITY          101     // 2 bytes, 1 lux unsigned
#define LPP_PRESENCE            102     // 1 byte, 1
#define LPP_TEMPERATURE         103     // 2 bytes, 0.1°C signed
#define LPP_RELATIVE_HUMIDITY   104     // 1 byte, 0.5% unsigned
#define LPP_ACCELEROMETER       113     // 2 bytes per axis, 0.001G
#define LPP_BAROMETRIC_PRESSURE 115     // 2 bytes 0.1 hPa Unsigned
#define LPP_GYROMETER           134     // 2 bytes per axis, 0.01 °/s
#define LPP_GPS                 136     // 3 byte lon/lat 0.0001 °, 3 bytes alt 0.01m


// Data ID + Data Type + Data Size
#define LPP_DIGITAL_INPUT_SIZE       3
#define LPP_DIGITAL_OUTPUT_SIZE      3
#define LPP_ANALOG_INPUT_SIZE        4
#define LPP_ANALOG_OUTPUT_SIZE       4
#define LPP_LUMINOSITY_SIZE          4
#define LPP_PRESENCE_SIZE            3
#define LPP_TEMPERATURE_SIZE         4
#define LPP_RELATIVE_HUMIDITY_SIZE   3
#define LPP_ACCELEROMETER_SIZE       8
#define LPP_BAROMETRIC_PRESSURE_SIZE 4
#define LPP_GYROMETER_SIZE           8
#define LPP_GPS_SIZE                 11

//! @endcond

#endif /* ARDUINO_CAYENNE_CAYENNELPP_CONSTANTS_H_ */
