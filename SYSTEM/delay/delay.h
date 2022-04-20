#ifndef _DELAY_H
#define _DELAY_H
#include <sys.h>

/*********************************************************************************
			  ___   _     _____  _____  _   _  _____  _____  _   __
			 / _ \ | |   |_   _||  ___|| \ | ||_   _||  ___|| | / /
			/ /_\ \| |     | |  | |__  |  \| |  | |  | |__  | |/ /
			|  _  || |     | |  |  __| | . ` |  | |  |  __| |    \
			| | | || |_____| |_ | |___ | |\  |  | |  | |___ | |\  \
			\_| |_/\_____/\___/ \____/ \_| \_/  \_/  \____/ \_| \_/

 *	******************************************************************************
 *	本程序只供学习使用，未经作者许可，不得用于其它任何用途
 *	ALIENTEK Pandora STM32L475 IOT开发板
 *	使用SysTick的普通计数模式对延迟进行管理
 *	包括delay_us,delay_ms
 *	正点原子@ALIENTEK
 *	技术论坛:www.openedv.com
 *	创建日期:2015/6/10
 *	版本：V1.1
 *	版权所有，盗版必究。
 *	Copyright(C) 广州市星翼电子科技有限公司 2014-2024
 *	All rights reserved
 *	******************************************************************************
 *	修改说明
 *	******************************************************************************/
 
 /**

 * @brief   STM32 TIM registers block.

 * @note    This is the most general known form, not all timers have

 *          necessarily all registers and bits.

 */

typedef struct {
  volatile uint32_t     CR1;
  volatile uint32_t     CR2;
  volatile uint32_t     SMCR;
  volatile uint32_t     DIER;
  volatile uint32_t     SR;
  volatile uint32_t     EGR;
  volatile uint32_t     CCMR1;
  volatile uint32_t     CCMR2;
  volatile uint32_t     CCER;
  volatile uint32_t     CNT;
  volatile uint32_t     PSC;
  volatile uint32_t     ARR;
  volatile uint32_t     RCR;
  volatile uint32_t     CCR[4];
  volatile uint32_t     BDTR;
#if defined(STM32G4)
  volatile uint32_t     CCXR[2];
  volatile uint32_t     CCMR3;
  volatile uint32_t     DTR2;
  volatile uint32_t     ECR;
  volatile uint32_t     TISEL;
  volatile uint32_t     AF1;
  volatile uint32_t     AF2;
  volatile uint32_t     OR;
  volatile uint32_t     RESERVED0[220];
  volatile uint32_t     DCR;
  volatile uint32_t     DMAR;
#else
  volatile uint32_t     DCR;
  volatile uint32_t     DMAR;
  volatile uint32_t     OR1;
	volatile uint32_t     OR2;	
#endif
} stm32_tim_t;

#define STM32_TIM2      ((stm32_tim_t *)TIM2_BASE)

void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);
#endif

