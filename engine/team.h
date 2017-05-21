#ifndef TEAM_H
#define TEAM_H

#include <string>

class Team {
  public:
	/** Constructor
	  * @param exe Executable command
	  */
	Team(const char *exe) : m_exe(exe) {}

	/** Get executable command **/
	const char *exe() { return m_exe.c_str(); }

  private:
	/** Executable command **/
	std::string m_exe;
};

#endif // TEAM_H
