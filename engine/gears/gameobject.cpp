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

void GameObject::moveDistance(double distance) {
	// See https://www.movable-type.co.uk/scripts/latlong.html#destPoint
	double ini_latitude = m_latitude;
	double ini_longitude = m_longitude;
	m_latitude = asin(sin(m_latitude) * cos(distance) +
	                  cos(m_latitude) * sin(distance) * cos(m_heading));
	m_longitude += atan2(sin(m_heading) * sin(distance) * cos(ini_latitude),
	                     cos(distance) - sin(ini_latitude) * sin(m_latitude));
	m_heading = fmod(atan2(sin(ini_longitude - m_longitude) * cos(ini_latitude),
	                       cos(m_latitude) * sin(ini_latitude) -
	                           sin(m_latitude) * cos(ini_latitude) *
	                               cos(ini_longitude - m_longitude)) +
	                     M_PI,
	                 2 * M_PI);
}
