/*
 * CayenneLPP.cpp
 *
 *  Created on: Feb 10, 2017
 *      Author: FB
 */


//------------------------------------------------------------------------------
//! @file CayenneLPP.cpp
//! @ingroup CayenneLPP
//! <!------------------------------------------------------------------------->
//! @brief Format converter for myDevices Cayenne LPP format definition
//! @version 0.1
//! <!------------------------------------------------------------------------->
//!
//!
//! THIS FILE IS BASED ON THE PUBLIC IMPLEMENTATION FOR THE MyDevices Cayenne
//! PLATFORM.
//!
//! see https://mdswp-staging.mydevices.com/cayenne/docs/#lora
//! see https://mydevices.com/cayenne/docs/lora/#lora-cayenne-low-power-payload
//!
//!
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//
// Section Includes Files
//
//------------------------------------------------------------------------------

#include "CayenneLPP_constants.h"
#include "CayenneLPP.h"

#include <string.h>

//------------------------------------------------------------------------------
//
// Section public functions
//
//------------------------------------------------------------------------------

// source: https://mdswp-staging.mydevices.com/cayenne/docs/#lora


//-----------------------------------------------------------------------------
/**
 * @brief Constructor of a simple Cayenne LPP wrapper
 *
 * @param buf   Pointer to a buffer where to build up cayenne compliant messages
 *
 * @param size  (max) size of the buffer in bytes
 *
 *
 * @code
 * #include <Cayenne/CayenneLPP.h>
 *
 *
 * #define BUF_SIZE_CAYENNE  80
 *
 * // buffer for cayenne
 * uint8_t BUF_SIZE_CAYENNE[80];
 *
 * // cayenne translation object
 * CayenneLPP cayenne(bufCayenne, BUF_SIZE_CAYENNE);
 *  ...
 * }
 * @endcode
 *
 * Orginal code: <a href="https://mdswp-staging.mydevices.com/cayenne/docs/#lora">Mydevices documentation</a>
 */

//Initialize the payload buffer with the given maximum size.
CayenneLPP::CayenneLPP(uint8_t* buf, uint8_t size)
	:  buffer(buf), maxsize(size)
{
    cursor = 0;
    if (buf == NULL) {
    	maxsize = 0x00;
    }
}

//-----------------------------------------------------------------------------
/**
 * @brief Destructor
 */
CayenneLPP::~CayenneLPP(void)
{
}

//-----------------------------------------------------------------------------
/**
 * @brief reset all internal data. Should by called prior building a new message
 */
//Reset the payload, to call before building a frame payload
void CayenneLPP::reset(void)
{
    cursor = 0;
}

//-----------------------------------------------------------------------------
/**
 * @brief get the current size of the build message in bytes
 *
 * @return number of bytes for the build up message
 */
// Returns the current size of the payload
uint8_t CayenneLPP::getSize(void)
{
    return cursor;
}

//-----------------------------------------------------------------------------
/**
 * @brief get a pointer to the internal data buffer
 *
 * @return pointer to the internal buffer
 */
//Return the payload buffer
uint8_t* CayenneLPP::getBuffer(void)
{
    return buffer;
}


//-----------------------------------------------------------------------------
/**
 * @brief copy the build up message to an external buffer
 *
 * @param dst   destination buffer where to copy the data to. WARNING: the
 *              destination must be big enough
 *
 * @return  number of bytes copied
 */
uint8_t CayenneLPP::copy(uint8_t* dst)
{
	if (buffer == NULL) return 0;
    memcpy(dst, buffer, cursor);
    return cursor;
}


//-----------------------------------------------------------------------------
/**
 * @brief intert value of type "digital input" into the message
 *
 * @param   channel the number of the channel to use
 * @param   value   the value of the digital input
 *
 * @return  the number of bytes currently used by the inserted values / size of the total message
 */
uint8_t CayenneLPP::addDigitalInput(uint8_t channel, uint8_t value)
{
    if ((cursor + LPP_DIGITAL_INPUT_SIZE) > maxsize) {
        return 0;
    }
    buffer[cursor++] = channel;
    buffer[cursor++] = LPP_DIGITAL_INPUT;
    buffer[cursor++] = value;
    return cursor;
}

//-----------------------------------------------------------------------------
/**
 * @brief intert value of type "digital output" into the message
 *
 * @param   channel the number of the channel to use
 * @param   value   the value of the digital output
 *
 * @return  the number of bytes currently used by the inserted values / size of the total message
 */
uint8_t CayenneLPP::addDigitalOutput(uint8_t channel, uint8_t value)
{
    if ((cursor + LPP_DIGITAL_OUTPUT_SIZE) > maxsize) {
        return 0;
    }
    buffer[cursor++] = channel;
    buffer[cursor++] = LPP_DIGITAL_OUTPUT;
    buffer[cursor++] = value;

    return cursor;
}

