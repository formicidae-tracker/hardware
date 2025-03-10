#include "Duration.hpp"

#include <iomanip>
#include <sstream>

std::string FormatDuration(const Duration &d) {
	auto               micros = d.count();
	std::ostringstream oss;

	if (std::abs(micros) < 1000) {
		oss << micros << "µs";
		return oss.str();
	}
	oss << std::fixed << std::setprecision(3);

	if (std::abs(micros) < 1000000) {
		oss << float(micros) / 1000.0f << "ms";

	} else {
		oss << float(micros) / 1e6 << "s";
	}
	return oss.str();
}
