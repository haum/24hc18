#ifndef PHEROMONE_H
#define PHEROMONE_H

#include "gears/gameobject.h"
#include "team.h"

/** Class representing a pheromone **/
class Pheromone : public GameObject {
  public:
	/** Constructor
	 * @param latitude Original lattitude
	 * @param longitude Original lattitude
	 */
	Pheromone(double latitude, double longitude, Team &team, int type);

	/** Get category **/
	static GameObject_t category() { return &s_category; }

	/** Get pheromone team **/
	Team &team() { return m_team; }

	/** Get pheromone type **/
	int type() { return m_type; }

	/** Get pheromone life **/
	int life() { return m_life; }

	/** Set pheromone life **/
	void setLife(int life) { this->m_life = life; }

	/** Set category **/
	void setCategory(int type) {
		this->m_life = 100;
		this->m_type = type;
	}

	/** Method called periodically by the engine **/
	void periodic() override;

  private:
	/** Pheromone category unique object **/
	static const GameObjectCategory s_category;

	/** Life of a pheromone **/
	int m_life;

	/** Team of a pheromone **/
	Team &m_team;

	/** Type of a pheromone **/
	int m_type;
};

#endif // PHEROMONE_H
