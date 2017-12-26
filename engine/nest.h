#ifndef NEST_H
#define NEST_H

#include "gears/agent.h"
#include "team.h"
#include <map>

/** Class representing a nest **/
class Nest : public Agent {
  public:
	/** Constructor
	 * @param team             Team whose agent belongs
	 * @param latitude         Lattitude
	 * @param longitude        Longitude
	 */
	Nest(Team &team, double latitude, double longitude);

	/** Get category **/
	static GameObject_t category() { return &s_category; }

	/** Actions when command are wrong **/
	void invalidAction();

	/** Method called periodically by the engine **/
	void periodic() override;

	/** Check if ant of this type already exist **/
	bool hasAntType(uint8_t type);

	/** action ANT_NEW **/
	void actionAntNew(bool valid, uint8_t type);

	/** action ANT_OUT **/
	void actionAntOut(bool valid, uint8_t type, uint8_t m0, uint8_t m1);

	/** action MEMORY **/
	void actionMemory(bool valid, uint8_t mem[20]);

  private:
	/** Stock of the nest**/
	uint32_t m_stock = 0;

	/** Number of ants currently in nest **/
	std::map<uint8_t, uint32_t> m_antNumber;

	/** Things to remember about ants come home since last call to strategy **/
	struct AntIn {
		uint8_t type;
		uint8_t m0;
		uint8_t m1;
	};

	/** List of Ant come home since last call to strategy **/
	std::vector<AntIn> m_antsIn;

	/** Memory **/
	uint8_t m_memory[20];

	virtual bool prelude(std::ostream &os) override;
	virtual void execute(uint8_t argc, const char **argv) override;

	/** Nest category unique object **/
	static const GameObjectCategory s_category;
};

#endif // NEST_H
