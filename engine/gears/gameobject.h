#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>

/** Description of type of game objects **/
class GameObjectCategory {
  public:
	GameObjectCategory(std::string name) : m_name{name} {}
	std::string name() const { return m_name; }

  private:
	std::string m_name;
};

typedef const GameObjectCategory *GameObject_t;

/** Game object base class (stores position) **/
class GameObject {
  public:
	constexpr GameObject(GameObject &) = default;
	constexpr GameObject(GameObject &&) = default;
	virtual ~GameObject();

	double latitude() { return m_latitude; }
	double longitude() { return m_longitude; }
	double heading() { return m_heading; }
	GameObject_t type() const { return m_type; }

	virtual void periodic() {}

  protected:
	GameObject(GameObject_t type, double latitude, double longitude,
			   double heading);
	double m_latitude, m_longitude, m_heading;
	GameObject_t m_type;
};

#endif // GAMEOBJECT_H
