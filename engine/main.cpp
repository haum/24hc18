#include "ant.h"
#include "rand.h"
#include <array>
#include <getopt.h>
#include <iostream>
#include <signal.h>
#include <sstream>

int main(int argc, char *argv[]) {
	const char *scenario = "";

	// Check arguments
	auto usage = [](const char *appname) {
		std::cerr << "USAGE: " << appname << " -s scenario [OPTION] <ias...>\n"
				  << "       -s, --scenario=file\tFile of the scenario\n"
				  << std::endl;
	};
	if (argc > 1) {
		struct option long_options[] = {{"scenario", required_argument, 0, 's'},
										{0, 0, 0, 0}};
		int c, option_index;
		while ((c = getopt_long(argc, argv, "s:", long_options,
								&option_index)) != -1) {
			switch (c) {
			case 's':
				scenario = optarg;
				break;
			default:
				std::cerr << "ERROR: Unknown option\n";
				usage(argv[0]);
				exit(EXIT_FAILURE);
			}
		}
	} else if (argc == 1) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	} else {
		usage("marabunta_engine");
		exit(EXIT_FAILURE);
	}
	if (!strcmp(scenario, "")) {
		std::cerr << "ERROR: Missing scenario\n";
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	if (argc - optind < 1) {
		std::cerr << "ERROR: No IA\n";
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	// Display info
	std::cout << "Scenario: " << scenario << '\n';
	char *ias[argc - optind];
	for (int i = 0, ind = optind; ind < argc; ++i, ++ind) {
		ias[i] = argv[ind];
		std::cout << "IA" << (i + 1) << ": " << argv[ind] << '\n';
	}
	std::cout << std::endl;

	signal(SIGPIPE, SIG_IGN);

	// Test two ants
	Team team{ias[0]};
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
