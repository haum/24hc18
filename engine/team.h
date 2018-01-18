#ifndef TEAM_H
#define TEAM_H

#include "gears/gameobject.h"
#include "gears/team_base.h"

class Nest;

/** Class managing a team (story part) **/
class Team : public TeamBase {
  public:
	/** Constructor **/
	using TeamBase::TeamBase;

	/** Get ID **/
	const std::vector<GameObject *> &getIds() const { return m_ids; }

	/** Add ID **/
	size_t addId(GameObject *go) {
		m_ids.push_back(go);
		return m_ids.size();
	}

	/* Get nest **/
	Nest *nest();

	/** Compute score **/
	size_t score();

	/** Reset IDs **/
	void resetIds() { m_ids.clear(); }

	/** Print team statistics **/
	void printStats(bool apioutput, size_t score_max);

  protected:
	std::vector<GameObject *> m_ids;
};

#endif // TEAM_H
