#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "dma.h"
#include "AP_Scheduler.h"

#define ARRAY_SIZE(_arr) (sizeof(_arr) / sizeof(_arr[0]))
	
#define SCHED_TASK(_interval_ticks, _max_time_micros, _prio) SCHED_TASK_CLASS(_interval_ticks, _max_time_micros, _prio)

const AP_Scheduler::Task scheduler_tasks[] = {
	SCHED_TASK(100,    130,  3),
    SCHED_TASK(50,     75,  6),
    SCHED_TASK_CLASS(50, 200,   9),
    SCHED_TASK_CLASS(200, 160,  12),
    SCHED_TASK(10,    120, 15),
    SCHED_TASK_CLASS(10,  50,  18),
    SCHED_TASK(10,     50, 21),
    SCHED_TASK(10,     50,  27),
    SCHED_TASK(10,     75,  30),
    SCHED_TASK(20,    100,  33),
    SCHED_TASK_CLASS(200,  50,  36),
    SCHED_TASK_CLASS(400,  50,  39),
    SCHED_TASK(10,    100,  42),
    SCHED_TASK(50,    100,  45),
    SCHED_TASK(100,     90,  48),
    SCHED_TASK_CLASS(3, 100,  51),
    SCHED_TASK_CLASS(3,  90,  54),
    SCHED_TASK(3,     75, 57),
    SCHED_TASK_CLASS(50,  75,  60),
    SCHED_TASK_CLASS(50,  90,  63),
    SCHED_TASK_CLASS(10, 100,  66),
    SCHED_TASK(400,     50,  69),
    SCHED_TASK(400,    50,  75),
    SCHED_TASK_CLASS(50,  90,  78),
    SCHED_TASK(1,    100,  81),
    SCHED_TASK(10,     75,  84),
    SCHED_TASK(10,     50,  87),
    SCHED_TASK(10,     50,  90),
    SCHED_TASK(10,     75,  93),
    SCHED_TASK(100,    75,  96),
    SCHED_TASK(10,     50,  99),
    SCHED_TASK_CLASS(400, 180, 102),
    SCHED_TASK_CLASS(400, 550, 105),
    SCHED_TASK_CLASS(50,  75, 108),
    SCHED_TASK_CLASS(50,  75, 111),
    SCHED_TASK(10,    350, 114),
    SCHED_TASK(25,    110, 117),
    SCHED_TASK_CLASS(400, 300, 120),
    SCHED_TASK_CLASS(400,  50, 123),
    SCHED_TASK_CLASS(0.1,  75, 126),
    SCHED_TASK(40,    200, 129),
    SCHED_TASK_CLASS(100, 100, 132),
    SCHED_TASK_CLASS(10, 100, 135),
    SCHED_TASK(10,    100, 138),
    SCHED_TASK(10,    100, 144),
    SCHED_TASK_CLASS(10,  75, 147),
    SCHED_TASK_CLASS(50,  50, 150),
    SCHED_TASK_CLASS(5, 100, 168),
    SCHED_TASK_CLASS(1, 100, 171),
};

void get_scheduler_tasks(const AP_Scheduler::Task *&tasks,
                                 uint8_t &task_count)
{
    tasks = &scheduler_tasks[0];
    task_count = ARRAY_SIZE(scheduler_tasks);
}

int main(void)
{	
	HAL_Init();
	SystemClock_Config();	//初始化系统时钟为80M
	uart_init(115200);		//初始化串口，波特率为115200
	delay_init(80); 		//初始化延时函数    80M系统时钟
	DMA_init(DMA1_Channel4, DMA_REQUEST_2);
	
	AP_Scheduler _scheduler;
	
	const AP_Scheduler::Task *tasks;
	uint8_t task_count;
	get_scheduler_tasks(tasks, task_count);
	_scheduler.init(tasks, task_count);
	
	_scheduler.loop();
	
	while(1)
	{
		
	}
}
