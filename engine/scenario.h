#ifndef SCENARIO_H
#define SCENARIO_H

#include "nest.h"
#include <chrono>
#include <memory>
#include <vector>

class Scenario {
  public:
	/** Constructor
	  * @param name  Scenario name
	  * @param teams List of teams
	  */
	Scenario(const char *name, std::vector<Team *> teams);

	/** Run scenario **/
	void run();

  private:
	/** Copy of team list **/
	std::vector<Team *> m_teams;

	/** List of nests **/
	std::vector<Nest *> m_nests;

	/** List of created nests **/
	std::vector<std::unique_ptr<Nest>> m_nestsStorage;

	/** Duration ×100ms (ie 20 = 2s) **/
	std::chrono::seconds m_duration;
};

#endif // SCENARIO_H
