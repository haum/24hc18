#ifndef ANT_H
#define ANT_H

#include "gears/agent.h"

/** Class representing an ant **/
class Ant : public Agent {
  public:
	/** Constructor
	 * @param team Team whose agent belongs
	 * @param latitude Original lattitude
	 * @param longitude Original lattitude
	 * @param heading Original heading
	 * @param ant_type Type of ant
	 * @param memory1 Initial memory1
	 * @param memory2 Initial memory2
	 */
	Ant(Team &team, int life, double latitude, double longitude, double heading,
		uint8_t ant_type = 0, uint8_t memory1 = 0, uint8_t memory2 = 0);

	/** Get category **/
	static GameObject_t category() { return &s_category; }

	/** Make ant to turn left **/
	void turnLeft();

	/** Make ant to turn right **/
	void turnRight();

	/** Make ant to go forward
	 * @note Ants do not walk straight
	 */
	void explore();

	/** get ant Life **/
	int life() { return m_life; }

	/** action MEMORY **/
	void actionMemory(bool valid, uint8_t m0, uint8_t m1);

	/** action MEMORY **/
	void actionSuicide(bool valid);

	/** action PUT_PHEROMONE **/
	void actionPutPheromone(bool valid, uint8_t type);

	/** action CHANGE_PHEROMONE **/
	void actionChangePheromone(bool valid, uint8_t type, int id);

	/** action RECHARGE_PHEROMONE **/
	void actionRechargePheromone(bool valid, int id);

	/** action Explore **/
	void actionExplore(bool valid);

	/** action ATTACK **/
	void actionAttack(bool valid, int id);

	/** set ant Life **/
	void setLife(int life) { this->m_life = life; }

	/** action WALK **/
	void actionTurnLeft(bool valid);

	/** action WALK **/
	void actionTurnRight(bool valid);

  private:
	/** Life of an ant **/
	int m_life;

	/** Type of ant **/
	uint8_t m_ant_type;

	/** Long term memory **/
	uint8_t m_memory[2];

	/** Is exclusive action done in this turn **/
	bool m_exclusiveDone = false;

	/** Signal an invalide action **/
	void invalidAction();

	/** List of action types **/
	enum ActionType {
		ALWAYS_ALLOWED,
		EXCLUSIVE,
	};

	/** Prelude of actions (to check state) **/
	bool actionPrelude(int cost, ActionType type, bool valid);

	virtual bool prelude(std::ostream &os) override;
	virtual void execute(uint8_t argc, const char **argv) override;

	/** Ant category unique object **/
	static const GameObjectCategory s_category;
};

#endif // ANT_H
