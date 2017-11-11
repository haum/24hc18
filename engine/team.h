#ifndef TEAM_H
#define TEAM_H

#include "gears/team_base.h"

class Team : public TeamBase {
  public:
	/** Constructor **/
	using TeamBase::TeamBase;

	/** Print team statistics **/
	void printStats();
};

#endif // TEAM_H