//-----------------------------------------------------------------------------
/**
 * @brief intert value of type "analog input" into the message
 *
 * @param   channel the number of the channel to use
 * @param   value   the value of the analog input
 *
 * @return  the number of bytes currently used by the inserted values / size of the total message
 */
uint8_t CayenneLPP::addAnalogInput(uint8_t channel, float value)
{
    if ((cursor + LPP_ANALOG_INPUT_SIZE) > maxsize) {
        return 0;
    }

    int16_t val = value * 100;
    buffer[cursor++] = channel;
    buffer[cursor++] = LPP_ANALOG_INPUT;
    buffer[cursor++] = val >> 8;
    buffer[cursor++] = val;

    return cursor;
}

//-----------------------------------------------------------------------------
/**
 * @brief intert value of type "digital output" into the message
 *
 * @param   channel the number of the channel to use
 * @param   value   the value of the digital output
 *
 * @return  the number of bytes currently used by the inserted values / size of the total message
 */
uint8_t CayenneLPP::addAnalogOutput(uint8_t channel, float value)
{
    if ((cursor + LPP_ANALOG_OUTPUT_SIZE) > maxsize) {
        return 0;
    }
    int16_t val = value * 100;
    buffer[cursor++] = channel;
    buffer[cursor++] = LPP_ANALOG_OUTPUT;
    buffer[cursor++] = value;
    buffer[cursor++] = val >> 8;
    buffer[cursor++] = val;

    return cursor;
}

//-----------------------------------------------------------------------------
/**
 * @brief intert value of type "Luminosity" into the message
 *
 * @param   channel the number of the channel to use
 * @param   lux     the value of the luminosity
 *
 * @return  the number of bytes currently used by the inserted values / size of the total message
 */
uint8_t CayenneLPP::addLuminosity(uint8_t channel, uint16_t lux)
{
    if ((cursor + LPP_LUMINOSITY_SIZE) > maxsize) {
        return 0;
    }
    buffer[cursor++] = channel;
    buffer[cursor++] = LPP_LUMINOSITY;
    buffer[cursor++] = lux >> 8;
    buffer[cursor++] = lux;

    return cursor;
}

//-----------------------------------------------------------------------------
/**
 * @brief intert value of type "presence" into the message
 *
 * @param   channel the number of the channel to use
 * @param   value   the value of the presence
 *
 * @return  the number of bytes currently used by the inserted values / size of the total message
 */
uint8_t CayenneLPP::addPresence(uint8_t channel, uint8_t value)
{
    if ((cursor + LPP_PRESENCE_SIZE) > maxsize) {
        return 0;
    }
    buffer[cursor++] = channel;
    buffer[cursor++] = LPP_PRESENCE;
    buffer[cursor++] = value;

    return cursor;
}

//-----------------------------------------------------------------------------
/**
 * @brief intert value of type "temperature" into the message
 *
 * @param   channel the number of the channel to use
 * @param   celsius the value of the temperature
 *
 * @return  the number of bytes currently used by the inserted values / size of the total message
 */
uint8_t CayenneLPP::addTemperature(uint8_t channel, float celsius)
{
    if ((cursor + LPP_TEMPERATURE_SIZE) > maxsize) {
        return 0;
    }
    int16_t val = celsius * 10;
    buffer[cursor++] = channel;
    buffer[cursor++] = LPP_TEMPERATURE;
    buffer[cursor++] = val >> 8;
    buffer[cursor++] = val;

    return cursor;
}

//-----------------------------------------------------------------------------
/**
 * @brief intert value of type "relative humidity" into the message
 *
 * @param   channel the number of the channel to use
 * @param   rh      the value of the relative humidity
 *
 * @return  the number of bytes currently used by the inserted values / size of the total message
 */
uint8_t CayenneLPP::addRelativeHumidity(uint8_t channel, float rh)
{
    if ((cursor + LPP_RELATIVE_HUMIDITY_SIZE) > maxsize) {
        return 0;
    }
    buffer[cursor++] = channel;
    buffer[cursor++] = LPP_RELATIVE_HUMIDITY;
    buffer[cursor++] = rh * 2;

    return cursor;
}

//-----------------------------------------------------------------------------
/**
 * @brief intert value of type "accelerometer" into the message
 *
 * @param   channel the number of the channel to use
 * @param   x      the value of the x-axis
 * @param   y      the value of the y-axis
 * @param   z      the value of the z-axis
 *
 * @return  the number of bytes currently used by the inserted values / size of the total message
 */
