#include "../scenario.h"
#include "utils.h"
#include <csignal>
#include <getopt.h>
#include <iostream>
#include <memory>
#include <sstream>

int main(int argc, char *argv[]) {
	const char *scenario_name = "scenarii/scenario.cfg";
	const char *host = "";
	uint16_t port = 2080;
	int debug = -1;
	bool nokill = false;

	// Check arguments
	auto usage = [](const char *appname) {
		std::clog << "USAGE: " << appname << " [OPTIONS] <ias...>\n"
		          << "       -s, --scenario=file\tFile of the scenario\n"
		          << "       -h, --host=ip_address\tIP of server\n"
		          << "       -p, --port=port_number\tport of server\n"
		          << "       -d, --debug=ia nb\tIA number for debug\n"
		          << "       -k, --nokill\tDo not random kill\n"
		          << std::endl;
	};
	if (argc > 1) {
		struct option long_options[] = {
		    {"scenario", required_argument, nullptr, 's'},
		    {"host", required_argument, nullptr, 'h'},
		    {"port", required_argument, nullptr, 'p'},
		    {"debug", required_argument, nullptr, 'd'},
		    {"nokill", no_argument, nullptr, 'k'},
		    {nullptr, 0, nullptr, 0}};
		int c, option_index;
		while ((c = getopt_long(argc, argv, "s:d:h:p:k", long_options,
		                        &option_index)) != -1) {
			switch (c) {
			case 's':
				scenario_name = optarg;
				break;
			case 'h':
				host = optarg;
				break;
			case 'p':
				port = static_cast<uint16_t>(atoi(optarg));
				break;
			case 'd':
				debug = atoi(optarg);
				break;
			case 'k':
				nokill = true;
				break;
			default:
				std::clog << "ERROR: Unknown option\n";
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
	if (argc - optind < 1) {
		std::clog << "ERROR: No IA\n";
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	signal(SIGPIPE, SIG_IGN);

	// Display info
	std::cout << "Scenario: " << scenario_name << '\n';
	Scenario scenario;
	Snitch snitch(host, port, scenario);
	std::vector<std::unique_ptr<Team>> teams;
	std::vector<Team *> teams_ptrs;
	for (int i = 0, ind = optind; ind < argc; ++i, ++ind) {
		auto team =
		    std::make_unique<Team>(scenario, argv[ind], debug == i, nokill);
		teams_ptrs.push_back(team.get());
		teams.push_back(std::move(team));
		std::cout << "IA" << (i + 1) << ": " << argv[ind] << '\n';
	}
	std::cout << std::endl;
	scenario.set_teams(teams_ptrs);
	scenario.set_snitch(&snitch);
	scenario.load(scenario_name);
	GameClock clock;
	scenario.run(clock);

	std::cout << "==========\n"
	          << "STATISTICS\n"
	          << "==========\n";
	for (auto &team : teams) {
		team->printStats();
		std::cout << "----------\n";
	}
	std::cout << std::flush;

	return 0;
}
