#include "AP_Scheduler.h"
#include "dma.h"
#include "delay.h"
#include "string.h"
#include "mem.h"
#include <stdlib.h>
#include <time.h>

/* Private function prototypes -----------------------------------------------*/
static __INLINE  void __SVC(void);

/* Private macro -------------------------------------------------------------*/
#if defined ( __CC_ARM   )
__asm void __SVC(void)
{
  SVC 0x01
  BX R14
}
#elif defined ( __ICCARM__ )
static __INLINE  void __SVC()
{
  __ASM("svc 0x01");
}
#elif defined   (  __GNUC__  )
static __INLINE void __SVC()
{
  __ASM volatile("svc 0x01");
}
#endif

__IO uint8_t PSPMemAlloc_Main[SP_PROCESS_SIZE];

__IO uint8_t PSPMemAlloc_Sub[SP_PROCESS_SIZE];

AP_Scheduler::AP_Scheduler()
{
}

void AP_Scheduler::init(const AP_Scheduler::Task *tasks, uint8_t num_tasks)
{
	_loop_rate_hz = 400;
	
	_vehicle_tasks = tasks;
	_num_vehicle_tasks = num_tasks;

	_num_tasks = _num_vehicle_tasks;
	
	_last_run = new uint16_t[_num_tasks];
	_excuted_times = new uint16_t[_num_tasks];
	memset(_last_run, 0, _num_tasks);
	memset(_excuted_times, 0, _num_tasks);
	_tick_counter = 0;
	
	_loop_timer_start_us= 0;
	_loop_period_us = 0;
	
	srand(STM32_TIM2->CNT);
}

void AP_Scheduler::tick()
{
	_tick_counter++;
}

void AP_Scheduler::run(uint32_t time_available)
{
	uint32_t run_started_usec = STM32_TIM2->CNT;
	uint32_t now = run_started_usec;
	
    for (uint8_t i=0; i<_num_tasks; i++) {

        const uint16_t dt = _tick_counter - _last_run[i];
		const AP_Scheduler::Task &task = _vehicle_tasks[i];
				
        uint32_t interval_ticks = ((task.rate_hz < 1.1920928955078125e-7F) ? 1 : _loop_rate_hz / task.rate_hz);
        
		if (interval_ticks < 1) {
            interval_ticks = 1;
        }
        if (dt < interval_ticks) {
            // this task is not yet scheduled to run again
            continue;
        }
        // this task is due to run. Do we have enough time to run it?
        _task_time_allowed = task.max_time_micros;

        if (_task_time_allowed > time_available) {
            // not enough time to run this task.  Continue loop -
            // maybe another task will fit into time remaining
            continue;
        }

        // run it
        _task_time_started = now;

		//DMA_printf((char *)"%d\r\n", task.priority);
		uint16_t run_time = task.run_time;
        task_delay(run_time);

        // record the tick counter when we ran. This drives
        // when we next run the event
        _last_run[i] = _tick_counter;
		_excuted_times[i]++;

        // work out how long the event actually took
        now = STM32_TIM2->CNT;
        uint32_t time_taken = now - _task_time_started;
				
        if (time_taken >= time_available) {
            time_available = 0;
            break;
        }
        time_available -= time_taken;
    }
}

