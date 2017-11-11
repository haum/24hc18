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
	Ant(Team &team, double latitude, double longitude, double heading,
		uint8_t ant_type = 0, uint8_t memory1 = 0, uint8_t memory2 = 0);

	/** Get category **/
	GameObject_t category() { return &s_category; }

	/** Make ant to turn left **/
	void turnLeft();

	/** Make ant to turn right **/
	void turnRight();

	/** Make ant to go forward
	 * @note Ants do not walk straight
	 */
	void walk();

  private:
	/** Type of ant **/
	uint8_t m_ant_type;

	/** Long term memory **/
	uint8_t m_memory[2];

	/** State of action in this turn **/
	enum {
		ACTION_FREE,
		ACTION_MADE,
		ACTION_MULTIPLE,
	} m_actionState{ACTION_FREE};

	virtual bool prelude(std::ostream &os) override;
	virtual void execute(uint8_t argc, const char **argv) override;

	static const GameObjectCategory s_category;
};

#endif // ANT_H
