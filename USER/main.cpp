#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "dma.h"
#include "rng.h"
#include "AP_Scheduler.h"

#define ARRAY_SIZE(_arr) (sizeof(_arr) / sizeof(_arr[0]))
	
#define SCHED_TASK(_interval_ticks, _max_time_micros, _prio, _run) SCHED_TASK_CLASS(_interval_ticks, _max_time_micros, _prio, _run)

//const AP_Scheduler::Task scheduler_tasks[] = {
//	SCHED_TASK(100,    130,  3),
//    SCHED_TASK(50,     75,  6),
//    SCHED_TASK_CLASS(50, 200,   9),
//    SCHED_TASK_CLASS(200, 160,  12),
//    SCHED_TASK(10,    120, 15),
//    SCHED_TASK_CLASS(10,  50,  18),
//    SCHED_TASK(10,     50, 21),
//    SCHED_TASK(10,     50,  27),
//    SCHED_TASK(10,     75,  30),
//    SCHED_TASK(20,    100,  33),
//    SCHED_TASK_CLASS(200,  50,  36),
//    SCHED_TASK_CLASS(400,  50,  39),
//    SCHED_TASK(10,    100,  42),
//    SCHED_TASK(50,    100,  45),
//    SCHED_TASK(100,     90,  48),
//    SCHED_TASK_CLASS(3, 100,  51),
//    SCHED_TASK_CLASS(3,  90,  54),
//    SCHED_TASK(3,     75, 57),
//    SCHED_TASK_CLASS(50,  75,  60),
//    SCHED_TASK_CLASS(50,  90,  63),
//    SCHED_TASK_CLASS(10, 100,  66),
//    SCHED_TASK(400,     50,  69),
//    SCHED_TASK(400,    50,  75),
//    SCHED_TASK_CLASS(50,  90,  78),
//    SCHED_TASK(1,    100,  81),
//    SCHED_TASK(10,     75,  84),
//    SCHED_TASK(10,     50,  87),
//    SCHED_TASK(10,     50,  90),
//    SCHED_TASK(10,     75,  93),
//    SCHED_TASK(100,    75,  96),
//    SCHED_TASK(10,     50,  99),
//    SCHED_TASK_CLASS(400, 180, 102),
//    SCHED_TASK_CLASS(400, 550, 105),
//    SCHED_TASK_CLASS(50,  75, 108),
//    SCHED_TASK_CLASS(50,  75, 111),
//    SCHED_TASK(10,    350, 114),
//    SCHED_TASK(25,    110, 117),
//    SCHED_TASK_CLASS(400, 300, 120),
//    SCHED_TASK_CLASS(400,  50, 123),
//    SCHED_TASK_CLASS(0.1,  75, 126),
//    SCHED_TASK(40,    200, 129),
//    SCHED_TASK_CLASS(100, 100, 132),
//    SCHED_TASK_CLASS(10, 100, 135),
//    SCHED_TASK(10,    100, 138),
//    SCHED_TASK(10,    100, 144),
//    SCHED_TASK_CLASS(10,  75, 147),
//    SCHED_TASK_CLASS(50,  50, 150),
//    SCHED_TASK_CLASS(5, 100, 168),
//    SCHED_TASK_CLASS(1, 100, 171),
//};

