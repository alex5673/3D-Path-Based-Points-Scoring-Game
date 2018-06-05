#include "Vertex.h"

CVertex::CVertex() 
{
	m_vertexPos = glm::vec3(0.0f, 0.0f, 0.0f);
	m_vertexTexCoord = glm::vec2(0.0f, 0.0f);
	m_vertexNorm = glm::vec3(0.0f, 0.0f, 0.0f);
}

CVertex::CVertex(const glm::vec3 &pos, const glm::vec2 &texCoord, const glm::vec3 &norm)
{
	m_vertexPos = pos;
	m_vertexTexCoord = texCoord;
	m_vertexNorm = norm;
}

CVertex::~CVertex() {}

glm::vec3 CVertex::GetVertexPos() const
{
	return m_vertexPos;
}

glm::vec2 CVertex::GetVertexTexCoord() const
{
	return m_vertexTexCoord;
}

glm::vec3 CVertex::GetVertexNorm() const
{
	return m_vertexNorm;
}

glm::vec3& CVertex::GetVertexPosRef()
{
	return m_vertexPos;
}

glm::vec2& CVertex::GetVertexTexCoordRef()
{
	return m_vertexTexCoord;
}

glm::vec3& CVertex::GetVertexNormRef()
{
	return m_vertexNorm;
}

void CVertex::SetVertexPos(glm::vec3 vertexPos)
{
	m_vertexPos = vertexPos;
}

void CVertex::SetVertexTexCoord(glm::vec2 vertexTexCoord)
{
	m_vertexTexCoord = vertexTexCoord;
}

void CVertex::SetVertexNorm(glm::vec3 vertexNorm)
{
	m_vertexNorm = vertexNorm;
}