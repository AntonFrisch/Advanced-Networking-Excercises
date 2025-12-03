//------------------------------------------------------------------------------
//! @file WiMOD_SAP_DEVMGMT_PLUS_IDs.h
//! @ingroup WiMOD_SAP_DEVMGMT_PLUS
//! <!------------------------------------------------------------------------->
//! @brief Supporting IDs and definitions for the DeviceManagement ServiceAccessPoint for LR BASE PLUS firmware
//! @version 0.1
//! <!------------------------------------------------------------------------->
//!
//!
//!
//! <!--------------------------------------------------------------------------
//! Copyright (c) 2019
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


/*
 * THIS IS AN EXAMPLE IMPLEMENTATION ACCORDING THE THE HCI SPEC: V1.2
 * FOR FIRMWARE: LR-BASE PLUS
 *
 * SEE FILE: WiMOD_LR_Base_Plus_HCI_Spec_V1_2.pdf for detailed information
 */


#ifndef ARDUINO_SAP_WIMOD_SAP_DEVMGMT_PLUS_IDS_H_
#define ARDUINO_SAP_WIMOD_SAP_DEVMGMT_PLUS_IDS_H_

//------------------------------------------------------------------------------
//
// Section Includes Files
//
//------------------------------------------------------------------------------

#include "WiMOD_SAP_DEVMGMT_IDs.h"


//! @cond Doxygen_Suppress


// bit field for Misc.Options in RadioConfig structure (additonal for LR-Base_PLUS)
#define DEVMGMT_RADIO_CFG_MISC_REMOTE_CTRL_ENABLED          (1 << 6)

// bit field for WRONG_PARAMETER bit mask of the DEVMGMT_SET_RADIO_CONFIG_RSP messasge

// byte 0
#define RADIO_CFG_BYTE0_WRONG_MODULATION          (1 << 0)
#define RADIO_CFG_BYTE0_WRONG_FREQ                (1 << 1)
#define RADIO_CFG_BYTE0_WRONG_BANDWIDTH           (1 << 2)
#define RADIO_CFG_BYTE0_WRONG_SF                  (1 << 3)
#define RADIO_CFG_BYTE0_WRONG_ERROR_CODE          (1 << 4)
#define RADIO_CFG_BYTE0_WRONG_POWER_LEVEL         (1 << 5)

// byte1
// -> reserved

// byte2
#define RADIO_CFG_BYTE2_WRONG_RADIO_MODE          (1 << 0)
#define RADIO_CFG_BYTE2_WRONG_RX_OPTIONS          (1 << 1)
#define RADIO_CFG_BYTE2_WRONG_LBT_THRESHOLD       (1 << 2)
#define RADIO_CFG_BYTE2_WRONG_GRP_ADR             (1 << 3)
#define RADIO_CFG_BYTE2_WRONG_DEV_ADR             (1 << 4)
#define RADIO_CFG_BYTE2_WRONG_PWR_SAVE_MODE       (1 << 5)

// byte3
// -> reserved

//! @endcond



/**
 * @brief general operation mode of the radio part (only for LR-BASE_PLUS)
 */
typedef enum TRadioCfg_RadioModePlus
{
	LRBASE_PLUS_RadioMode_Standard = 0,                                         /*!< normal operation mode */
	LRBASE_PLUS_RadioMode_Reserved,                                             /*!< DO NOT USE */
	LRBASE_PLUS_RadioMode_Sniffer,                                              /*!< DO NOT USE */
} TRadioCfg_RadioModePlus;

/**
 * @brief Modulation technique supported by the WiMOD (only for LR-BASE_PLUS)
 */
typedef enum TRadioCfg_ModulationPlus
{
    LRBASE_PLUS_Modulation_LoRa = 0,                                            /*!< LoRa modulation */
    LRBASE_PLUS_Modulation_FLRC,                                                /*!< FLRC modulation  */
    LRBASE_PLUS_Modulation_FSK,                                                 /*!< FSK modulation  */

    //! @cond Doxygen_Suppress
    LRBASE_PLUS_Modulation_Fail,  // dummy
    //! @endcond
} TRadioCfg_ModulationPlus;


