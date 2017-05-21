#include "ant.h"
#include "rand.h"
#include <array>
#include <iostream>
#include <sstream>

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

	// Test two ants
	Team team{argv[2]};
	std::array<Ant, 2> ants = {{Ant{team, 0, 0, 0}, Ant{team, 0, 0, 0}}};
	for (int i = 0; i < 2000; ++i) {
		for (auto &ant : ants) {
			ant.run();
			std::cout << ant.longitude() << ' ' << ant.latitude() << ' '
					  << ant.heading() << ' ';
		}
		std::cout << '\n';
	}
	std::cout.flush();

	return 0;
}
