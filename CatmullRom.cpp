#include "CatmullRom.h"
#define _USE_MATH_DEFINES
#include <math.h>

CCatmullRom::CCatmullRom()
{
	m_vertexCount = 0;
}

CCatmullRom::~CCatmullRom()
{}

// Perform Catmull Rom spline interpolation between four points, interpolating the space between p1 and p2
glm::vec3 CCatmullRom::Interpolate(glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3, float t)
{
	float t2 = t * t;
	float t3 = t2 * t;

	glm::vec3 a = p1;
	glm::vec3 b = 0.5f * (-p0 + p2);
	glm::vec3 c = 0.5f * (2.0f*p0 - 5.0f*p1 + 4.0f*p2 - p3);
	glm::vec3 d = 0.5f * (-p0 + 3.0f*p1 - 3.0f*p2 + p3);

	return a + b * t + c * t2 + d * t3;

}


void CCatmullRom::SetControlPoints()
{
	// Set control points (m_controlPoints) here, or load from disk

	// Optionally, set upvectors (m_controlUpVectors, one for each control point as well)

	m_controlPoints.push_back(glm::vec3(10, 5, 0));
	m_controlPoints.push_back(glm::vec3(150, 5, -300));
	m_controlPoints.push_back(glm::vec3(400, 5, 100));
	m_controlPoints.push_back(glm::vec3(700, 5, -500));
	m_controlPoints.push_back(glm::vec3(800, 5, 200));
	m_controlPoints.push_back(glm::vec3(500, 5, 150));
	m_controlPoints.push_back(glm::vec3(-400, 5, 600));
	m_controlPoints.push_back(glm::vec3(-500, 5, 200));

}


// Determine lengths along the control points, which is the set of control points forming the closed curve
void CCatmullRom::ComputeLengthsAlongControlPoints()
{
	int M = (int)m_controlPoints.size();

	float fAccumulatedLength = 0.0f;
	m_distances.push_back(fAccumulatedLength);
	for (int i = 1; i < M; i++) {
		fAccumulatedLength += glm::distance(m_controlPoints[i - 1], m_controlPoints[i]);
		m_distances.push_back(fAccumulatedLength);
	}

	// Get the distance from the last point to the first
	fAccumulatedLength += glm::distance(m_controlPoints[M - 1], m_controlPoints[0]);
	m_distances.push_back(fAccumulatedLength);
}


// Return the point (and upvector, if control upvectors provided) based on a distance d along the control polygon
bool CCatmullRom::Sample(float d, glm::vec3 &p, glm::vec3 &up)
{
	if (d < 0)
		return false;

	int M = (int)m_controlPoints.size();
	if (M == 0)
		return false;


	float fTotalLength = m_distances[m_distances.size() - 1];

	// The the current length along the control polygon; handle the case where we've looped around the track
	float fLength = d - (int)(d / fTotalLength) * fTotalLength;

	// Find the current segment
	int j = -1;
	for (int i = 0; i < (int)m_distances.size(); i++) {
		if (fLength >= m_distances[i] && fLength < m_distances[i + 1]) {
			j = i; // found it!
			break;
		}
	}

	if (j == -1)
		return false;

	// Interpolate on current segment -- get t
	float fSegmentLength = m_distances[j + 1] - m_distances[j];
	float t = (fLength - m_distances[j]) / fSegmentLength;

	// Get the indices of the four points along the control polygon for the current segment
	int iPrev = ((j - 1) + M) % M;
	int iCur = j;
	int iNext = (j + 1) % M;
	int iNextNext = (j + 2) % M;

	// Interpolate to get the point (and upvector)
	p = Interpolate(m_controlPoints[iPrev], m_controlPoints[iCur], m_controlPoints[iNext], m_controlPoints[iNextNext], t);
	if (m_controlUpVectors.size() == m_controlPoints.size())
		up = glm::normalize(Interpolate(m_controlUpVectors[iPrev], m_controlUpVectors[iCur], m_controlUpVectors[iNext], m_controlUpVectors[iNextNext], t));


	return true;
}

// Sample a set of control points using an open Catmull-Rom spline, to produce a set of iNumSamples that are (roughly) equally spaced
void CCatmullRom::UniformlySampleControlPoints(int numSamples)
{
	glm::vec3 p, up;

	// Compute the lengths of each segment along the control polygon, and the total length
	ComputeLengthsAlongControlPoints();
	float fTotalLength = m_distances[m_distances.size() - 1];

	// The spacing will be based on the control polygon
	float fSpacing = fTotalLength / numSamples;

	// Call PointAt to sample the spline, to generate the points
	for (int i = 0; i < numSamples; i++) {
		Sample(i * fSpacing, p, up);
		m_centrelinePoints.push_back(p);
		if (m_controlUpVectors.size() > 0)
			m_centrelineUpVectors.push_back(up);

	}


	// Repeat once more for truly equidistant points
	m_controlPoints = m_centrelinePoints;
	m_controlUpVectors = m_centrelineUpVectors;
	m_centrelinePoints.clear();
	m_centrelineUpVectors.clear();
	m_distances.clear();
	ComputeLengthsAlongControlPoints();
	fTotalLength = m_distances[m_distances.size() - 1];
	fSpacing = fTotalLength / numSamples;
	for (int i = 0; i < numSamples; i++) {
		Sample(i * fSpacing, p, up);
		m_centrelinePoints.push_back(p);
		if (m_controlUpVectors.size() > 0)
			m_centrelineUpVectors.push_back(up);
	}

}