/**
 * @brief Bandwidth settings for the LoRa modulation (only for LR-BASE_PLUS)
 */
typedef enum TRadioCfg_LoRaBandwidthPlus
{
	LRBASE_PLUS_LoRaBandwith_200kHz  = 2,                                       /*!< 200 kHz */
	LRBASE_PLUS_LoRaBandwith_400kHz  = 3,                                       /*!< 400 kHz */
	LRBASE_PLUS_LoRaBandwith_800kHz  = 4,                                       /*!< 800 kHz */
	LRBASE_PLUS_LoRaBandwith_1600kHz = 5,                                       /*!< 1600 kHz */

    //! @cond Doxygen_Suppress
	LRBASE_PLUS_LoRaBandwith_fail,                                              // dummy
    //! @endcond

} TRadioCfg_LoRaBandwidthPlus;

/**
 * @brief Bandwidth settings for the LoRa modulation (only for LR-BASE_PLUS)
 */
typedef enum TRadioCfg_FLRCBandwidthPlus
{
	LRBASE_PLUS_FLRCBandwith_0_260MBs_0_3_MHz_DSB  = 1,                         /*!< 260 kbps */
	LRBASE_PLUS_FLRCBandwith_0_325MBs_0_3_MHz_DSB  = 2,                         /*!< 325 kbps */
	LRBASE_PLUS_FLRCBandwith_0_520MBs_0_6_MHz_DSB  = 3,                         /*!< 520 kbps */
	LRBASE_PLUS_FLRCBandwith_0_650MBs_0_6_MHz_DSB  = 4,                         /*!< 625 kbps */
	LRBASE_PLUS_FLRCBandwith_1_040MBs_1_2_MHz_DSB  = 5,                         /*!< 1040 kbps */
	LRBASE_PLUS_FLRCBandwith_1_300MBs_1_2_MHz_DSB  = 6,                         /*!< 1300 kbps */
	//! @cond Doxygen_Suppress
	LRBASE_PLUS_FLRCBandwith_fail,                                              // dummy
    //! @endcond
} TRadioCfg_FLRCBandwidthPlus;

/**
 * @brief Bitrate/Bandwidth settings for the FSK modulation (only for LR-BASE_PLUS)
 */
typedef enum TRadioCfg_FSKBandwidthPlus
{
    LRBASE_PLUS_FSKBandwith_2_0MBs_2_4_MHz   = 0,                               /*!< 2,000 MB/s 2,4 MHz */
    LRBASE_PLUS_FSKBandwith_1_0MBs_1_2_MHz   = 3,                               /*!< 1,000 MB/s 1,2 MHz */
    LRBASE_PLUS_FSKBandwith_0_250MBs_0_3_MHz = 11,                              /*!< 0,250 MB/s 0,3 MHz */
    LRBASE_PLUS_FSKBandwith_0_125MBs_0_3_MHz = 12,                              /*!< 0,125 MB/s 0,3 MHz */
    //! @cond Doxygen_Suppress
    LRBASE_PLUS_FSKBandwith_fail,                                               // dummy
    //! @endcond
} TRadioCfg_FSKBandwidthPlus;

/**
 * @brief SpreadingFactors for LoRa modulation (only for LR-BASE_PLUS)
 */
typedef enum TRadioCfg_LoRaSpreadingFactorPlus
{
	LRBASE_PLUS_LoRa_SF5  = 5,                                                  /*!< Spreading Factor 5 */
	LRBASE_PLUS_LoRa_SF6  = 6,                                                  /*!< Spreading Factor 6 */
	LRBASE_PLUS_LoRa_SF7  = 7,                                                  /*!< Spreading Factor 7 */
	LRBASE_PLUS_LoRa_SF8  = 8,                                                  /*!< Spreading Factor 8 */
	LRBASE_PLUS_LoRa_SF9  = 9,                                                  /*!< Spreading Factor 9 */
	LRBASE_PLUS_LoRa_SF10 = 10,                                                 /*!< Spreading Factor 10 */
	LRBASE_PLUS_LoRa_SF11 = 11,                                                 /*!< Spreading Factor 11 */
	LRBASE_PLUS_LoRa_SF12 = 12,                                                 /*!< Spreading Factor 12 */

	//! @cond Doxygen_Suppress
	LRBASE_PLUS_LoRa_SF_fail,                                                   // dummy
    //! @endcond
} TRadioCfg_LoRaSpreadingFactorPlus;


