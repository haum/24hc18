#ifndef FOOD_H
#define FOOD_H

#include "gears/gameobject.h"

/** Class representing a food source **/
class Food : public GameObject {
  public:
	/** Constructor
	 * @param latitude Original lattitude
	 * @param longitude Original lattitude
	 */
	Food(double latitude, double longitude);

	/** Get category **/
	static GameObject_t category() { return &s_category; }

  private:
	/** Food category unique object **/
	static const GameObjectCategory s_category;
};

#endif // FOOD_H
