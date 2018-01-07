#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "team_base.h"
#include <string>

/** Description of type of game objects **/
class GameObjectCategory {
  public:
	/** Constructor
	 * @param name Human readable name of object type
	 */
	GameObjectCategory(std::string name) : m_name{name} {}

	/** Get name **/
	std::string name() const { return m_name; }

  private:
	/** Type human readable name **/
	std::string m_name;
};

typedef const GameObjectCategory *GameObject_t;
constexpr const GameObject_t undefined_category = nullptr;

/** Game object base class (stores position) **/
class GameObject {
  public:
	/** Default constructors **/
	constexpr GameObject(GameObject &) = default;
	constexpr GameObject(GameObject &&) = default;

	/** Destructor **/
	virtual ~GameObject();

	/** Get latitude **/
	double latitude() const { return m_latitude; }

	/** Get longitude **/
	double longitude() const { return m_longitude; }

	/** Get heading **/
	double heading() const { return m_heading; }

	/** Move object of distance in current heading **/
	void moveDistance(double distance);

	/** Orient toward another object **/
	void orientToward(const GameObject &go);

	/** Get type **/
	GameObject_t category() const { return m_category; }

	/** Get team base **/
	TeamBase *teamBase() const { return m_teamBase; }

	/** Compute distance to another object
	 * @note The computed distance is the angle of the shorstest path in
	 *       radians. It shoud be multiplied by globe radius to get actual
	 *       distance on the surface.
	 */
	double distance(const GameObject &) const;

	/** Method called periodically by the engine **/
	virtual void periodic() {}

  protected:
	/** Constructor
	 * @param category  Category of game object
	 * @param latitude  Initial latitude
	 * @param longitude Initial longitude
	 * @param heading   Initial heading
	 */
	GameObject(GameObject_t category, double latitude, double longitude,
	           double heading);

	/** Latitude **/
	double m_latitude;

	/** Longitude **/
	double m_longitude;

	/** Heading **/
	double m_heading;

	/** Type **/
	GameObject_t m_category;

	/** Team base pointer **/
	TeamBase *m_teamBase{nullptr};
};

#endif // GAMEOBJECT_H
