#ifndef TEAM_H
#define TEAM_H

class Agent; // Forward declaration
#include "lineparser.h"
#include <string>
#include <vector>

class Team {
  public:
	/** Constructor
	 * @param exe Executable command
	 */
	Team(const char *exe) : m_exe{exe} {
		m_parser.setExecute([this](uint8_t argc, const char **argv) {
			processLine(argc, argv);
		});
	}

	/** Start team manager as subprocess **/
	void start_subprocess();

	/** Send data to team manager
	 * @param data Data buffer
	 * @param len  Buffer length
	 */
	void send(const char *data, size_t len);

	/** Kill subprocess
	 * @param str String to log
	 */
	void kill(const char *str);

	/** Gets event file descriptor
	 * @return File descriptor
	 */
	int eventFd();

	/** Process died events **/
	void eventProcessDied();

	/** Process read events **/
	void eventProcessRead();

	/** Process one line of process **/
	void processLine(uint8_t argc, const char **argv);

	/** Add an agent **/
	void agentAdd(Agent *);

	/** Remove an agent **/
	void agentRm(Agent *);

  private:
	/** Executable command **/
	std::string m_exe;

	/** File descriptors to communicate with team manager **/
	int m_fdin = -1, m_fdout = -1, m_fderr = -1, m_log = -1;

	/** Pid of subprocess **/
	int m_pid = -1;

	/** Line parser **/
	LineParser<80> m_parser;

	/** List of agents **/
	std::vector<Agent *> m_agents;

	/** Current agent **/
	std::vector<Agent *>::iterator m_currentAgent;

	/** Go to next agent
	 * @return true if sucessful
	 *         false if end of list was reached
	 */
	bool nextAgent();

	/** Send prelude to agent
	 * @return true on success, false on failure
	 */
	bool sendPrelude();
};

#endif // TEAM_H
