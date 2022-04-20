#ifndef _SYS_H
#define _SYS_H
#include "stm32l4xx.h"

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
 *	系统时钟初始化
 *	包括时钟设置/中断管理/GPIO设置等
 *	正点原子@ALIENTEK
 *	技术论坛:www.openedv.com
 *	创建日期:2016/1/5
 *	版本：V1.0
 *	版权所有，盗版必究。
 *	Copyright(C) 广州市星翼电子科技有限公司 2014-2024
 *	All rights reserved
 *	******************************************************************************
 *	版本修改说明
 *	无
 *	******************************************************************************/
 
//0,不支持os
//1,支持os
#define SYSTEM_SUPPORT_OS		0		//定义系统文件夹是否支持OS

#define SP_PROCESS                  0x02   /* Process stack */
#define SP_MAIN                     0x00   /* Main stack */
#define THREAD_MODE_PRIVILEGED      0x00   /* Thread mode has privileged access */
#define THREAD_MODE_UNPRIVILEGED    0x01   /* Thread mode has unprivileged access */

#define EXAMPLE_RAM_ADDRESS_START                (0x20000000UL)
#define EXAMPLE_RAM_SIZE                         MPU_REGION_SIZE_128KB
#define EXAMPLE_PERIPH_ADDRESS_START             (0x40000000)
#define EXAMPLE_PERIPH_SIZE                      MPU_REGION_SIZE_512MB
#define EXAMPLE_FLASH_ADDRESS_START              (0x08000000)
#define EXAMPLE_FLASH_SIZE                       MPU_REGION_SIZE_1MB
#define EXAMPLE_RAM_REGION_NUMBER                MPU_REGION_NUMBER0
#define EXAMPLE_FLASH_REGION_NUMBER              MPU_REGION_NUMBER1
#define EXAMPLE_PERIPH_REGION_NUMBER             MPU_REGION_NUMBER2
#define portMPU_REGION_READ_WRITE                MPU_REGION_FULL_ACCESS
#define portMPU_REGION_PRIVILEGED_READ_ONLY      MPU_REGION_PRIV_RO
#define portMPU_REGION_READ_ONLY                 MPU_REGION_PRIV_RO_URO
#define portMPU_REGION_PRIVILEGED_READ_WRITE     MPU_REGION_PRIV_RW

#define SP_PROCESS_SIZE             0x200  /* Process stack size */

//定义一些常用的数据类型短关键字
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;
typedef const int16_t sc16;
typedef const int8_t sc8;

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;
typedef __I int16_t vsc16;
typedef __I int8_t vsc8;

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;
typedef const uint16_t uc16;
typedef const uint8_t uc8;

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;
typedef __I uint16_t vuc16;
typedef __I uint8_t vuc8;


void SystemClock_Config(void);//时钟系统配置
//以下为汇编函数
void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(u32 addr);	//设置堆栈地址 

#endif



