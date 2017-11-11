#ifndef SCENARIOBASE_H
#define SCENARIOBASE_H

#include "../nest.h"
#include "lineparser.h"
#include <chrono>
#include <memory>
#include <vector>

class ScenarioBase {
  public:
	/** Constructor **/
	ScenarioBase();

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
	virtual void processLine(uint8_t argc, const char **argv) = 0;

	/** Add game object **/
	void addGameObject(std::unique_ptr<GameObject> obj);

	/** Remove game object **/
	void rmGameObject(GameObject *obj);

  protected:
	/** File parser **/
	LineParser<100> m_parser;

	/** Copy of team list **/
	std::vector<Team *> m_teams;

	/** List of created nests **/
	std::vector<std::shared_ptr<GameObject>> m_gameObjectsStorage;

	/** Duration ×100ms (ie 20 = 2s) **/
	std::chrono::seconds m_duration;
};

#endif // SCENARIOBASE_H