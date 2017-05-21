#include "rand.h"
#include <iostream>

namespace {
void usage(const char *appname) {
	std::cout << "Usage: " << appname
			  << " <scenario> <ia command 1> [ia command 2]..." << std::endl;
}
}

int main(int argc, char *argv[]) {
	// Configuration reading
	if (argc <= 0) {
		usage("marabunta_engine");
		return 1;
	}
	if (argc < 3) {
		usage(argv[0]);
		return 1;
	}
	std::cout << "Scenario: " << argv[1] << '\n';
	for (int i = 1; i < argc - 1; ++i)
		std::cout << "IA" << i << ": " << argv[i + 1] << '\n';
	std::cout << std::endl;

	// Test randomness
	for (int i = 0; i < 2000; ++i) {
		std::cout << random_angle() << ' ' << random_unit() << '\n';
	}
	std::cout.flush();

	return 0;
}
