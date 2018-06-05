#pragma once
#include "Common.h"

class CJeep
{
public:
	CJeep();
	CJeep(glm::vec3 position, float rotation);
	~CJeep();

	void Update(double dt);

	glm::vec3 GetPosition() const;
	bool GetActive() const;
	float GetRotation() const;

	void SetPosition(glm::vec3 position);
	void SetActive(bool active);
	void SetRotation(float rotation);

private:
	glm::vec3 m_position;
	bool m_active;
	double m_timer;
	float m_rotation;
};