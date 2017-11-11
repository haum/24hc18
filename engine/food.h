#ifndef FOOD_H
#define FOOD_H

#include "gears/gameobject.h"

class Food : public GameObject {
  public:
	/** Constructor
	 * @param latitude Original lattitude
	 * @param longitude Original lattitude
	 */
	Food(double latitude, double longitude);
};

#endif // FOOD_H
