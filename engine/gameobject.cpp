#include "gameobject.h"

GameObject::GameObject(GameObject_t type, double latitude, double longitude,
					   double heading)
	: m_latitude{latitude},
	  m_longitude{longitude}, m_heading{heading}, m_type{type} {}

GameObject::~GameObject() {}
