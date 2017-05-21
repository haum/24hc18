#include "rand.h"
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
}

double random_angle() { return angle_distribution(random_generator); }

double random_unit() { return unit_distribution(random_generator); }
