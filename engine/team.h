#ifndef TEAM_H
#define TEAM_H

#include "gears/team_base.h"

/** Class managing a team (story part) **/
class Team : public TeamBase {
  public:
	/** Constructor **/
	using TeamBase::TeamBase;

	/** Print team statistics **/
	void printStats();
};

#endif // TEAM_H
