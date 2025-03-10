#pragma once

#include <chrono>
#include <string>

typedef std::chrono::duration<int64_t, std::micro> Duration;

std::string FormatDuration(const Duration &);
