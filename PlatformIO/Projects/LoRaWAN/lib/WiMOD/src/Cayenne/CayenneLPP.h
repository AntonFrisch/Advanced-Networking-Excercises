/*
 * CayenneLPP.h
 *
 *  Created on: Feb 10, 2017
 *      Author: FB
 */

//------------------------------------------------------------------------------
//! @file CayenneLPP.h
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


#ifndef ARDUINO_CAYENNE_CAYENNELPP_H_
#define ARDUINO_CAYENNE_CAYENNELPP_H_

//------------------------------------------------------------------------------
//
// Section Includes Files
//
//------------------------------------------------------------------------------

#include <stdint.h>



// source: https://mdswp-staging.mydevices.com/cayenne/docs/#lora

/**
 * @brief A simple Cayenne LPP message builder class
 *
 * @see <a href="https://mdswp-staging.mydevices.com/cayenne/docs/#lora">Mydevices documentation</a>
 */
class CayenneLPP {
    public:
        CayenneLPP(uint8_t* buf, uint8_t size);
        ~CayenneLPP();

        void reset(void);
        uint8_t getSize(void);
        uint8_t* getBuffer(void);
        uint8_t copy(uint8_t* buffer);

        uint8_t addDigitalInput(uint8_t channel, uint8_t value);
        uint8_t addDigitalOutput(uint8_t channel, uint8_t value);

        uint8_t addAnalogInput(uint8_t channel, float value);
        uint8_t addAnalogOutput(uint8_t channel, float value);

        uint8_t addLuminosity(uint8_t channel, uint16_t lux);
        uint8_t addPresence(uint8_t channel, uint8_t value);
        uint8_t addTemperature(uint8_t channel, float celsius);
        uint8_t addRelativeHumidity(uint8_t channel, float rh);
        uint8_t addAccelerometer(uint8_t channel, float x, float y, float z);
        uint8_t addBarometricPressure(uint8_t channel, float hpa);
        uint8_t addGyrometer(uint8_t channel, float x, float y, float z);
        uint8_t addGPS(uint8_t channel, float latitude, float longitude, float meters);

        uint8_t addCustomValue(uint8_t channel, uint8_t type, uint8_t valueSize, uint8_t* value);
    private:
        uint8_t *buffer;
        uint8_t maxsize;
        uint8_t cursor;
};

#endif /* ARDUINO_CAYENNE_CAYENNELPP_H_ */