void AP_Scheduler::loop()
{	
	uint32_t start_time = STM32_TIM2->CNT;
	DMA_printf((char *)"st:%d,\r\n", start_time);
	for (;;)
	{
		MPU_Config();
		
		__IO uint8_t *PSP_Mem = (uint8_t *)mem_alloc(SP_PROCESS_SIZE);
		
		//DMA_printf((char *)"psp:0x%x,\r\n", (uint32_t)PSP_Mem);
		
		for(int i = 0; i < SP_PROCESS_SIZE; i++)
		{
			PSP_Mem[i] = 0x00;
		}
		
		/* Set Process stack value */
		__set_PSP((uint32_t)PSP_Mem + SP_PROCESS_SIZE);
		/* Select Process Stack as Thread mode Stack */
		__set_CONTROL(SP_PROCESS);
		/* Execute ISB instruction to flush pipeline as recommended by Arm */
		__ISB();
		
		__set_CONTROL(THREAD_MODE_UNPRIVILEGED | SP_PROCESS);
		__ISB();
		
		const uint32_t sample_time_us = STM32_TIM2->CNT;
		
		if (_loop_timer_start_us == 0) {
			_loop_timer_start_us = sample_time_us;
		}	

		//DMA_printf((char *)"%d\r\n", 0);
		task_delay(1400);
		
		tick();
		
		const uint32_t loop_us = get_loop_period_us();
		uint32_t now = STM32_TIM2->CNT;
		uint32_t time_available = 0;
		const uint32_t loop_tick_us = now - sample_time_us;
		if (loop_tick_us < loop_us) {
			// get remaining time available for this loop
			time_available = loop_us - loop_tick_us;
		}
		
		run(time_available);
		
		_loop_timer_start_us = sample_time_us;
		
		__SVC();
		
		/* Select Process Stack as Thread mode Stack */
		__set_CONTROL(SP_MAIN);
		/* Execute ISB instruction to flush pipeline as recommended by Arm */
		__ISB();
		
		mem_free((void *)PSP_Mem);
		//memfree_all();
		
		uint32_t end_time = STM32_TIM2->CNT;
		if (end_time - start_time >= 1000000)
		{
			//DMA_printf((char *)"et:%d,\r\n", end_time);
			break;
		}
	}
	
	for (int i = 0; i < _num_tasks; i++)
	{		
		//DMA_printf((char *)"%d, %d,\r\n", _vehicle_tasks[i].priority, _excuted_times[i]);
	}
	
//	uint32_t s_time = STM32_TIM2->CNT;
//	for (int i = 0; i < 50; i++)
//	{
//		int *p = (int *)memalloc_bitmap(64);
//	}
//	uint32_t e_time = STM32_TIM2->CNT;
//	DMA_printf((char *)"ct:%d,\r\n", e_time - s_time);
//	
	uint16_t alloc_size[] = {64, 64, 112, 144, 304, 1024};
	
	for (int i = 0; i < 6; i++)
	{
		uint32_t s_time = STM32_TIM2->CNT;
		int *p = (int *)mem_alloc(alloc_size[i]);
		uint32_t e_time = STM32_TIM2->CNT;
		DMA_printf((char *)"ct, %d, %d,\r\n", e_time - s_time, p);
	}
	
	mem_free_all();
	
	for (int i = 5; i >= 0; i--)
	{
		uint32_t s_time = STM32_TIM2->CNT;
		int *p = (int *)mem_alloc(alloc_size[i]);
		uint32_t e_time = STM32_TIM2->CNT;
		DMA_printf((char *)"ct, %d, %d,\r\n", e_time - s_time, p);
	}
}

void AP_Scheduler::task_delay(uint32_t time)
{
//	MPU_Config();
//	for(int i = 0; i < SP_PROCESS_SIZE; i++)
//	{
//		PSPMemAlloc_Sub[i] = 0x00;
//	}
//		
//	/* Set Process stack value */
//	__set_PSP((uint32_t)PSPMemAlloc_Sub + SP_PROCESS_SIZE);
//	/* Select Process Stack as Thread mode Stack */
//	__set_CONTROL(SP_PROCESS);
//	/* Execute ISB instruction to flush pipeline as recommended by Arm */
//	__ISB();
//	
//	__set_CONTROL(THREAD_MODE_UNPRIVILEGED | SP_PROCESS);
//	__ISB();
	
	delay_us(time);

//	__SVC();
//	
//	/* Select Process Stack as Thread mode Stack */
//	__set_CONTROL(SP_MAIN);
//	/* Execute ISB instruction to flush pipeline as recommended by Arm */
//	__ISB();
}

void AP_Scheduler::MPU_Config(void)
{
	MPU_Region_InitTypeDef MPU_InitStruct;

	/* Disable MPU */
	HAL_MPU_Disable();

	/* Configure RAM region as Region N°0, 256KB of size and R/W region */
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress = EXAMPLE_RAM_ADDRESS_START;
	MPU_InitStruct.Size = EXAMPLE_RAM_SIZE;
	MPU_InitStruct.AccessPermission = portMPU_REGION_READ_WRITE;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.Number = EXAMPLE_RAM_REGION_NUMBER;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  
	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Configure FLASH region as REGION N°1, 1MB of size and R/W region */
	MPU_InitStruct.BaseAddress = EXAMPLE_FLASH_ADDRESS_START;
	MPU_InitStruct.Size = EXAMPLE_FLASH_SIZE;
	MPU_InitStruct.Number = EXAMPLE_FLASH_REGION_NUMBER;
  
	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Configure Peripheral region as REGION N°2, 512MB of size, R/W and Execute
	Never region */
	MPU_InitStruct.BaseAddress = EXAMPLE_PERIPH_ADDRESS_START;
	MPU_InitStruct.Size = EXAMPLE_PERIPH_SIZE;
	MPU_InitStruct.Number = EXAMPLE_PERIPH_REGION_NUMBER;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Enable MPU (any access not covered by any enabled region will cause a fault) */
	HAL_MPU_Enable(MPU_HFNMI_PRIVDEF_NONE);
}
