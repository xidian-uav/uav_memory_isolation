#ifndef _AP_SCHEDULER_H
#define _AP_SCHEDULER_H

#include "sys.h"

#define SCHED_TASK_CLASS(_rate_hz, _max_time_micros, _priority, _run_time) \
        {_rate_hz, _max_time_micros, _priority, _run_time}

class AP_Scheduler
{
	public:
		AP_Scheduler();
		struct Task {
			float rate_hz;
			uint16_t max_time_micros;
			uint8_t priority; // task priority
			uint16_t run_time;
		};
	
		void init(const AP_Scheduler::Task *tasks, uint8_t num_tasks);
		void loop();
	
	private:
		void tick();
		uint16_t ticks() const { return _tick_counter; }
		
		void run(uint32_t time_available);
		
		void task_delay(uint32_t time);
		
		void MPU_Config(void);
	
		// return the number of microseconds available for the current task
		uint16_t time_available_usec(void) const;
		
		// calculated loop period in usec
		uint16_t _loop_period_us;
		
		// overall scheduling rate in Hz
		int16_t _loop_rate_hz;
		
		// get the time-allowed-per-loop in microseconds
		uint32_t get_loop_period_us() {
			if (_loop_period_us == 0) {
				_loop_period_us = 1000000UL / _loop_rate_hz;
			}
			return _loop_period_us;
		}
		
		uint16_t _tick_counter;
		
		// start of loop timing
		uint32_t _loop_timer_start_us;
		
		// list of tasks to run
		const struct Task *_vehicle_tasks;
		uint8_t _num_vehicle_tasks;
		
		// total number of tasks in _tasks and _common_tasks list
		uint8_t _num_tasks;

		// tick counter at the time we last ran each task
		uint16_t *_last_run;
		uint16_t *_excuted_times;
		
		// number of microseconds allowed for the current task
		uint32_t _task_time_allowed;

		// the time in microseconds when the task started
		uint32_t _task_time_started;
};

#endif