/**
 * @brief ErrorCoding settings (for LoRa modulation) (only for LR-BASE_PLUS)
 */
typedef enum TRadioCfg_LoRaErrorCodingPlus
{
	LRBASE_PLUS_LoRa_ErrorCoding_4_5 = 1,                                       /*!< ErrorCoding 4/5 */
	LRBASE_PLUS_LoRa_ErrorCoding_4_6 = 2,                                       /*!< ErrorCoding 4/6 */
	LRBASE_PLUS_LoRa_ErrorCoding_4_7 = 3,                                       /*!< ErrorCoding 4/7 */
	LRBASE_PLUS_LoRa_ErrorCoding_4_8 = 4,                                       /*!< ErrorCoding 4/8 */
	// new in V 1.2
    LRBASE_PLUS_LoRa_ErrorCoding_LI_4_5 = 5,                                    /*!< ErrorCoding LongInterleave 4/5 */
    LRBASE_PLUS_LoRa_ErrorCoding_LI_4_6 = 6,                                    /*!< ErrorCoding LongInterleave 4/6 */
    LRBASE_PLUS_LoRa_ErrorCoding_LI_4_8 = 7,                                    /*!< ErrorCoding LongInterleave 4/8 */

    //! @cond Doxygen_Suppress
    LRBASE_PLUS_LoRa_ErrorCoding_fail,                                          // dummy
    //! @endcond

} TRadioCfg_LoRaErrorCodingPlus;

/**
 * @brief ErrorCoding settings (for FLRC modulation) (only for LR-BASE_PLUS)
 */
typedef enum TRadioCfg_FLRCErrorCodingPlus
{
	LRBASE_PLUS_FLRC_ErrorCoding_1_2 = 1,                                       /*!< ErrorCoding 1/2 */
	LRBASE_PLUS_FLRC_ErrorCoding_3_4 = 2,                                       /*!< ErrorCoding 3/4 */
	LRBASE_PLUS_FLRC_ErrorCoding_1_1 = 3,                                       /*!< ErrorCoding 1/1 */

    //! @cond Doxygen_Suppress
	LRBASE_PLUS_FLRC_ErrorCoding_fail,                                          // dummy
    //! @endcond
} TRadioCfg_FLRCErrorCodingPlus;


/**
 * @brief TxPowerLevel settings (only for LR-BASE_PLUS)
 */
