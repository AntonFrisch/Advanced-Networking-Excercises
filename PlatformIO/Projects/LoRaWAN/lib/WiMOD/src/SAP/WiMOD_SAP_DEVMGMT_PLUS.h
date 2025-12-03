//------------------------------------------------------------------------------
//! @file WiMOD_SAP_DEVMGMT_PLUS.h
//! @ingroup WiMOD_SAP_DEVMGMT_PLUS
//! <!------------------------------------------------------------------------->
//! @brief Declarations for the DeviceManagement SericeAccessPoint for LR BASE PLUS firmware
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


#ifndef ARDUINO_SAP_WIMOD_SAP_DEVMGMT_PLUS_H_
#define ARDUINO_SAP_WIMOD_SAP_DEVMGMT_PLUS_H_

//------------------------------------------------------------------------------
//
// Section Includes Files
//
//------------------------------------------------------------------------------

#include "WiMOD_SAP_DEVMGMT.h"
#include "WiMOD_SAP_DEVMGMT_PLUS_IDs.h"
//------------------------------------------------------------------------------
//
// Section class
//
//------------------------------------------------------------------------------

/**
 * @brief Implementation for the ServiceAccessPoint DeviceManagement
 */
class WiMOD_SAP_DevMgmt_Plus : public WiMOD_SAP_DevMgmt
{
public:
	WiMOD_SAP_DevMgmt_Plus(TWiMODLRHCI* hci, UINT8* buffer, UINT16 bufferSize);
    virtual ~WiMOD_SAP_DevMgmt_Plus(void);

    virtual TWiMODLRResultCodes GetRadioConfig(TWiMODLR_DevMgmt_RadioConfig* radioCfg, UINT8* statusRsp);
    virtual TWiMODLRResultCodes GetRadioConfig(TWiMODLR_DevMgmt_RadioConfigPlus* radioCfg, UINT8* statusRsp);

    virtual TWiMODLRResultCodes SetRadioConfig(const TWiMODLR_DevMgmt_RadioConfig* radioCfg, UINT8* statusRsp);
    virtual TWiMODLRResultCodes SetRadioConfig(const TWiMODLR_DevMgmt_RadioConfigPlus* radioCfg, UINT8* statusRsp,  UINT8 wrgParamBM[] = NULL);

    virtual TWiMODLRResultCodes GetSystemStatus(TWiMODLR_DevMgmt_SystemStatus* info, UINT8* statusRsp);
    virtual TWiMODLRResultCodes GetSystemStatus(TWiMODLR_DevMgmt_SystemStatusPlus* info, UINT8* statusRsp);

protected:
    virtual TWiMODLRResultCodes unsupportedCmd(UINT8* statusRsp);

private:
};


#endif /* ARDUINO_SAP_WIMOD_SAP_DEVMGMT_PLUS_H_ */
