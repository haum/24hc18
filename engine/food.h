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

	/** Get category **/
	GameObject_t category() const { return &s_category; }

  private:
	static const GameObjectCategory s_category;
};

#endif // FOOD_H
