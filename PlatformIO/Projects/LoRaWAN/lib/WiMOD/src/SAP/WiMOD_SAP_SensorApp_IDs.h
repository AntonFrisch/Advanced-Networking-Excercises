//------------------------------------------------------------------------------
//! @file WiMOD_SAP_SensorApp_IDs.h
//! @ingroup WiMOD_SAP_SensorApp
//! <!------------------------------------------------------------------------->
//! @brief Supporting IDs and definitions for the SensorApplication ServiceAccessPoint
//! @version 0.1
//! <!------------------------------------------------------------------------->
//!
//!
//!
//! <!--------------------------------------------------------------------------
//! Copyright (c) 2018
//! IMST GmbH
//! Carl-Friedrich Gauss Str. 2
//! 47475 Kamp-Lintfort
//! --------------------------------------------------------------------------->
//! @author (FB), IMST
//! <!--------------------------------------------------------------------------
//! Target OS:    none
//! Target CPU:   tbd
//! Compiler:     tbd
//! --------------------------------------------------------------------------->
//! @internal
//! @par Revision History:
//! <PRE>
//!-----------------------------------------------------------------------------
//! Version | Date       | Author | Comment
//!-----------------------------------------------------------------------------
//!
//! </PRE>
//------------------------------------------------------------------------------


#ifndef ARDUINO_SAP_WIMOD_SAP_SENSORAPP_IDS_H_
#define ARDUINO_SAP_WIMOD_SAP_SENSORAPP_IDS_H_

//------------------------------------------------------------------------------
//
// Section Includes Files
//
//------------------------------------------------------------------------------

#include "utils/WMDefs.h"

//------------------------------------------------------------------------------
//
// Service Access Point Identifier
//
//------------------------------------------------------------------------------

/** SAP ID for the RadioLink service */
#define SENSORAPP_SAP_ID                      0x05

//------------------------------------------------------------------------------
//
//  Sensor App Services Identifier
//
//------------------------------------------------------------------------------
//! @cond Doxygen_Suppress

#define SENSORAPP_MSG_SET_CONFIG_REQ	0x09
#define SENSORAPP_MSG_SET_CONFIG_RSP	0x0A
#define SENSORAPP_MSG_GET_CONFIG_REQ	0x0B
#define SENSORAPP_MSG_GET_CONFIG_RSP	0x0C
#define SENSORAPP_MSG_SEND_DATA_IND		0x06
#define SENSORAPP_MSG_ACK_IND			0x08
//! @endcond


//------------------------------------------------------------------------------
//
//  SensorApp Status Codes
//
//------------------------------------------------------------------------------

#define SENSORAPP_STATUS_OK  	                        0x00                    /*!< Ok, no error */
#define SENSORAPP_STATUS_ERROR                          0x01                    /*!< generic error */
#define SENSORAPP_STATUS_WRONG_DEVICEMODE          		0x04                    /*!< wrong device mode */

// extended HCI format for optional fiels in the SensorApp
#define SENSORAPP_FORMAT_EXT_HCI_OUT_ACTIVE				0x01                    /*!< extended HCI output format*/

//------------------------------------------------------------------------------
//
//  Options
//
//------------------------------------------------------------------------------
#define SENSORAPP_OPTION_SENSOR_ACK_MSG					BV(0)                   /*!< Option: Ack */
#define SENSORAPP_OPTION_ACK_MSG_HCI_OUTPUT				BV(1)                   /*!< send Ack */
#define SENSORAPP_OPTION_LINK_MON_SIGNAL_OUTPUT			BV(2)                   /*!< use Link Monitor feature */
#define SENSORAPP_OPTION_PUSH_BUTTON_ONE				BV(3)                   /*!< Push Button1 pressed (remoteDevice) */


#define SENSORAPP_DIGITAL_INPUT_ONE_SET				    BV(0)                   /*!< PushButton1 (of DemoBoard) */
#define SENSORAPP_DIGITAL_INPUT_TWO_SET				    BV(1)                   /*!< PushButton2 (of DemoBoard) */
#define SENSORAPP_DIGITAL_INPUT_THREE_SET				BV(2)                   /*!< PushButton3 (of DemoBoard) */
//#define SENSORAPP_DIGITAL_INPUT_DIP_SW1_SET				BV(3)                   /*!< DipSwitch1 (of DemoBoard) */
#define SENSORAPP_ANALOG_INPUT_ONE_SET                  BV(3)                   /*!< DipSwitch1 (of DemoBoard) */


