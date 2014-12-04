/** @file crc.c
*   @brief CRC Driver Source File
*   @date 9.Sep.2014
*   @version 04.01.00
*
*   This file contains:
*   - API Functions
*   - Interrupt Handlers
*   .
*   which are relevant for the CRC driver.
*/

/* (c) Texas Instruments 2009-2014, All rights reserved. */


/* USER CODE BEGIN (0) */
/* USER CODE END */

#include "crc.h"
#include "sys_vim.h"
/* USER CODE BEGIN (1) */
/* USER CODE END */

/** @fn void crcInit(void)
*   @brief Initializes the crc Driver
*
*   This function initializes the crc module.
*/
/* SourceId : CRC_SourceId_001 */
/* DesignId : CRC_DesignId_001 */
/* Requirements: HL_SR107 */
void crcInit(void)
{
/* USER CODE BEGIN (2) */
/* USER CODE END */
    /** @b initialize @b CRC */

    /** - Reset  PSA*/
    crcREG->CTRL0 =   (uint32)((uint32)1U << 0U)
                    | (uint32)((uint32)1U << 8U);

    /** - Pulling PSA out of reset */
    crcREG->CTRL0=0x00000000U;
	
   /** - Setup the Data trace for channel1 */
    crcREG->CTRL2 |= (uint32)0U << 4U;

   /** - Setup the Channel mode */
    crcREG->CTRL2 |= (uint32)(CRC_FULL_CPU)  | (uint32)((uint32)CRC_FULL_CPU << 8U);

/* USER CODE BEGIN (3) */
/* USER CODE END */
}


/** @fn void crcSendPowerDown(crcBASE_t *crc)
*   @brief Send crc power down
*   @param[in] crc - crc module base address
*
*   Send crc power down signal to enter into sleep mode
*/
/* SourceId : CRC_SourceId_002 */
/* DesignId : CRC_DesignId_002 */
/* Requirements: HL_SR108 */
void crcSendPowerDown(crcBASE_t *crc)
{
/* USER CODE BEGIN (4) */
/* USER CODE END */

    crc->CTRL1 |= 0x00000001U;

/* USER CODE BEGIN (5) */
/* USER CODE END */
}

/** @fn void crcSignGen(crcBASE_t *crc,crcModConfig_t *param)
*   @brief set the mode specific parameters for signature generation
*   @param[in] crc - crc module base address
*   @param[in] param - structure holding mode specific parameters
*   Generate CRC signature. This device supports only FULL CPU mode of operation.
*/
/* SourceId : CRC_SourceId_003 */
/* DesignId : CRC_DesignId_003 */
/* Requirements: HL_SR109 */
void crcSignGen(crcBASE_t *crc,crcModConfig_t *param)
{
/* USER CODE BEGIN (6) */
/* USER CODE END */
    uint32 i = 0U, psaSigx;
	volatile uint64 * ptr64, * psaSigx_ptr64;
    ptr64=param->src_data_pat;
	
	/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are only allowed in this driver" */
	/*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Pointer Manupulation required to find offset - Advisory as per MISRA" */
	psaSigx = (uint32)(&crc->PSA_SIGREGL1) + ((uint32)(param->crc_channel) * 0x40U);
	psaSigx_ptr64 = (uint64 *) (psaSigx);

	if(param->mode==CRC_FULL_CPU)
    {
        for(i=0U;i<param->data_length;i++)
        {
		    /*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are only allowed in this driver" */
            *psaSigx_ptr64 = *ptr64;
			/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are only allowed in this driver" */
			/*SAFETYMCUSW 567 S MR:17.1,17.4 <APPROVED> "Pointer increment needed" */
			ptr64++;
        }
/* USER CODE BEGIN (7) */
/* USER CODE END */
    }
    else
	{
        /* Empty */
    }
/* USER CODE BEGIN (8) */
/* USER CODE END */
/* USER CODE BEGIN (9) */
/* USER CODE END */
}


