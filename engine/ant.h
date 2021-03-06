#ifndef ANT_H
#define ANT_H

#include "gears/agent.h"
#include <cmath>

/** Class representing an ant **/
class Ant : public Agent {
  public:
	constexpr static const int MAX_STOCK = 1000;
	constexpr static const int MAX_STAMINA = 10000;
	constexpr static const double WALK_DISTANCE = 2 * M_PI / 10000;
	constexpr static const double NEAR_DISTANCE = 3 * WALK_DISTANCE;
	constexpr static const double FAR_DISTANCE = 10 * WALK_DISTANCE;

	/** Constructor
	 * @param team Team whose agent belongs
	 * @param latitude Original lattitude
	 * @param longitude Original lattitude
	 * @param heading Original heading
	 * @param ant_type Type of ant
	 * @param food Initial food
	 * @param memory1 Initial memory1
	 * @param memory2 Initial memory2
	 */
	Ant(Team &team, int stamina, double latitude, double longitude,
	    double heading, uint8_t ant_type = 0, int food = 0, uint8_t memory1 = 0,
	    uint8_t memory2 = 0);

	/** Get category **/
	static GameObject_t category() { return &s_category; }

	/** get ant's stamina **/
	int stamina() { return m_stamina; }

	/** action SET_MEMORY **/
	void actionSetMemory(bool valid, uint8_t m0, uint8_t m1);

	/** action MEMORY **/
	void actionSuicide(bool valid);

	/** action PUT_PHEROMONE **/
	void actionPutPheromone(bool valid, int type);

	/** action CHANGE_PHEROMONE **/
	void actionChangePheromone(bool valid, int type, int id);

	/** action RECHARGE_PHEROMONE **/
	void actionRechargePheromone(bool valid, int id);

	/** action COLLECT **/
	void actionCollect(bool valid, int id, int quantity);

	/** action DO_TROPHALLAXIS **/
	void actionDoTrophallaxis(bool valid, int id, int quantity);

	/** action Explore **/
	void actionExplore(bool valid);

	/** action ATTACK **/
	void actionAttack(bool valid, int id, int force);

	/** action EAT **/
	void actionEat(bool valid, int quantity);

	/** action MOVE_TO **/
	void actionMoveTo(bool valid, int id);

	/** action NEST **/
	void actionNest(bool valid, int id);

	/** action TURN **/
	void actionTurn(bool valid, int angle);

  private:
	/** Stamina **/
	int m_stamina;

	/** Food stock **/
	int m_stock;

	/** Food stock **/
	int m_initial_stock;

	/** Was ant attacked **/
	bool m_attacked;

	/** Type of ant **/
	uint8_t m_ant_type;

	/** Long term memory **/
	uint8_t m_memory[2];

	/** Is exclusive action done in this turn **/
	bool m_exclusiveDone = false;

	/** Signal an invalide action **/
	void invalidAction();

	/** Engine message log function for convenience **/
	std::ostream &w() { return team().log(TeamLogType::ENGINE_MSG); }

	/** List of action types **/
	enum ActionType {
		ALWAYS_ALLOWED,
		EXCLUSIVE,
	};

	/** Prelude of actions (to check state) **/
	bool preludeActionHelper(int cost, ActionType type, bool valid);

	/** Get object by ID
	 * @param id      ID to search
	 * @param invalid (out) true if ID wad invalid, false otherwise
	 * @param cat     (optional) Expected category
	 */
	GameObject *getObjectActionHelper(int id, bool &invalid,
	                                  GameObject_t cat = undefined_category);

	virtual bool prelude(std::ostream &os) override;
	virtual void execute(uint8_t argc, const char **argv) override;

	/** Ant category unique object **/
	static const GameObjectCategory s_category;

	/** Method called periodically by the engine **/
	void periodic() override;
};

#endif // ANT_H