//------------------------------------------------------------------------------
//
// structures
//
//------------------------------------------------------------------------------


/**
 * @brief SensorApp Modes
 */
typedef enum TSensorApp_Mode {
	SensorApp_Mode_Off                     = 0,									/*!< SensorApp of off / de-activated */
	SensorApp_Mode_SensorDataTransmitter   = 1,									/*!< SensorApp is avtive and acts as Transmitter device */
	SensorApp_Mode_SensorDataReceiver      = 2,									/*!< SensorApp is avtive and acts as Receiver device */
} TSensorApp_Mode;


/**
 * @brief Structure containing a general RadioLink HCI message
 */
typedef struct TWiMODLR_SensorApp_Config
{
    // only for RX
    UINT8       	Status;                                                     /*!< Status/Format Field set by WiMOD; (WiMOD --> Host only) */

    TSensorApp_Mode	Mode;														/*!< Mode of operation */
    UINT8			Options;													/*!< (activated) Options; @see SENSORAPP_OPTION_* defines */
    UINT32			SendingPeriod;												/*!< Tx  Sensor Data Period in ms */
    UINT32			LinkTimeout;												/*!< Rx  Link Timeout in ms */
} TWiMODLR_SensorApp_Config;



/**
 * @brief Structure containing  SensorApp Data information
 */
typedef struct TWiMODLR_SensorApp_SensorData
{
    // only for RX
    UINT8       	StatusFormat;                                               /*!< Status/Format Field set by WiMOD; (WiMOD --> Host only) */
    UINT8			DestGroupAddress;											/*!< destination group address */
    UINT16			DestDevAddress;												/*!< destination device address */
    UINT8			SourceGroupAddress;											/*!< group address of message sender */
    UINT16			SourceDevAddress;											/*!< device address of message sender */

    UINT16			Voltage;													/*!< Sensor Values*/
    UINT16			AdcValue;													/*!< Sensor Values*/
    UINT8			Temperature;												/*!< Sensor Values*/
    UINT8			DigitalInputs;												/*!< Sensor Values; @see SENSORAPP_DIGITAL_INPUT* defines*/

	//optional
    INT16			RSSI;														/*!< Received Signal Strength Indicator [dBm], signed integer*/
    INT8			SNR;														/*!< Signal to Noise Ratio [dB], signed integer*/
    UINT32			RxTime;														/*!< timestamp from RTC */

    // not part of the HCI message
    bool 			OptionalInfoAvaiable;                                       /*!< internal flag indicating of optional fields are present / valid */

} TWiMODLR_SensorApp_SensorData;

/**
 * @brief Structure containing the data of an Ack of the SensorApp
 */
typedef struct TWiMODLR_SensorApp_AckSensorData
{
    // only for RX
    UINT8       	StatusFormat;                                               /*!< Status/Format Field set by WiMOD; (WiMOD --> Host only) */
    UINT8			DestGroupAddress;											/*!< destination group address */
    UINT16			DestDevAddress;												/*!< destination device address */
    UINT8			SourceGroupAddress;											/*!< group address of message sender */
    UINT16			SourceDevAddress;											/*!< device address of message sender */

    UINT8			DigitalInputs;												/*!< Sensor Values; @see SENSORAPP_DIGITAL_INPUT* defines*/

	//optional
    INT16			RSSI;														/*!< Received Signal Strength Indicator [dBm], signed integer*/
    INT8			SNR;														/*!< Signal to Noise Ratio [dB], signed integer*/
    UINT32			RxTime;														/*!< timestamp from RTC */

    // not part of the HCI message
    bool 			OptionalInfoAvaiable;                                       /*!< internal flag indicating of optional fields are present / valid */

} TWiMODLR_SensorApp_AckSensorData;


#endif /* ARDUINO_SAP_WIMOD_SAP_SENSORAPP_IDS_H_ */
