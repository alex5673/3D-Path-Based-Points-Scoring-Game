#include "Torus.h"
CTorus::CTorus()
{
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_rotation = 0.0f;
	m_timer = 0.0;
	m_active = true;

	vertex1 = CVertex(glm::vec3(-1.0f, 2.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::cross(glm::vec3(-2.0f, -4.0f, 0.0f), glm::vec3(1.0f, 2.0f, 0.0f)));
	vertex2 = CVertex(glm::vec3(1.0f, 2.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::cross(glm::vec3(-1.0f, 2.0f, 0.0f), glm::vec3(2.0f, 4.0f, 0.0f)));
	vertex3 = CVertex(glm::vec3(2.0f, 4.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::cross(glm::vec3(1.0f, 2.0f, 0.0f), glm::vec3(-2.0f, 4.0f, 0.0f)));
	vertex4 = CVertex(glm::vec3(-2.0f, 4.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::cross(glm::vec3(-1.0f, 2.0f, 0.0f), glm::vec3(2.0f, 4.0f, 0.0f)));
	vertex5 = CVertex(glm::vec3(1.0f, 2.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::cross(glm::vec3(2.0f, 4.0f, 0.0f), glm::vec3(2.0f, 0.0f, 0.0f)));
	vertex6 = CVertex(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::cross(glm::vec3(4.0f, 0.0f, 0.0f), glm::vec3(1.0f, 2.0f, 0.0f)));
	vertex7 = CVertex(glm::vec3(4.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::cross(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(2.0f, 4.0f, 0.0f)));
	vertex8 = CVertex(glm::vec3(2.0f, 4.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::cross(glm::vec3(4.0f, 0.0f, 0.0f), glm::vec3(1.0f, 2.0f, 0.0f)));
	vertex9 = CVertex(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::cross(glm::vec3(4.0f, 0.0f, 0.0f), glm::vec3(1.0f, -2.0f, 0.0f)));
	vertex10 = CVertex(glm::vec3(1.0f, -2.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::cross(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(2.0f, -4.0f, 0.0f)));
	vertex11 = CVertex(glm::vec3(2.0f, -4.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::cross(glm::vec3(1.0f, -2.0f, 0.0f), glm::vec3(4.0f, 0.0f, 0.0f)));
	vertex12 = CVertex(glm::vec3(4.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::cross(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(2.0f, -4.0f, 0.0f)));
	vertex13 = CVertex(glm::vec3(1.0f, -2.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::cross(glm::vec3(-1.0f, -2.0f, 0.0f), glm::vec3(2.0f, -4.0f, 0.0f)));
	vertex14 = CVertex(glm::vec3(-1.0f, -2.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::cross(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(-2.0f, -4.0f, 0.0f)));
	vertex15 = CVertex(glm::vec3(-2.0f, -4.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::cross(glm::vec3(-1.0f, -2.0f, 0.0f), glm::vec3(2.0f, -4.0f, 0.0f)));
	vertex16 = CVertex(glm::vec3(2.0f, -4.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::cross(glm::vec3(1.0f, -2.0f, 0.0f), glm::vec3(-2.0f, -4.0f, 0.0f)));
	vertex17 = CVertex(glm::vec3(-1.0f, -2.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::cross(glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(-2.0f, -4.0f, 0.0f)));
	vertex18 = CVertex(glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::cross(glm::vec3(-4.0f, 0.0f, 0.0f), glm::vec3(-1.0f, -2.0f, 0.0f)));
	vertex19 = CVertex(glm::vec3(-4.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::cross(glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(-2.0f, -4.0f, 0.0f)));
	vertex20 = CVertex(glm::vec3(-2.0f, -4.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::cross(glm::vec3(-4.0f, 0.0f, 0.0f), glm::vec3(-1.0f, -2.0f, 0.0f)));
	vertex21 = CVertex(glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::cross(glm::vec3(-1.0f, 2.0f, 0.0f), glm::vec3(-4.0f, 0.0f, 0.0f)));
	vertex22 = CVertex(glm::vec3(-1.0f, 2.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::cross(glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(-2.0f, 4.0f, 0.0f)));
	vertex23 = CVertex(glm::vec3(-2.0f, 4.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::cross(glm::vec3(-1.0f, 2.0f, 0.0f), glm::vec3(-4.0f, 0.0f, 0.0f)));
	vertex24 = CVertex(glm::vec3(-4.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::cross(glm::vec3(-2.0f, 4.0f, 0.0f), glm::vec3(-2.0f, 0.0f, 0.0f)));

	vertex25 = CVertex(glm::vec3(-0.5f, 1.0f, 2.0f), glm::vec2(0.0f, 0.0f), glm::cross(glm::vec3(-1.0f, 2.0f, 0.0f), glm::vec3(0.5f, 1.0f, 2.0f)));
	vertex26 = CVertex(glm::vec3(0.5f, 1.0f, 2.0f), glm::vec2(1.0f, 0.0f), glm::cross(glm::vec3(-0.5f, 1.0f, 2.0f), glm::vec3(1.0f, 2.0f, 0.0f)));
	vertex27 = CVertex(glm::vec3(1.0f, 2.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::cross(glm::vec3(0.5f, 1.0f, 2.0f), glm::vec3(-1.0f, 2.0f, 0.0f)));
	vertex28 = CVertex(glm::vec3(-1.0f, 2.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::cross(glm::vec3(1.0f, 2.0f, 0.0f), glm::vec3(-0.5f, 1.0f, 2.0f)));
	vertex29 = CVertex(glm::vec3(0.5f, 1.0f, 2.0f), glm::vec2(0.0f, 0.0f), glm::cross(glm::vec3(1.0f, 2.0f, 0.0f), glm::vec3(1.0f, 0.0f, 2.0f)));
	vertex30 = CVertex(glm::vec3(1.0f, 0.0f, 2.0f), glm::vec2(1.0f, 0.0f), glm::cross(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(0.5f, 1.0f, 2.0f)));
	vertex31 = CVertex(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::cross(glm::vec3(1.0f, 0.0f, 2.0f), glm::vec3(1.0f, 2.0f, 0.0f)));
	vertex32 = CVertex(glm::vec3(1.0f, 2.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::cross(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(0.5f, 1.0f, 2.0f)));
	vertex33 = CVertex(glm::vec3(1.0f, 0.0f, 2.0f), glm::vec2(0.0f, 0.0f), glm::cross(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(0.5f, -1.0f, 2.0f)));
	vertex34 = CVertex(glm::vec3(0.5f, -1.0f, 2.0f), glm::vec2(1.0f, 0.0f), glm::cross(glm::vec3(1.0f, 0.0f, 2.0f), glm::vec3(1.0f, -2.0f, 0.0f)));
	vertex35 = CVertex(glm::vec3(1.0f, -2.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::cross(glm::vec3(0.5f, -1.0f, 2.0f), glm::vec3(2.0f, 0.0f, 0.0f)));
	vertex36 = CVertex(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::cross(glm::vec3(1.0f, -2.0f, 0.0f), glm::vec3(1.0f, 0.0f, 2.0f)));
	vertex37 = CVertex(glm::vec3(0.5f, -1.0f, 2.0f), glm::vec2(0.0f, 0.0f), glm::cross(glm::vec3(1.0f, -2.0f, 0.0f), glm::vec3(0.5f, -1.0f, 2.0f)));
	vertex38 = CVertex(glm::vec3(-0.5f, -1.0f, 2.0f), glm::vec2(1.0f, 0.0f), glm::cross(glm::vec3(0.5f, -1.0f, 2.0f), glm::vec3(-1.0f, -2.0f, 0.0f)));
	vertex39 = CVertex(glm::vec3(-1.0f, -2.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::cross(glm::vec3(-0.5f, -1.0f, 2.0f), glm::vec3(1.0f, -2.0f, 0.0f)));
	vertex40 = CVertex(glm::vec3(1.0f, -2.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::cross(glm::vec3(-1.0f, -2.0f, 0.0f), glm::vec3(-0.5f, -1.0f, 2.0f)));
	vertex41 = CVertex(glm::vec3(-0.5f, -1.0f, 2.0f), glm::vec2(0.0f, 0.0f), glm::cross(glm::vec3(-1.0f, 0.0f, 2.0f), glm::vec3(-1.0f, -2.0f, 0.0f)));
	vertex42 = CVertex(glm::vec3(-1.0f, 0.0f, 2.0f), glm::vec2(1.0f, 0.0f), glm::cross(glm::vec3(-0.5f, -1.0f, 2.0f), glm::vec3(-2.0f, 0.0f, 0.0f)));
	vertex43 = CVertex(glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::cross(glm::vec3(-1.0f, -2.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 2.0f)));
	vertex44 = CVertex(glm::vec3(-1.0f, -2.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::cross(glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 2.0f)));
	vertex45 = CVertex(glm::vec3(-1.0f, 0.0f, 2.0f), glm::vec2(0.0f, 0.0f), glm::cross(glm::vec3(-0.5f, 1.0f, 2.0f), glm::vec3(-2.0f, 0.0f, 0.0f)));
	vertex46 = CVertex(glm::vec3(-0.5f, 1.0f, 2.0f), glm::vec2(1.0f, 0.0f), glm::cross(glm::vec3(-1.0f, 0.0f, 2.0f), glm::vec3(-1.0f, 2.0f, 0.0f)));
	vertex47 = CVertex(glm::vec3(-1.0f, 2.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::cross(glm::vec3(-0.5f, 1.0f, 2.0f), glm::vec3(-2.0f, 0.0f, 0.0f)));
	vertex48 = CVertex(glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::cross(glm::vec3(-1.0f, 2.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 2.0f)));
	
	vertices.push_back(vertex1);
	vertices.push_back(vertex2);
	vertices.push_back(vertex4);
	vertices.push_back(vertex3);
	vertices.push_back(vertex5);
	vertices.push_back(vertex6);
	vertices.push_back(vertex8);
	vertices.push_back(vertex7);
	vertices.push_back(vertex9);
	vertices.push_back(vertex10);
	vertices.push_back(vertex12);
	vertices.push_back(vertex11);
	vertices.push_back(vertex13);
	vertices.push_back(vertex14);
	vertices.push_back(vertex16);
	vertices.push_back(vertex15);
	vertices.push_back(vertex17);
	vertices.push_back(vertex18);
	vertices.push_back(vertex20);
	vertices.push_back(vertex19);
	vertices.push_back(vertex21);
	vertices.push_back(vertex22);
	vertices.push_back(vertex24);
	vertices.push_back(vertex23);

	
	vertices.push_back(vertex25);
	vertices.push_back(vertex26);
	vertices.push_back(vertex28);
	vertices.push_back(vertex27);
	vertices.push_back(vertex29);
	vertices.push_back(vertex30);
	vertices.push_back(vertex32);
	vertices.push_back(vertex31);
	vertices.push_back(vertex33);
	vertices.push_back(vertex34);
	vertices.push_back(vertex36);
	vertices.push_back(vertex35);
	vertices.push_back(vertex37);
	vertices.push_back(vertex38);
	vertices.push_back(vertex40);
	vertices.push_back(vertex39);
	vertices.push_back(vertex41);
	vertices.push_back(vertex42);
	vertices.push_back(vertex44);
	vertices.push_back(vertex43);
	vertices.push_back(vertex45);
	vertices.push_back(vertex46);
	vertices.push_back(vertex48);
	vertices.push_back(vertex47);
}

CTorus::~CTorus()
{
	Release();
}

void CTorus::Create(string filename)
{
	m_texture.Load(filename);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	m_vbo.Create();
	m_vbo.Bind();

	// Write the code to add interleaved vertex attributes to the VBO
	for (unsigned int i = 0; i < 24; i++) {
		m_vbo.AddData(&vertices[i].GetVertexPos(), sizeof(glm::vec3));
		m_vbo.AddData(&vertices[i].GetVertexTexCoord(), sizeof(glm::vec2));
		m_vbo.AddData(&vertices[i].GetVertexNorm(), sizeof(glm::vec3));
	}

	// Upload data to GPU
	m_vbo.UploadDataToGPU(GL_STATIC_DRAW);
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride,
		(void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}

void CTorus::Render()
{
	glBindVertexArray(m_vao);
	m_texture.Bind();
	// Call glDrawArrays to render each side
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);

	glDrawArrays(GL_TRIANGLE_STRIP, 24, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 28, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 32, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 36, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 40, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 44, 4);
}

void CTorus::Release()
{
	m_texture.Release();
	glDeleteVertexArrays(1, &m_vao);
	m_vbo.Release();
}


void CTorus::Update(double dt)
{
	m_timer += dt;
	if (m_timer > 20000)
	{
		m_active = false;
	}
}

glm::vec3 CTorus::GetPosition() const
{
	return m_position;
}

bool CTorus::GetActive() const
{
	return m_active;
}

float CTorus::GetRotation() const
{
	return m_rotation;
}

void CTorus::SetPosition(glm::vec3 position)
{
	m_position = position;
}

void CTorus::SetActive(bool active)
{
	m_active = active;
}

void CTorus::SetRotation(float rotation)
{
	m_rotation = rotation;
}