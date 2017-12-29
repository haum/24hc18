#ifndef FOOD_H
#define FOOD_H

#include "gears/gameobject.h"

/** Class representing a food source **/
class Food : public GameObject {
  public:
	/** Constructor
	 * @param latitude       Lattitude
	 * @param longitude      Lattitude
	 * @param initialValue   Initial amount
	 * @param chargeRate     Charge rate
	 * @param chargeMax      Maximum charge
	 * @param totalChargeMax Maximum charge in entire life
	 * @param deadIfEmpty    Should stop recharge when empty
	 */
	Food(double latitude, double longitude, float initialValue,
	     float chargeRate, int chargeMax, float totalChargeMax,
	     bool deadIfEmpty);

	/** Get food quantity **/
	int available() const { return static_cast<int>(m_available); }

	/** Eat food
	 * @param quantity Requested quantity
	 * @return Eaten quantity
	 */
	int eat(int quantity);

	/** Get category **/
	static GameObject_t category() { return &s_category; }

  private:
	/** Method called periodically by the engine **/
	void periodic() override;

	/** Food category unique object **/
	static const GameObjectCategory s_category;

	/** available food **/
	float m_available;

	/** Charge rate **/
	float m_chargeRate;

	/** Maximum charge **/
	int m_chargeMax;

	/** Maximum charge in whole life **/
	float m_totalChargeMax;

	/** Die if empty **/
	bool m_deadIfEmpty;
};

#endif // FOOD_H
