#ifndef SCENARIO_H
#define SCENARIO_H

#include "gears/scenario_base.h"
#include <stdint.h>

class Scenario : public ScenarioBase {
  public:
	/** Constructor **/
	using ScenarioBase::ScenarioBase;

	/** Process one line of scenario **/
	void processLine(uint8_t argc, const char **argv);
};

#endif // SCENARIO_H
