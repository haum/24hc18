#ifndef TEAMBASE_H
#define TEAMBASE_H

class Agent;    // Forward declaration
class Scenario; // Forward declaration

#include "lineparser.h"
#include <string>
#include <vector>

/** Class managing a team (gear part) **/
class TeamBase {
  public:
	/** Constructor
	 * @param scenario Reference to scenario
	 * @param exe      Executable command
	 * @param debug    Should debug be printed or this team
	 */
	TeamBase(Scenario &scenario, const char *exe, bool debug, bool nokill)
	    : m_scenario{scenario}, m_exe{exe}, m_debug(debug), m_nokill(nokill) {
		m_parser.setExecute([this](uint8_t argc, const char **argv) {
			processLine(argc, argv);
		});
	}

	/** Destructor **/
	virtual ~TeamBase();

	/** Get scenario **/
	Scenario &scenario() const { return m_scenario; }

	/** Start team manager as subprocess **/
	void start_subprocess();

	/** Send data to team manager
	 * @param data Data buffer
	 * @param len  Buffer length
	 */
	void send(const char *data, size_t len);

	/** Write a line of debug information in team log
	 * @note Do not use newline characters in message (not checked)
	 * @param msg Null-terminated C-string message to display
	 * @param prefix Prefix character
	 */
	void log(const char *msg, char prefix = '!');

	/** Kill subprocess
	 * @param str String to log
	 */
	void kill(const char *str);

	/** Execute one agent only **/
	void oneShot(bool on);

	/** Current agent is paused **/
	bool paused() { return m_paused; }

	/** Gets event file descriptor
	 * @return File descriptor
	 */
	int eventFd() const;

	/** Process died events **/
	void eventProcessDied();

	/** Process read events **/
	void eventProcessRead();

	/** Process one line of process **/
	void processLine(uint8_t argc, const char **argv);

  protected:
	/** Reference to scenario **/
	Scenario &m_scenario;

	/** Executable command **/
	std::string m_exe;

	/** Log file descriptor **/
	int m_log = -1;

	/** Team manager strategy program **/
	struct TeamManager {
		/** File descriptors to communicate with team manager **/
		int m_fdin = -1, m_fdout = -1, m_fderr = -1;

		/** Pid of subprocess **/
		int m_pid = -1;
	};

	/** Team managers **/
	std::array<TeamManager, 2> m_teamManagers;

	/** Do we display debug informations **/
	bool m_debug;

	/** Do we kill randomly **/
	bool m_nokill;

	/** Is current agent dead **/
	bool m_dead;

	/** Is current agent paused **/
	bool m_paused = false;

	/** Do we execute one shot agent **/
	bool m_oneshot = false;

	/** Number of agents strategies executed (for stats) **/
	unsigned long m_stats_agents = 0;

	/** Line parser **/
	LineParser<100> m_parser;

	/** List of agents **/
	std::vector<Agent *> m_agents;

	/** List of agents **/
	std::vector<Agent *> m_agentsToAdd;

	/** List of agents **/
	std::vector<Agent *> m_agentsToRemove;

	/** Current agent **/
	decltype(m_agents)::iterator m_currentAgent = m_agents.begin();

	/** Current team manager index **/
	decltype(m_teamManagers)::iterator m_currentManager =
	    m_teamManagers.begin();

	/** Go to next agent
	 * @return true if sucessful
	 *         false if end of list was reached
	 */
	bool nextAgent();

	/** Send prelude to current agent (or next one if needed) */
	void sendPrelude();

	/** Add an agent **/
	void agentAdd(Agent *);

	/** Remove an agent **/
	void agentRm(Agent *);

	friend class Agent;
};

#endif // TEAMBASE_H
