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

	/** Get food quantity **/
	int available() const { return m_available; }

	/** Get category **/
	static GameObject_t category() { return &s_category; }

  private:
	/** Food category unique object **/
	static const GameObjectCategory s_category;

	/** available food **/
	int m_available{0};
};

#endif // FOOD_H
