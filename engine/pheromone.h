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
	GameObject_t category() const { return &s_category; }

  private:
	static const GameObjectCategory s_category;
};

#endif // PHEROMONE_H