/** @fn void crcChannelReset(crcBASE_t *crc,uint32 channel)
*   @brief Reset the channel configurations
*   @param[in] crc - crc module base address
*   @param[in] channel-crc channel
*                      CRC_CH1 - channel1
*                      CRC_CH2 - channel2
*                      CRC_CH3 - channel3
*                      CRC_CH4 - channel4
*
*   Reset configurations of the selected channels.
*/
/* SourceId : CRC_SourceId_008 */
/* DesignId : CRC_DesignId_009 */
/* Requirements: HL_SR114 */
void crcChannelReset(crcBASE_t *crc,uint32 channel)
{
/* USER CODE BEGIN (16) */
/* USER CODE END */

    if(channel == 0U)
    {
        crc->CTRL0 |= (uint32)((uint32)1U << 0U); /** Reset the CRC channel  */
        crc->CTRL0 &= ~(uint32)((uint32)1U << 0U); /** Exit the reset  */
     }
     else if(channel == 1U)
     {
        crc->CTRL0 |= (uint32)((uint32)1U << 8U); /** Reset the CRC channel  */
        crc->CTRL0 &= ~(uint32)((uint32)1U << 8U); /** Exit the reset  */
     }
     else
	 {
      /** Empty  */
	 }
/* USER CODE BEGIN (17) */
/* USER CODE END */
 }
 
 
/** @fn uint32 crcGetPSASig(crcBASE_t *crc,uint32 channel)
*   @brief get genearted PSA signature used for FULL CPU mode
*   @param[in] crc - crc module base address
*   @param[in] channel - crc channel
*                      CRC_CH1 - channel1
*                      CRC_CH2 - channel2
*                      CRC_CH3 - channel3
*                      CRC_CH4 - channel4
*
*   Get PSA signature used for FULL CPU mode of selected channel
*/
/* SourceId : CRC_SourceId_011 */
/* DesignId : CRC_DesignId_005 */
/* Requirements: HL_SR525 */
uint64 crcGetPSASig(crcBASE_t *crc,uint32 channel)
{
    uint64 status = 0U;
	uint32 CRC_PSA_SIGREGH1 = crcREG->PSA_SIGREGH1;
	uint32 CRC_PSA_SIGREGL1 = crcREG->PSA_SIGREGL1;
	uint32 CRC_PSA_SIGREGH2 = crcREG->PSA_SIGREGH2;
	uint32 CRC_PSA_SIGREGL2 = crcREG->PSA_SIGREGL2;

/* USER CODE BEGIN (22) */
/* USER CODE END */
    switch (channel)
      {
    case 0U:
        /*SAFETYMCUSW 51 S MR:12.3 <APPROVED> "Needs shifting for 64-bit value" */
		status = (((uint64)(CRC_PSA_SIGREGL1) << 32U) | (uint64)(CRC_PSA_SIGREGH1));
		break;
    case 1U:
        /*SAFETYMCUSW 51 S MR:12.3 <APPROVED> "Needs shifting for 64-bit value" */
		status = (((uint64)(CRC_PSA_SIGREGL2) << 32U) | (uint64)(CRC_PSA_SIGREGH2));
		break;
	default :
	    break;		
    }
    return status;

/* USER CODE BEGIN (23) */
/* USER CODE END */
}


/** @fn void crcGetConfigValue(crc_config_reg_t *config_reg, config_value_type_t type)
*   @brief Get the initial or current values of the CRC configuration registers
*
*	@param[in] *config_reg: pointer to the struct to which the initial or current 
*                           value of the configuration registers need to be stored
*	@param[in] type: 	whether initial or current value of the configuration registers need to be stored
*						- InitialValue: initial value of the configuration registers will be stored 
*                                       in the struct pointed by config_reg
*						- CurrentValue: initial value of the configuration registers will be stored 
*                                       in the struct pointed by config_reg
*
*   This function will copy the initial or current value (depending on the parameter 'type') 
*   of the configuration registers to the struct pointed by config_reg
*
*/
/* SourceId : CRC_SourceId_012 */
/* DesignId : CRC_DesignId_012 */
/* Requirements: HL_SR119 */
void crcGetConfigValue(crc_config_reg_t *config_reg, config_value_type_t type)
{
	if (type == InitialValue)
	{
		config_reg->CONFIG_CTRL0        = CRC_CTRL0_CONFIGVALUE;
		config_reg->CONFIG_CTRL1        = CRC_CTRL1_CONFIGVALUE;
		config_reg->CONFIG_CTRL2        = CRC_CTRL2_CONFIGVALUE;
	}
	else
	{
	/*SAFETYMCUSW 134 S MR:12.2 <APPROVED> "LDRA Tool issue" */
		config_reg->CONFIG_CTRL0        = crcREG->CTRL0;
		config_reg->CONFIG_CTRL1        = crcREG->CTRL1;
		config_reg->CONFIG_CTRL2        = crcREG->CTRL2;
	}
}

