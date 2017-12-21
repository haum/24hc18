#ifndef SCENARIO_H
#define SCENARIO_H

#include "gears/scenario_base.h"
#include <stdint.h>

/** Class managing scenario execution (story part) **/
class Scenario : public ScenarioBase {
  public:
	/** Constructor **/
	using ScenarioBase::ScenarioBase;

	/** Process one line of scenario **/
	void processLine(uint8_t argc, const char **argv);

	/** action MAXTEAMS **/
	void actionMaxteams(uint8_t teams);

	/** action DURATION **/
	void actionDuration(int s);

	/** action NEST_POSITION **/
	void actionNestPosition(uint8_t team, int latitude, int longitude);

	/** action NEST_POPULATION **/
	void actionNestPopulation(uint8_t team, uint8_t type, int nb);

	/** action NEST_FOOD **/
	void actionNestFood(uint8_t team, int amount);

	/** action ANT **/
	void actionAnt(uint8_t team, uint8_t type, int latitude, int longitude,
	               int heading, int stamina, uint8_t memory1, uint8_t memory2);

	/** action PHEROMONE **/
	void actionPheromone(uint8_t team, uint8_t type, int latitude,
	                     int longitude);

	/** action FOOD **/
	void actionFood(int latitude, int longitude);

  private:
	/** Find team
	 * @param team Team to search
	 * @return Found team pointer or nullptr
	 */
	Team *findTeam(uint8_t team);

	/** Find nest
	 * @param team Team to search
	 * @return Found nest pointer or nullptr
	 */
	Nest *findNest(uint8_t team);
};

#endif // SCENARIO_H
