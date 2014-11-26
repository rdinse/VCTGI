#include "CVK_Plane.h"


CVK::Plane::Plane()
{
	create( glm::vec3( -1, 1, 0), glm::vec3( -1, -1, 0), glm::vec3( 1, -1, 0), glm::vec3( 1, 1, 0),
		    glm::vec2( 0, 1), glm::vec2( 0, 0), glm::vec2( 1, 0), glm::vec2( 1, 1));
}

CVK::Plane::Plane( glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, glm::vec2 tca, glm::vec2 tcb, glm::vec2 tcc, glm::vec2 tcd)
{
	create( a, b, c, d, tca, tcb, tcc, tcd);
}

CVK::Plane::~Plane()
{
}

void CVK::Plane::create( glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, glm::vec2 tca, glm::vec2 tcb, glm::vec2 tcc, glm::vec2 tcd)
{
	m_vertices.push_back( glm::vec4(a, 1.0f));
	m_vertices.push_back( glm::vec4(b, 1.0f));
	m_vertices.push_back( glm::vec4(c, 1.0f));
	m_vertices.push_back( glm::vec4(d, 1.0f));

	glm::vec3 n = glm::normalize( glm::cross( b-a, c-a));

	m_normals.push_back( n);
	m_normals.push_back( n);
	m_normals.push_back( n);
	m_normals.push_back( n);

	m_uvs.push_back( tca);
	m_uvs.push_back( tcb);
	m_uvs.push_back( tcc);
	m_uvs.push_back( tcd);

	m_points = 4;

	m_index.push_back( 0);
	m_index.push_back( 1);
	m_index.push_back( 2);

	m_index.push_back( 0);
	m_index.push_back( 2);
	m_index.push_back( 3);

	m_indices = 6;

	createBuffers();
}

void CVK::Plane::set_Points( glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d)
{
	m_vertices.clear();
	m_vertices.push_back( glm::vec4(a, 1.0f));
	m_vertices.push_back( glm::vec4(b, 1.0f));
	m_vertices.push_back( glm::vec4(c, 1.0f));
	m_vertices.push_back( glm::vec4(d, 1.0f));
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, m_points * sizeof(glm::vec4), &m_vertices[0], GL_STATIC_DRAW);
}

void CVK::Plane::set_Tcoords( glm::vec2 tca, glm::vec2 tcb, glm::vec2 tcc, glm::vec2 tcd)
{
	m_uvs.clear();
	m_uvs.push_back( tca);
	m_uvs.push_back( tcb);
	m_uvs.push_back( tcc);
	m_uvs.push_back( tcd);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, m_points * sizeof(glm::vec2), &m_uvs[0], GL_STATIC_DRAW);
}
