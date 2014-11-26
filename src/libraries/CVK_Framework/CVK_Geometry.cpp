#include "CVK_Geometry.h"

CVK::Geometry::Geometry()
{
	m_vertexbuffer = INVALID_OGL_VALUE;
	m_normalbuffer = INVALID_OGL_VALUE;
	m_uvbuffer = INVALID_OGL_VALUE;
	m_indexlist = INVALID_OGL_VALUE;
	m_tangentbuffer = INVALID_OGL_VALUE;

	m_vao = INVALID_OGL_VALUE;
}

CVK::Geometry::~Geometry()
{
	if (m_vertexbuffer != INVALID_OGL_VALUE)  glDeleteBuffers( 1, &m_vertexbuffer);
	if (m_normalbuffer != INVALID_OGL_VALUE)  glDeleteBuffers( 1, &m_normalbuffer);
	if (m_uvbuffer != INVALID_OGL_VALUE)  glDeleteBuffers( 1, &m_uvbuffer);
	if (m_indexlist != INVALID_OGL_VALUE)  glDeleteBuffers( 1, &m_indexlist);
	if (m_tangentbuffer != INVALID_OGL_VALUE)  glDeleteBuffers( 1, &m_tangentbuffer);

	if (m_vao != INVALID_OGL_VALUE) glDeleteVertexArrays(1, &m_vao);
}

void CVK::Geometry::createBuffers()
{
	m_points = m_vertices.size();
	m_indices = m_index.size();

	// create the buffers and bind the data 
	if ( m_vertexbuffer == INVALID_OGL_VALUE)
	{	
		glGenBuffers(1, &m_vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, m_points * sizeof(glm::vec4), &m_vertices[0], GL_STATIC_DRAW);
	}

	if ( m_normalbuffer == INVALID_OGL_VALUE && m_normals.size()>0)
	{
		glGenBuffers(1, &m_normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, m_points * sizeof(glm::vec3), &m_normals[0], GL_STATIC_DRAW);
	}

	if ( m_uvbuffer == INVALID_OGL_VALUE && m_uvs.size()>0)
	{
		glGenBuffers(1, &m_uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, m_points * sizeof(glm::vec2), &m_uvs[0], GL_STATIC_DRAW);
	}

	if ( m_tangentbuffer == INVALID_OGL_VALUE && m_uvs.size()>0) // TODO: Why does this depend on tex coords?
	{	
		if(m_tangents.empty())
		{	
			computeTangents();
		}
		glGenBuffers(1, &m_tangentbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_tangentbuffer);
		glBufferData(GL_ARRAY_BUFFER, m_tangents.size() * sizeof(glm::vec3), &m_tangents[0], GL_STATIC_DRAW);
	}

	// Generate a buffer for the indices as well 
	if (m_indexlist == INVALID_OGL_VALUE)
	{
		glGenBuffers(1, &m_indexlist);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexlist);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices * sizeof(unsigned int), &m_index[0] , GL_STATIC_DRAW);
	}
	
	if (m_vao == INVALID_OGL_VALUE)
		glGenVertexArrays(1, &m_vao);

	glBindVertexArray(m_vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
	glEnableVertexAttribArray(VERTICES);
	glVertexAttribPointer( VERTICES, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_normalbuffer);
	glEnableVertexAttribArray(NORMALS);
	glVertexAttribPointer(NORMALS, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_uvbuffer);
	glEnableVertexAttribArray(TEXTURECOORDS);
	glVertexAttribPointer(TEXTURECOORDS, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_tangentbuffer);
	glEnableVertexAttribArray(TANGENTS);
	glVertexAttribPointer(TANGENTS, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexlist);

	glBindVertexArray(0);
	
	glDisableVertexAttribArray(VERTICES); 
	glDisableVertexAttribArray(NORMALS);  
	glDisableVertexAttribArray(TEXTURECOORDS);
	glDisableVertexAttribArray(TANGENTS);
}

void CVK::Geometry::render()
{
	glBindVertexArray(m_vao);

	glDrawElements(GL_TRIANGLES, m_indices, GL_UNSIGNED_INT, 0);
}

void CVK::Geometry::computeTangents()
{		
	m_tangents.resize(m_vertices.size());

	for(int i = 0; i <m_indices; i+=3)
	{	
    	// Edges of the triangle : postion delta
    	int index_v0 = m_index[i];
    	int index_v1 = m_index[i+1];
    	int index_v2 = m_index[i+2];    	
    	
    	glm::vec4 deltaPos1 = m_vertices[index_v1]-m_vertices[index_v0];
    	glm::vec4 deltaPos2 = m_vertices[index_v2]-m_vertices[index_v0];
    	/*
    	glm::vec4 deltaPos1 = m_vertices[index_v0]-m_vertices[index_v1];
    	glm::vec4 deltaPos2 = m_vertices[index_v2]-m_vertices[index_v1];
 		*/
    	// UV delta
    	
    	glm::vec2 deltaUV1 = m_uvs[index_v1]-m_uvs[index_v0];
    	glm::vec2 deltaUV2 = m_uvs[index_v2]-m_uvs[index_v0];
    	/*
    	glm::vec2 deltaUV1 = m_uvs[index_v0]-m_uvs[index_v1];
    	glm::vec2 deltaUV2 = m_uvs[index_v2]-m_uvs[index_v1];
		*/
    	float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec4 tmp = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y)*r;
		glm::vec3 tangent = glm::vec3(tmp.x,tmp.y,tmp.z);

		// Set the same tangent for all three vertices of the triangle
		m_tangents[index_v0] = tangent;
    	m_tangents[index_v1] = tangent;
    	m_tangents[index_v2] = tangent;
	}
}

std::vector<glm::vec4>* CVK::Geometry::getVertices()
{
	return &m_vertices;
}

std::vector<glm::vec3>* CVK::Geometry::getNormals()
{
	return &m_normals;
}
 
std::vector<glm::vec2>* CVK::Geometry::getUVs()
{
	return &m_uvs;
}
 
std::vector<unsigned int>* CVK::Geometry::getIndex()
{
	return &m_index;
}
 
std::vector<glm::vec3>* CVK::Geometry::getTangents()
{
	return &m_tangents;
}



