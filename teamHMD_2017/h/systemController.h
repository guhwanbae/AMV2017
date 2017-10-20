
/*
 * systemController.h
 *
 *  Created on: 2017. 6. 26.
 *      Author: ghbae
 */

#ifndef SYSTEMCONTROLLER_H_
#define SYSTEMCONTROLLER_H_

/*******************************************************************************
* Include
*******************************************************************************/
#include "typedefinition.h"

/*******************************************************************************
* Constant
*******************************************************************************/
#define BASE_PRD			10000
#define RESET_CMP_INT_FLAG	0x1
#define CMP_REGISTER_SIZE	0b11111
#define CMP_LSB_POSITION	0b00000
#define CMP_EN_EVENT		1

/*******************************************************************************
* Define & MACRO
*******************************************************************************/
#define UNLOCK_SAFETY_WDT	unlock_safety_wdtcon()
#define LOCK_SAFETY_WDT		lock_safety_wdtcon()

#define UNLOCK_WDT_CON		unlock_wdtcon()
#define LOCK_WDT_CON		lock_wdtcon()

/*	PHASE LOCK LOOP	*/
#define PLL_UNLOCK_STS			0x0
#define PLL_GTM_DIV_VAL			0b1000
#define PLL_OSC_MODE_SEL		0b00
#define PLL_OSC_DIV				0b00111
#define PLL_OSC_SRC_SEL			1
#define PLL_CCUCON_REQ			1
#define PLL_ENABLE_OSC			0x1
#define PLL_VCO_STS_UNLOCK		0x0

#define PLL_VCO_MODE_BYPASS		0x1
#define PLL_BLOCK_OSC			1
#define PLL_SYNTHESIZER_N_VAL	0x3B
#define PLL_DIV_P_VAL			0x1
#define	PLL_PSMODE				0x1
#define PLL_DIV_K_VAL			0x2
#define PLL_PASS_OSC			0x1

#define PLL_CLK_SEL				0x1
#define PLL_CCU_CONFIG_REQ		0x1
#define PLL_VCO_MODE_NORMAL		0x0

/*	INTERRUPTS	*/
/*
#define INTSRC_CLK_CAM
#define INTSRC_GET_CAM_FRAME
#define INTSRC_COUNT_FRONT
#define INTSRC_COUNT_BACK
#define INTSRC_BASETICK
#define	INTSRC_CALC_RPM
*/

/*******************************************************************************
* Type Definition
*******************************************************************************/

/*******************************************************************************
* Member
*******************************************************************************/

/*******************************************************************************
* Member Functions
*******************************************************************************/
void systemInit(void);
void systemTimerInit(void);
void phaseLockLoopInit(void);

#endif /* SYSTEMCONTROLLER_H_ */
