#include "Duration.hpp"

std::string FormatDuration(const Duration &d) {
	auto micros = d.count();
	char buffer[40];

	if (std::abs(micros) < 1000) {
		sprintf(buffer, "%dus", micros);
		return buffer;
	}

	if (std::abs(micros) < 1000000) {
		sprintf(buffer, "%.03fms", float(micros) / 1000.0f);
		return buffer;
	}
	sprintf(buffer, "%.03fs", float(micros) / 1000000.0f);
	return buffer;
}
