#ifndef NEST_H
#define NEST_H

#include "agent.h"
#include "gameobject.h"

/** Class representing a nest **/
class Nest : public Agent {
  public:
	/** Constructor
	 * @param team             Team whose agent belongs
	 * @param latitude         Lattitude
	 * @param longitude        Longitude
	 * @param initialAntNumber Initial number of ants in nest
	 */
	Nest(Team &team, double latitude, double longitude,
		 uint64_t initialAntNumber);

  private:
	/** Number of  ants currently in nest **/
	uint64_t m_antNumber;

	virtual bool prelude(std::ostream &os) override;
	virtual void execute(uint8_t argc, const char **argv) override;
};

#endif // NEST_H