typedef enum TRadioCdf_TxPowerLevelPlus
{
	LRBASE_PLUS_TxPowerLevel_m18_dBm = -18,                                     /*!< -18 dBm */
	LRBASE_PLUS_TxPowerLevel_m17_dBm,                                           /*!< -17 dBm */
	LRBASE_PLUS_TxPowerLevel_m16_dBm,                                           /*!< -16 dBm */
	LRBASE_PLUS_TxPowerLevel_m15_dBm,                                           /*!< -15 dBm */
	LRBASE_PLUS_TxPowerLevel_m14_dBm,                                           /*!< -14 dBm */
	LRBASE_PLUS_TxPowerLevel_m13_dBm,                                           /*!< -13 dBm */
	LRBASE_PLUS_TxPowerLevel_m12_dBm,                                           /*!< -12 dBm */
	LRBASE_PLUS_TxPowerLevel_m11_dBm,                                           /*!< -11 dBm */
	LRBASE_PLUS_TxPowerLevel_m10_dBm,                                           /*!< -10 dBm */
	LRBASE_PLUS_TxPowerLevel_m09_dBm,                                           /*!< -9 dBm */
	LRBASE_PLUS_TxPowerLevel_m08_dBm,                                           /*!< -8 dBm */
	LRBASE_PLUS_TxPowerLevel_m07_dBm,                                           /*!< -7 dBm */
	LRBASE_PLUS_TxPowerLevel_m06_dBm,                                           /*!< -6 dBm */
	LRBASE_PLUS_TxPowerLevel_m05_dBm,                                           /*!< -5 dBm */
	LRBASE_PLUS_TxPowerLevel_m04_dBm,                                           /*!< -4 dBm */
	LRBASE_PLUS_TxPowerLevel_m03_dBm,                                           /*!< -3 dBm */
	LRBASE_PLUS_TxPowerLevel_m02_dBm,                                           /*!< -2 dBm */
	LRBASE_PLUS_TxPowerLevel_m01_dBm,                                           /*!< -1 dBm */
	LRBASE_PLUS_TxPowerLevel_m00_dBm,                                           /*!< 0 dBm */
	LRBASE_PLUS_TxPowerLevel_p01_dBm,                                           /*!< 1 dBm */
	LRBASE_PLUS_TxPowerLevel_p02_dBm,                                           /*!< 2 dBm */
	LRBASE_PLUS_TxPowerLevel_p03_dBm,                                           /*!< 3 dBm */
	LRBASE_PLUS_TxPowerLevel_p04_dBm,                                           /*!< 4 dBm */
	LRBASE_PLUS_TxPowerLevel_p05_dBm,                                           /*!< 5 dBm */
	LRBASE_PLUS_TxPowerLevel_p06_dBm,                                           /*!< 6 dBm */
	LRBASE_PLUS_TxPowerLevel_p07_dBm,                                           /*!< 7 dBm */
	LRBASE_PLUS_TxPowerLevel_p08_dBm,                                           /*!< 8 dBm */
	LRBASE_PLUS_TxPowerLevel_p09_dBm,                                           /*!< 9 dBm */
	LRBASE_PLUS_TxPowerLevel_p10_dBm,                                           /*!< 10 dBm */
	LRBASE_PLUS_TxPowerLevel_p11_dBm,                                           /*!< 11 dBm */
	LRBASE_PLUS_TxPowerLevel_p12_dBm,                                           /*!< 12 dBm */
	LRBASE_PLUS_TxPowerLevel_p13_dBm,                                           /*!< 13 dBm */

	//! @cond Doxygen_Suppress
	LRBASE_PLUS_TxPowerLevel_fail,                                              // dummy
	//! @endcond

} TRadioCfg_PowerLevelPlus;


/**
 * @brief RX Operation modes (only for LR-BASE_PLUS)
 */
typedef enum TRadioCfg_RxControlPlus
{
	LRBASE_PLUS_RxCtrl_Receiver_Off = 0,                                        /*!< Disable the RX operation completely */
	LRBASE_PLUS_RxCtrl_Receiver_AlwaysOn,                                       /*!< RX operation is always ON*/
	LRBASE_PLUS_RxCtrl_Receiver_RxWindowed,                                     /*!< RX operation is only used in defined time windows */
} TRadioCfg_RxControlPlus;


/**
 * @brief TX Operation modes (only for LR-BASE_PLUS)
 */
typedef enum TRadioCfg_TxControlPlus
{
    LRBASE_PLUS_TxCtrl_LBT_Off = 0,                                             /*!< Disable LBT mode */
    LRBASE_PLUS_TxCtrl_LBT_On = 1,                                              /*!< Enable LBT mode: Tx will only be done with repsect to LBT thresholds*/
} TRadioCfg_TxControlPlus;

/**
 * @brief Settings general PowerSaving mode (aka. Sleep mode) (only for LR-BASE_PLUS)
 */
typedef enum TRadioCfg_PowerSavingModePlus
{
	LRBASE_PLUS_PowerSaving_Off = 0,                                            /*!< Entering sleep mode is disabled */
	LRBASE_PLUS_PowerSaving_On  = 1,                                            /*!< Entering sleep mode is allowed; (may influence LED and radio RX settings */
} TRadioCfg_PowerSavingModePlus;


/**
 * @brief Structure containing the radio (stack) config parameters of the WiMOD (only for LR-BASE)
 */
