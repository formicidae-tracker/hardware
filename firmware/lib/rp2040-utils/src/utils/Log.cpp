#include "Log.hpp"

#include <hardware/sync.h>
#include <hardware/sync/spin_lock.h>
#include <pico/multicore.h>

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <optional>
#include <stdio.h>

void Logger::Logf(Level level, const char *fmt, va_list args) {
	if (level > d_level) {
		return;
	}

	auto now = get_absolute_time();

	auto written =
	    vsnprintf(d_buffer.data() + d_start, BufferSize - d_start, fmt, args);

	while (d_start + written >= BufferSize) {
		if (d_start == 0) {
			d_buffer[BufferSize - 1] = 0;

			written = BufferSize - 1;
		} else {

			d_start = 0;
			written = vsnprintf(d_buffer.data(), BufferSize, fmt, args);
		}
	}
	va_end(args);

	Message m = {
	    .Value = d_buffer.data() + d_start,
	    .Time  = now,
	    .Level = level,
	};

	if (d_queue.TryAdd(std::move(m)) == true) {
		d_start += written + 1;
	}
}

Logger::Logger() {
	lock_init(&d_lock, next_striped_spin_lock_num());
	d_buffer[BufferSize] = 0;
}

constexpr static size_t LineWidth = 80;

void printTime(absolute_time_t time) {
	uint us = to_us_since_boot(time);
	uint s  = us / 1000000;
	us -= s * 1000000;
	printf("       %06d.%06ds: ", s, us);
}

void Logger::FormatsPendingLogsLoop() {
	multicore_lockout_victim_init();

	auto &lock = Logger::Get().d_lock;

	auto saved = spin_lock_blocking(lock.spin_lock);
	lock_internal_spin_unlock_with_notify(&lock, saved);

	static uint8_t colors[6] = {
	    1, // FATAL - RED
	    1, // ERROR - RED
	    3, // WARNING - YELLOW
	    6, // INFO - CYAN
	    7, // DEBUG - WHITE
	    4, // TRACE - BLUE
	};

	struct Message msg;

	while (true) {
		// we need to save interrupt to not enter in deadlock.

		Logger::Get().d_queue.RemoveBlocking(msg);

		char  *msgStr = const_cast<char *>(msg.Value);
		size_t n      = strlen(msgStr);
		auto   c      = colors[size_t(msg.Level)];

		for (size_t i = 0; i < n;) {
			size_t written = LineWidth - 26;
			char  *ch      = std::find(msgStr + i, msgStr + i + written, '\n');
			written        = ch - msgStr - i;

			auto willWrite = written;
			if (written < LineWidth - 26) {
				willWrite += 1;
			}

			written = std::min(written, n - i);

			printf("\033[30;4%dm", c);
			if (i == 0) {
				printTime(msg.Time);
			} else {
				printf("                       ");
			}

			auto space = LineWidth - 26 - written;
			printf("\033[m\033[3%dm %-.*s ", c, written, msgStr + i);
			printf("%*s┃\n", space, "");
			i += willWrite;
		}
	}
}

void Logger::InitLogsOnSecondCore() {
	auto &lock = Logger::Get().d_lock;
	multicore_launch_core1(FormatsPendingLogsLoop);
	auto saved = spin_lock_blocking(lock.spin_lock);
	lock_internal_spin_unlock_with_wait(&lock, saved);
}
