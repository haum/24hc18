#ifndef PHEROMONE_H
#define PHEROMONE_H

#include "gears/gameobject.h"

/** Class representing a pheromone **/
class Pheromone : public GameObject {
  public:
	/** Constructor
	 * @param latitude Original lattitude
	 * @param longitude Original lattitude
	 */
	Pheromone(double latitude, double longitude);
};

#endif // PHEROMONE_H
