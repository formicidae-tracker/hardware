#include "Scheduler.hpp"
#include <pico/time.h>

std::multimap<uint8_t, Scheduler::TaskData> Scheduler::d_tasks;

void Scheduler::Work() {

	for (auto it = d_tasks.begin(); it != d_tasks.end();) {
		auto now = get_absolute_time();

		if (absolute_time_diff_us(now, it->second.Next) > 0) {
			++it;
			continue;
		}

		auto res = it->second.Task(now);
		if (res.has_value()) {
			if (res.value() < 0) {
				it = d_tasks.erase(it);
				continue;
			}
			it->second.Period = res.value();
		}

		it->second.Next += it->second.Period;
		++it;
	}
}

void Scheduler::Schedule(uint8_t priority, int64_t period_us, Task &&task) {
	d_tasks.emplace(std::make_pair(
	    priority,
	    TaskData{
	        .Next   = get_absolute_time(),
	        .Task   = std::move(task),
	        .Period = uint64_t(period_us)}
	));
}
