#pragma once

#include "Common.h"

class CVertex 
{
public:
	CVertex();
	CVertex(const glm::vec3 &pos, const glm::vec2 &texCoord, const glm::vec3 &norm);
	~CVertex();

	glm::vec3 GetVertexPos() const;
	glm::vec2 GetVertexTexCoord() const;
	glm::vec3 GetVertexNorm() const;

	glm::vec3& GetVertexPosRef();
	glm::vec2& GetVertexTexCoordRef();
	glm::vec3& GetVertexNormRef();

	void SetVertexPos(glm::vec3 vertexPos);
	void SetVertexTexCoord(glm::vec2 vertexTexCoord);
	void SetVertexNorm(glm::vec3 vertexNorm);

private:
	glm::vec3 m_vertexPos;
	glm::vec2 m_vertexTexCoord;
	glm::vec3 m_vertexNorm;
};