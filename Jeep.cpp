#include "Jeep.h"

CJeep::CJeep() {};

CJeep::CJeep(glm::vec3 position, float rotation) 
{
	m_position = position;
	m_rotation = rotation;
	m_timer = 0.0;
	m_active = true;
}

CJeep::~CJeep() 
{
};

void CJeep::Update(double dt)
{
	m_timer += dt;
	if (m_timer > 20000) 
	{
		m_active = false;
	}
}

glm::vec3 CJeep::GetPosition() const
{
	return m_position;
}

bool CJeep::GetActive() const
{
	return m_active;
}

float CJeep::GetRotation() const
{
	return m_rotation;
}

void CJeep::SetPosition(glm::vec3 position) 
{
	m_position = position;
}

void CJeep::SetActive(bool active)
{
	m_active = active;
}

void CJeep::SetRotation(float rotation)
{
	m_rotation = rotation;
}