void CCatmullRom::CreateCentreline()
{
	// Call Set Control Points
	SetControlPoints();

	// Call UniformlySampleControlPoints with the number of samples required
	UniformlySampleControlPoints(500);

	// Create a VAO called m_vaoCentreline and a VBO to get the points onto the graphics card
	// Use VAO to store state associated with vertices
	glGenVertexArrays(1, &m_vaoCentreline);
	glBindVertexArray(m_vaoCentreline);
	// Create a VBO
	CVertexBufferObject vbo;
	vbo.Create();
	vbo.Bind();
	glm::vec2 texCoord(0.0f, 0.0f);
	glm::vec3 normal(0.0f, 1.0f, 0.0f);
	for (unsigned int p = 0; p < m_centrelinePoints.size()/*p <= (0 + (4 * ((m_controlPoints.size() / 4) - 1)))*/; p++/*p + 4*/) {
		//for (unsigned int i = 0; i < 100; i++) {
			//float t = (float)i / 100.0f;
			//glm::vec3 v = Interpolate(m_controlPoints[p], m_controlPoints[p+1], m_controlPoints[p+2], m_controlPoints[p+3], 1.0f);
			vbo.AddData(&m_centrelinePoints[p], sizeof(glm::vec3));
			vbo.AddData(&texCoord, sizeof(glm::vec2));
			vbo.AddData(&normal, sizeof(glm::vec3));
		//}
	}
	// Upload the VBO to the GPU
	vbo.UploadDataToGPU(GL_STATIC_DRAW);
	// Set the vertex attribute locations
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
		+ sizeof(glm::vec2)));
}


void CCatmullRom::CreateOffsetCurves()
{
	// Compute the offset curves, one left, and one right.  Store the points in m_leftOffsetPoints and m_rightOffsetPoints respectively
	for (unsigned int i = 0; i < m_centrelinePoints.size() - 1; i++)
	{
		// work out current point on the path the camera should be at
		glm::vec3 p = m_centrelinePoints[i];

		// work out a point further up the path to work out the direction the path is going
		glm::vec3 pNext = m_centrelinePoints[i + 1];

		// work out the tangent between the camera postion and the direction the path is going - to find the direction the camera should be pointed in
		glm::vec3 T = pNext - p;
		T = glm::normalize(T);

		// N vector in TNB frame - the vector going left and right
		glm::vec3 N = glm::cross(T, glm::vec3(0.0f, 1.0f, 0.0f));
		N = glm::normalize(N);

		// B vector in TNB frame - the vector going up and down
		glm::vec3 B = glm::cross(N, T);
		B = glm::normalize(B);

		// work out left and right points from the centreline to create a track width
		glm::vec3 l = p - ((80.0f / 2.0f) * N);
		glm::vec3 r = p + ((80.0f / 2.0f) * N);

		// store the left and right points along the centrline
		m_leftOffsetPoints.push_back(l);
		m_rightOffsetPoints.push_back(r);
	}

	// Generate two VAOs called m_vaoLeftOffsetCurve and m_vaoRightOffsetCurve, each with a VBO, and get the offset curve points on the graphics card
	// Note it is possible to only use one VAO / VBO with all the points instead.
	
	// VAO for left offset curve
	glGenVertexArrays(1, &m_vaoLeftOffsetCurve);
	glBindVertexArray(m_vaoLeftOffsetCurve);
	// Create a VBO
	CVertexBufferObject vbo;
	vbo.Create();
	vbo.Bind();
	glm::vec2 texCoord(0.0f, 0.0f);
	glm::vec3 normal(0.0f, 1.0f, 0.0f);
	for (unsigned int p = 0; p < m_leftOffsetPoints.size(); p++) {
		vbo.AddData(&m_leftOffsetPoints[p], sizeof(glm::vec3));
		vbo.AddData(&texCoord, sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));
	}
	// Upload the VBO to the GPU
	vbo.UploadDataToGPU(GL_STATIC_DRAW);
	// Set the vertex attribute locations
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
		+ sizeof(glm::vec2)));


	// VAO for right offset curve
	glGenVertexArrays(1, &m_vaoRightOffsetCurve);
	glBindVertexArray(m_vaoRightOffsetCurve);
	// Create a VBO
	CVertexBufferObject vbo2;
	vbo2.Create();
	vbo2.Bind();
	glm::vec2 texCoord2(0.0f, 0.0f);
	glm::vec3 normal2(0.0f, 1.0f, 0.0f);
	for (unsigned int p = 0; p < m_rightOffsetPoints.size(); p++) {
		vbo2.AddData(&m_rightOffsetPoints[p], sizeof(glm::vec3));
		vbo2.AddData(&texCoord2, sizeof(glm::vec2));
		vbo2.AddData(&normal2, sizeof(glm::vec3));
	}
	// Upload the VBO to the GPU
	vbo2.UploadDataToGPU(GL_STATIC_DRAW);
	// Set the vertex attribute locations
	GLsizei stride2 = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride2, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride2, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride2, (void*)(sizeof(glm::vec3)
		+ sizeof(glm::vec2)));
}

