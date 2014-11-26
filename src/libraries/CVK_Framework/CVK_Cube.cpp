#include "CVK_Cube.h"

CVK::Cube::Cube()
{
	create( 1.0f);
}

CVK::Cube::Cube(float size)
{
	create( size);
}

CVK::Cube::~Cube()
{
}

void CVK::Cube::create( float size)
{

        GLfloat vertices[] = { 
         1.0,  1.0,  1.0,   -1.0,  1.0,  1.0,   -1.0, -1.0,  1.0,    1.0, -1.0,  1.0,
        -1.0, -1.0, -1.0,   -1.0,  1.0, -1.0,    1.0,  1.0, -1.0,    1.0, -1.0, -1.0,
         1.0,  1.0,  1.0,    1.0,  1.0, -1.0,   -1.0,  1.0, -1.0,   -1.0,  1.0,  1.0,
        -1.0, -1.0, -1.0,    1.0, -1.0, -1.0,    1.0, -1.0,  1.0,   -1.0, -1.0,  1.0,
         1.0,  1.0,  1.0,    1.0, -1.0,  1.0,    1.0, -1.0, -1.0,    1.0,  1.0, -1.0,
        -1.0, -1.0, -1.0,   -1.0, -1.0,  1.0,   -1.0,  1.0,  1.0,   -1.0,  1.0, -1.0
    	};
    
    GLfloat normals[] = {        
         0.0,  0.0,  1.0,    0.0,  0.0,  1.0,    0.0,  0.0,  1.0,    0.0,  0.0,  1.0,
         0.0,  0.0, -1.0,    0.0,  0.0, -1.0,    0.0,  0.0, -1.0,    0.0,  0.0, -1.0,
         0.0,  1.0,  0.0,    0.0,  1.0,  0.0,    0.0,  1.0,  0.0,    0.0,  1.0,  0.0,
         0.0, -1.0,  0.0,    0.0, -1.0,  0.0,    0.0, -1.0,  0.0,    0.0, -1.0,  0.0,
         1.0,  0.0,  0.0,    1.0,  0.0,  0.0,    1.0,  0.0,  0.0,    1.0,  0.0,  0.0,
        -1.0,  0.0,  0.0,   -1.0,  0.0,  0.0,   -1.0,  0.0,  0.0,   -1.0,  0.0,  0.0
    	};        
    
    GLfloat texCoords[] = {
         0.0,  0.0,    0.0,  1.0,    1.0,  1.0,    1.0,  0.0,
         0.0,  0.0,    0.0,  1.0,    1.0,  1.0,    1.0,  0.0,
         0.0,  0.0,    0.0,  1.0,    1.0,  1.0,    1.0,  0.0,
         0.0,  0.0,    0.0,  1.0,    1.0,  1.0,    1.0,  0.0,
         0.0,  0.0,    0.0,  1.0,    1.0,  1.0,    1.0,  0.0,
         0.0,  0.0,    0.0,  1.0,    1.0,  1.0,    1.0,  0.0
    	};

	m_points = 24;
	m_indices = 36;

	for(int i=0; i<m_points; i++)
	{
		m_vertices.push_back(glm::vec4( vertices[i*3] * size, vertices[i*3+1] * size, vertices[i*3+2] * size, 1.0f));
		m_normals.push_back(glm::vec3( normals[i*3], normals[i*3+1], normals[i*3+2]));
		m_uvs.push_back(glm::vec2( texCoords[i*2], texCoords[i*2+1]));
	}

	for(int i=0; i<6; i++)
	{
		m_index.push_back( i*4+0);
		m_index.push_back( i*4+1);
		m_index.push_back( i*4+2);
		m_index.push_back( i*4+2);
		m_index.push_back( i*4+3);
		m_index.push_back( i*4+0);
	}

	createBuffers();
}

void CVK::Cube::setSize( float size)
{	
	
	m_vertices.clear();
	
	GLfloat vertices[] = { 
         1.0,  1.0,  1.0,   -1.0,  1.0,  1.0,   -1.0, -1.0,  1.0,    1.0, -1.0,  1.0,
        -1.0, -1.0, -1.0,   -1.0,  1.0, -1.0,    1.0,  1.0, -1.0,    1.0, -1.0, -1.0,
         1.0,  1.0,  1.0,    1.0,  1.0, -1.0,   -1.0,  1.0, -1.0,   -1.0,  1.0,  1.0,
        -1.0, -1.0, -1.0,    1.0, -1.0, -1.0,    1.0, -1.0,  1.0,   -1.0, -1.0,  1.0,
         1.0,  1.0,  1.0,    1.0, -1.0,  1.0,    1.0, -1.0, -1.0,    1.0,  1.0, -1.0,
        -1.0, -1.0, -1.0,   -1.0, -1.0,  1.0,   -1.0,  1.0,  1.0,   -1.0,  1.0, -1.0
    	};
	
	for(int i=0; i<m_points; i++)
	{
		m_vertices.push_back(glm::vec4( vertices[i*3] * size, vertices[i*3+1] * size, vertices[i*3+2] * size, 1.0f));
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, m_points * sizeof(glm::vec4), &m_vertices[0], GL_STATIC_DRAW);
	
}

