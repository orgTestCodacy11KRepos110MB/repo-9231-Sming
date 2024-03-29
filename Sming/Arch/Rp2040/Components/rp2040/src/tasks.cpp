#include "include/esp_tasks_ll.h"
#include <stringutil.h>
#include <debug_progmem.h>
#include <hardware/sync.h>

class TaskQueue
{
public:
	TaskQueue(os_task_t callback, os_event_t* events, uint8_t length)
	{
		this->callback = callback;
		this->events = events;
		this->length = length;
		read = count = 0;
	}

	bool IRAM_ATTR post(os_signal_t sig, os_param_t par)
	{
		auto level = save_and_disable_interrupts();

		bool full = (count == length);
		if(!full) {
			events[(read + count) % length] = os_event_t{sig, par};
			++count;
		}

		restore_interrupts(level);
		return !full;
	}

	void process()
	{
		// Don't service any newly queued events
		for(unsigned n = count; n != 0; --n) {
			auto evt = events[read];
			read = (read + 1) % length;
			--count;
			callback(&evt);
		}
	}

private:
	os_task_t callback;
	os_event_t* events;
	uint8_t read;
	uint8_t count;
	uint8_t length;
};

static TaskQueue* task_queues[USER_TASK_PRIO_MAX + 1];

const uint8_t SYSTEM_TASK_PRIO = USER_TASK_PRIO_MAX;

bool system_os_task(os_task_t callback, os_task_priority_t prio, os_event_t* events, uint8_t qlen)
{
	if(prio >= USER_TASK_PRIO_MAX) {
		debug_e("[TQ] Invalid priority %u", prio);
		return false;
	}
	auto& queue = task_queues[prio];
	if(queue != nullptr) {
		debug_e("[TQ] Queue %u already initialised", prio);
		return false;
	}

	queue = new TaskQueue(callback, events, qlen);
	return queue != nullptr;
}

bool IRAM_ATTR system_os_post(os_task_priority_t prio, os_signal_t sig, os_param_t par)
{
	if(prio >= USER_TASK_PRIO_MAX) {
		return false;
	}
	auto& queue = task_queues[prio];
	if(queue == nullptr) {
		return false;
	}

	return task_queues[prio]->post(sig, par);
}

void system_init_tasks()
{
	static os_event_t events[8];

	auto systemTaskCallback = [](os_event_t* event) {
		auto callback = system_task_callback_t(event->sig);
		if(callback != nullptr) {
			callback(event->par);
		}
	};

	task_queues[SYSTEM_TASK_PRIO] = new TaskQueue(systemTaskCallback, events, ARRAY_SIZE(events));
}

void system_service_tasks()
{
	for(int prio = SYSTEM_TASK_PRIO; prio >= 0; --prio) {
		auto queue = task_queues[prio];
		if(queue != nullptr) {
			queue->process();
		}
	}
}

bool system_queue_callback(system_task_callback_t callback, uint32_t param)
{
	return task_queues[SYSTEM_TASK_PRIO]->post(os_signal_t(callback), param);
}