const AP_Scheduler::Task scheduler_tasks[] = {
	SCHED_TASK(100,    130,  3,	30),
    SCHED_TASK(50,     75,  6,	30),
    SCHED_TASK_CLASS(50, 200,   9,	34),
    SCHED_TASK_CLASS(200, 160,  12,	4),
    SCHED_TASK(10,    120, 15,	130),
    SCHED_TASK_CLASS(10,  50,  18,	3),
    SCHED_TASK(10,     50, 21,	26),
    SCHED_TASK(10,     50,  27,	2),
    SCHED_TASK(10,     75,  30,	35),
    SCHED_TASK(20,    100,  33,	77),
    SCHED_TASK_CLASS(200,  50,  36,	2),
    SCHED_TASK_CLASS(400,  50,  39,	2),
    SCHED_TASK(10,    100,  42,	56),
    SCHED_TASK(50,    100,  45,	13),
    SCHED_TASK(100,     90,  48,	3),
    SCHED_TASK_CLASS(3, 100,  51,	2),
    SCHED_TASK_CLASS(3,  90,  54,	2),
    SCHED_TASK(3,     75, 57,	15),
    SCHED_TASK_CLASS(50,  75,  60,	2),
    SCHED_TASK_CLASS(50,  90,  63,	2),
    SCHED_TASK_CLASS(10, 100,  66,	2),
    SCHED_TASK(400,     50,  69,	8),
    SCHED_TASK(400,    50,  75,	10),
    SCHED_TASK_CLASS(50,  90,  78,	10),
    SCHED_TASK(1,    100,  81,	123),
    SCHED_TASK(10,     75,  84,	5),
    SCHED_TASK(10,     50,  87,	13),
    SCHED_TASK(10,     50,  90,	9),
    SCHED_TASK(10,     75,  93,	3),
    SCHED_TASK(100,    75,  96,	2),
    SCHED_TASK(10,     50,  99,	20),
    SCHED_TASK_CLASS(400, 180, 102,	10),
    SCHED_TASK_CLASS(400, 550, 105,	65),
    SCHED_TASK_CLASS(50,  75, 108,	52),
    SCHED_TASK_CLASS(50,  75, 111,	2),
    SCHED_TASK(10,    350, 114,	646),
    SCHED_TASK(25,    110, 117,	43),
    SCHED_TASK_CLASS(400, 300, 120,	2),
    SCHED_TASK_CLASS(400,  50, 123,	2),
    SCHED_TASK_CLASS(0.1,  75, 126,	21),
    SCHED_TASK(40,    200, 129,	22),
    SCHED_TASK_CLASS(100, 100, 132,	2),
    SCHED_TASK_CLASS(10, 100, 135,	2),
    SCHED_TASK(10,    100, 138,	10),
    SCHED_TASK(10,    100, 144,	2),
    SCHED_TASK_CLASS(10,  75, 147,	2),
    SCHED_TASK_CLASS(50,  50, 150,	2),
    SCHED_TASK_CLASS(5, 100, 168,	2),
    SCHED_TASK_CLASS(1, 100, 171,	2),
};

//const AP_Scheduler::Task scheduler_tasks[] = {
//	SCHED_TASK(100,    30,  3),
//    SCHED_TASK(50,     30,  6),
//    SCHED_TASK_CLASS(50, 34,   9),
//    SCHED_TASK_CLASS(200, 4,  12),
//    SCHED_TASK(10,    130, 15),
//    SCHED_TASK_CLASS(10,  3,  18),
//    SCHED_TASK(10,     26, 21),
//    SCHED_TASK(10,     2,  27),
//    SCHED_TASK(10,     35,  30),
//    SCHED_TASK(20,    77,  33),
//    SCHED_TASK_CLASS(200,  2,  36),
//    SCHED_TASK_CLASS(400,  2,  39),
//    SCHED_TASK(10,    56,  42),
//    SCHED_TASK(50,    13,  45),
//    SCHED_TASK(100,     3,  48),
//    SCHED_TASK_CLASS(3, 2,  51),
//    SCHED_TASK_CLASS(3,  2,  54),
//    SCHED_TASK(3,     15, 57),
//    SCHED_TASK_CLASS(50,  2,  60),
//    SCHED_TASK_CLASS(50,  2,  63),
//    SCHED_TASK_CLASS(10, 2,  66),
//    SCHED_TASK(400,     8,  69),
//    SCHED_TASK(400,    10,  75),
//    SCHED_TASK_CLASS(50,  10,  78),
//    SCHED_TASK(1,    123,  81),
//    SCHED_TASK(10,     5,  84),
//    SCHED_TASK(10,     13,  87),
//    SCHED_TASK(10,     9,  90),
//    SCHED_TASK(10,     3,  93),
//    SCHED_TASK(100,    2,  96),
//    SCHED_TASK(10,     20,  99),
//    SCHED_TASK_CLASS(400, 10, 102),
//    SCHED_TASK_CLASS(400, 65, 105),
//    SCHED_TASK_CLASS(50,  52, 108),
//    SCHED_TASK_CLASS(50,  2, 111),
//    SCHED_TASK(10,    646, 114),
//    SCHED_TASK(25,    43, 117),
//    SCHED_TASK_CLASS(400, 2, 120),
//    SCHED_TASK_CLASS(400,  2, 123),
//    SCHED_TASK_CLASS(0.1,  21, 126),
//    SCHED_TASK(40,    22, 129),
//    SCHED_TASK_CLASS(100, 2, 132),
//    SCHED_TASK_CLASS(10, 2, 135),
//    SCHED_TASK(10,    10, 138),
//    SCHED_TASK(10,    2, 144),
//    SCHED_TASK_CLASS(10,  2, 147),
//    SCHED_TASK_CLASS(50,  2, 150),
//    SCHED_TASK_CLASS(5, 2, 168),
//    SCHED_TASK_CLASS(1, 2, 171),
//};

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
	
	while(RNG_Init())
	{
		delay_ms(200);
	}
	
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
