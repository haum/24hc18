#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

/** Enumeration of type of game objects **/
enum GameObject_t {
	NEST,
	ANT,
};

/** Game object base class (stores position) **/
class GameObject {
  public:
	constexpr GameObject(GameObject &) = default;
	constexpr GameObject(GameObject &&) = default;
	virtual ~GameObject();

	double latitude() { return m_latitude; }
	double longitude() { return m_longitude; }
	double heading() { return m_heading; }

	virtual void periodic() {}

  protected:
	GameObject(GameObject_t type, double latitude, double longitude,
			   double heading);
	double m_latitude, m_longitude, m_heading;
	GameObject_t m_type;
};

#endif // GAMEOBJECT_H
