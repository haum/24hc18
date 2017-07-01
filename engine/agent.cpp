#include "agent.h"
#include <sstream>

Agent::Agent(Team &team) : m_team{team} {
    m_team.agentAdd(this);
}

Agent::~Agent() {
    m_team.agentRm(this);
}

void Agent::run() {
	std::ostringstream os;
	if (prelude(os)) {
		FILE *agent;
		char buff[81];
		if ((agent = popen(m_team.exe(), "r")) != nullptr) {
			// fwrite(os.rdbuf(), os.str().length(), 1, agent); // TODO pipe is
			// unidirectional, have to find another way

			while (fgets(buff, sizeof(buff), agent) != nullptr) {
				// fgets already breaks by lines, so we only break tokens
				size_t len = strlen(buff);
				if (len >= 1 && buff[len - 1] == '\n')
					buff[len - 1] = 0;
				char *arg = strtok(buff, " ");
				uint8_t argc = 0;
				const char *args[20];
				while (arg != nullptr && argc < sizeof(args)) {
					args[argc] = arg;
					++argc;
					arg = strtok(NULL, " ");
				}
				if (argc > 0)
					execute(argc, args);
			}
			pclose(agent);
		}
		epilogue();
	}
}

int32_t Agent::param_int(const char *str, bool &ok) {
	char *endptr;
	long int value = strtol(str, &endptr, 0);
	ok = (*endptr == 0 && endptr != str && value <= INT32_MAX &&
		  value >= INT32_MIN);
	return static_cast<int>(value);
}

int32_t Agent::param_int(const char *str, bool &ok, int min, int max) {
	int ret = param_int(str, ok);
	if (ok) {
		if (ret < min)
			ok = false;
		if (ret > max)
			ok = false;
	}
	return ret;
}