uint8_t CayenneLPP::addAccelerometer(uint8_t channel, float x, float y, float z)
{
    if ((cursor + LPP_ACCELEROMETER_SIZE) > maxsize) {
        return 0;
    }
    int16_t vx = x * 1000;
    int16_t vy = y * 1000;
    int16_t vz = z * 1000;

    buffer[cursor++] = channel;
    buffer[cursor++] = LPP_ACCELEROMETER;
    buffer[cursor++] = vx >> 8;
    buffer[cursor++] = vx;
    buffer[cursor++] = vy >> 8;
    buffer[cursor++] = vy;
    buffer[cursor++] = vz >> 8;
    buffer[cursor++] = vz;

    return cursor;
}

//-----------------------------------------------------------------------------
/**
 * @brief intert value of type "barometric pressure" into the message
 *
 * @param   channel the number of the channel to use
 * @param   hpa     the value of the presure
 *
 * @return  the number of bytes currently used by the inserted values / size of the total message
 */
uint8_t CayenneLPP::addBarometricPressure(uint8_t channel, float hpa)
{
    if ((cursor + LPP_BAROMETRIC_PRESSURE_SIZE) > maxsize) {
        return 0;
    }
    int16_t val = hpa * 10;

    buffer[cursor++] = channel;
    buffer[cursor++] = LPP_BAROMETRIC_PRESSURE;
    buffer[cursor++] = val >> 8;
    buffer[cursor++] = val;

    return cursor;
}

//-----------------------------------------------------------------------------
/**
 * @brief intert value of type "gyrometer" into the message
 *
 * @param   channel the number of the channel to use
 * @param   x      the value of the x-axis
 * @param   y      the value of the y-axis
 * @param   z      the value of the z-axis
 *
 * @return  the number of bytes currently used by the inserted values / size of the total message
 */
uint8_t CayenneLPP::addGyrometer(uint8_t channel, float x, float y, float z)
{
    if ((cursor + LPP_GYROMETER_SIZE) > maxsize) {
        return 0;
    }
    int16_t vx = x * 100;
    int16_t vy = y * 100;
    int16_t vz = z * 100;

    buffer[cursor++] = channel;
    buffer[cursor++] = LPP_GYROMETER;
    buffer[cursor++] = vx >> 8;
    buffer[cursor++] = vx;
    buffer[cursor++] = vy >> 8;
    buffer[cursor++] = vy;
    buffer[cursor++] = vz >> 8;
    buffer[cursor++] = vz;

    return cursor;
}

//-----------------------------------------------------------------------------
/**
 * @brief intert value of type "GPS" into the message
 *
 * @param   channel the number of the channel to use
 * @param   latitude    latitude infos
 * @param   longitude   longitude infos
 * @param   meters      hight
 *
 * @return  the number of bytes currently used by the inserted values / size of the total message
 */
uint8_t CayenneLPP::addGPS(uint8_t channel, float latitude, float longitude, float meters)
{
    if ((cursor + LPP_GPS_SIZE) > maxsize) {
        return 0;
    }
    int32_t lat = latitude * 10000;
    int32_t lon = longitude * 10000;
    int32_t alt = meters * 100;

    buffer[cursor++] = channel;
    buffer[cursor++] = LPP_GPS;

    buffer[cursor++] = lat >> 16;
    buffer[cursor++] = lat >> 8;
    buffer[cursor++] = lat;
    buffer[cursor++] = lon >> 16;
    buffer[cursor++] = lon >> 8;
    buffer[cursor++] = lon;
    buffer[cursor++] = alt >> 16;
    buffer[cursor++] = alt >> 8;
    buffer[cursor++] = alt;

    return cursor;
}

//-----------------------------------------------------------------------------
/**
 * @brief function to add a generic value
 *
 * This function can be used to add a value of a "sensor" type that is currently
 * not pre-defines as API function.
 * The type of sensor can be manually defined here
 *
 * @param   channel     the number of the channel to use
 * @param   type        the type of sensor to add
 * @param   valueSize   number of byte of the value to add
 * @param   value       pointer to the sensor value itself
 *
 * @return  the number of bytes currently used by the inserted values / size of the total message
 */

uint8_t CayenneLPP::addCustomValue(uint8_t channel, uint8_t type, uint8_t valueSize, uint8_t* value) {
	uint8_t i;

	if (valueSize < 2) return 0;
    if ((cursor + valueSize) > maxsize) {
        return 0;
    }

    buffer[cursor++] = channel;
    buffer[cursor++] = type;

    for (i=0; i < valueSize-2; i++) {
        buffer[cursor++] = *value++;
    }

    return cursor;
}


//------------------------------------------------------------------------------
//
// Section protected functions
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//
// Section private functions
//
//------------------------------------------------------------------------------


