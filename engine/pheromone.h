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

	/** Get category **/
	static GameObject_t category() { return &s_category; }

  private:
	/** Pheromone category unique object **/
	static const GameObjectCategory s_category;
};

#endif // PHEROMONE_H
