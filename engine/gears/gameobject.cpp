#include "gameobject.h"
#include <math.h>

GameObject::GameObject(GameObject_t category, double latitude, double longitude,
					   double heading)
	: m_latitude{latitude}, m_longitude{longitude}, m_heading{heading},
	  m_category{category} {}

GameObject::~GameObject() {}

double GameObject::distance(const GameObject &go) const {
	double d = cos(latitude()) * cos(go.latitude());
	d *= 0.5 * (1 - cos(longitude() - go.longitude()));
	d += 0.5 * (1 - cos(latitude() - go.latitude()));
	return 2 * asin(sqrt(d));
}
