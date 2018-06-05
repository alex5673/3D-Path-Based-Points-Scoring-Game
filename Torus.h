#pragma once

#include "Common.h"
#include "Texture.h"
#include "VertexBufferObject.h"
#include "Vertex.h"
// Class for generating a unit cube
class CTorus
{
public:
	CTorus();
	~CTorus();
	void Create(string filename);
	void Render();
	void Release();

	void Update(double dt);

	glm::vec3 GetPosition() const;
	bool GetActive() const;
	float GetRotation() const;

	void SetPosition(glm::vec3 position);
	void SetActive(bool active);
	void SetRotation(float rotation);

private:
	GLuint m_vao;
	CVertexBufferObject m_vbo;
	CTexture m_texture;

	glm::vec3 m_position;
	bool m_active;
	double m_timer;
	float m_rotation;

	CVertex vertex1;
	CVertex vertex2;
	CVertex vertex3;
	CVertex vertex4;
	CVertex vertex5;
	CVertex vertex6;
	CVertex vertex7;
	CVertex vertex8;
	CVertex vertex9;
	CVertex vertex10;
	CVertex vertex11;
	CVertex vertex12;
	CVertex vertex13;
	CVertex vertex14;
	CVertex vertex15;
	CVertex vertex16;
	CVertex vertex17;
	CVertex vertex18;
	CVertex vertex19;
	CVertex vertex20;
	CVertex vertex21;
	CVertex vertex22;
	CVertex vertex23;
	CVertex vertex24;
	CVertex vertex25;
	CVertex vertex26;
	CVertex vertex27;
	CVertex vertex28;
	CVertex vertex29;
	CVertex vertex30;
	CVertex vertex31;
	CVertex vertex32;
	CVertex vertex33;
	CVertex vertex34;
	CVertex vertex35;
	CVertex vertex36;
	CVertex vertex37;
	CVertex vertex38;
	CVertex vertex39;
	CVertex vertex40;
	CVertex vertex41;
	CVertex vertex42;
	CVertex vertex43;
	CVertex vertex44;
	CVertex vertex45;
	CVertex vertex46;
	CVertex vertex47;
	CVertex vertex48;

	vector<CVertex> vertices;
};
