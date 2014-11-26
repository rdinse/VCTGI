#include "CVK_Cone.h"

CVK::Cone::Cone()
{
	m_baseRadius = 1.0f;
	m_topRadius = 1.0f;
	m_height = 2.0f;
	m_resolution = 20;
	create( m_baseRadius, m_topRadius, m_height, m_resolution);
}

CVK::Cone::Cone( float baseRadius, float topRadius, float height)
{
	m_baseRadius = baseRadius;
	m_topRadius = topRadius;
	m_height = height;
	m_resolution = 20;
	create( m_baseRadius, m_topRadius, m_height, m_resolution);
}

CVK::Cone::Cone( int resolution)
{
	m_baseRadius = 1.0f;
	m_topRadius = 1.0f;
	m_height = 2.0f;
	m_resolution = resolution;
	create( m_baseRadius, m_topRadius, m_height, m_resolution);
}

CVK::Cone::Cone( float baseRadius, float topRadius, float height, int resolution)
{
	m_baseRadius = baseRadius;
	m_topRadius = topRadius;
	m_height = height;
	m_resolution = resolution;
	create( m_baseRadius, m_topRadius, m_height, m_resolution);
}

CVK::Cone::~Cone()
{
}

void CVK::Cone::create( float baseRadius, float topRadius, float height, int resolution)
{
	// iniatialize the variable we are going to use
	float u, v;
	float radius, phi;
	float x, y, z;
	int offset = 0, i, j;
	glm::vec3 n1, n2, n;

	// create points

	// Envelope
 	for ( j = 0; j <= resolution; j++)  //radius
		for ( i = 0; i <= resolution; i++) //phi
		{
			u = i /(float)resolution;		phi   = 2* glm::pi<float>() * u;
			v = 1 - j /(float)resolution;		

			radius = (1-v)*baseRadius + v*topRadius;

 			x = radius * cos(phi);
			y = v * height;
			z = -radius * sin(phi);

			glm::vec3 n1( -radius*sin(phi), 0, -radius*cos(phi));
			glm::vec3 n2( (topRadius-baseRadius)*cos(phi), height, (baseRadius-topRadius)*sin(phi));
			glm::vec3 n = glm::normalize(glm::cross( n1, n2));
			m_vertices.push_back(glm::vec4( x, y, z, 1.0f));
			m_normals.push_back(n);
			m_uvs.push_back(glm::vec2( u, v));
		}

	m_points = m_vertices.size();

	// create index list
	for ( j = 0; j < resolution; j++)
	{
		for ( i = 0; i < resolution; i++)
		{
			// 1. Triangle
			m_index.push_back( offset + i );
			m_index.push_back( offset + i + resolution+1);
			m_index.push_back( offset + i + 1);

			// 2. Triangle
			m_index.push_back( offset + i + resolution+1);
			m_index.push_back( offset + i + resolution+1 + 1);
			m_index.push_back( offset + i + 1);
		}
		offset += resolution+1;
	}
	m_indices = m_index.size();

	createBuffers();
}
