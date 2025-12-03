//------------------------------------------------------------------------------
//! @file WiMOD_SAP_RemoteCtrl_IDs.h
//! @ingroup WiMOD_SAP_RemoteCtrl
//! <!------------------------------------------------------------------------->
//! @brief Supporting IDs and definitions for the DeviceManagement ServiceAccessPoint
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


#ifndef ARDUINO_SAP_WIMOD_SAP_REMOTECTRL_IDS_H_
#define ARDUINO_SAP_WIMOD_SAP_REMOTECTRL_IDS_H_

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

/** SAP ID for the RemoteCtrl service */
//! @cond Doxygen_Suppress
#define REMOTECTRL_SAP_ID                      0x04
//! @endcond



//------------------------------------------------------------------------------
//
//  Radio Link Services Identifier
//
//------------------------------------------------------------------------------
//! @cond Doxygen_Suppress
#define REMOTE_CTRL_MSG_BUTTON_PRESSED_ID               0x02
//! @endcond


//------------------------------------------------------------------------------
//
//  Misc. Defines
//
//------------------------------------------------------------------------------
#define REMOTE_CTRL_BUTTON_ONE						BV(0)                       /*!< Button1 ID (bitmap) */
#define REMOTE_CTRL_BUTTON_TWO						BV(1)                       /*!< Button2 ID (bitmap) */
#define REMOTE_CTRL_BUTTON_THREE					BV(2)                       /*!< Button3 ID (bitmap) */

//------------------------------------------------------------------------------
//
// structures
//
//------------------------------------------------------------------------------

/**
 * @brief Structure containing a general RadioLink HCI message
 */
typedef struct TWiMODLR_RemoteCtrl_BtnPressed
{
    // only for RX
    UINT8       StatusFormat;                                                   /*!< Status/Format Field set by WiMOD; (WiMOD --> Host only) */

    UINT8       DestinationGroupAddress;                                        /*!< Address of destination group this message is targeting to  */
    UINT16      DestinationDeviceAddress;                                       /*!< Address of destination device this message is targeting to */

    UINT8       SourceGroupAddress;                                             /*!< Address of source group this message was send from */
    UINT16      SourceDeviceAddress;                                            /*!< Address of source device this message was send from */

    UINT8       ButtonBitmap;                                                   /*!< button state bitmap */
} TWiMODLR_RemoteCtrl_BtnPressed;



#endif /* ARDUINO_SAP_WIMOD_SAP_REMOTECTRL_IDS_H_ */
