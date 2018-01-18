#include "utils.h"
#include <chrono>
#include <random>

namespace {
std::default_random_engine random_generator;
std::uniform_real_distribution<> unit_distribution(0, 1);
std::uniform_real_distribution<> angle_distribution(0, 2 * M_PI);

struct random_init {
	random_init() {
		auto now = std::chrono::system_clock::now();
		random_generator.seed(
		    static_cast<unsigned long>(now.time_since_epoch().count()));
	}
} r;
} // namespace

double random_angle() { return angle_distribution(random_generator); }

double random_unit() { return unit_distribution(random_generator); }

int32_t parse_int(const char *str, bool &ok) {
	char *endptr;
	long int value = strtol(str, &endptr, 0);
	ok = (*endptr == 0 && endptr != str && value <= INT32_MAX &&
	      value >= INT32_MIN);
	return static_cast<int>(value);
}

int32_t parse_int(const char *str, bool &ok, int min, int max) {
	int ret = parse_int(str, ok);
	if (ok) {
		if (ret < min)
			ok = false;
		if (ret > max)
			ok = false;
	}
	return ret;
}

float parse_float(const char *str, bool &ok) {
	char *endptr;
	double value = strtod(str, &endptr);
	ok = (*endptr == 0 && endptr != str);
	return static_cast<float>(value);
}

float parse_float(const char *str, bool &ok, float min, float max) {
	auto ret = parse_float(str, ok);
	if (ok) {
		if (ret < min)
			ok = false;
		if (ret > max)
			ok = false;
	}
	return ret;
}

void breakAnchor(const char *name) {
	(void)name; // Do nothing useful
}
