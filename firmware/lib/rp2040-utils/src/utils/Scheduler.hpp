#pragma once

#include <pico/time.h>

#include <functional>
#include <map>
#include <memory>

class Scheduler {
public:
	typedef std::function<std::optional<int64_t>(absolute_time_t)> Task;

	static void Work();

	static void Schedule(uint8_t priority, int64_t period_us, Task &&task);

	static void After(uint8_t priority, absolute_time_t at, Task &&task);

private:
	struct TaskData {
		absolute_time_t Next;
		Scheduler::Task Task;
		int64_t         Period;
	};

	static std::multimap<uint8_t, TaskData> d_tasks;
};