void CCatmullRom::CreateTrack(string filename)
{
	// Load texture
	m_texture.Load(filename);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Generate a VAO called m_vaoTrack and a VBO to get the offset curve points and indices on the graphics card
	glGenVertexArrays(1, &m_vaoTrack);
	glBindVertexArray(m_vaoTrack);
	// Create a VBO
	CVertexBufferObject vbo;
	vbo.Create();
	vbo.Bind();
	glm::vec2 texCoord(0.0f, 0.0f);
	glm::vec3 normal(0.0f, 1.0f, 0.0f);
	for (unsigned int p = 0; p < m_rightOffsetPoints.size(); p++) {
		vbo.AddData(&m_leftOffsetPoints[p], sizeof(glm::vec3));
		if ((p % 2) == 1) {
			texCoord = glm::vec2(0.0f, 0.0f);
		}
		else {
			texCoord = glm::vec2(0.0f, 1.0f);
		}
		vbo.AddData(&texCoord, sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));
		vbo.AddData(&m_rightOffsetPoints[p], sizeof(glm::vec3));
		if ((p % 2) == 1) {
			texCoord = glm::vec2(1.0f, 0.0f);
		}
		else {
			texCoord = glm::vec2(1.0f, 1.0f);
		}
		vbo.AddData(&texCoord, sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));
		m_vertexCount += 2;
	}
	unsigned int p2 = 0;
	// Add the first two points so that the path goes back to the start
	vbo.AddData(&m_leftOffsetPoints[p2], sizeof(glm::vec3));
	//texCoord = glm::vec2(0.0f, 1.0f);
	vbo.AddData(&texCoord, sizeof(glm::vec2));
	vbo.AddData(&normal, sizeof(glm::vec3));
	vbo.AddData(&m_rightOffsetPoints[p2], sizeof(glm::vec3));
	//texCoord = glm::vec2(1.0f, 1.0f);
	vbo.AddData(&texCoord, sizeof(glm::vec2));
	vbo.AddData(&normal, sizeof(glm::vec3));

	// Upload the VBO to the GPU
	vbo.UploadDataToGPU(GL_STATIC_DRAW);
	// Set the vertex attribute locations
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
		+ sizeof(glm::vec2)));
}


void CCatmullRom::RenderCentreline()
{
	// Bind the VAO m_vaoCentreline and render it
	glLineWidth(5.0f);
	glBindVertexArray(m_vaoCentreline);
	glColor3f(1.0f, 1.0f, 1.0f);
	//glDrawArrays(GL_LINE_STRIP, 0, 100);

	
	glDrawArrays(GL_LINE_LOOP, 0, m_centrelinePoints.size());
	glDrawArrays(GL_POINTS, 0, m_centrelinePoints.size());

	/*
	glBindVertexArray(m_vaoCentreline);
	glDrawElements(GL_POINTS, m_centrelinePoints.size(), GL_UNSIGNED_INT, 0);
	*/
}

void CCatmullRom::RenderOffsetCurves()
{
	// Bind the VAO m_vaoLeftOffsetCurve and render it
	glLineWidth(5.0f);
	glBindVertexArray(m_vaoLeftOffsetCurve);
	glColor3f(1.0f, 1.0f, 1.0f);
	glDrawArrays(GL_LINE_LOOP, 0, m_leftOffsetPoints.size());
	glDrawArrays(GL_POINTS, 0, m_leftOffsetPoints.size());

	// Bind the VAO m_vaoRightOffsetCurve and render it
	glLineWidth(5.0f);
	glBindVertexArray(m_vaoRightOffsetCurve);
	glColor3f(1.0f, 1.0f, 1.0f);
	glDrawArrays(GL_LINE_LOOP, 0, m_rightOffsetPoints.size());
	glDrawArrays(GL_POINTS, 0, m_rightOffsetPoints.size());
}


void CCatmullRom::RenderTrack()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Bind the VAO m_vaoTrack and render it
	glBindVertexArray(m_vaoTrack);

	m_texture.Bind();

	for (int i = 0; i < m_vertexCount; i++) 
	{
		//if ((i % 5) == 0) 
		//{
			glDrawArrays(GL_TRIANGLE_STRIP, i, 5);
		//}
	}

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int CCatmullRom::CurrentLap(float d)
{

	return (int)(d / m_distances.back());

}