typedef struct TWiMODLR_DevMgmt_RadioConfigPlus
{
    // only for reading
    UINT8                           Status;                                     /*!< Status Flag of HCI get operation; only vaild if config has been requested*/

    // only for writing
    UINT8                           StoreNwmFlag;                               /*!< Store the config in NVM or RAM; 1 = store in NVM*/

    TRadioCfg_RadioModePlus         RadioMode;                                  /*!< Radio Mode setting */
    UINT8                           GroupAddress;                               /*!< Group address of WiMOD (for RX)*/
    UINT8                           TxGroupAddress;                             /*!< Group address to be used for TX*/
    UINT16                          DeviceAddress;                              /*!< RF device address used for RX*/
    UINT16                          TxDeviceAddress;                            /*!< RF device address used for TX*/
    TRadioCfg_ModulationPlus        Modulation;                                 /*!< RF Modulation to be used for TX*/
    UINT8                           RfFreq_LSB;                                 /*!< lower part of the 24bit frequency register of the transceiver*/
    UINT8                           RfFreq_MID;                                 /*!< mid part of the 24bit frequency register of the transceiver*/
    UINT8                           RfFreq_MSB;                                 /*!< high part of the 24bit frequency register of the transceiver*/
    TRadioCfg_LoRaBandwidthPlus     LoRaBandWidth;                              /*!< Bandwidth setting for the LoRa modulation */
    TRadioCfg_FLRCBandwidthPlus		FLRCBandWidth;								/*!< Bandwidth setting for the FLRC modulation */
    TRadioCfg_FSKBandwidthPlus      FSKBandWidth;                               /*!< Bandwidth setting for the FSK modulation */
    TRadioCfg_LoRaSpreadingFactorPlus   LoRaSpreadingFactor;                    /*!< LoRa Spreading factor*/
    TRadioCfg_LoRaErrorCodingPlus   LoRaErrorCoding;                            /*!< LoRa Error Coding */
    TRadioCfg_FLRCErrorCodingPlus   FLRCErrorCoding;                            /*!< FLRC Error Coding */
    TRadioCfg_PowerLevelPlus        PowerLevel;                                 /*!< Power Level setting*/
    TRadioCfg_TxControlPlus         TxControl;                                  /*!< Options related to TX operations*/
    TRadioCfg_RxControlPlus         RxControl;                                  /*!< Options related to RX operations*/
    UINT16                          RxWindowTime;                               /*!< Time settings related to the RX Window */
    UINT8                           LedControl;                                 /*!< control byte for the LED (of the demoboard)*/
    UINT8                           MiscOptions;                                /*!< misc options field*/
    TRadioCfg_PowerSavingModePlus   PowerSavingMode;                            /*!< PowerSaving; 1 = enable/ on; 0 = disable / off*/
    INT16                           LbtThreshold;                               /*!< ListenBeforeTalk threshold value*/
} TWiMODLR_DevMgmt_RadioConfigPlus;




/**
 * @brief Basic information about system status of the WiMOD
 */
typedef struct TWiMODLR_DevMgmt_SystemStatusPlus
{
    UINT8       Status;                                                         /*!< status flag; indicates if other values are vaild */
    UINT8       SysTickResolution;                                              /*!< systick interval time in ms */
    UINT32      SysTickCounter;                                                 /*!< virtual systick counter value */
    UINT32      RtcTime;                                                        /*!< 32bit encoded current RTC timestamp (@see WIMOD_RTC_* macros)*/
    UINT16      NvmStatus;                                                      /*!< Status of the internal NVM slots */
    UINT16      BatteryStatus;                                                  /*!< current voltage level of power supply in mV */
    UINT16      ExtraStatus;                                                    /*!< extra status bits */
    UINT32 		RxPackets;														/*!< number of received radio packets with CRC OK */
	UINT32 		RxAddressMatch;													/*!< number of received radio packets with CRC and Address OK */
	UINT32 		RxCRCError;														/*!< number of received radio packets with CRC Error */
	UINT32 		TxPackets;														/*!< number of transmitted radio packets */
	UINT32 		TxError;														/*!< number of not transmitted radio packets */
    UINT32      TxMediaBusyEvents;                                              /*!< number of not transmitted packets due to LBT result “media busy” */
} TWiMODLR_DevMgmt_SystemStatusPlus;



#endif /* ARDUINO_SAP_WIMOD_SAP_DEVMGMT_PLUS_IDS_H_ */
