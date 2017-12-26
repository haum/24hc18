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

	/** Create a new ant in the nest **/
	void actionAntNew(bool valid, uint8_t type);

	/** An ant already created get out the nest **/
	void actionAntOut(bool valid, uint8_t type, uint8_t m0, uint8_t m1);

	/** Check if ant of this type already exist **/
	bool checkAntType(uint8_t type) {
		if (m_antNumber.count(type) > 0) {
			return true;
		}
		return false;
	}

	/** Add or Remove ants **/
	void antQuantityChanger(uint8_t type, int changeValue) {
		m_antNumber[type] = m_antNumber[type] + changeValue;
	}

	/** Set new type of ant **/
	void addNewAntType(uint8_t type) { m_antNumber[type] = 1; }

	/** Return how many ant of a type there are **/
	int getAntNumberOfType(uint8_t type) {
		if (!checkAntType(type)) {
			addNewAntType(type);
			antQuantityChanger(type, -1);
		}
		return m_antNumber[type];
	}

  private:
	/** Stock of the nest**/
	int m_stock = 0;

	/** Number of ants currently in nest **/
	std::map<uint8_t, uint32_t> m_antNumber;

	virtual bool prelude(std::ostream &os) override;
	virtual void execute(uint8_t argc, const char **argv) override;

	/** Nest category unique object **/
	static const GameObjectCategory s_category;
};

#endif // NEST_H
