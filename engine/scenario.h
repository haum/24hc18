#ifndef SCENARIO_H
#define SCENARIO_H

#include "lineparser.h"
#include "nest.h"
#include <chrono>
#include <memory>
#include <vector>

class Scenario {
  public:
	/** Constructor **/
	Scenario();

	/** Set list of teams
	 * @param teams List of teams
	 */
	void set_teams(std::vector<Team *> teams);

	/** Load scenario
	 * @param scenario_name  Scenario name
	 */
	void load(const char *scenario_name);

	/** Run scenario **/
	void run();

	/** Process one line of scenario **/
	void processLine(uint8_t argc, const char **argv);

  private:
	/** File parser **/
	LineParser<100> m_parser;

	/** Copy of team list **/
	std::vector<Team *> m_teams;

	/** List of created nests **/
	std::vector<std::unique_ptr<GameObject>> m_gameObjectsStorage;

	/** Duration ×100ms (ie 20 = 2s) **/
	std::chrono::seconds m_duration;
};

#endif // SCENARIO_H
