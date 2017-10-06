#ifndef TEAM_H
#define TEAM_H

class Agent; // Forward declaration
#include <string>
#include <vector>

class Team {
  public:
	/** Constructor
	 * @param exe Executable command
	 */
	Team(const char *exe) : m_exe(exe) {}

	/** Get executable command **/
	const char *exe() { return m_exe.c_str(); }

	/** Gets event file descriptor
	 * @return File descriptor
	 */
	int eventFd();

	/** Process read events **/
	void eventProcessRead();

	/** Add an agent **/
	void agentAdd(Agent *);

	/** Remove an agent **/
	void agentRm(Agent *);

  private:
	/** Executable command **/
	std::string m_exe;

	/** List of agents **/
	std::vector<Agent *> m_agents;
};

#endif // TEAM_H
